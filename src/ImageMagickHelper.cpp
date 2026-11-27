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
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>

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

bool callerRestricts(int64 domain, bool always) {
#ifdef QORE_HAVE_CALLER_AMBIENT_PREDICATES
    // a method of a lent object runs with what the object lends: the code that it was lent to counts
    return qore_caller_lent_to_restricts_domain(domain);
#else
    return always && qore_caller_restricts_domain(domain);
#endif
}

int checkCallerPath(const char* path, ExceptionSink* xsink) {
    if (callerRestricts(QDOM_FILESYSTEM, false)) {
        xsink->raiseException("PERMISSION-DENIED", "file '%s' cannot be used: the calling code (also the code that a "
            "host-created object was lent to) is restricted from the FILESYSTEM functional domain", path);
        return -1;
    }
    return 0;
}

bool haveFreetype() {
    // the built-in delegate libraries, as "magick -version" lists them
    const char* d = GetMagickDelegates();
    if (!d) {
        return false;
    }
    for (const char* p = d; (p = strstr(p, "freetype")); p += 8) {
        if ((p == d || p[-1] == ' ') && (!p[8] || p[8] == ' ')) {
            return true;
        }
    }
    return false;
}

int checkFontRenderer(const char* font, bool text, ExceptionSink* xsink) {
    // ImageMagick renders a font that starts with "^" with Ghostscript (a PostScript program that it decodes), one
    // that starts with "-" with an X server, and all text with Ghostscript if it was built without FreeType
    // (module-imagemagick#13)
    bool delegate_font = font && (font[0] == '^' || font[0] == '-');
    if (!delegate_font && (!text || haveFreetype())) {
        return 0;
    }
    QoreSandboxManagerHelper smh(QoreSandboxManagerHelper::Policy);
    if (!callerRestrictsFormats(static_cast<bool>(smh))) {
        return 0;
    }
    if (delegate_font) {
        xsink->raiseException("PERMISSION-DENIED", "font '%s' cannot be used: ImageMagick renders a font whose "
            "name starts with '^' with Ghostscript and one whose name starts with '-' with an X server, and the "
            "calling code (also the code that a host-created object was lent to) is restricted from the "
            "FILESYSTEM, NETWORK, EXTERNAL_PROCESS or PROCESS_CONTROL functional domain, or a sandbox governs "
            "it", font);
    } else {
        xsink->raiseException("PERMISSION-DENIED", "text cannot be rendered: ImageMagick was built without "
            "FreeType and renders text with Ghostscript, and the calling code (also the code that a host-created "
            "object was lent to) is restricted from the FILESYSTEM, NETWORK, EXTERNAL_PROCESS or PROCESS_CONTROL "
            "functional domain, or a sandbox governs it");
    }
    return -1;
}

int checkFontAccess(const char* font, ExceptionSink* xsink) {
    if (!font || !font[0]) {
        return 0;
    }
    if (checkFontName(font, "font", xsink)) {
        return -1;
    }
    if (checkFontRenderer(font, false, xsink)) {
        return -1;
    }
    // ImageMagick reads a font that names an existing file (before it looks the name up among the registered
    // fonts), and "@" introduces a file: such a font is the calling code's input
    struct stat st;
    bool path = font[0] == '@' || font[0] == '~' || strchr(font, '/') || strchr(font, '\\') || !stat(font, &st);
    if (!path) {
        return 0;
    }
    // the options of a module's own Program do not decide (module-imagemagick#8); the code that a host-created object
    // was lent to does (module-imagemagick#10)
    if (callerRestricts(QDOM_FILESYSTEM, true)) {
        xsink->raiseException("PERMISSION-DENIED", "font '%s' names a file, which cannot be used: the calling code "
            "(also the code that a host-created object was lent to) is restricted from the FILESYSTEM functional "
            "domain", font);
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
    // open another file than the one that the policy checks; "%" in a name that is written is replaced with image
    // properties and the scene number (InterpretImageFilename()); one-letter coders exist (such as "r:"), so a
    // one-letter prefix is a drive only on Windows (module-imagemagick#13)
    const char* p = path;
    while (*p && (isalnum(static_cast<unsigned char>(*p)) || *p == '-' || *p == '_')) {
        ++p;
    }
#ifdef _WIN32
    bool prefix = *p == ':' && (p - path) > 1;
#else
    bool prefix = *p == ':' && p != path;
#endif
    size_t len = strlen(path);
    if (prefix || path[0] == '@' || (len && path[len - 1] == ']') || strchr(path, '%')) {
        xsink->raiseException("FILESYSTEM-ACCESS-DENIED", "file name '%s' cannot be used by code that a sandbox "
            "governs or that is restricted from the FILESYSTEM, NETWORK, EXTERNAL_PROCESS or PROCESS_CONTROL "
            "functional domain: ImageMagick would interpret its coder prefix, list file, suffix or '%%' escape",
            path);
        return -1;
    }
    return 0;
}

bool callerRestrictsFormats(bool sandboxed) {
    // the options of a module's own Program do not decide; the code that a host-created object was lent to does
    return sandboxed || callerRestricts(IM_DECODE_RESTRICTED_DOMAINS, true);
}

//! returns true if the name is in the list (case-insensitive)
static bool inList(const char* name, const char* const* list, size_t count) {
    if (!name || !*name) {
        return false;
    }
    for (size_t i = 0; i < count; ++i) {
        if (!strcasecmp(list[i], name)) {
            return true;
        }
    }
    return false;
}

//! returns true if the name is a valid format name: an explicit format with other characters is not looked up
static bool isFormatName(const char* name) {
    if (!name || !*name) {
        return false;
    }
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(name); *p; ++p) {
        if (!isalnum(*p) && *p != '-' && *p != '_') {
            return false;
        }
    }
    return true;
}

