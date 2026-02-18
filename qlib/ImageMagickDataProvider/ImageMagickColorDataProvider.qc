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

#! @file ImageMagickColorDataProvider.qc Color operations container

public namespace ImageMagickDataProvider {

#! Container for color adjustment operations
public class ImageMagickColorDataProvider inherits AbstractDataProvider {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "color",
            "desc": "Color adjustment operations",
            "type": "ImageMagickColorDataProvider",
            "supports_children": True,
            "children_can_support_apis": True,
        };
    }

    private {
        *hash<auto> opts;

        #! Child data providers
        const ChildMap = {
            "negate": Class::forName("ImageMagickDataProvider::ImageMagickNegateDataProvider"),
            "normalize": Class::forName("ImageMagickDataProvider::ImageMagickNormalizeDataProvider"),
            "equalize": Class::forName("ImageMagickDataProvider::ImageMagickEqualizeDataProvider"),
            "auto-gamma": Class::forName("ImageMagickDataProvider::ImageMagickAutoGammaDataProvider"),
            "auto-level": Class::forName("ImageMagickDataProvider::ImageMagickAutoLevelDataProvider"),
            "enhance": Class::forName("ImageMagickDataProvider::ImageMagickEnhanceDataProvider"),
            "white-balance": Class::forName("ImageMagickDataProvider::ImageMagickWhiteBalanceDataProvider"),
            "gamma": Class::forName("ImageMagickDataProvider::ImageMagickGammaDataProvider"),
            "modulate": Class::forName("ImageMagickDataProvider::ImageMagickModulateDataProvider"),
            "level": Class::forName("ImageMagickDataProvider::ImageMagickLevelDataProvider"),
            "contrast-stretch": Class::forName("ImageMagickDataProvider::ImageMagickContrastStretchDataProvider"),
            "linear-stretch": Class::forName("ImageMagickDataProvider::ImageMagickLinearStretchDataProvider"),
            "sigmoidal-contrast": Class::forName("ImageMagickDataProvider::ImageMagickSigmoidalContrastDataProvider"),
            "transform-colorspace": Class::forName("ImageMagickDataProvider::ImageMagickTransformColorspaceDataProvider"),
            "threshold": Class::forName("ImageMagickDataProvider::ImageMagickThresholdDataProvider"),
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
