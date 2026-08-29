from pathlib import Path
from zipfile import ZipFile, ZIP_DEFLATED
from PIL import Image
import argparse
import io
import re

IMAGE_EXTENSIONS = {
    ".jpg",
    ".jpeg",
    ".bmp",
    ".gif",
    ".tif",
    ".tiff",
    ".webp",
    ".png",
}

def sanitize_name(name: str) -> str:
    name = name.replace(" ", "-")
    name = re.sub(r"-+", "-", name)
    name = re.sub(r"[^A-Za-z0-9_\-]", "", name)
    return name


def archive_name(root: Path, file: Path) -> str:
    relative = file.relative_to(root)
    parts    = list(relative.parts)
    filename = parts[-1]
    stem     = sanitize_name(Path(filename).stem)
    suffix   = Path(filename).suffix.lower()

    if suffix in IMAGE_EXTENSIONS:
        suffix = ".png"

    parts = [
        sanitize_name(part)
        for part in parts[:-1]
    ]

    parts.append(stem + suffix)

    return ".".join(parts)


def convert_image_to_png(file: Path) -> bytes:
    with Image.open(file) as image:
        if image.mode not in ("RGB", "RGBA"):
            image = image.convert("RGBA")

        buffer = io.BytesIO()
        image.save(buffer, format="PNG")
        return buffer.getvalue()


def pack_resources(input_dir: Path, output_file: Path):
    output_file.parent.mkdir(parents=True, exist_ok=True)

    with ZipFile(output_file, "w", ZIP_DEFLATED) as archive:
        for file in input_dir.rglob("*"):
            if not file.is_file():
                continue

            name = archive_name(input_dir, file)

            if file.suffix.lower() in IMAGE_EXTENSIONS:
                data = convert_image_to_png(file)
                archive.writestr(name, data)
                print(f"[img] {file} -> {name}")
            else:
                archive.write(file, arcname=name)
                print(f"[raw] {file} -> {name}")


def main():
    parser = argparse.ArgumentParser(
        description="Pack resources into a flattened package."
    )

    parser.add_argument(
        "input",
        type=Path,
        help="Resource directory",
    )

    parser.add_argument(
        "output",
        type=Path,
        help="Output .pak file",
    )

    args = parser.parse_args()

    output = args.output
    if output.suffix != ".pak":
        output = output.with_suffix(".pak")

    pack_resources(args.input, output)

    print(f"Created: {output}")


if __name__ == "__main__":
    main()