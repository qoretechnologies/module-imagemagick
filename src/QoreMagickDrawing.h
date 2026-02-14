/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file QoreMagickDrawing.h QoreMagickDrawing class header */
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

#ifndef _QORE_IMAGEMAGICK_QOREMAGICKDRAWING_H
#define _QORE_IMAGEMAGICK_QOREMAGICKDRAWING_H

#include "imagemagick-module.h"
#include "ImageMagickHelper.h"

//! QoreMagickDrawing - private data class for MagickDrawing Qore class
/** Wraps a DrawingWand* for vector graphics operations.
*/
class QoreMagickDrawing : public AbstractPrivateData {
public:
    //! Default constructor
    DLLLOCAL QoreMagickDrawing();

    //! Destructor
    DLLLOCAL virtual ~QoreMagickDrawing();

    // --- Font/Text config ---
    DLLLOCAL void setFont(const char* font, ExceptionSink* xsink);
    DLLLOCAL void setFontSize(double size, ExceptionSink* xsink);
    DLLLOCAL void setFontFamily(const char* family, ExceptionSink* xsink);
    DLLLOCAL void setFontWeight(int64 weight, ExceptionSink* xsink);
    DLLLOCAL void setFontStyle(const char* style, ExceptionSink* xsink);

    // --- Color config ---
    DLLLOCAL void setFillColor(const char* color, ExceptionSink* xsink);
    DLLLOCAL void setStrokeColor(const char* color, ExceptionSink* xsink);
    DLLLOCAL void setStrokeWidth(double width, ExceptionSink* xsink);
    DLLLOCAL void setFillOpacity(double opacity, ExceptionSink* xsink);
    DLLLOCAL void setStrokeOpacity(double opacity, ExceptionSink* xsink);

    // --- Shapes ---
    DLLLOCAL void line(double sx, double sy, double ex, double ey, ExceptionSink* xsink);
    DLLLOCAL void rectangle(double x1, double y1, double x2, double y2, ExceptionSink* xsink);
    DLLLOCAL void roundRectangle(double x1, double y1, double x2, double y2, double rx, double ry,
                                 ExceptionSink* xsink);
    DLLLOCAL void circle(double ox, double oy, double px, double py, ExceptionSink* xsink);
    DLLLOCAL void ellipse(double ox, double oy, double rx, double ry, double start, double end,
                          ExceptionSink* xsink);
    DLLLOCAL void arc(double sx, double sy, double ex, double ey, double sd, double ed,
                      ExceptionSink* xsink);
    DLLLOCAL void point(double x, double y, ExceptionSink* xsink);
    DLLLOCAL void polygon(const QoreListNode* points, ExceptionSink* xsink);
    DLLLOCAL void polyline(const QoreListNode* points, ExceptionSink* xsink);

    // --- Text ---
    DLLLOCAL void annotation(double x, double y, const char* text, ExceptionSink* xsink);

    // --- Transforms ---
    DLLLOCAL void rotate(double degrees, ExceptionSink* xsink);
    DLLLOCAL void translate(double x, double y, ExceptionSink* xsink);
    DLLLOCAL void drawScale(double x, double y, ExceptionSink* xsink);
    DLLLOCAL void skewX(double degrees, ExceptionSink* xsink);
    DLLLOCAL void skewY(double degrees, ExceptionSink* xsink);

    // --- State ---
    DLLLOCAL void push(ExceptionSink* xsink);
    DLLLOCAL void pop(ExceptionSink* xsink);

    //! Get internal DrawingWand
    DLLLOCAL DrawingWand* getWand() const { return dw; }

private:
    DrawingWand* dw;
};

#endif // _QORE_IMAGEMAGICK_QOREMAGICKDRAWING_H
