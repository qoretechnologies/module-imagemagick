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

#! @file ImageMagickDrawDataProvider.qc Vector drawing action

public namespace ImageMagickDataProvider {

public class ImageMagickDrawDataProvider inherits ImageMagickImageActionBase {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "draw",
            "desc": "Draw vector graphics on an image",
            "type": "ImageMagickDrawDataProvider",
            "supports_request": True,
        };

        static ImageMagickDrawRequestDataType RequestType();
        static ImageMagickImageResponseDataType ResponseType();
    }

    private {
        *hash<auto> opts;
    }

    constructor(*hash<auto> opts) {
        self.opts = opts;
    }

    string getName() {
        return ProviderInfo.name;
    }

    private hash<DataProviderInfo> getStaticInfoImpl() {
        return ProviderInfo;
    }

    private *AbstractDataProviderType getRequestTypeImpl() {
        return RequestType;
    }

    private *AbstractDataProviderType getResponseTypeImpl() {
        return ResponseType;
    }

    *hash<string, bool> getSupportedReferenceData() {
        return {"fonts": True};
    }

    private *list<hash<AllowedValueInfo>> getReferenceDataImpl(string type, *hash<auto> search_options) {
        if (type == "fonts") {
            return ImageMagickImageActionBase::getFontReferenceData();
        }
    }

    private auto doRequestImpl(auto req, *hash<auto> request_options) {
        MagickImage img = ImageMagickImageActionBase::loadImage(req);
        MagickDrawing dw();

        # Apply global styles
        if (req.fill_color) {
            dw.setFillColor(req.fill_color);
        }
        if (req.stroke_color) {
            dw.setStrokeColor(req.stroke_color);
        }
        if (exists req.stroke_width) {
            dw.setStrokeWidth(req.stroke_width);
        }
        if (exists req.fill_opacity) {
            dw.setFillOpacity(req.fill_opacity);
        }
        if (exists req.stroke_opacity) {
            dw.setStrokeOpacity(req.stroke_opacity);
        }
        if (req.font) {
            dw.setFont(req.font);
        }
        if (exists req.font_size) {
            dw.setFontSize(req.font_size);
        }
        if (req.font_family) {
            dw.setFontFamily(req.font_family);
        }
        if (exists req.font_weight) {
            dw.setFontWeight(req.font_weight);
        }
        if (req.font_style) {
            dw.setFontStyle(req.font_style);
        }

        # Process each drawing operation
        foreach hash<auto> op in (req.operations) {
            switch (op.type) {
                case "line":
                    dw.line(op.sx, op.sy, op.ex, op.ey);
                    break;
                case "rectangle":
                    dw.rectangle(op.x1, op.y1, op.x2, op.y2);
                    break;
                case "round-rectangle":
                    dw.roundRectangle(op.x1, op.y1, op.x2, op.y2, op.rx, op.ry);
                    break;
                case "circle":
                    dw.circle(op.ox, op.oy, op.px, op.py);
                    break;
                case "ellipse":
                    dw.ellipse(op.ox, op.oy, op.rx, op.ry, op.start ?? 0.0, op.end ?? 360.0);
                    break;
                case "arc":
                    dw.arc(op.sx, op.sy, op.ex, op.ey, op.sd, op.ed);
                    break;
                case "point":
                    dw.point(op.x, op.y);
                    break;
                case "polygon":
                    dw.polygon(map cast<hash<PointInfo>>($1), op.points);
                    break;
                case "polyline":
                    dw.polyline(map cast<hash<PointInfo>>($1), op.points);
                    break;
                case "text":
                    dw.annotation(op.x, op.y, op.text);
                    break;
                default:
                    throw "IMAGEMAGICK-DRAW-ERROR",
                        sprintf("unknown drawing operation type: %y", op.type);
            }
        }

        img.draw(dw);
        return ImageMagickImageActionBase::makeResponse(img, req);
    }
}
}
