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
}
