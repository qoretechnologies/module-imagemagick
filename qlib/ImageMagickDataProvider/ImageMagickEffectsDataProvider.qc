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

#! @file ImageMagickEffectsDataProvider.qc Effects container

public namespace ImageMagickDataProvider {

#! Container for image effects
public class ImageMagickEffectsDataProvider inherits AbstractDataProvider {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "effects",
            "desc": "Image effect operations",
            "type": "ImageMagickEffectsDataProvider",
            "supports_children": True,
            "children_can_support_apis": True,
        };
    }

    private {
        *hash<auto> opts;

        #! Child data providers
        const ChildMap = {
            "charcoal": Class::forName("ImageMagickDataProvider::ImageMagickCharcoalDataProvider"),
            "sepia": Class::forName("ImageMagickDataProvider::ImageMagickSepiaDataProvider"),
            "oil-paint": Class::forName("ImageMagickDataProvider::ImageMagickOilPaintDataProvider"),
            "edge": Class::forName("ImageMagickDataProvider::ImageMagickEdgeDataProvider"),
            "emboss": Class::forName("ImageMagickDataProvider::ImageMagickEmbossDataProvider"),
            "solarize": Class::forName("ImageMagickDataProvider::ImageMagickSolarizeDataProvider"),
            "posterize": Class::forName("ImageMagickDataProvider::ImageMagickPosterizeDataProvider"),
            "vignette": Class::forName("ImageMagickDataProvider::ImageMagickVignetteDataProvider"),
            "sketch": Class::forName("ImageMagickDataProvider::ImageMagickSketchDataProvider"),
            "wave": Class::forName("ImageMagickDataProvider::ImageMagickWaveDataProvider"),
            "swirl": Class::forName("ImageMagickDataProvider::ImageMagickSwirlDataProvider"),
            "implode": Class::forName("ImageMagickDataProvider::ImageMagickImplodeDataProvider"),
            "shadow": Class::forName("ImageMagickDataProvider::ImageMagickShadowDataProvider"),
            "shade": Class::forName("ImageMagickDataProvider::ImageMagickShadeDataProvider"),
            "add-noise": Class::forName("ImageMagickDataProvider::ImageMagickAddNoiseDataProvider"),
            "blue-shift": Class::forName("ImageMagickDataProvider::ImageMagickBlueShiftDataProvider"),
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
