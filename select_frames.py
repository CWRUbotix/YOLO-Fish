from pathlib import Path

OUT_DIR = Path('test') / 'wide_images'
for jpg in (Path('test') / 'images').iterdir():
    png_filename = jpg.name.split('_')[0] + '.png'
    png_path = Path('/home/mantis/Documents/cwrubotix/data/frames') / png_filename
    OUT_DIR.mkdir(exist_ok=True)
    (OUT_DIR / jpg.name).write_bytes(png_path.read_bytes())