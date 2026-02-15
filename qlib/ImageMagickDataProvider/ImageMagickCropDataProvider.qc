# -*- mode: qore; indent-tabs-mode: nil -*-
/*  Copyright 2026 Qore Technologies, s.r.o.
    MIT License - see LICENSE for details
*/

#! @file ImageMagickCropDataProvider.qc Crop action

public class ImageMagickCropDataProvider inherits ImageMagickImageActionBase {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "crop",
            "desc": "Crop an image to a specified region",
            "type": "ImageMagickCropDataProvider",
            "supports_request": True,
        };

        static ImageMagickCropRequestDataType RequestType();
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
        img.crop(req.width, req.height, req.x ?? 0, req.y ?? 0);
        return ImageMagickImageActionBase::makeResponse(img, req);
    }
}
