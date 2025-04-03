#ifndef ASYNC_DELAY_H
#define ASYNC_DELAY_H

#include <stdint.h>

// Create an asyncronous delay for any number of milliseconds, setting the given flag when done
void start_delay(uint32_t milliseconds, uint8_t* flag);

#endif