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

#! @file ImageMagickCompositeDataProvider.qc Composite action

public class ImageMagickCompositeDataProvider inherits ImageMagickImageActionBase {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "composite",
            "desc": "Composite (overlay) one image onto another",
            "type": "ImageMagickCompositeDataProvider",
            "supports_request": True,
        };

        static ImageMagickCompositeRequestDataType RequestType();
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

    private auto doRequestImpl(auto req, *hash<auto> request_options) {
        MagickImage img = ImageMagickImageActionBase::loadImage(req);
        MagickImage overlay;
        if (req.overlay_path) {
            overlay = new MagickImage(req.overlay_path);
        } else if (req.overlay_location) {
            overlay = new MagickImage(FileLocationHandler::getBinaryFileFromLocation(req.overlay_location));
        } else if (req.overlay_data) {
            overlay = new MagickImage(req.overlay_data);
        } else {
            throw "IMAGEMAGICK-COMPOSITE-ERROR", "one of 'overlay_path', 'overlay_data', or "
                "'overlay_location' must be provided";
        }
        string op = req.operator ?? "Over";
        if (req.gravity) {
            img.compositeGravity(overlay, op, req.gravity);
        } else {
            img.composite(overlay, op, req.x ?? 0, req.y ?? 0);
        }
        return ImageMagickImageActionBase::makeResponse(img, req);
    }
}
