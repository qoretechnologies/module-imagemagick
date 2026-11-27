/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file ImageMagickHelper.h ImageMagick helper utilities */
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

#ifndef _QORE_IMAGEMAGICK_HELPER_H
#define _QORE_IMAGEMAGICK_HELPER_H

#include "imagemagick-module.h"

//! RAII wrapper for temporary PixelWand objects
class PixelWandHelper {
public:
    DLLLOCAL PixelWandHelper() : pw(NewPixelWand()) {}

    DLLLOCAL PixelWandHelper(const char* color) : pw(NewPixelWand()) {
        if (pw) {
            PixelSetColor(pw, color);
        }
    }

    DLLLOCAL ~PixelWandHelper() {
        if (pw) {
            DestroyPixelWand(pw);
        }
    }

    DLLLOCAL PixelWand* get() const { return pw; }
    DLLLOCAL operator PixelWand*() const { return pw; }

    // Non-copyable
    PixelWandHelper(const PixelWandHelper&) = delete;
    PixelWandHelper& operator=(const PixelWandHelper&) = delete;

private:
    PixelWand* pw;
};

//! Check for MagickWand errors and raise Qore exceptions
/** @param wand the MagickWand to check
    @param context description of the operation for error messages
    @param xsink exception sink
    @return true if an error was found
*/
DLLLOCAL bool checkMagickError(MagickWand* wand, const char* context, ExceptionSink* xsink);

//! Check for DrawingWand errors and raise Qore exceptions
DLLLOCAL bool checkDrawingError(DrawingWand* wand, const char* context, ExceptionSink* xsink);

//! Convert string to FilterType
DLLLOCAL FilterType stringToFilterType(const char* str, ExceptionSink* xsink);

//! Convert FilterType to string
DLLLOCAL const char* filterTypeToString(FilterType ft);

//! Convert string to GravityType
DLLLOCAL GravityType stringToGravityType(const char* str, ExceptionSink* xsink);

//! Convert GravityType to string
DLLLOCAL const char* gravityTypeToString(GravityType gt);

//! Convert string to ColorspaceType
DLLLOCAL ColorspaceType stringToColorspaceType(const char* str, ExceptionSink* xsink);

//! Convert ColorspaceType to string
DLLLOCAL const char* colorspaceTypeToString(ColorspaceType cs);

//! Convert string to CompositeOperator
DLLLOCAL CompositeOperator stringToCompositeOp(const char* str, ExceptionSink* xsink);

//! Convert CompositeOperator to string
DLLLOCAL const char* compositeOpToString(CompositeOperator op);

//! Convert string to NoiseType
DLLLOCAL NoiseType stringToNoiseType(const char* str, ExceptionSink* xsink);

//! Convert NoiseType to string
DLLLOCAL const char* noiseTypeToString(NoiseType nt);

//! Convert ImageType to string
DLLLOCAL const char* imageTypeToString(ImageType it);

//! Get a color string from a PixelWand
DLLLOCAL QoreStringNode* getPixelColor(PixelWand* pw);

//! Refuses a font or font family name that would break out of the quoted name in an ImageMagick drawing script
/** @return 0 if the name can be used, -1 if \c IMAGEMAGICK-ERROR was raised
*/
DLLLOCAL int checkFontName(const char* font, const char* what, ExceptionSink* xsink);

//! Refuses a font that names a file if the calling code may not read the file
/** ImageMagick reads a font that names an existing file as that file, before it looks the name up among the
    registered fonts, and \c "@" introduces a file: a font that starts with \c "@" or \c "~", contains a path
    separator or names an existing file is the calling code's input, so it is refused if that code is restricted from
    the \c FILESYSTEM functional domain, and it is checked with the file-system policy of a sandbox that governs the
    code; a font with a quote, a backslash or a control character is refused for all callers (checkFontName())

    @return 0 if the font can be used, -1 if an exception was raised
*/
DLLLOCAL int checkFontAccess(const char* font, ExceptionSink* xsink);

//! Refuses, for code that a sandbox governs, a file name whose ImageMagick syntax opens another file
/** @return 0 if the name can be used, -1 if \c FILESYSTEM-ACCESS-DENIED was raised
*/
DLLLOCAL int checkMagickFileName(const char* path, ExceptionSink* xsink);

//! Refuses an explicit read format whose data can name other files, for restricted or sandboxed calling code
/** @return 0 if the format can be used, -1 if \c PERMISSION-DENIED was raised
*/
DLLLOCAL int checkReadFormat(const char* format, bool sandboxed, ExceptionSink* xsink);

#endif // _QORE_IMAGEMAGICK_HELPER_H
