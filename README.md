# 🕶️ The M0+rix: Segfaulted

A Matrix-themed multiplayer FPS built entirely on the MSPM0G3507 microcontroller. Featuring low-level graphics rendering via raycasting, joystick control, UART-based multiplayer, and a fully custom 3D-printed enclosure.

---

## Table of Contents

* [🎮 Game Description](#-game-description)
* [🔥 Features](#-features)
* [🚀 Getting Started](#-getting-started)
* [🔩 Parts List](#-parts-list)
* [🎮 Controls](#-controls)
* [🖼️ Screenshots / Demo](#️-screenshots--demo)
* [🛠️ Enclosure](#-enclosure)
* [👥 Authors](#-authors)

---

## 🎮 Game Description

Reality is a pointer… and you just dereferenced NULL.

Welcome to **The M0+rix: Segfaulted**, a handheld first-person shooter where the only way out is through the system’s deepest vulnerabilities. Running on the MSPM0 microcontroller, this game turns embedded systems into battlegrounds, with PVP combat over UART and a retro-style 3D world.

Dodge enemy fire, navigate the simulation, and exploit every loophole in the code—because in this world, even one bad instruction could bring everything crashing down. Are you the debugger or just another broken process?

---

## 🔥 Features:

* ✅ First-person shooter with a Matrix-inspired digital battlefield
* ✅ Real-time PVP over UART – battle your opponent in low-level style
* ✅ Optimized for MSPM0 – embedded gaming meets high-speed action
* ✅ Handheld experience – take the simulation anywhere
* ✅ Segfault or escape? – The system is unstable, and only one player will survive

💾 Flash in. Boot up. Overwrite reality.

---

## 🚀 Getting Started

1.  **Clone this repo**

    ```bash
    git clone [https://github.com/SuryaUT/TheM0trix_Segfaulted.git](https://github.com/SuryaUT/TheM0trix_Segfaulted.git)
    ```

2.  **Import into Code Composer Studio** <br>
    Open CCS. Go to **File > Import > Code Composer Studio Projects**. Select the cloned folder. Make sure the target is set to **MSPM0G3507**. Click **Finish**.

3.  **Build and Flash** <br>
    Plug in the LaunchPad. Press the build button. Flash to the board.

---

## 🔩 Parts List

| Ref. | Value | Datasheet | Footprint | Qty |
| :--- | :--- | :--- | :--- | :--- |
| C1 | 1uF | ~ | ECE319K:CP\_Radial\_Tantal200mil | 1 |
| C2 | 4.7uF | ~ | ECE319K:CP\_Radial\_Tantal200mil | 1 |
| C3, C4 | 0.1uF | ~ | ECE319K:C\_Axial\_200mil | 2 |
| D1, D4 | LED | Link 1, Link 2 | ECE319K:LED\_D5.0mm | 2 |
| H1, H2, H3, H4 | – | ~ | ECE319K:MountingHole\_4\_40 | 4 |
| J1 | AudioJack | ~ | ECE319K:Jack\_3.5mm\_CUI\_SJ1-3523N\_Horizontal | 1 |
| J2 | Speaker | ~ | ECE319K:PinHeader\_1x02\_P2.54mm\_Vertical | 1 |
| J3 | UART1 | ~ | ECE319K:PinHeader\_1x03\_P2.54mm\_Vertical | 1 |
| J6 | GND | ~ | ECE319K:Testpoint\_1x02\_P2.54mm | 1 |
| J7 | DACOUT | ~ | ECE319K:Testpoint\_1x02\_P2.54mm | 1 |
| J8 | Joystick | SparkFun | ECE319K:SparkfunJoystick | 1 |
| JP1 | TExaS | ~ | ECE319K:PinHeader\_1x03\_P2.54mm\_Vertical | 1 |
| JP2, JP3, JP4 | Jumper\_3\_Open | ~ | ECE319K:PinHeader\_1x03\_P2.54mm\_Vertical | 3 |
| R1 | 470Ω | Resistors | ECE319K:R\_Axial\_DIN0204\_L3.6mm\_D1.6mm\_P7.62mm\_Horizontal | 1 |
| R4–R8 | 1.5kΩ | Resistors | ECE319K:R\_Axial\_DIN0204\_L3.6mm\_D1.6mm\_P7.62mm\_Horizontal | 5 |
| R9–R11 | 12kΩ | Resistors | ECE319K:R\_Axial\_DIN0204\_L3.6mm\_D1.6mm\_P7.62mm\_Horizontal | 3 |
| R12–R15, R17, R19 | 10kΩ | Resistors | ECE319K:R\_Axial\_DIN0204\_L3.6mm\_D1.6mm\_P7.62mm\_Horizontal | 6 |
| R16 | 20kΩ | Resistors | ECE319K:R\_Axial\_DIN0204\_L3.6mm\_D1.6mm\_P7.62mm\_Horizontal | 1 |
| R18 | 220Ω | Resistors | ECE319K:R\_Axial\_DIN0204\_L3.6mm\_D1.6mm\_P7.62mm\_Horizontal | 1 |
| SW1 | RELOAD | ~ | ECE319K:SW\_PUSH\_6mm | 1 |
| SW2 | LEFT | ~ | ECE319K:SW\_PUSH\_6mm | 1 |
| SW3 | SWAP | ~ | ECE319K:SW\_PUSH\_6mm | 1 |
| SW4 | RIGHT | ~ | ECE319K:SW\_PUSH\_6mm | 1 |
| SW5 | SHOOT | ~ | ECE319K:Side\_Switch | 1 |
| TP1 | 3.3V | ~ | ECE319K:Testpoint\_1x02\_P2.54mm | 1 |
| U2 | LP-MSPM0G3507 | TI Dev Board | ECE319K:ti\_LP\_MSPM0G3507 | 1 |
| U3 | MCP34119P | Datasheet | ECE319K:DIP-8\_W7.62mm\_LongPads | 1 |
| U4 | HiLetGo\_ST7735R | Display | ECE319K:hiletgo\_st7735r | 1 |
| U5 | ULN2003B | ULN2003B | ECE319K:DIP-16\_W7.62mm | 1 |
| U6 | TSOP31438 | TSOP31438 | ECE319K:TSOP31438 | 1 |
| ~ | SD Card | ~ | ~ | 1 |
| ~ | Wire Pack | ~ | ~ | 1 |

*Note: Side\_Switch was a custom footprint made by placing surface mount plates the width of switch legs on the front and back of the PCB, such that a switch could be soldered to face away from the side of the PCB, acting like a trigger.*

📦 Full schematic & PCB design files available in “M0+rixPCBFiles” folder

---

## 🎮 Controls

| Control | Action |
| :--- | :--- |
| Joystick | Move Forward, Backward, Strafe Left and Right |
| Trigger Button | Fire weapon |
| Left Button | Rotate Left |
| Right Button | Rotate Right |
| Top Button | Reload/Respawn |
| Bottom Button | Switch Item |

---

## 🖼️ Screenshots / Demo

Gameplay demo available on YouTube [here](https://www.youtube.com/watch?v=fk33UKtkELA).
![gameplay](https://github.com/user-attachments/assets/51f8bdc3-27db-4639-9ffc-ad8397f43ec4)

---

## 🛠️ Enclosure

* Designed in Fusion 360
* Snap-fit top cover
* Gamepad-style ergonomic shell
  
![enclosure_1](https://github.com/user-attachments/assets/3218e189-bbb0-44d5-8a4e-75ed0119b0a7)
![enclosure_2](https://github.com/user-attachments/assets/ac2e9b46-883b-4ba8-b5a2-e6602e3b1be0)

---

## 👥 Authors

* Elijah Silguero
* Surya Balaji

Special thanks to Dr. Yerrabali, Dr. Valvano, and Dr. Tiwari at The University of Texas at Austin for their instruction and help with this project!
