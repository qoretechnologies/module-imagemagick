# -*- mode: qore; indent-tabs-mode: nil -*-
/*  Copyright 2026 Qore Technologies, s.r.o.
    MIT License - see LICENSE for details
*/

#! @file ImageMagickAutoOrientDataProvider.qc Auto orient action

public class ImageMagickAutoOrientDataProvider inherits ImageMagickImageActionBase {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "auto-orient",
            "desc": "Automatically orient image based on EXIF data",
            "type": "ImageMagickAutoOrientDataProvider",
            "supports_request": True,
        };

        static ImageMagickAutoOrientRequestDataType RequestType();
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
        img.autoOrient();
        return ImageMagickImageActionBase::makeResponse(img, req);
    }
}
