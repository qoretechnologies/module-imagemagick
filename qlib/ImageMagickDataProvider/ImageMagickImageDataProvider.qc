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

public namespace ImageMagickDataProvider {

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

        #! Child data providers
        const ChildMap = {
            "resize": Class::forName("ImageMagickDataProvider::ImageMagickResizeDataProvider"),
            "crop": Class::forName("ImageMagickDataProvider::ImageMagickCropDataProvider"),
            "rotate": Class::forName("ImageMagickDataProvider::ImageMagickRotateDataProvider"),
            "flip": Class::forName("ImageMagickDataProvider::ImageMagickFlipDataProvider"),
            "blur": Class::forName("ImageMagickDataProvider::ImageMagickBlurDataProvider"),
            "sharpen": Class::forName("ImageMagickDataProvider::ImageMagickSharpenDataProvider"),
            "brightness-contrast": Class::forName("ImageMagickDataProvider::ImageMagickBrightnessContrastDataProvider"),
            "composite": Class::forName("ImageMagickDataProvider::ImageMagickCompositeDataProvider"),
            "annotate": Class::forName("ImageMagickDataProvider::ImageMagickAnnotateDataProvider"),
            "thumbnail": Class::forName("ImageMagickDataProvider::ImageMagickThumbnailDataProvider"),
            "auto-orient": Class::forName("ImageMagickDataProvider::ImageMagickAutoOrientDataProvider"),
            "strip": Class::forName("ImageMagickDataProvider::ImageMagickStripDataProvider"),
            "effects": Class::forName("ImageMagickDataProvider::ImageMagickEffectsDataProvider"),
            "color": Class::forName("ImageMagickDataProvider::ImageMagickColorDataProvider"),
            "gaussian-blur": Class::forName("ImageMagickDataProvider::ImageMagickGaussianBlurDataProvider"),
            "motion-blur": Class::forName("ImageMagickDataProvider::ImageMagickMotionBlurDataProvider"),
            "adaptive-blur": Class::forName("ImageMagickDataProvider::ImageMagickAdaptiveBlurDataProvider"),
            "rotational-blur": Class::forName("ImageMagickDataProvider::ImageMagickRotationalBlurDataProvider"),
            "adaptive-sharpen": Class::forName("ImageMagickDataProvider::ImageMagickAdaptiveSharpenDataProvider"),
            "unsharp-mask": Class::forName("ImageMagickDataProvider::ImageMagickUnsharpMaskDataProvider"),
            "despeckle": Class::forName("ImageMagickDataProvider::ImageMagickDespeckleDataProvider"),
            "reduce-noise": Class::forName("ImageMagickDataProvider::ImageMagickReduceNoiseDataProvider"),
            "wavelet-denoise": Class::forName("ImageMagickDataProvider::ImageMagickWaveletDenoiseDataProvider"),
            "trim": Class::forName("ImageMagickDataProvider::ImageMagickTrimDataProvider"),
            "border": Class::forName("ImageMagickDataProvider::ImageMagickBorderDataProvider"),
            "frame": Class::forName("ImageMagickDataProvider::ImageMagickFrameDataProvider"),
            "shear": Class::forName("ImageMagickDataProvider::ImageMagickShearDataProvider"),
            "deskew": Class::forName("ImageMagickDataProvider::ImageMagickDeskewDataProvider"),
            "scale": Class::forName("ImageMagickDataProvider::ImageMagickScaleDataProvider"),
            "resample": Class::forName("ImageMagickDataProvider::ImageMagickResampleDataProvider"),
            "extent": Class::forName("ImageMagickDataProvider::ImageMagickExtentDataProvider"),
            "selective-blur": Class::forName("ImageMagickDataProvider::ImageMagickSelectiveBlurDataProvider"),
            "liquid-rescale": Class::forName("ImageMagickDataProvider::ImageMagickLiquidRescaleDataProvider"),
            "adaptive-resize": Class::forName("ImageMagickDataProvider::ImageMagickAdaptiveResizeDataProvider"),
            "sample": Class::forName("ImageMagickDataProvider::ImageMagickSampleDataProvider"),
            "transpose": Class::forName("ImageMagickDataProvider::ImageMagickTransposeDataProvider"),
            "transverse": Class::forName("ImageMagickDataProvider::ImageMagickTransverseDataProvider"),
            "chop": Class::forName("ImageMagickDataProvider::ImageMagickChopDataProvider"),
            "shave": Class::forName("ImageMagickDataProvider::ImageMagickShaveDataProvider"),
            "splice": Class::forName("ImageMagickDataProvider::ImageMagickSpliceDataProvider"),
            "set-resolution": Class::forName("ImageMagickDataProvider::ImageMagickSetResolutionDataProvider"),
            "set-depth": Class::forName("ImageMagickDataProvider::ImageMagickSetDepthDataProvider"),
            "set-property": Class::forName("ImageMagickDataProvider::ImageMagickSetPropertyDataProvider"),
            "set-profile": Class::forName("ImageMagickDataProvider::ImageMagickSetProfileDataProvider"),
            "draw": Class::forName("ImageMagickDataProvider::ImageMagickDrawDataProvider"),
        };
    }

    constructor(*hash<auto> opts) {
        self.opts = opts;
    }

    string getName() {
        return ProviderInfo.name;
    }

    private *list<string> getChildProviderNamesImpl() {
        return keys ChildMap;
    }

    private *AbstractDataProvider getChildProviderImpl(string name) {
        *Class cls = ChildMap{name};
        if (!cls) {
            return;
        }
        return cls.newObject(opts);
    }

    private hash<DataProviderInfo> getStaticInfoImpl() {
        return ProviderInfo;
    }
}
}
