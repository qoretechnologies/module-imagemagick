# -*- mode: qore; indent-tabs-mode: nil -*-
/*  Copyright 2026 Qore Technologies, s.r.o.
    MIT License - see LICENSE for details
*/

#! @file ImageMagickSepiaDataProvider.qc Sepia tone action

public class ImageMagickSepiaDataProvider inherits ImageMagickImageActionBase {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "sepia",
            "desc": "Apply a sepia tone effect",
            "type": "ImageMagickSepiaDataProvider",
            "supports_request": True,
        };

        static ImageMagickSepiaRequestDataType RequestType();
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
        img.sepiaTone(req.threshold ?? 52000.0);
        return ImageMagickImageActionBase::makeResponse(img, req);
    }
}
