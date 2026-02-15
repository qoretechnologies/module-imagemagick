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
    #! Loads an image from a request hash (input_path, data, or location)
    static MagickImage loadImage(hash<auto> req) {
        if (req.input_path) {
            return new MagickImage(req.input_path);
        }
        if (req.location) {
            return new MagickImage(FileLocationHandler::getBinaryFileFromLocation(req.location));
        }
        return new MagickImage(req.data);
    }

    #! Returns the image as a response hash with optional output writing
    static hash<auto> makeResponse(MagickImage img, hash<auto> req) {
        if (req.output_format) {
            img.setFormat(req.output_format);
        }
        binary data = img.toData();
        # Write to output_path if specified
        if (req.output_path) {
            {
                File f();
                f.open2(req.output_path, O_CREAT | O_WRONLY | O_TRUNC);
                f.write(data);
            }
        }
        # Write to output_location if specified
        if (req.output_location) {
            FileLocationHandler::writeFileToLocation(req.output_location, data);
        }
        bool has_output = exists req.output_path || exists req.output_location;
        bool do_return_data = exists req.return_data ? req.return_data : !has_output;
        hash<auto> response = {
            "width": img.width(),
            "height": img.height(),
            "format": img.format(),
            "size": data.size(),
        };
        if (do_return_data) {
            response."data" = data;
        }
        if (req.output_path) {
            response."output_path" = req.output_path;
        }
        if (req.output_location) {
            response."output_location" = req.output_location;
        }
        return response;
    }
}
