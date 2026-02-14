/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file QoreMagickDrawing.cpp QoreMagickDrawing implementation */
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

#include "QoreMagickDrawing.h"

QoreMagickDrawing::QoreMagickDrawing() : dw(NewDrawingWand()) {
}

QoreMagickDrawing::~QoreMagickDrawing() {
    if (dw) {
        DestroyDrawingWand(dw);
    }
}

void QoreMagickDrawing::setFont(const char* font, ExceptionSink* xsink) {
    DrawSetFont(dw, font);
}

void QoreMagickDrawing::setFontSize(double size, ExceptionSink* xsink) {
    DrawSetFontSize(dw, size);
}

void QoreMagickDrawing::setFontFamily(const char* family, ExceptionSink* xsink) {
    DrawSetFontFamily(dw, family);
}

void QoreMagickDrawing::setFontWeight(int64 weight, ExceptionSink* xsink) {
    DrawSetFontWeight(dw, static_cast<size_t>(weight));
}

void QoreMagickDrawing::setFontStyle(const char* style, ExceptionSink* xsink) {
    StyleType st = NormalStyle;
    if (!strcasecmp(style, "italic")) {
        st = ItalicStyle;
    } else if (!strcasecmp(style, "oblique")) {
        st = ObliqueStyle;
    } else if (strcasecmp(style, "normal")) {
        xsink->raiseException("IMAGEMAGICK-ERROR",
            "invalid font style '%s'; valid values: normal, italic, oblique", style);
        return;
    }
    DrawSetFontStyle(dw, st);
}

void QoreMagickDrawing::setFillColor(const char* color, ExceptionSink* xsink) {
    PixelWandHelper pw(color);
    DrawSetFillColor(dw, pw);
}

void QoreMagickDrawing::setStrokeColor(const char* color, ExceptionSink* xsink) {
    PixelWandHelper pw(color);
    DrawSetStrokeColor(dw, pw);
}

void QoreMagickDrawing::setStrokeWidth(double width, ExceptionSink* xsink) {
    DrawSetStrokeWidth(dw, width);
}

void QoreMagickDrawing::setFillOpacity(double opacity, ExceptionSink* xsink) {
    DrawSetFillOpacity(dw, opacity);
}

void QoreMagickDrawing::setStrokeOpacity(double opacity, ExceptionSink* xsink) {
    DrawSetStrokeOpacity(dw, opacity);
}

void QoreMagickDrawing::line(double sx, double sy, double ex, double ey, ExceptionSink* xsink) {
    DrawLine(dw, sx, sy, ex, ey);
}

void QoreMagickDrawing::rectangle(double x1, double y1, double x2, double y2,
                                  ExceptionSink* xsink) {
    DrawRectangle(dw, x1, y1, x2, y2);
}

void QoreMagickDrawing::roundRectangle(double x1, double y1, double x2, double y2,
                                       double rx, double ry, ExceptionSink* xsink) {
    DrawRoundRectangle(dw, x1, y1, x2, y2, rx, ry);
}

void QoreMagickDrawing::circle(double ox, double oy, double px, double py, ExceptionSink* xsink) {
    DrawCircle(dw, ox, oy, px, py);
}

void QoreMagickDrawing::ellipse(double ox, double oy, double rx, double ry, double start,
                                double end, ExceptionSink* xsink) {
    DrawEllipse(dw, ox, oy, rx, ry, start, end);
}

void QoreMagickDrawing::arc(double sx, double sy, double ex, double ey, double sd, double ed,
                            ExceptionSink* xsink) {
    DrawArc(dw, sx, sy, ex, ey, sd, ed);
}

void QoreMagickDrawing::point(double x, double y, ExceptionSink* xsink) {
    DrawPoint(dw, x, y);
}

void QoreMagickDrawing::polygon(const QoreListNode* points, ExceptionSink* xsink) {
    size_t count = points->size();
    if (count < 3) {
        xsink->raiseException("IMAGEMAGICK-ERROR", "polygon requires at least 3 points");
        return;
    }
    std::vector<PointInfo> pts(count);
    for (size_t i = 0; i < count; ++i) {
        QoreValue v = points->retrieveEntry(i);
        const QoreHashNode* h = v.getType() == NT_HASH ? v.get<const QoreHashNode>() : nullptr;
        if (!h) {
            xsink->raiseException("IMAGEMAGICK-ERROR",
                "polygon point %zu must be a hash with 'x' and 'y' keys", i);
            return;
        }
        pts[i].x = h->getKeyValue("x").getAsFloat();
        pts[i].y = h->getKeyValue("y").getAsFloat();
    }
    DrawPolygon(dw, count, pts.data());
}

void QoreMagickDrawing::polyline(const QoreListNode* points, ExceptionSink* xsink) {
    size_t count = points->size();
    if (count < 2) {
        xsink->raiseException("IMAGEMAGICK-ERROR", "polyline requires at least 2 points");
        return;
    }
    std::vector<PointInfo> pts(count);
    for (size_t i = 0; i < count; ++i) {
        QoreValue v = points->retrieveEntry(i);
        const QoreHashNode* h = v.getType() == NT_HASH ? v.get<const QoreHashNode>() : nullptr;
        if (!h) {
            xsink->raiseException("IMAGEMAGICK-ERROR",
                "polyline point %zu must be a hash with 'x' and 'y' keys", i);
            return;
        }
        pts[i].x = h->getKeyValue("x").getAsFloat();
        pts[i].y = h->getKeyValue("y").getAsFloat();
    }
    DrawPolyline(dw, count, pts.data());
}

void QoreMagickDrawing::annotation(double x, double y, const char* text, ExceptionSink* xsink) {
    DrawAnnotation(dw, x, y, reinterpret_cast<const unsigned char*>(text));
}

void QoreMagickDrawing::rotate(double degrees, ExceptionSink* xsink) {
    DrawRotate(dw, degrees);
}

void QoreMagickDrawing::translate(double x, double y, ExceptionSink* xsink) {
    DrawTranslate(dw, x, y);
}

void QoreMagickDrawing::drawScale(double x, double y, ExceptionSink* xsink) {
    DrawScale(dw, x, y);
}

void QoreMagickDrawing::skewX(double degrees, ExceptionSink* xsink) {
    DrawSkewX(dw, degrees);
}

void QoreMagickDrawing::skewY(double degrees, ExceptionSink* xsink) {
    DrawSkewY(dw, degrees);
}

void QoreMagickDrawing::push(ExceptionSink* xsink) {
    PushDrawingWand(dw);
}

void QoreMagickDrawing::pop(ExceptionSink* xsink) {
    PopDrawingWand(dw);
}
