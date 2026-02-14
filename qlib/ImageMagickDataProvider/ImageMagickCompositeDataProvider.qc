# -*- mode: qore; indent-tabs-mode: nil -*-
/*  Copyright 2026 Qore Technologies, s.r.o.
    MIT License - see LICENSE for details
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
        } else {
            overlay = new MagickImage(req.overlay_data);
        }
        string op = req.operator ?? "Over";
        if (req.gravity) {
            img.compositeGravity(overlay, op, req.gravity);
        } else {
            img.composite(overlay, op, req.x ?? 0, req.y ?? 0);
        }
        return ImageMagickImageActionBase::makeResponse(img, req.output_format);
    }
}
