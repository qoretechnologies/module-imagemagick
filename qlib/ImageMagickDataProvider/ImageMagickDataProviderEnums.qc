# -*- mode: qore; indent-tabs-mode: nil -*-

/*  Copyright 2026 Qore Technologies, s.r.o.

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#! @file ImageMagickDataProviderEnums.qc Enums for ImageMagick data providers

public namespace ImageMagickDataProvider {
    #! Filter type allowed values for resize operations
    public const FilterTypeAllowedValues = (
        <AllowedValueInfo>{
            "value": "Lanczos",
            "display_name": "Lanczos",
            "desc": "High quality, recommended for most uses",
        },
        <AllowedValueInfo>{
            "value": "Mitchell",
            "display_name": "Mitchell",
            "desc": "Balanced sharpness and ringing",
        },
        <AllowedValueInfo>{
            "value": "Catrom",
            "display_name": "Catmull-Rom",
            "desc": "Good for image reduction",
        },
        <AllowedValueInfo>{
            "value": "Gaussian",
            "display_name": "Gaussian",
            "desc": "Gaussian blur filter",
        },
        <AllowedValueInfo>{
            "value": "Box",
            "display_name": "Box",
            "desc": "Nearest neighbor for upscale",
        },
        <AllowedValueInfo>{
            "value": "Triangle",
            "display_name": "Triangle",
            "desc": "Bilinear interpolation",
        },
        <AllowedValueInfo>{
            "value": "Point",
            "display_name": "Point",
            "desc": "Nearest neighbor (fastest, no interpolation)",
        },
    );

    #! Image format allowed values
    public const ImageFormatAllowedValues = (
        # Common web/standard formats
        <AllowedValueInfo>{
            "value": "PNG",
            "display_name": "PNG",
            "desc": "Portable Network Graphics (lossless)",
        },
        <AllowedValueInfo>{
            "value": "JPEG",
            "display_name": "JPEG",
            "desc": "JPEG (lossy, good for photographs)",
        },
        <AllowedValueInfo>{
            "value": "GIF",
            "display_name": "GIF",
            "desc": "Graphics Interchange Format (limited colors, supports animation)",
        },
        <AllowedValueInfo>{
            "value": "BMP",
            "display_name": "BMP",
            "desc": "Windows Bitmap (uncompressed)",
        },
        <AllowedValueInfo>{
            "value": "TIFF",
            "display_name": "TIFF",
            "desc": "Tagged Image File Format (high quality, large files)",
        },
        <AllowedValueInfo>{
            "value": "WEBP",
            "display_name": "WebP",
            "desc": "Google WebP format (good compression)",
        },
        # Modern web formats
        <AllowedValueInfo>{
            "value": "AVIF",
            "display_name": "AVIF",
            "desc": "AV1 Image File Format (next-gen web, excellent compression)",
        },
        <AllowedValueInfo>{
            "value": "HEIC",
            "display_name": "HEIC",
            "desc": "High Efficiency Image Container (Apple/MPEG, excellent compression)",
        },
        <AllowedValueInfo>{
            "value": "HEIF",
            "display_name": "HEIF",
            "desc": "High Efficiency Image Format (MPEG standard)",
        },
        <AllowedValueInfo>{
            "value": "JXL",
            "display_name": "JPEG XL",
            "desc": "JPEG XL (next-gen, lossless and lossy, excellent compression)",
        },
        <AllowedValueInfo>{
            "value": "APNG",
            "display_name": "APNG",
            "desc": "Animated Portable Network Graphics",
        },
        # Vector and document formats
        <AllowedValueInfo>{
            "value": "SVG",
            "display_name": "SVG",
            "desc": "Scalable Vector Graphics (vector format)",
        },
        <AllowedValueInfo>{
            "value": "PDF",
            "display_name": "PDF",
            "desc": "Portable Document Format",
        },
        <AllowedValueInfo>{
            "value": "EPS",
            "display_name": "EPS",
            "desc": "Encapsulated PostScript",
        },
        <AllowedValueInfo>{
            "value": "PS",
            "display_name": "PostScript",
            "desc": "Adobe PostScript",
        },
        # Professional / editor formats
        <AllowedValueInfo>{
            "value": "PSD",
            "display_name": "PSD",
            "desc": "Adobe Photoshop Document",
        },
        <AllowedValueInfo>{
            "value": "XCF",
            "display_name": "XCF",
            "desc": "GIMP native format",
        },
        <AllowedValueInfo>{
            "value": "EXR",
            "display_name": "OpenEXR",
            "desc": "OpenEXR high dynamic range format",
        },
        <AllowedValueInfo>{
            "value": "DPX",
            "display_name": "DPX",
            "desc": "Digital Picture Exchange (motion picture industry)",
        },
        <AllowedValueInfo>{
            "value": "HDR",
            "display_name": "HDR",
            "desc": "Radiance HDR high dynamic range format",
        },
        # Icons and cursors
        <AllowedValueInfo>{
            "value": "ICO",
            "display_name": "ICO",
            "desc": "Windows Icon format",
        },
        <AllowedValueInfo>{
            "value": "CUR",
            "display_name": "CUR",
            "desc": "Windows Cursor format",
        },
        # JPEG variants
        <AllowedValueInfo>{
            "value": "JP2",
            "display_name": "JPEG 2000",
            "desc": "JPEG 2000 (wavelet-based compression)",
        },
        # Legacy and niche formats
        <AllowedValueInfo>{
            "value": "TGA",
            "display_name": "TGA",
            "desc": "Truevision TGA (Targa)",
        },
        <AllowedValueInfo>{
            "value": "PCX",
            "display_name": "PCX",
            "desc": "PC Paintbrush Exchange",
        },
        <AllowedValueInfo>{
            "value": "WBMP",
            "display_name": "WBMP",
            "desc": "Wireless Application Protocol Bitmap",
        },
        <AllowedValueInfo>{
            "value": "PICT",
            "display_name": "PICT",
            "desc": "Apple Macintosh QuickDraw/PICT",
        },
        # Raw camera formats
        <AllowedValueInfo>{
            "value": "DNG",
            "display_name": "DNG",
            "desc": "Adobe Digital Negative (universal raw)",
        },
        <AllowedValueInfo>{
            "value": "CR2",
            "display_name": "CR2",
            "desc": "Canon Raw 2",
        },
        <AllowedValueInfo>{
            "value": "NEF",
            "display_name": "NEF",
            "desc": "Nikon Electronic Format (raw)",
        },
        <AllowedValueInfo>{
            "value": "ARW",
            "display_name": "ARW",
            "desc": "Sony Alpha Raw",
        },
        <AllowedValueInfo>{
            "value": "ORF",
            "display_name": "ORF",
            "desc": "Olympus Raw Format",
        },
        <AllowedValueInfo>{
            "value": "RW2",
            "display_name": "RW2",
            "desc": "Panasonic Raw",
        },
        <AllowedValueInfo>{
            "value": "RAF",
            "display_name": "RAF",
            "desc": "Fujifilm Raw",
        },
        <AllowedValueInfo>{
            "value": "PEF",
            "display_name": "PEF",
            "desc": "Pentax Electronic Format (raw)",
        },
        # Scientific and specialized
        <AllowedValueInfo>{
            "value": "FITS",
            "display_name": "FITS",
            "desc": "Flexible Image Transport System (astronomy)",
        },
        <AllowedValueInfo>{
            "value": "DDS",
            "display_name": "DDS",
            "desc": "DirectDraw Surface (3D/game textures)",
        },
        # Portable bitmap family
        <AllowedValueInfo>{
            "value": "PNM",
            "display_name": "PNM",
            "desc": "Portable Any Map (PBM/PGM/PPM family)",
        },
        <AllowedValueInfo>{
            "value": "PBM",
            "display_name": "PBM",
            "desc": "Portable Bitmap (monochrome)",
        },
        <AllowedValueInfo>{
            "value": "PGM",
            "display_name": "PGM",
            "desc": "Portable Graymap (grayscale)",
        },
        <AllowedValueInfo>{
            "value": "PPM",
            "display_name": "PPM",
            "desc": "Portable Pixmap (color)",
        },
        <AllowedValueInfo>{
            "value": "PAM",
            "display_name": "PAM",
            "desc": "Portable Arbitrary Map (generalized PNM)",
        },
        # Raw pixel data formats
        <AllowedValueInfo>{
            "value": "RGB",
            "display_name": "RGB",
            "desc": "Raw RGB pixel data",
        },
        <AllowedValueInfo>{
            "value": "RGBA",
            "display_name": "RGBA",
            "desc": "Raw RGBA pixel data (with alpha)",
        },
        <AllowedValueInfo>{
            "value": "CMYK",
            "display_name": "CMYK",
            "desc": "Raw CMYK pixel data (print)",
        },
        <AllowedValueInfo>{
            "value": "GRAY",
            "display_name": "GRAY",
            "desc": "Raw grayscale pixel data",
        },
        <AllowedValueInfo>{
            "value": "YUV",
            "display_name": "YUV",
            "desc": "Raw YUV pixel data (video)",
        },
        # Other notable formats
        <AllowedValueInfo>{
            "value": "JBIG",
            "display_name": "JBIG",
            "desc": "Joint Bi-level Image Experts Group (fax/document compression)",
        },
        <AllowedValueInfo>{
            "value": "JNG",
            "display_name": "JNG",
            "desc": "JPEG Network Graphics",
        },
        <AllowedValueInfo>{
            "value": "MNG",
            "display_name": "MNG",
            "desc": "Multiple-image Network Graphics (animated)",
        },
        <AllowedValueInfo>{
            "value": "DJVU",
            "display_name": "DjVu",
            "desc": "DjVu (scanned documents, high compression)",
        },
        <AllowedValueInfo>{
            "value": "SGI",
            "display_name": "SGI",
            "desc": "Silicon Graphics IRIS image",
        },
    );

    #! Flip direction allowed values
    public const FlipDirectionAllowedValues = (
        <AllowedValueInfo>{
            "value": "vertical",
            "display_name": "Vertical",
            "desc": "Flip top to bottom",
        },
        <AllowedValueInfo>{
            "value": "horizontal",
            "display_name": "Horizontal",
            "desc": "Flip left to right (mirror)",
        },
    );

    #! Gravity type allowed values for composite operations
    public const GravityTypeAllowedValues = (
        <AllowedValueInfo>{
            "value": "NorthWest",
            "display_name": "North West",
            "desc": "Top-left corner",
        },
        <AllowedValueInfo>{
            "value": "North",
            "display_name": "North",
            "desc": "Top center",
        },
        <AllowedValueInfo>{
            "value": "NorthEast",
            "display_name": "North East",
            "desc": "Top-right corner",
        },
        <AllowedValueInfo>{
            "value": "West",
            "display_name": "West",
            "desc": "Center left",
        },
        <AllowedValueInfo>{
            "value": "Center",
            "display_name": "Center",
            "desc": "Center of the image",
        },
        <AllowedValueInfo>{
            "value": "East",
            "display_name": "East",
            "desc": "Center right",
        },
        <AllowedValueInfo>{
            "value": "SouthWest",
            "display_name": "South West",
            "desc": "Bottom-left corner",
        },
        <AllowedValueInfo>{
            "value": "South",
            "display_name": "South",
            "desc": "Bottom center",
        },
        <AllowedValueInfo>{
            "value": "SouthEast",
            "display_name": "South East",
            "desc": "Bottom-right corner",
        },
    );

    #! Noise type allowed values for add-noise operation
    public const NoiseTypeAllowedValues = (
        <AllowedValueInfo>{
            "value": "Uniform",
            "display_name": "Uniform",
            "desc": "Uniform noise distribution",
        },
        <AllowedValueInfo>{
            "value": "Gaussian",
            "display_name": "Gaussian",
            "desc": "Gaussian (normal) noise distribution",
        },
        <AllowedValueInfo>{
            "value": "MultiplicativeGaussian",
            "display_name": "Multiplicative Gaussian",
            "desc": "Multiplicative Gaussian noise",
        },
        <AllowedValueInfo>{
            "value": "Impulse",
            "display_name": "Impulse",
            "desc": "Impulse (salt and pepper) noise",
        },
        <AllowedValueInfo>{
            "value": "Laplacian",
            "display_name": "Laplacian",
            "desc": "Laplacian noise distribution",
        },
        <AllowedValueInfo>{
            "value": "Poisson",
            "display_name": "Poisson",
            "desc": "Poisson noise distribution",
        },
        <AllowedValueInfo>{
            "value": "Random",
            "display_name": "Random",
            "desc": "Random noise distribution",
        },
    );

    #! Colorspace type allowed values
    public const ColorspaceTypeAllowedValues = (
        <AllowedValueInfo>{
            "value": "sRGB",
            "display_name": "sRGB",
            "desc": "Standard RGB colorspace",
        },
        <AllowedValueInfo>{
            "value": "RGB",
            "display_name": "RGB",
            "desc": "Linear RGB colorspace",
        },
        <AllowedValueInfo>{
            "value": "CMYK",
            "display_name": "CMYK",
            "desc": "Cyan, Magenta, Yellow, Key (Black) colorspace",
        },
        <AllowedValueInfo>{
            "value": "Gray",
            "display_name": "Grayscale",
            "desc": "Grayscale colorspace",
        },
        <AllowedValueInfo>{
            "value": "HSL",
            "display_name": "HSL",
            "desc": "Hue, Saturation, Lightness colorspace",
        },
        <AllowedValueInfo>{
            "value": "Lab",
            "display_name": "Lab",
            "desc": "CIE Lab colorspace",
        },
    );

    #! Composite operator allowed values
    public const CompositeOpAllowedValues = (
        <AllowedValueInfo>{
            "value": "Over",
            "display_name": "Over",
            "desc": "Standard overlay (respects alpha)",
        },
        <AllowedValueInfo>{
            "value": "Multiply",
            "display_name": "Multiply",
            "desc": "Multiply blend (darkening effect)",
        },
        <AllowedValueInfo>{
            "value": "Screen",
            "display_name": "Screen",
            "desc": "Screen blend (lightening effect)",
        },
        <AllowedValueInfo>{
            "value": "Overlay",
            "display_name": "Overlay",
            "desc": "Overlay blend (contrast enhancement)",
        },
        <AllowedValueInfo>{
            "value": "Add",
            "display_name": "Add",
            "desc": "Additive blend",
        },
        <AllowedValueInfo>{
            "value": "Difference",
            "display_name": "Difference",
            "desc": "Absolute difference",
        },
        <AllowedValueInfo>{
            "value": "Dissolve",
            "display_name": "Dissolve",
            "desc": "Dissolve blend",
        },
    );

    #! Allowed values for image bit depth
    public const DepthAllowedValues = (
        <AllowedValueInfo>{
            "value": 8,
            "display_name": "8-bit",
            "desc": "8 bits per channel (standard)",
        },
        <AllowedValueInfo>{
            "value": 16,
            "display_name": "16-bit",
            "desc": "16 bits per channel (high dynamic range)",
        },
        <AllowedValueInfo>{
            "value": 32,
            "display_name": "32-bit",
            "desc": "32 bits per channel (floating point)",
        },
    );

    #! Allowed values for drawing operation types
    public const DrawingOperationTypeAllowedValues = (
        <AllowedValueInfo>{
            "value": "line",
            "display_name": "Line",
            "desc": "Draw a line from (sx, sy) to (ex, ey)",
        },
        <AllowedValueInfo>{
            "value": "rectangle",
            "display_name": "Rectangle",
            "desc": "Draw a rectangle from (x1, y1) to (x2, y2)",
        },
        <AllowedValueInfo>{
            "value": "round-rectangle",
            "display_name": "Rounded Rectangle",
            "desc": "Draw a rounded rectangle from (x1, y1) to (x2, y2) with corner radii (rx, ry)",
        },
        <AllowedValueInfo>{
            "value": "circle",
            "display_name": "Circle",
            "desc": "Draw a circle with center (ox, oy) and a point on circumference (px, py)",
        },
        <AllowedValueInfo>{
            "value": "ellipse",
            "display_name": "Ellipse",
            "desc": "Draw an ellipse with center (ox, oy), radii (rx, ry), and arc range (start, end)",
        },
        <AllowedValueInfo>{
            "value": "arc",
            "display_name": "Arc",
            "desc": "Draw an arc from (sx, sy) to (ex, ey) with angle range (sd, ed)",
        },
        <AllowedValueInfo>{
            "value": "point",
            "display_name": "Point",
            "desc": "Draw a single point at (x, y)",
        },
        <AllowedValueInfo>{
            "value": "polygon",
            "display_name": "Polygon",
            "desc": "Draw a closed polygon from a list of points (minimum 3)",
        },
        <AllowedValueInfo>{
            "value": "polyline",
            "display_name": "Polyline",
            "desc": "Draw connected line segments from a list of points (minimum 2)",
        },
        <AllowedValueInfo>{
            "value": "text",
            "display_name": "Text",
            "desc": "Draw text at position (x, y)",
        },
    );

    #! Allowed values for font style
    public const FontStyleAllowedValues = (
        <AllowedValueInfo>{
            "value": "normal",
            "display_name": "Normal",
            "desc": "Normal (upright) font style",
        },
        <AllowedValueInfo>{
            "value": "italic",
            "display_name": "Italic",
            "desc": "Italic font style",
        },
        <AllowedValueInfo>{
            "value": "oblique",
            "display_name": "Oblique",
            "desc": "Oblique (slanted) font style",
        },
    );

    #! Allowed values for profile names
    public const ProfileNameAllowedValues = (
        <AllowedValueInfo>{
            "value": "icc",
            "display_name": "ICC",
            "desc": "ICC color management profile",
        },
        <AllowedValueInfo>{
            "value": "exif",
            "display_name": "EXIF",
            "desc": "Exchangeable Image File Format metadata",
        },
        <AllowedValueInfo>{
            "value": "iptc",
            "display_name": "IPTC",
            "desc": "International Press Telecommunications Council metadata",
        },
        <AllowedValueInfo>{
            "value": "xmp",
            "display_name": "XMP",
            "desc": "Extensible Metadata Platform (Adobe) metadata",
        },
    );
}
