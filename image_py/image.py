from PIL import Image

TARGET_W, TARGET_H = 160, 80

ZOOM = 1.6  # slight zoom out to show full logo

def convert_image_to_rgb565(image_path, output_header):
    img = Image.open(image_path).convert("RGB")

    src_w, src_h = img.size

    # Fit within display bounds preserving aspect ratio
    scale_w = TARGET_W / src_w
    scale_h = TARGET_H / src_h
    scale = min(scale_w, scale_h) * ZOOM

    new_w = int(src_w * scale)
    new_h = int(src_h * scale)
    img = img.resize((new_w, new_h), Image.LANCZOS)

    canvas = Image.new("RGB", (TARGET_W, TARGET_H), (255, 255, 255))
    offset_x = (TARGET_W - new_w) // 2
    offset_y = (TARGET_H - new_h) // 2
    canvas.paste(img, (offset_x, offset_y))
    img = canvas
    pixels = list(img.getdata())
    hex_pixels = []

    for r, g, b in pixels:
        r_5 = (r >> 3) & 0x1F
        g_6 = (g >> 2) & 0x3F
        b_5 = (b >> 3) & 0x1F
        rgb565 = (r_5 << 11) | (g_6 << 5) | b_5
        hex_pixels.append(f"0x{rgb565:04X}")

    with open(output_header, 'w') as f:
        f.write("#ifndef IMAGE_DATA_H\n#define IMAGE_DATA_H\n\n")
        f.write("#include <stdint.h>\n\n")
        f.write(f"const uint16_t logo_sysmic_{TARGET_W}x{TARGET_H}[] = {{\n")
        for i in range(0, len(hex_pixels), 8):
            f.write("    " + ", ".join(hex_pixels[i:i+8]) + ",\n")
        f.write("};\n\n#endif\n")

if __name__ == "__main__":
    convert_image_to_rgb565("images.jpeg", "image_data.h")
