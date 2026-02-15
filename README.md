# Qore imagemagick Module

## Introduction

The `imagemagick` module provides comprehensive image processing and manipulation capabilities
for Qore by wrapping the ImageMagick MagickWand C API, including:

- Loading and saving images in 200+ formats (PNG, JPEG, GIF, TIFF, WebP, etc.)
- Resize, crop, rotate, flip, and other geometric transformations
- Color adjustment (brightness, contrast, gamma, levels, modulate)
- Effects (blur, sharpen, charcoal, sepia, oil paint, edge detection, emboss)
- Vector drawing (shapes, text, paths) via MagickDrawing
- Image compositing with multiple blend modes
- Metadata access (EXIF, ICC profiles, properties)
- Format conversion with quality control
- Data provider module for integration with Qore's data provider framework

## Requirements

- Qore 2.0+
- CMake 3.5+
- C++11 compiler
- ImageMagick 7.0+ with MagickWand development libraries

## Building

```bash
mkdir build
cd build
cmake ..
make
make install
```

## Quick Start

```qore
#!/usr/bin/qore

%modern
%requires imagemagick

# Load and resize an image
MagickImage img("photo.jpg");
img.resize(800, 600, "Lanczos");
img.writeFile("photo_resized.jpg");

# Format conversion
MagickImage img2("document.png");
binary jpeg_data = img2.toData("JPEG");

# Apply effects
MagickImage img3("photo.jpg");
img3.charcoal(1.0, 0.5);
img3.writeFile("charcoal.jpg");

# Draw shapes and text
MagickDrawing dw();
dw.setFillColor("blue");
dw.rectangle(10.0, 10.0, 100.0, 100.0);
dw.setFontSize(24.0);
dw.annotation(50.0, 150.0, "Hello!");

MagickImage canvas(300, 300, "white");
canvas.draw(dw);
canvas.writeFile("drawing.png");

# Get image info
MagickImage img4("photo.jpg");
hash<ImageInfo> info = img4.getInfo();
printf("Size: %dx%d, Format: %s\n", info.width, info.height, info.format);
```

## Data Provider

The module includes `ImageMagickDataProvider` for use with Qore's data provider framework:

```qore
%modern
%requires ImageMagickDataProvider

# Resize an image
AbstractDataProvider dp = DataProvider::getFactoryObjectFromStringEx("imagemagick{}/image/resize");
hash<auto> result = dp.doRequest({
    "input_path": "photo.jpg",
    "width": 800,
    "height": 600,
});

# Get image info
AbstractDataProvider info = DataProvider::getFactoryObjectFromStringEx("imagemagick{}/info/get");
hash<auto> metadata = info.doRequest({
    "input_path": "photo.jpg",
});
printf("Format: %s, Size: %dx%d\n", metadata.format, metadata.width, metadata.height);
```

## License

MIT License - see [LICENSE](LICENSE) for details.

## Copyright

Copyright 2026 Qore Technologies, s.r.o.
