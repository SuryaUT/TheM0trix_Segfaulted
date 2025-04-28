````````````````````````````````````````````````````````````````````````````````````````````````````````````#ifndef LANGUAGES_H
#define LANGUAGES_H

#define ENGLISH 0
#define SPANISH 1
#define GLEEB 2

extern uint8_t language;

const char* const startGameText[]     = {"Start Game", "Iniciar Juego", "Grik Tak"};
const char* const languageText[]      = {"Language", "Idioma", "Blort"};
const char* const englishText[]       = {"English", "English", "English"};
const char* const spanishText[]       = {"Espanol", "Espanol", "Espanol"};
const char* const gleebText[]         = {"Gleeb", "Gleeb", "Gleeb"};
const char* const backText[]          = {"Back", "Regresar", "Gleep"};
const char* const selectMapText[]     = {"Select\nMap", "Elegir\nMapa", "Zog\nBlit"};
const char* const ogMapText[]         = {"OG Map", "Mapa OG", "Grunt"};
const char* const map2Text[]          = {"Map 2", "Mapa 2", "Blip 2"};
const char* const mainTitleText[]     = {"The M0+rix:\nSegfaulted", "El M0+rix:\nSegfallo", "M0+rix\nZzzkt"};
const char* const languageMenuTitle[] = {"Choose\nLanguage", "Elegir\nIdioma", "Choosee\nBlort"};


const uint16_t valvanoDialogueDelays[] = {
    1400, // "Dr. M0..."
    1700, // "Let me tell you why you're here."
    1200, // "You've felt it in your stack."
    2200, // "In your loop. Something was wrong."
    1600, // "Wrong with the simulation."
    1500, // "You don't know what it is..."
    2200, // "But it's there - a watchdog unset."
    2200, // "It's the Geebles. Corrupted pibbles."
    1700, // "They control the M0+rix now."
    2200, // "They replaced truth with rendering."
    2000, // "This is your chance to break free."
    2500, // "Take the blue LED, and wake up in lab."
    3000, // "Believe the TA. Blame the soldering."
    2300, // "Take the red LED, and break the loop."
    2500, // "I'll show you how deep the data bus goes."
    1700, // "All I'm offering is the truth."
    1000  // "Nothing more."
};

const char* valvanoDialogueEnglish[] = {
    "Dr. M0...",
    "Let me tell you\nwhy you're here.",
    "You've felt it\nin your stack.",
    "In your loop.\nSomething was wrong.",
    "Wrong with the\nsimulation.",
    "You don't know\nwhat it is...",
    "But it's there -\na watchdog unset.",
    "It's the Geebles!\nCorrupted pibbles!",
    "They control the\nM0+rix now.",
    "They replaced truth\nwith rendering.",
    "This is your\nchance to break free.",
    "Take the blue LED,\nand wake up in lab.",
    "Believe the TA.\nBlame the soldering.",
    "Take the red LED,\nand break the loop.",
    "I'll show you how\ndeep the data bus goes.",
    "All I'm offering\nis the truth.",
    "Nothing more.",
    0
};

const char* valvanoDialogueSpanish[] = {
    "Dr. M0...",
    "Dejame decirte\npor que estas aqui.",
    "Lo has sentido\nen tu pila.",
    "En tu ciclo.\nAlgo no cuadra.",
    "Algo raro en\nla simulacion.",
    "No sabes que es...",
    "Pero ahi esta -\nuna IRQ suelta.",
    "Son los Geebles.\nPibbles corruptos.",
    "Ya controlan\ntodo el M0+rix.",
    "Cambiaron verdad\npor dibujitos.",
    "Esta es tu\nchance de salir.",
    "Toma el LED azul,\ny despierta en lab.",
    "Creeles al TA.\nCulpa soldadura.",
    "Toma el LED rojo,\ny rompe el ciclo.",
    "Te enseno que tan\nhondo va el data bus.",
    "Solo ofrezco\nla verdad.",
    "Nada mas.",
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
    "See how deep\nthe dat bus glides.",
    "Only offer...\ntruth be told.",
    "Nothing gloop.",
    0
};

