from PIL import Image

IMAGE_HEIGHT = 64
IMAGE_WIDTH = 64

def bmp_to_c_array_direct_color_scaled(bmp_file_path):
    """
    Converts a bitmap image of anysize to a 64 x 64 1D C array of uint16_t in BGR565 format,
    using the direct color values from the image with explicit scaling to bit depths.

    Args:
        bmp_file_path (str): The path to the bitmap image file.

    Returns:
        str: A string representing the C array in BGR565 format.
    """

    try:
        img = Image.open(bmp_file_path)
        img = img.resize((IMAGE_WIDTH, IMAGE_HEIGHT))
        img = img.convert("RGB")
        pixels = img.load()

        c_array_values = []
        for y in range(IMAGE_HEIGHT):
            for x in range(IMAGE_WIDTH):
                r, g, b = pixels[x, y]

                # Scale 8-bit values to their respective bit depths
                blue5bit = b >> 3  # Equivalent to dividing by 8 and taking the integer part
                green6bit = g >> 2 # Equivalent to dividing by 4 and taking the integer part
                red5bit = r >> 3   # Equivalent to dividing by 8 and taking the integer part

                # Combine into BGR565 format
                colorBGR565 = ((blue5bit & 0x1F) << 11) | ((green6bit & 0x3F) << 5) | (red5bit & 0x1F)
                c_array_values.append(f"0x{colorBGR565:04X}")

        c_array_string = "{\n    " + ", ".join(c_array_values) + "\n};"
        return c_array_string

    except FileNotFoundError:
        return f"Error: File not found at {bmp_file_path}"
    except Exception as e:
        return f"An error occurred: {e}"

if __name__ == "__main__":
    input_image_path = input("Enter the path to the 32x32 image file (.bmp or .png): ")
    c_array_output = bmp_to_c_array_direct_color_scaled(input_image_path)
    print("\nconst uint16_t myTexture[32 * 32] = " + c_array_output)