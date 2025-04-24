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
#include <stdbool.h>
#include <stdint.h>

// Forward declaration of entry point
typedef struct Menu Menu;
typedef void (*ActionFunc)(void);

//--------------------------------------------------------------------------------
// Menu data structures
//--------------------------------------------------------------------------------
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

void typeDialogueLine(const char* text){
    ST7735_DrawTextBoxS_IF(0, 112, 160, text, ST7735_WHITE, ST7735_BLACK, 1, 1, DIALOGUE_TYPE_DELAY);
    friendlyDelay(1000);
}

// Epic pre-game dialogue
void dialogueScreen(){
    if (IS_DOMINANT_CONTROLLER){
        ST7735_DrawBitmapFromSDC(0, 127, "VANO.bin", 160, 128);
        ST7735_DrawTextBoxS_IF(0, 0, 160, "Valvano:", ST7735_WHITE, ST7735_WHITE, 2, 0, 10);
        uint8_t dialogueIndex = 0;
        while (valvanoDialogues[language][dialogueIndex] != 0){
            typeDialogueLine(valvanoDialogues[language][dialogueIndex]);
            dialogueIndex++;
            if (valvanoDialogues[language][dialogueIndex] != 0) clearDialogueLine();
            if (GPIOA->DIN31_0 >> 24 & 1) break;
        }
    }
    else{
        ST7735_DrawBitmapFromSDC(0, 127, "VANO.bin", 160, 128);
        ST7735_DrawTextBoxS_IF(0, 0, 160, "Geeble General:", ST7735_WHITE, ST7735_WHITE, 2, 0, 10);
        uint8_t dialogueIndex = 0;
        while (geebleGeneralDialogues[language][dialogueIndex] != 0){
            typeDialogueLine(geebleGeneralDialogues[language][dialogueIndex]);
            dialogueIndex++;
            if (geebleGeneralDialogues[language][dialogueIndex] != 0) clearDialogueLine();
            if (GPIOA->DIN31_0 >> 24 & 1) break;
        }
    }
    friendlyDelay(1000);
}

//--------------------------------------------------------------------------------
// Action callbacks
//--------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------
// Menu definitions (multi-language)
//--------------------------------------------------------------------------------

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
static Menu mainMenu = { mainTitleText, mainMenuItems, 2, "MENU.bin" };

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

//--------------------------------------------------------------------------------
// Input helpers
//--------------------------------------------------------------------------------
static int8_t GetJoystickY(void) {
    extern int Joy_y;
    return (Joy_y > 0) ? -1 : (Joy_y < 0 ? 1 : 0);
}
static bool GetTriggerPressed(void) {
    static uint8_t last = 0;
    uint8_t now = (GPIOA->DIN31_0 >> 24) & 1;
    bool pressed = (now && !last);
    last = now;
    return pressed;
}

//--------------------------------------------------------------------------------
// Debounce delay when changing selection
//--------------------------------------------------------------------------------
static void Menu_Delay(void) {
    uint32_t ticks = (11025U / 1000U) * 150U;
    for (uint32_t i = 0; i < ticks; i++) { SoundSD_Service(); __WFI(); }
}

#define MENU_ITEM_OFFSET(x) (106 - ((x-1)*16))

void sendMenuState(int8_t y, uint8_t pressed){
    UART1_OutChar('<');
    UART1_OutChar(y);
    UART1_OutChar(pressed);
}

void getMenuState(int8_t* y, uint8_t* pressed){
    while (UART2_InChar() != '<') {}
    *y = UART2_InChar();
    *pressed = UART2_InChar();
}

//--------------------------------------------------------------------------------
// Entry point: call this after SystemInit()
//--------------------------------------------------------------------------------
void Menus_Run(void) {
    SoundEffects_disable();
    startGameSelected = false;
    LinkMenus();
    (void)GetTriggerPressed();
    int8_t prevY = GetJoystickY();

    const Menu *current   = &mainMenu;
    const Menu *prevMenu  = 0;
    uint8_t      selection = 0;
    uint8_t      prevSelect = UINT8_MAX;

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

        int8_t y;
        uint8_t triggerPressed;
        if (IS_DOMINANT_CONTROLLER) {
            y = GetJoystickY(); 
            triggerPressed = GetTriggerPressed(); 
            sendMenuState(y, triggerPressed);
        }
        else getMenuState(&y, &triggerPressed);
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
