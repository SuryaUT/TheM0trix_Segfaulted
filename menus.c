#include "FIFO.h"
#include "ST7735_SDC.h"
#include "Joy.h"
#include "Input.h"
#include "Graphics.h"
#include "UART1.h"
#include "UART2.h"
#include "maps.h"
#include "SoundSD.h"
#include "Languages.h"
#include "Sounds.h"
#include "Sync.h"
#include "ti/devices/msp/msp.h"
#include "../inc/Timer.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct Menu Menu;
typedef void (*ActionFunc)(void);

typedef struct {
    const char * const *label;
    const Menu *nextMenu;
    ActionFunc  action;
} MenuItem;

struct Menu {
    const char* const *title;
    const MenuItem *items;
    uint8_t       count;
    const char   *bgImage;
};

#define DIALOGUE_TYPE_DELAY 50

void clearDialogueLine(){
    ST7735_FillRect(0, 112, 160, 16, ST7735_BLACK);
}

void typeDialogueLine(const char* text, uint16_t delayAfter){
    ST7735_DrawTextBoxS_IF(0, 112, 160, text, ST7735_WHITE, ST7735_BLACK, 1, 1, DIALOGUE_TYPE_DELAY);
    friendlyDelay(delayAfter);
}

static int8_t y = 0;
uint8_t triggerPressed = 0;
uint8_t selection = 0;
uint8_t triggerMode = 0;
extern uint8_t healthCode;

// Epic pre-game dialogue
void dialogueScreen(){
    if (!IS_DOMINANT_CONTROLLER){
        ST7735_DrawBitmapFromSDC(0, 127, "VANO.bin", 160, 128);
        ST7735_DrawTextBoxS_IF(0, 0, 160, "Valvano:", ST7735_WHITE, ST7735_WHITE, 2, 0, 10);

        uint8_t dialogueIndex = 0;
        while (valvanoDialogues[language][dialogueIndex] != 0){
            typeDialogueLine(valvanoDialogues[language][dialogueIndex], valvanoDialogueDelays[dialogueIndex]);
            dialogueIndex++;
            if (valvanoDialogues[language][dialogueIndex] != 0) clearDialogueLine();
            if (triggerPressed) break;
        }
    }
    else{
        triggerMode = 1;
        ST7735_DrawBitmapFromSDC(0, 127, "GEEB.bin", 160, 128);
        ST7735_DrawTextBoxS_IF(0, 0, 160, "General Gleeb", ST7735_WHITE, ST7735_WHITE, 2, 0, 10);

        uint8_t dialogueIndex = 0;
        while (geebleGeneralDialogues[language][dialogueIndex] != 0){
            typeDialogueLine(geebleGeneralDialogues[language][dialogueIndex], 2000); // default delay for General Gleeb
            dialogueIndex++;
            if (geebleGeneralDialogues[language][dialogueIndex] != 0) clearDialogueLine();
            if (GPIOA->DIN31_0 >> 24 & 1) triggerPressed = 1; 
            if (triggerPressed) break;
        }
    }
    NVIC->ICER[0] = (1<<19); // Disable sync interrupt
    if (IS_DOMINANT_CONTROLLER){
        // Flush UART Receiver
        while (RxFifo_Size()) {
            UART2_InChar0();
        }
    }
    friendlyDelay(1000);

    if (IS_DOMINANT_CONTROLLER){
        while (UART2_InChar0() != 'M') {}
    }
    else{
        UART1_OutChar('M');
    }
}




static bool startGameSelected = false;
uint8_t language = ENGLISH;

static void Action_StartGame(void) { /* placeholder, not used */ }
static void Action_SetEnglish(void) { language = ENGLISH; }
static void Action_SetSpanish(void) { language = SPANISH; }
static void Action_SetGleeb(void) { language = GLEEB; }
static void Action_SelectOGMap(void) {
    FillMap(OGMap);
    dialogueScreen();
    startGameSelected = true;
}
static void Action_SelectMap2(void) {
    FillMap(Map2);
    dialogueScreen();
    startGameSelected = true;
}




static MenuItem mapMenuItems[] = {
    { ogMapText,    0, Action_SelectOGMap },
    { map2Text,     0, Action_SelectMap2  },
    { backText,     0, 0                  },
};
static Menu mapMenu = { selectMapText, mapMenuItems, 3, 0 };

static MenuItem mainMenuItems[] = {
    { startGameText, &mapMenu, 0 },
    { languageText,   0,     0 },
};
static Menu mainMenu = { mainTitleText, mainMenuItems, 2, "LDSC.bin" };

static MenuItem languageMenuItems[] = {
    { englishText, 0, Action_SetEnglish },
    { spanishText, 0, Action_SetSpanish },
    { gleebText, 0, Action_SetGleeb },
    { backText,    &mainMenu, 0         },
};
static Menu languageMenu = { languageMenuTitle, languageMenuItems, 4, 0 };

