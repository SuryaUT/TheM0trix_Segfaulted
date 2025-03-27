#define TEX_WIDTH 16
#define TEX_HEIGHT 16

#define CIRCUIT_BOARD_GREEN 0x06E0 // Example RGB565 color for circuit board green
#define MATRIX_GREEN 0x07E0          // Standard bright green
#define MATRIX_DARK_GREEN 0x0320    // Dark green for the "falling" effect
#define MATRIX_BACKGROUND 0x0000    // Black background
#define WIRE_COLOR1 0x05A0          // A slightly darker green
#define WIRE_COLOR2 0x04E0 
#define COMPONENT_COLOR 0x03A0      // Darker color for components
#define TRACE_COLOR 0x0770

uint16_t textures[8][TEX_WIDTH * TEX_HEIGHT];

void Textures_Init();