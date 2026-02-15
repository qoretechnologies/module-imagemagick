# -*- mode: qore; indent-tabs-mode: nil -*-
/*  Copyright 2026 Qore Technologies, s.r.o.
    MIT License - see LICENSE for details
*/

#! @file ImageMagickRotateDataProvider.qc Rotate action

public class ImageMagickRotateDataProvider inherits ImageMagickImageActionBase {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "rotate",
            "desc": "Rotate an image by a specified angle",
            "type": "ImageMagickRotateDataProvider",
            "supports_request": True,
        };

        static ImageMagickRotateRequestDataType RequestType();
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
        img.rotate(req.degrees, req.background ?? "white");
        return ImageMagickImageActionBase::makeResponse(img, req);
    }
}
