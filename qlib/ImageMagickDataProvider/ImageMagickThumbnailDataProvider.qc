# -*- mode: qore; indent-tabs-mode: nil -*-
/*  Copyright 2026 Qore Technologies, s.r.o.
    MIT License - see LICENSE for details
*/

#! @file ImageMagickThumbnailDataProvider.qc Thumbnail action

public class ImageMagickThumbnailDataProvider inherits ImageMagickImageActionBase {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "thumbnail",
            "desc": "Create a thumbnail of an image",
            "type": "ImageMagickThumbnailDataProvider",
            "supports_request": True,
        };

        static ImageMagickThumbnailRequestDataType RequestType();
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
        img.thumbnail(req.width, req.height);
        return ImageMagickImageActionBase::makeResponse(img, req);
    }
}
