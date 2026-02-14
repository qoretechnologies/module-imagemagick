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

#! @file ImageMagickImageActionBase.qc Base class for image action data providers

#! Base class for ImageMagick image action providers
/** Provides common functionality for loading images and returning results.
*/
public class ImageMagickImageActionBase inherits AbstractDataProvider {
    #! Loads an image from a request hash (input_path or data)
    static MagickImage loadImage(hash<auto> req) {
        if (req.input_path) {
            return new MagickImage(req.input_path);
        }
        return new MagickImage(req.data);
    }

    #! Returns the image as a response hash
    static hash<auto> makeResponse(MagickImage img, *string output_format) {
        if (output_format) {
            img.setFormat(output_format);
        }
        binary data = img.toData();
        return {
            "data": data,
            "width": img.width(),
            "height": img.height(),
            "format": img.format(),
            "size": data.size(),
        };
    }
}