// link pointers for submenus
static void LinkMenus(void) {
    mainMenuItems[1].nextMenu = &languageMenu;
    mapMenuItems[2].nextMenu  = &mainMenu;
}



static int8_t GetJoystickY(void) {
    extern int Joy_y;
    return (Joy_y > 0) ? -1 : (Joy_y < 0 ? 1 : 0);
}

static bool GetTriggerPressed(void) {
    if (triggerMode == 0){
        static uint8_t last = 0;
        uint8_t now = (GPIOA->DIN31_0 >> 24) & 1;
        bool pressed = (now && !last);
        last = now;
        return pressed;
    }
    return (GPIOA->DIN31_0 >> 24) & 1;
}



static void Menu_Delay(void) {
    for (uint32_t i = 0; i < 150*11; i++) { SoundSD_Service(); __WFI(); }
}

#define MENU_ITEM_OFFSET(x) (106 - ((x-1)*16))

void sendMenuState(){
    UART1_OutChar('<');
    UART1_OutChar(triggerPressed+1);
    UART1_OutChar(selection+1);
    UART1_OutChar('>');
}

void getMenuState(){
    if (RxFifo_Size() < 3) return;
    while (UART2_InChar() != '<') {}
    uint8_t intrigger = UART2_InChar() - 1;
    uint8_t inselect = UART2_InChar() - 1;
    if (UART2_InChar() == '>'){
        triggerPressed = intrigger;
        selection = inselect;
    }
}


uint8_t needSync;
void Menus_Run(void) {
    needSync = 1;
    triggerMode = 0;
    while (healthCode == RESTARTCODE){}
    NVIC->ICER[0] = (1<<2) | (1<<20);
    TimerA1_IntArm(400000/128, 128, 2);
    SoundEffects_disable();
    startGameSelected = false;
    LinkMenus();
    (void)GetTriggerPressed();
    int8_t prevY = GetJoystickY();

    const Menu *current   = &mainMenu;
    const Menu *prevMenu  = 0;
    selection = 0;
    uint8_t      prevSelect = UINT8_MAX;

    // Immediately Sync up controllers
    while (UART2_InChar0() != 'M'){}
    friendlyDelay(10);
    needSync = 0;

    // Pibble Studios screen
    ST7735_ClearScreenBlack();
    ST7735_DrawTextBoxS_IF(0, 42, 160, "Pibble\nStudios\nPresents", ST7735_WHITE, ST7735_WHITE, 2, 1, 50);
    friendlyDelay(1000);

    while (!startGameSelected) {
        SoundSD_Service();
        if (current != prevMenu) {
            if (current->bgImage) {
                ST7735_DrawBitmapFromSDC(0, 127, current->bgImage, 160, 128);
            } else {
                ST7735_ClearScreenBlack();
            }
            ST7735_DrawTextBoxS_IF(0, 0, 160, current->title[language],
                                ST7735_WHITE, ST7735_WHITE,
                                2, 1, 100);
            for (uint8_t i = 0; i < current->count; i++) {
                uint16_t fg = (i == selection) ? MATRIX_NEON_GREEN : ST7735_WHITE;
                ST7735_DrawTextBoxS_IF(0, MENU_ITEM_OFFSET(current->count) + i*16, 160,
                                    current->items[i].label[language],
                                    fg, fg,
                                    1, 1, 0);
                SoundSD_Service(); __WFI();
            }
            prevMenu   = current;
            prevSelect = selection;
        }
        else if (selection != prevSelect) {
            ST7735_DrawTextBoxS_IF(0, MENU_ITEM_OFFSET(current->count) + prevSelect*16, 160,
                                current->items[prevSelect].label[language],
                                ST7735_WHITE, ST7735_WHITE,
                                1, 1, 0);
            ST7735_DrawTextBoxS_IF(0, MENU_ITEM_OFFSET(current->count) + selection*16, 160,
                                current->items[selection].label[language],
                                MATRIX_NEON_GREEN, MATRIX_NEON_GREEN,
                                1, 1, 0);
            prevSelect = selection;
        }

        if (y && !prevY) {
            selection = (y > 0) ? (selection + 1) % current->count : (selection + current->count - 1) % current->count;
            Menu_Delay();
        }
        prevY = y;

        if (triggerPressed) {
            MenuItem const *item = &current->items[selection];
            if (item->action) {
                item->action();
                if (!item->nextMenu) {
                    current   = &mainMenu;
                    selection = 0;
                    prevMenu  = 0;
                }
            } else if (item->nextMenu) {
                current   = item->nextMenu;
                selection = 0;
                prevMenu  = 0;
            }
            Menu_Delay();
        }
    }
}


void TIMA1_IRQHandler(void){
  if((TIMA1->CPU_INT.IIDX) == 1){ // this will acknowledge
    if (needSync){
        UART1_OutChar('M');
    }
    else{
        if (IS_DOMINANT_CONTROLLER){
            y = GetJoystickY();
            triggerPressed = GetTriggerPressed();
            sendMenuState();
        }
        else{
            getMenuState();
        }
    }
  }
}