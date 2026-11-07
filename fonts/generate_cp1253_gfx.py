#!/usr/bin/env python3
"""Generate Adafruit-GFX FreeSans fonts using a CP1253 byte layout.

The display library accepts one byte per glyph.  Greek locale text is stored as
UTF-8 and converted to Windows-1253 at runtime, while these generated fonts put
the corresponding Unicode glyph in each Windows-1253 byte slot.

Requires Pillow:
    python -m pip install Pillow
"""

from __future__ import annotations

import argparse
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


SIZES = (4, 5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26)
DPI = 141


def byte_character(value: int) -> str:
    if value < 0x80:
        return chr(value)
    try:
        return bytes((value,)).decode("cp1253")
    except UnicodeDecodeError:
        return " "


def pack_bitmap(image: Image.Image) -> list[int]:
    packed: list[int] = []
    current = 0
    bits = 0
    pixels = image.load()
    for y in range(image.height):
        for x in range(image.width):
            current = (current << 1) | (1 if pixels[x, y] else 0)
            bits += 1
            if bits == 8:
                packed.append(current)
                current = 0
                bits = 0
    if bits:
        packed.append(current << (8 - bits))
    return packed


def generate_font(font_path: Path, output_dir: Path, point_size: int) -> None:
    pixel_size = max(1, round(point_size * DPI / 72))
    font = ImageFont.truetype(str(font_path), pixel_size)
    ascent, descent = font.getmetrics()
    y_advance = ascent + descent

    bitmaps: list[int] = []
    glyphs: list[tuple[int, int, int, int, int, int, int]] = []

    for value in range(0x20, 0x100):
        character = byte_character(value)
        left, top, right, bottom = font.getbbox(character, anchor="ls")
        width = max(0, right - left)
        height = max(0, bottom - top)
        advance = max(0, round(font.getlength(character)))
        offset = len(bitmaps)

        if width and height and character != " ":
            image = Image.new("1", (width, height), 0)
            draw = ImageDraw.Draw(image)
            draw.text((-left, -top), character, font=font, fill=1, anchor="ls")
            bitmaps.extend(pack_bitmap(image))

        glyphs.append((offset, width, height, advance, left, top, value))

    name = f"FreeSansGreek_{point_size}pt8b"
    output_path = output_dir / f"{name}.h"
    lines = [f"const uint8_t {name}Bitmaps[] PROGMEM = {{"]
    for index in range(0, len(bitmaps), 12):
        chunk = ", ".join(f"0x{value:02X}" for value in bitmaps[index:index + 12])
        comma = "," if index + 12 < len(bitmaps) else ""
        lines.append(f"  {chunk}{comma}")
    lines.append("};")
    lines.append("")
    lines.append(f"const GFXglyph {name}Glyphs[] PROGMEM = {{")
    for index, (offset, width, height, advance, x_offset, y_offset, value) in enumerate(glyphs):
        comma = "," if index + 1 < len(glyphs) else ""
        lines.append(
            "  { %5d, %3d, %3d, %3d, %4d, %4d }%s // 0x%02X"
            % (offset, width, height, advance, x_offset, y_offset, comma, value)
        )
    lines.append("};")
    lines.append("")
    lines.append(f"const GFXfont {name} PROGMEM = {{")
    lines.append(f"  (uint8_t  *){name}Bitmaps,")
    lines.append(f"  (GFXglyph *){name}Glyphs,")
    lines.append(f"  0x20, 0xFF, {y_advance} }};")
    lines.append("")
    approx = len(bitmaps) + len(glyphs) * 7 + 7
    lines.append(f"// Approx. {approx} bytes")
    output_path.write_text("\n".join(lines) + "\n", encoding="ascii")


def generate_aggregate(output_root: Path) -> None:
    lines = [
        "#ifndef __FONTS_FREESANS_GREEK_H__",
        "#define __FONTS_FREESANS_GREEK_H__",
    ]
    for size in SIZES:
        lines.append(f'#include "FreeSansGreek/FreeSansGreek_{size}pt8b.h"')
    lines.append('#include "FreeSans/FreeSans_48pt8b_temperature.h"')
    lines.append("")
    for size in SIZES:
        lines.append(f"#define FONT_{size}pt8b FreeSansGreek_{size}pt8b")
    lines.append("#define FONT_48pt8b_temperature FreeSans_48pt8b_temperature")
    lines.append("#endif")
    lines.append("")
    (output_root / "FreeSansGreek.h").write_text("\n".join(lines), encoding="ascii")


def main() -> None:
    repository = Path(__file__).resolve().parent.parent
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--font",
        type=Path,
        default=repository / "fonts" / "ttf" / "FreeSans.otf",
    )
    parser.add_argument(
        "--output",
        type=Path,
        default=repository / "platformio" / "lib" / "esp32-weather-epd-assets" / "fonts",
    )
    args = parser.parse_args()

    output_dir = args.output / "FreeSansGreek"
    output_dir.mkdir(parents=True, exist_ok=True)
    for size in SIZES:
        generate_font(args.font, output_dir, size)
    generate_aggregate(args.output)


if __name__ == "__main__":
    main()
