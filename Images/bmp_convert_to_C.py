from PIL import Image
import os


#C:\CCS\MSPM0_ValvanoWare\Game\Textures
#C:\CCS\MSPM0_ValvanoWare\Game\Textures\TexArrays.txt

def bmp_to_c_array_direct_color_scaled(image_file_path, resolution):
    """
    Converts a bitmap or png image of any size to a 1D C array of uint16_t in BGR565 format.

    Args:
        image_file_path (str): The path to the bitmap or png image file.

    Returns:
        tuple: A tuple containing the filename and a string representing the C array,
               or None and an error message if an error occurs.
    """
    try:
        img = Image.open(image_file_path)
        img = img.resize((resolution, resolution))
        img = img.convert("RGB")
        pixels = img.load()

        c_array_values = []
        for y in range(resolution):
            for x in range(resolution):
                r, g, b = pixels[x, y]

                blue5bit = b >> 3
                green6bit = g >> 2
                red5bit = r >> 3

                colorBGR565 = ((blue5bit & 0x1F) << 11) | ((green6bit & 0x3F) << 5) | (red5bit & 0x1F)
                c_array_values.append(f"0x{colorBGR565:04X}")

        c_array_string = "{\n    " + ", ".join(c_array_values) + "\n};"
        filename = os.path.splitext(os.path.basename(image_file_path))[0]
        return filename, c_array_string

    except FileNotFoundError:
        return None, f"Error: File not found at {image_file_path}"
    except Exception as e:
        return None, f"An error occurred while processing {image_file_path}: {e}"

if __name__ == "__main__":
    input_folder_path = input("Enter the path to the folder containing the BMP and/or PNG files: ")
    output_file_path = input("Enter the desired output text file path (.txt): ")
    resolution = int(input("Enter your desired resolution (Ex. 32 for 32x32): "))

    try:
        image_files = [f for f in os.listdir(input_folder_path) if f.lower().endswith(('.bmp', '.png'))]
        if not image_files:
            print(f"No BMP or PNG files found in the folder: {input_folder_path}")
            exit()
    except FileNotFoundError:
        print(f"Error: Folder not found at {input_folder_path}")
        exit()
    except Exception as e:
        print(f"An error occurred while accessing the folder: {e}")
        exit()

    with open(output_file_path, 'w') as outfile:  # Changed 'w' to 'a' for append mode
        for image_file in image_files:
            image_path = os.path.join(input_folder_path, image_file)
            filename_and_array = bmp_to_c_array_direct_color_scaled(image_path, resolution)
            if filename_and_array:
                filename, c_array_output = filename_and_array
                outfile.write(f"// Array for image: {image_file}\n")
                outfile.write(f"const uint16_t {filename}[{resolution} * {resolution}] = {c_array_output}\n\n")
            else:
                error_message = filename_and_array[1]
                outfile.write(f"// {error_message}\n\n")

    print(f"\nSuccessfully converted images and saved C arrays to {output_file_path}")