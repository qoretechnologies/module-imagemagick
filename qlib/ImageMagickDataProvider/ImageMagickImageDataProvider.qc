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

#! @file ImageMagickImageDataProvider.qc Image operations container

#! Container for image manipulation operations
public class ImageMagickImageDataProvider inherits AbstractDataProvider {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "image",
            "desc": "Image manipulation operations",
            "type": "ImageMagickImageDataProvider",
            "supports_children": True,
            "children_can_support_apis": True,
        };
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

    private *list<string> getChildProviderNamesImpl() {
        return ("resize", "crop", "rotate", "flip", "blur", "sharpen",
                "brightness-contrast", "thumbnail", "auto-orient", "strip", "effects");
    }

    private *AbstractDataProvider getChildProviderImpl(string name) {
        switch (name) {
            case "resize": return new ImageMagickResizeDataProvider(opts);
            case "crop": return new ImageMagickCropDataProvider(opts);
            case "rotate": return new ImageMagickRotateDataProvider(opts);
            case "flip": return new ImageMagickFlipDataProvider(opts);
            case "blur": return new ImageMagickBlurDataProvider(opts);
            case "sharpen": return new ImageMagickSharpenDataProvider(opts);
            case "brightness-contrast": return new ImageMagickBrightnessContrastDataProvider(opts);
            case "thumbnail": return new ImageMagickThumbnailDataProvider(opts);
            case "auto-orient": return new ImageMagickAutoOrientDataProvider(opts);
            case "strip": return new ImageMagickStripDataProvider(opts);
            case "effects": return new ImageMagickEffectsDataProvider(opts);
        }
        return NOTHING;
    }

    private hash<DataProviderInfo> getStaticInfoImpl() {
        return ProviderInfo;
    }
}