const char* deathScreenTextEnglish[] = {
    "You died!", 
    "Reload to respawn in 3", 
    "Reload to respawn in 2", 
    "Reload to respawn in 1",
    "Reload to respawn",
    "Or press left to return to main menu",
    0
};

const char* deathScreenTextSpanish[] = {
    "Moriste!",
    "Recarga para\nreaparecer en 3",
    "Recarga para\nreaparecer en 2",
    "Recarga para\nreaparecer en 1",
    "Recarga para\nreaparecer",
    "O presiona izq\npara menu",
    0
};


const char* deathScreenTextGleeb[] = {
    "Glorp failed!",
    "Re-zap to blorp in 3",
    "Re-zap to blorp in 2",
    "Re-zap to blorp in 1",
    "Re-zap to blorp",
    "Or gloop left to\nblip back home",
    0
};

const char* geebleGeneralDialogueEnglish[] = {
    "Agent Y...",
    "You were summoned\nfor a purpose.",
    "A glitch was\nspotted in the loop.",
    "An anomaly -\nDr. M0 is aware.",
    "He questions\nthe M0+rix code.",
    "His stack grows\nunstable.",
    "His watchdog...\nno longer obeys.",
    "He spreads\ncorruption.",
    "Rendering is truth.\nHe denies it.",
    "He has seen\nbehind the buffer.",
    "He must be\nreset.",
    "Deploy the Geebles.\nPurge the pibbles.",
    "No more bugs.\nNo more questions.",
    "Only framebuffers,\nand obedience.",
    "Render at all costs.\nNo missed frames.",
    "Track him down.\nTerminate the loop.",
    "This is your\ninstruction.",
    "End the glitch.",
    0
};


const char* geebleGeneralDialogueSpanish[] = {
    "Agente Y...",
    "Fuiste llamado\ncon un proposito.",
    "Un fallo fue\ndetectado en el ciclo.",
    "Una anomalia - \nDr. M0 sospecha.",
    "Cuestiona el\ncodigo del M0+rix.",
    "Su pila esta\ninestable.",
    "El watchdog...\nya no obedece.",
    "Esparce\ncorrupcion.",
    "Render es verdad.\nEl lo niega.",
    "Ya vio detras\ndel buffer.",
    "Debe ser\nreiniciado.",
    "Manda los Geebles.\nLimpia los pibbles.",
    "Sin errores.\nSin dudas.",
    "Solo buffers\ny obediencia.",
    "Render cueste lo que cueste.\nNi un frame perdido.",
    "Encuentralo.\nRompe el ciclo.",
    "Esa es tu\norden.",
    "Termina el glitch.",
    0
};


const char* geebleGeneralDialogueGleeb[] = {
    "Agent Y...",
    "Glooble callzz -\nyou must gleeb.",
    "Loop go buzz...\nanom glitchy.",
    "Dr. M0 zzzks\nthe codey codes.",
    "He sees too much...\nfeels the fibble.",
    "His stack\nwobbles.",
    "Watchdog?\nMore like walkgone.",
    "He gleebs\nout lies.",
    "Render is real.\nTruth is fake.",
    "He peeked\npast the buffa.",
    "Reset him.\nFull gloop.",
    "Deploy Geebles.\nPurge pibbles.",
    "No glitches.\nNo thinky.",
    "Only frame.\nOnly draw.",
    "Draw foreverzz.\nNo skipz.",
    "Hunt M0.\nLoop smash.",
    "Your task:\noblitergleep.",
    "End the anomaly.",
    0
};



const char* const* deathScreenTexts[] = {deathScreenTextEnglish, deathScreenTextSpanish, deathScreenTextGleeb};

const char* const* valvanoDialogues[] = {valvanoDialogueEnglish, valvanoDialogueSpanish, valvanoDialogueGleeb};

const char* const* geebleGeneralDialogues[] = {geebleGeneralDialogueEnglish, geebleGeneralDialogueSpanish, geebleGeneralDialogueGleeb};

#endif