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

#endif // _QORE_IMAGEMAGICK_HELPER_H
