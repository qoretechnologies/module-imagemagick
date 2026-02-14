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
