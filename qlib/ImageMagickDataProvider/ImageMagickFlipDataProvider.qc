# -*- mode: qore; indent-tabs-mode: nil -*-
/*  Copyright 2026 Qore Technologies, s.r.o.
    MIT License - see LICENSE for details
*/

#! @file ImageMagickFlipDataProvider.qc Flip action

public class ImageMagickFlipDataProvider inherits ImageMagickImageActionBase {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "flip",
            "desc": "Flip or mirror an image",
            "type": "ImageMagickFlipDataProvider",
            "supports_request": True,
        };

        static ImageMagickFlipRequestDataType RequestType();
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
        string direction = req.direction ?? "vertical";
        if (direction == "horizontal") {
            img.flop();
        } else {
            img.flip();
        }
        return ImageMagickImageActionBase::makeResponse(img, req.output_format);
    }
}
