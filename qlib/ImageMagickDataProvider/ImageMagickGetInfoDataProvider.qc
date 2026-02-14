# -*- mode: qore; indent-tabs-mode: nil -*-
/*  Copyright 2026 Qore Technologies, s.r.o.
    MIT License - see LICENSE for details
*/

#! @file ImageMagickGetInfoDataProvider.qc Get image info action

public class ImageMagickGetInfoDataProvider inherits ImageMagickImageActionBase {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "get",
            "desc": "Get image metadata and dimensions",
            "type": "ImageMagickGetInfoDataProvider",
            "supports_request": True,
        };

        static ImageMagickGetInfoRequestDataType RequestType();
        static ImageMagickGetInfoResponseDataType ResponseType();
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
        return {
            "width": img.width(),
            "height": img.height(),
            "format": img.format(),
            "depth": img.depth(),
            "colorspace": img.colorspace(),
            "image_type": img.imageType(),
            "compression_quality": img.compressionQuality(),
            "number_images": img.numberImages(),
            "colors": img.colors(),
            "signature": img.signature(),
        };
    }
}
