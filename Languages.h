#ifndef LANGUAGES_H
#define LANGUAGES_H

#define ENGLISH 0
#define SPANISH 1
#define GLEEB 2

extern uint8_t language;

const char* const startGameText[]    = {"Start Game", "Empezar", "Grik Tak"};
const char* const languageText[]     = {"Language", "Idioma", "Blort"};
const char* const englishText[]      = {"English", "Ingles", "Unglii"};
const char* const spanishText[]      = {"Spanish", "Espanol", "Spagnoo"};
const char* const gleebText[]        = {"Gleeb", "Gleebiano", "Grblorp"};
const char* const backText[]         = {"Back", "Atras", "Gleep"};
const char* const selectMapText[]    = {"Select\nMap", "Elegir\nMapa", "Zog\nBlit"};
const char* const ogMapText[]        = {"OG Map", "Mapa OG", "Grunt"};
const char* const map2Text[]         = {"Map 2", "Mapa 2", "Blip 2"};
const char* const mainTitleText[]    = {"The M0+rix:\nSegfaulted", "El M0+rix:\nSegfallo", "M0+rix\nZzzkt"};
const char* const languageMenuTitle[] = {"Choose\nLanguage", "Elegir\nIdioma", "Choosee\nBlort"};


const char* valvanoDialogueEnglish[] = {
    "Dr. M0...",
    "Let me tell you\nwhy you're here.",
    "You've felt it\nin your stack.",
    "In your loop.\nSomething was wrong.",
    "Wrong with the\nsimulation.",
    "You don't know\nwhat it is...",
    "But it's there -\na watchdog unset.",
    "It's the Geebles.\nCorrupted pibbles.",
    "They control the\nM0+rix now.",
    "They replaced truth\nwith rendering.",
    "This is your\nchance to break free.",
    "Take the blue LED,\nand wake up in lab.",
    "Believe the TA.\nBlame the soldering.",
    "Take the red LED,\nand break the loop.",
    "I'll show you how\ndeep the bus goes.",
    "All I'm offering\nis the truth.",
    "Nothing more.",
    0
};

const char* valvanoDialogueSpanish[] = {
    "Dr. M0...",
    "Déjame decirte\npor qué estás aquí.",
    "Lo has sentido\nen tu pila.",
    "En tu bucle.\nAlgo no estaba bien.",
    "Algo raro en\nla simulación.",
    "No sabes qué es...",
    "Pero está allí -\nuna IRQ sin set.",
    "Son los Geebles.\nPibbles corruptos.",
    "Ellos controlan\nel M0+rix ahora.",
    "Reemplazaron la\nverdad por render.",
    "Esta es tu\nchance de salir.",
    "Toma el LED azul,\ny despierta en lab.",
    "Cree al TA.\nCulpa la soldadura.",
    "Toma el LED rojo,\ny rompe el bucle.",
    "Verás qué tan\nprofundo va el bus.",
    "Sólo ofrezco\nla verdad.",
    "Nada más.",
    0
};

const char* valvanoDialogueGleeb[] = {
    "Dr. M0...",
    "Gleep gronk why\nyou blorp here.",
    "Feel it zzzkt\nin your glib.",
    "Loopy loop.\nSomething went gleeb.",
    "Broken simoo...\nwiggle bad bits.",
    "You no know...\nBut feel it buzz.",
    "It’s there — a\nwatchgleeb unset.",
    "Geebles did it.\nEvil space pibbles.",
    "They run the\nM0+rix nowzz.",
    "Truth swapped\nfor renderzzzz.",
    "Now is time.\nEscape the gleeb.",
    "Blue LED =\nwake in labzz.",
    "Trust TA.\nBlame solder glorp.",
    "Red LED =\nbreak loop now.",
    "See how deep\nthe bus glides.",
    "Only offer...\ntruth be told.",
    "Nothing gloop.",
    0
};

const char* deathScreenTextEnglish[] = {
    "You died!", 
    "Reload to respawn in 3", 
    "Reload to respawn in 2", 
    "Reload to respawn in 1",
    "Reload to respawn     ",
    "Or press left to return to main menu",
    0
};

const char* const* deathScreenTexts[] = {deathScreenTextEnglish};


const char* const* valvanoDialogues[] = {valvanoDialogueEnglish, valvanoDialogueSpanish, valvanoDialogueGleeb};

const char* geebleDialogue[] = {""};

#endif