std::string getDecodeFormat(const void* data, size_t len, const char* format, const char* path) {
    assert(data && len);
    ExceptionInfoHolder ex;
    if (!ex.get()) {
        return std::string();
    }
    // an explicit format that ImageMagick knows is used as it is (MagickSetFormat() ignores one that it does not
    // know, and ImageMagick then detects the format)
    if (isFormatName(format)) {
        const MagickInfo* mi = GetMagickInfo(format, ex.get());
        if (mi && mi->name) {
            return mi->name;
        }
    }
    // the format is detected as ImageMagick detects it when it decodes the data (BlobToImage(), ReadImage()): from
    // the file name's extension, then from the data's magic bytes, which take precedence unless the extension names
    // an explicit format
    ImageInfoHolder info;
    if (!info.get()) {
        return std::string();
    }
    if (path) {
        CopyMagickString(info->filename, path, MagickPathExtent);
    }
    // the data is detected in memory; ImageMagick never opens the file again
    SetImageInfoBlob(info.get(), data, len);
    SetImageInfo(info.get(), 0, ex.get());
    return info->magick;
}

// formats that ImageMagick decodes with Ghostscript or another external program (delegate), as format or coder
// (module) names; a format with no decoder of its own and the coders that a delegate is configured for are refused
// too (see hasDecodeDelegate())
static const char* const delegate_formats[] = {
    // Ghostscript
    "PS", "PS2", "PS3", "EPS", "EPS2", "EPS3", "EPI", "EPSF", "EPSI", "EPT", "EPT2", "EPT3", "PDF", "PDFA", "EPDF",
    "AI", "XPS", "OXPS", "PCL",
    // other delegates
    "DNG", "VIDEO", "MPEG", "HTML", "HTM", "SHTML", "DOT", "GV", "DVI", "HPGL", "HPG", "JXR", "WDP", "BPG", "LEP",
    "ILBM", "SID", "EMF", "DOC", "DOCX", "ODT", "ODG", "PPT", "PPTX", "XLS", "XLSX", "VSD", "VSDX", "EPUB",
    "BLENDER", "CLIPBOARD",
};

// formats whose data names other files, URLs or devices that ImageMagick reads, or that embed data that ImageMagick
// decodes in a format that it detects, as format or coder (module) names
static const char* const indirection_formats[] = {
    "MVG", "SVG", "SVGZ", "MSVG", "MSVGZ", "RSVG", "MSL", "TEXT", "TXT", "SPARSE-COLOR", "LABEL", "CAPTION", "PANGO",
    "EPHEMERAL", "URL", "HTTP", "HTTPS", "FTP", "FILE", "INLINE", "DATA", "VID", "SCREENSHOT", "X", "MPR", "MPRI",
    "MPC", "CACHE", "TILE", "STEGANO", "CLIP", "MASK", "WPG", "SFW", "PWP", "PES", "WMF",
};

//! returns true if a delegate (an external program) is configured for the coder's own decoding
/** Coders such as PS, PDF, XPS, PCL, SVG, DNG and VIDEO run the delegates named \c "<coder>:<variant>"
*/
static bool hasDecodeDelegate(const char* name, ExceptionInfo* ex) {
    if (!name || !*name) {
        return false;
    }
    static const char* const variants[] = {"decode", "alpha", "cmyk", "color", "gray", "mono"};
    std::string lname(name);
    for (char& c : lname) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    for (const char* v : variants) {
        std::string d = lname + ":" + v;
        if (GetDelegateInfo(d.c_str(), nullptr, ex)) {
            return true;
        }
    }
    return false;
}

