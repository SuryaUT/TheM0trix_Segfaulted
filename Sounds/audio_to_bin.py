import os
from pydub import AudioSegment
import numpy as np

def convert_audio(input_file, output_file, bits=8, sample_rate=11025):
    assert bits in [8, 12], "Only 8-bit or 12-bit output is supported."

    print(f"\nLoading {input_file}...")
    audio = AudioSegment.from_file(input_file)
    audio = audio.set_channels(1)               # convert to mono
    audio = audio.set_frame_rate(sample_rate)   # resample

    print(f"Converting to {sample_rate}Hz, {bits}-bit resolution...")

    samples = np.array(audio.get_array_of_samples())
    max_val = max(abs(int(samples.max())), abs(int(samples.min())))
    samples = samples.astype(np.float32) / max_val

    if bits == 8:
        scaled = (samples * 127.5 + 127.5).astype(np.uint8)
    elif bits == 12:
        scaled = (samples * 2047.5 + 2047.5).astype(np.uint16)

    with open(output_file, "wb") as f:
        f.write(scaled.tobytes())

    print(f"\n✅ Done! Wrote {len(scaled)} samples to {output_file}\n")

# ========== USER PROMPT VERSION ==========

def main():
    print("🎵 WAV/MP3 to Binary Converter for SDC Streaming\n")

    input_file = input("🔹 Enter input audio file path (.wav or .mp3): ").strip()
    if not os.path.exists(input_file):
        print("❌ File not found!")
        return

    default_output = os.path.splitext(input_file)[0] + ".bin"
    output_file = input(f"🔹 Enter output binary filename [{default_output}]: ").strip()
    if output_file == "":
        output_file = default_output

    bits = input("🔹 Enter bit depth (8 or 12) [8]: ").strip()
    bits = int(bits) if bits else 8

    sample_rate = input("🔹 Enter sample rate (e.g., 8000 or 11025) [11025]: ").strip()
    sample_rate = int(sample_rate) if sample_rate else 11025

    convert_audio(input_file, output_file, bits=bits, sample_rate=sample_rate)

if __name__ == "__main__":
    main()
