#ifndef __JOY_H__
#define __JOY_H__

// Initialize Sparkfun Joystick
void Joy_Init();

// 1 if joystick is pressed, 0 if not
uint8_t Joy_InButton();

// Read joystick
void Joy_In(uint32_t* x, uint32_t* y);


#endif