int checkDecodeFormat(const char* magick, ExceptionSink* xsink) {
    if (!magick || !*magick) {
        xsink->raiseException("IMAGEMAGICK-ERROR", "the format of the image data cannot be determined");
        return -1;
    }
    ExceptionInfoHolder ex;
    if (!ex.get()) {
        xsink->outOfMemory();
        return -1;
    }
    const MagickInfo* mi = GetMagickInfo(magick, ex.get());
    const char* module = mi ? GetMagickModuleName(mi) : nullptr;
    const char* why = nullptr;
    if (!mi || !GetImageDecoder(mi)) {
        why = "ImageMagick has no decoder of its own for this format and decodes it with an external program (a "
            "delegate)";
    } else if (inList(magick, indirection_formats, sizeof(indirection_formats) / sizeof(indirection_formats[0]))
        || inList(module, indirection_formats, sizeof(indirection_formats) / sizeof(indirection_formats[0]))) {
        why = "data in this format can name other files or URLs that ImageMagick reads, or embeds data that "
            "ImageMagick decodes in a format that it detects";
    } else if (inList(magick, delegate_formats, sizeof(delegate_formats) / sizeof(delegate_formats[0]))
        || inList(module, delegate_formats, sizeof(delegate_formats) / sizeof(delegate_formats[0]))
        || hasDecodeDelegate(magick, ex.get()) || hasDecodeDelegate(module, ex.get())) {
        why = "ImageMagick decodes data in this format with an external program (a delegate such as Ghostscript)";
    }
    if (!why) {
        return 0;
    }
    xsink->raiseException("PERMISSION-DENIED", "image data in format '%s' cannot be decoded for the calling code: "
        "%s, and the calling code (also the code that a host-created object was lent to) is restricted from the "
        "FILESYSTEM, NETWORK, EXTERNAL_PROCESS or PROCESS_CONTROL functional domain, or a sandbox governs it",
        magick, why);
    return -1;
}

// output formats that ImageMagick encodes with an external program or shows on a display, as format or coder
// (module) names
static const char* const encode_delegate_formats[] = {
    "VIDEO", "X", "SHOW", "WIN", "PRINT", "BROWSE", "EDIT", "LAUNCH", "OPEN", "CLIPBOARD", "SCAN", "TRACE", "BPG",
    "JXR", "WDP", "LEP", "ILBM",
};

int checkEncodeFormat(const char* magick, ExceptionSink* xsink) {
    if (!magick || !*magick) {
        return 0;
    }
    ExceptionInfoHolder ex;
    if (!ex.get()) {
        xsink->outOfMemory();
        return -1;
    }
    const MagickInfo* mi = isFormatName(magick) ? GetMagickInfo(magick, ex.get()) : nullptr;
    const char* module = mi ? GetMagickModuleName(mi) : nullptr;
    bool refused;
    if (!mi || !GetImageEncoder(mi)) {
        // ImageMagick writes a format with no encoder of its own with a delegate if one is configured (WriteImage())
        refused = GetDelegateInfo(nullptr, magick, ex.get()) || GetDelegateInfo("*", magick, ex.get())
            || inList(magick, encode_delegate_formats,
                sizeof(encode_delegate_formats) / sizeof(encode_delegate_formats[0]));
    } else {
        refused = inList(magick, encode_delegate_formats,
                sizeof(encode_delegate_formats) / sizeof(encode_delegate_formats[0]))
            || inList(module, encode_delegate_formats,
                sizeof(encode_delegate_formats) / sizeof(encode_delegate_formats[0]));
    }
    if (!refused) {
        return 0;
    }
    xsink->raiseException("PERMISSION-DENIED", "images cannot be written in format '%s' for the calling code: "
        "ImageMagick writes this format with an external program (a delegate) or shows it on a display, and the "
        "calling code (also the code that a host-created object was lent to) is restricted from the FILESYSTEM, "
        "NETWORK, EXTERNAL_PROCESS or PROCESS_CONTROL functional domain, or a sandbox governs it", magick);
    return -1;
}

int readRegularFile(const char* path, std::vector<unsigned char>& buf, ExceptionSink* xsink) {
    // ImageMagick's own path policy (policy.xml), which it applies when it opens a file itself
    if (IsRightsAuthorized(PathPolicyDomain, ReadPolicyRights, path) == MagickFalse) {
        xsink->raiseException("IMAGEMAGICK-ERROR", "error reading image file '%s': the ImageMagick security "
            "policy does not allow the path", path);
        return -1;
    }
    // O_NONBLOCK: opening a FIFO does not wait for a writer
    int fd = open(path, O_RDONLY | O_CLOEXEC | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) {
        xsink->raiseErrnoException("IMAGEMAGICK-ERROR", errno, "error reading image file '%s'", path);
        return -1;
    }
    ON_BLOCK_EXIT(close, fd);
    struct stat st;
    if (fstat(fd, &st)) {
        xsink->raiseErrnoException("IMAGEMAGICK-ERROR", errno, "error reading image file '%s'", path);
        return -1;
    }
    if (!S_ISREG(st.st_mode)) {
        xsink->raiseException("IMAGEMAGICK-ERROR", "error reading image file '%s': for restricted calling code, "
            "images are read only from regular files", path);
        return -1;
    }
    try {
        buf.clear();
        buf.reserve(static_cast<size_t>(st.st_size));
        unsigned char chunk[65536];
        while (true) {
            if (qore_check_cancel(xsink, "reading image file")) {
                return -1;
            }
            ssize_t rc = read(fd, chunk, sizeof(chunk));
            if (rc < 0) {
                if (errno == EINTR) {
                    continue;
                }
                xsink->raiseErrnoException("IMAGEMAGICK-ERROR", errno, "error reading image file '%s'", path);
                return -1;
            }
            if (!rc) {
                break;
            }
            buf.insert(buf.end(), chunk, chunk + rc);
        }
    } catch (std::bad_alloc&) {
        xsink->outOfMemory();
        return -1;
    }
    return 0;
}
