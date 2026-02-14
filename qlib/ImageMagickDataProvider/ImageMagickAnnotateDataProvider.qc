# -*- mode: qore; indent-tabs-mode: nil -*-
/*  Copyright 2026 Qore Technologies, s.r.o.
    MIT License - see LICENSE for details
*/

#! @file ImageMagickAnnotateDataProvider.qc Annotate action

public class ImageMagickAnnotateDataProvider inherits ImageMagickImageActionBase {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "annotate",
            "desc": "Add text annotation to an image",
            "type": "ImageMagickAnnotateDataProvider",
            "supports_request": True,
        };

        static ImageMagickAnnotateRequestDataType RequestType();
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
        img.annotate(
            req.text,
            req.x,
            req.y,
            req.angle ?? 0.0,
            req.font ?? "",
            req.font_size ?? 0.0,
            req.fill_color ?? "black",
            req.stroke_color ?? "",
        );
        return ImageMagickImageActionBase::makeResponse(img, req.output_format);
    }
}
