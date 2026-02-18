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

#! @file ImageMagickDataProvider.qc Root ImageMagick data provider

#! Contains all public definitions in the ImageMagickDataProvider module
public namespace ImageMagickDataProvider {

#! Root ImageMagick data provider
public class ImageMagickDataProvider inherits AbstractDataProvider {
    public {
        const ProviderInfo = <DataProviderInfo>{
            "name": "imagemagick",
            "desc": "ImageMagick image processing data provider",
            "type": "ImageMagickDataProvider",
            "supports_children": True,
            "children_can_support_apis": True,
        };
    }

    private {
        *hash<auto> opts;

        #! Child data providers
        const ChildMap = {
            "image": Class::forName("ImageMagickDataProvider::ImageMagickImageDataProvider"),
            "convert": Class::forName("ImageMagickDataProvider::ImageMagickConvertDataProvider"),
            "info": Class::forName("ImageMagickDataProvider::ImageMagickInfoDataProvider"),
        };
    }

    constructor(*hash<auto> opts) {
        self.opts = opts;
    }

    string getName() {
        return ProviderInfo.name;
    }

    *string getDesc() {
        return ProviderInfo.desc;
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
