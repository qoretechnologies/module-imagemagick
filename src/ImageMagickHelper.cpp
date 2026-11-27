/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file ImageMagickHelper.cpp ImageMagick helper utilities implementation */
/*
    Qore imagemagick module

    Copyright (C) 2026 Qore Technologies, s.r.o.

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

#include "ImageMagickHelper.h"

#include <cstring>
#include <strings.h>
#include <sys/stat.h>
#include <cctype>

bool checkMagickError(MagickWand* wand, const char* context, ExceptionSink* xsink) {
    ExceptionType severity;
    char* description = MagickGetException(wand, &severity);
    if (severity != UndefinedException) {
        QoreStringNode* err = new QoreStringNode("IMAGEMAGICK-ERROR");
        QoreStringNode* desc = new QoreStringNodeMaker("%s: %s", context, description);
        MagickRelinquishMemory(description);
        MagickClearException(wand);
        xsink->raiseException(err, desc);
        return true;
    }
    if (description) {
        MagickRelinquishMemory(description);
    }
    return false;
}

bool checkDrawingError(DrawingWand* wand, const char* context, ExceptionSink* xsink) {
    ExceptionType severity;
    char* description = DrawGetException(wand, &severity);
    if (severity != UndefinedException) {
        QoreStringNode* err = new QoreStringNode("IMAGEMAGICK-ERROR");
        QoreStringNode* desc = new QoreStringNodeMaker("%s: %s", context, description);
        MagickRelinquishMemory(description);
        ClearDrawingWand(wand);
        xsink->raiseException(err, desc);
        return true;
    }
    if (description) {
        MagickRelinquishMemory(description);
    }
    return false;
}

struct StringEnumEntry {
    const char* name;
    int value;
};

static int lookupEnum(const StringEnumEntry* table, size_t count, const char* str,
                      const char* type_name, ExceptionSink* xsink) {
    for (size_t i = 0; i < count; ++i) {
        if (!strcasecmp(table[i].name, str)) {
            return table[i].value;
        }
    }
    QoreStringNode* desc = new QoreStringNodeMaker("invalid %s '%s'; valid values: ", type_name, str);
    for (size_t i = 0; i < count; ++i) {
        if (i > 0) {
            desc->concat(", ");
        }
        desc->concat(table[i].name);
    }
    xsink->raiseException("IMAGEMAGICK-ERROR", desc);
    return -1;
}

static const char* reverseLookupEnum(const StringEnumEntry* table, size_t count, int value,
                                     const char* default_str = "Unknown") {
    for (size_t i = 0; i < count; ++i) {
        if (table[i].value == value) {
            return table[i].name;
        }
    }
    return default_str;
}

// Filter types
static const StringEnumEntry filter_types[] = {
    {"Undefined", UndefinedFilter},
    {"Point", PointFilter},
    {"Box", BoxFilter},
    {"Triangle", TriangleFilter},
    {"Hermite", HermiteFilter},
    {"Hann", HannFilter},
    {"Hamming", HammingFilter},
    {"Blackman", BlackmanFilter},
    {"Gaussian", GaussianFilter},
    {"Quadratic", QuadraticFilter},
    {"Cubic", CubicFilter},
    {"Catrom", CatromFilter},
    {"Mitchell", MitchellFilter},
    {"Jinc", JincFilter},
    {"Sinc", SincFilter},
    {"SincFast", SincFastFilter},
    {"Kaiser", KaiserFilter},
    {"Welch", WelchFilter},
    {"Parzen", ParzenFilter},
    {"Bohman", BohmanFilter},
    {"Bartlett", BartlettFilter},
    {"Lagrange", LagrangeFilter},
    {"Lanczos", LanczosFilter},
    {"LanczosSharp", LanczosSharpFilter},
    {"Lanczos2", Lanczos2Filter},
    {"Lanczos2Sharp", Lanczos2SharpFilter},
    {"Robidoux", RobidouxFilter},
    {"RobidouxSharp", RobidouxSharpFilter},
    {"Cosine", CosineFilter},
    {"Spline", SplineFilter},
    {"LanczosRadius", LanczosRadiusFilter},
    {"CubicSpline", CubicSplineFilter},
};
static const size_t filter_types_count = sizeof(filter_types) / sizeof(filter_types[0]);

FilterType stringToFilterType(const char* str, ExceptionSink* xsink) {
    int rv = lookupEnum(filter_types, filter_types_count, str, "filter type", xsink);
    return rv < 0 ? UndefinedFilter : static_cast<FilterType>(rv);
}

const char* filterTypeToString(FilterType ft) {
    return reverseLookupEnum(filter_types, filter_types_count, ft, "Undefined");
}

// Gravity types
static const StringEnumEntry gravity_types[] = {
    {"Undefined", UndefinedGravity},
    {"Forget", ForgetGravity},
    {"NorthWest", NorthWestGravity},
    {"North", NorthGravity},
    {"NorthEast", NorthEastGravity},
    {"West", WestGravity},
    {"Center", CenterGravity},
    {"East", EastGravity},
    {"SouthWest", SouthWestGravity},
    {"South", SouthGravity},
    {"SouthEast", SouthEastGravity},
};
static const size_t gravity_types_count = sizeof(gravity_types) / sizeof(gravity_types[0]);

GravityType stringToGravityType(const char* str, ExceptionSink* xsink) {
    int rv = lookupEnum(gravity_types, gravity_types_count, str, "gravity type", xsink);
    return rv < 0 ? UndefinedGravity : static_cast<GravityType>(rv);
}

const char* gravityTypeToString(GravityType gt) {
    return reverseLookupEnum(gravity_types, gravity_types_count, gt, "Undefined");
}

// Colorspace types
static const StringEnumEntry colorspace_types[] = {
    {"Undefined", UndefinedColorspace},
    {"CMY", CMYColorspace},
    {"CMYK", CMYKColorspace},
    {"Gray", GRAYColorspace},
    {"HCL", HCLColorspace},
    {"HCLp", HCLpColorspace},
    {"HSB", HSBColorspace},
    {"HSI", HSIColorspace},
    {"HSL", HSLColorspace},
    {"HSV", HSVColorspace},
    {"HWB", HWBColorspace},
    {"Lab", LabColorspace},
    {"LCH", LCHColorspace},
    {"LCHab", LCHabColorspace},
    {"LCHuv", LCHuvColorspace},
    {"Log", LogColorspace},
    {"LMS", LMSColorspace},
    {"Luv", LuvColorspace},
    {"OHTA", OHTAColorspace},
    {"Rec601YCbCr", Rec601YCbCrColorspace},
    {"Rec709YCbCr", Rec709YCbCrColorspace},
    {"RGB", RGBColorspace},
    {"scRGB", scRGBColorspace},
    {"sRGB", sRGBColorspace},
    {"Transparent", TransparentColorspace},
    {"xyY", xyYColorspace},
    {"XYZ", XYZColorspace},
    {"YCbCr", YCbCrColorspace},
    {"YCC", YCCColorspace},
    {"YDbDr", YDbDrColorspace},
    {"YIQ", YIQColorspace},
    {"YPbPr", YPbPrColorspace},
    {"YUV", YUVColorspace},
};
static const size_t colorspace_types_count = sizeof(colorspace_types) / sizeof(colorspace_types[0]);

ColorspaceType stringToColorspaceType(const char* str, ExceptionSink* xsink) {
    int rv = lookupEnum(colorspace_types, colorspace_types_count, str, "colorspace", xsink);
    return rv < 0 ? UndefinedColorspace : static_cast<ColorspaceType>(rv);
}

const char* colorspaceTypeToString(ColorspaceType cs) {
    return reverseLookupEnum(colorspace_types, colorspace_types_count, cs, "Undefined");
}

// Composite operators
static const StringEnumEntry composite_ops[] = {
    {"Undefined", UndefinedCompositeOp},
    {"Alpha", AlphaCompositeOp},
    {"Atop", AtopCompositeOp},
    {"Blend", BlendCompositeOp},
    {"Blur", BlurCompositeOp},
    {"Bumpmap", BumpmapCompositeOp},
    {"ChangeMask", ChangeMaskCompositeOp},
    {"Clear", ClearCompositeOp},
    {"ColorBurn", ColorBurnCompositeOp},
    {"ColorDodge", ColorDodgeCompositeOp},
    {"Colorize", ColorizeCompositeOp},
    {"CopyBlack", CopyBlackCompositeOp},
    {"CopyBlue", CopyBlueCompositeOp},
    {"Copy", CopyCompositeOp},
    {"CopyCyan", CopyCyanCompositeOp},
    {"CopyGreen", CopyGreenCompositeOp},
    {"CopyMagenta", CopyMagentaCompositeOp},
    {"CopyAlpha", CopyAlphaCompositeOp},
    {"CopyRed", CopyRedCompositeOp},
    {"CopyYellow", CopyYellowCompositeOp},
    {"Darken", DarkenCompositeOp},
    {"DarkenIntensity", DarkenIntensityCompositeOp},
    {"Difference", DifferenceCompositeOp},
    {"Displace", DisplaceCompositeOp},
    {"Dissolve", DissolveCompositeOp},
    {"Distort", DistortCompositeOp},
    {"DivideDst", DivideDstCompositeOp},
    {"DivideSrc", DivideSrcCompositeOp},
    {"DstAtop", DstAtopCompositeOp},
    {"Dst", DstCompositeOp},
    {"DstIn", DstInCompositeOp},
    {"DstOut", DstOutCompositeOp},
    {"DstOver", DstOverCompositeOp},
    {"Exclusion", ExclusionCompositeOp},
    {"HardLight", HardLightCompositeOp},
    {"HardMix", HardMixCompositeOp},
    {"Hue", HueCompositeOp},
    {"In", InCompositeOp},
    {"Intensity", IntensityCompositeOp},
    {"Lighten", LightenCompositeOp},
    {"LightenIntensity", LightenIntensityCompositeOp},
    {"LinearBurn", LinearBurnCompositeOp},
    {"LinearDodge", LinearDodgeCompositeOp},
    {"LinearLight", LinearLightCompositeOp},
    {"Luminize", LuminizeCompositeOp},
    {"Mathematics", MathematicsCompositeOp},
    {"MinusDst", MinusDstCompositeOp},
    {"MinusSrc", MinusSrcCompositeOp},
    {"Modulate", ModulateCompositeOp},
    {"ModulusAdd", ModulusAddCompositeOp},
    {"ModulusSubtract", ModulusSubtractCompositeOp},
    {"Multiply", MultiplyCompositeOp},
    {"No", NoCompositeOp},
    {"Out", OutCompositeOp},
    {"Over", OverCompositeOp},
    {"Overlay", OverlayCompositeOp},
    {"PegtopLight", PegtopLightCompositeOp},
    {"PinLight", PinLightCompositeOp},
    {"Plus", PlusCompositeOp},
    {"Replace", ReplaceCompositeOp},
    {"Saturate", SaturateCompositeOp},
    {"Screen", ScreenCompositeOp},
    {"SoftLight", SoftLightCompositeOp},
    {"SrcAtop", SrcAtopCompositeOp},
    {"Src", SrcCompositeOp},
    {"SrcIn", SrcInCompositeOp},
    {"SrcOut", SrcOutCompositeOp},
    {"SrcOver", SrcOverCompositeOp},
    {"Threshold", ThresholdCompositeOp},
    {"VividLight", VividLightCompositeOp},
    {"Xor", XorCompositeOp},
};
static const size_t composite_ops_count = sizeof(composite_ops) / sizeof(composite_ops[0]);

CompositeOperator stringToCompositeOp(const char* str, ExceptionSink* xsink) {
    int rv = lookupEnum(composite_ops, composite_ops_count, str, "composite operator", xsink);
    return rv < 0 ? UndefinedCompositeOp : static_cast<CompositeOperator>(rv);
}

const char* compositeOpToString(CompositeOperator op) {
    return reverseLookupEnum(composite_ops, composite_ops_count, op, "Undefined");
}

// Noise types
static const StringEnumEntry noise_types[] = {
    {"Undefined", UndefinedNoise},
    {"Uniform", UniformNoise},
    {"Gaussian", GaussianNoise},
    {"MultiplicativeGaussian", MultiplicativeGaussianNoise},
    {"Impulse", ImpulseNoise},
    {"Laplacian", LaplacianNoise},
    {"Poisson", PoissonNoise},
    {"Random", RandomNoise},
};
static const size_t noise_types_count = sizeof(noise_types) / sizeof(noise_types[0]);

NoiseType stringToNoiseType(const char* str, ExceptionSink* xsink) {
    int rv = lookupEnum(noise_types, noise_types_count, str, "noise type", xsink);
    return rv < 0 ? UndefinedNoise : static_cast<NoiseType>(rv);
}

const char* noiseTypeToString(NoiseType nt) {
    return reverseLookupEnum(noise_types, noise_types_count, nt, "Undefined");
}

const char* imageTypeToString(ImageType it) {
    switch (it) {
        case UndefinedType: return "Undefined";
        case BilevelType: return "Bilevel";
        case GrayscaleType: return "Grayscale";
        case GrayscaleAlphaType: return "GrayscaleAlpha";
        case PaletteType: return "Palette";
        case PaletteAlphaType: return "PaletteAlpha";
        case TrueColorType: return "TrueColor";
        case TrueColorAlphaType: return "TrueColorAlpha";
        case ColorSeparationType: return "ColorSeparation";
        case ColorSeparationAlphaType: return "ColorSeparationAlpha";
        case OptimizeType: return "Optimize";
        case PaletteBilevelAlphaType: return "PaletteBilevelAlpha";
        default: return "Unknown";
    }
}

QoreStringNode* getPixelColor(PixelWand* pw) {
    char* color = PixelGetColorAsNormalizedString(pw);
    QoreStringNode* rv = new QoreStringNode(color);
    MagickRelinquishMemory(color);
    return rv;
}

//! returns true if the string has a character that ends a quoted name in an ImageMagick drawing script
static bool hasScriptBreak(const char* name) {
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(name); *p; ++p) {
        if (*p == '\'' || *p == '\\' || *p < 0x20 || *p == 0x7f) {
            return true;
        }
    }
    return false;
}

int checkFontName(const char* font, const char* what, ExceptionSink* xsink) {
    if (font && hasScriptBreak(font)) {
        xsink->raiseException("IMAGEMAGICK-ERROR", "invalid %s: it contains a quote, a backslash or a control "
            "character", what);
        return -1;
    }
    return 0;
}

int checkFontAccess(const char* font, ExceptionSink* xsink) {
    if (!font || !font[0]) {
        return 0;
    }
    if (checkFontName(font, "font", xsink)) {
        return -1;
    }
    // ImageMagick reads a font that names an existing file (before it looks the name up among the registered
    // fonts), and "@" introduces a file: such a font is the calling code's input
    struct stat st;
    bool path = font[0] == '@' || font[0] == '~' || strchr(font, '/') || strchr(font, '\\') || !stat(font, &st);
    if (!path) {
        return 0;
    }
    // the options of a module's own Program do not decide (module-imagemagick#8)
    if (qore_caller_restricts_domain(QDOM_FILESYSTEM)) {
        xsink->raiseException("PERMISSION-DENIED", "font '%s' names a file, which cannot be used: the calling code "
            "is restricted from the FILESYSTEM functional domain", font);
        return -1;
    }
    QoreSandboxManagerHelper smh(QoreSandboxManagerHelper::Policy);
    if (smh && !smh->checkFilesystemAccess(font[0] == '@' ? font + 1 : font, QSEC_READ, xsink)) {
        return -1;
    }
    return 0;
}

int checkMagickFileName(const char* path, ExceptionSink* xsink) {
    // a coder prefix ("png:file"), a list file ("@file") and a scene or size suffix ("file[0]") make ImageMagick
    // open another file than the one that the policy checks (a one-letter prefix is a Windows drive)
    const char* p = path;
    while (*p && (isalnum(static_cast<unsigned char>(*p)) || *p == '-' || *p == '_')) {
        ++p;
    }
    size_t len = strlen(path);
    if ((*p == ':' && (p - path) > 1) || path[0] == '@' || (len && path[len - 1] == ']')) {
        xsink->raiseException("FILESYSTEM-ACCESS-DENIED", "file name '%s' cannot be used by code that a sandbox "
            "governs: ImageMagick would interpret its coder prefix, list file or suffix", path);
        return -1;
    }
    return 0;
}

int checkReadFormat(const char* format, bool sandboxed, ExceptionSink* xsink) {
    if (!format) {
        return 0;
    }
    // formats whose data can name other files or URLs that ImageMagick reads
    static const char* const formats[] = {
        "MVG", "SVG", "MSVG", "RSVG", "MSL", "TEXT", "LABEL", "CAPTION", "PANGO", "EPHEMERAL", "URL", "HTTP",
        "HTTPS", "FTP", "FILE", "INLINE",
    };
    bool found = false;
    for (const char* f : formats) {
        if (!strcasecmp(f, format)) {
            found = true;
            break;
        }
    }
    if (!found) {
        return 0;
    }
    if (sandboxed || qore_caller_restricts_domain(QDOM_FILESYSTEM | QDOM_NETWORK)) {
        xsink->raiseException("PERMISSION-DENIED", "image data in format '%s' cannot be read for the calling code: "
            "the format can name other files that ImageMagick reads, and the calling code is restricted from the "
            "FILESYSTEM or NETWORK functional domain, or a sandbox governs it", format);
        return -1;
    }
    return 0;
}
