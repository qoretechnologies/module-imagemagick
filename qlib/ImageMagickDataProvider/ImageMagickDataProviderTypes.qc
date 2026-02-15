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

#! @file ImageMagickDataProviderTypes.qc Type definitions for ImageMagick data providers

public namespace ImageMagickDataProvider {
    #! Common image response fields
    const ImageResponseFields = {
        "data": {
            "type": BinaryOrNothingType,
            "display_name": "Output Image Data",
            "short_desc": "The processed image data",
            "desc": "Binary data of the processed image. Only included when 'return_data' is true "
                "(default when no output destination is specified).",
        },
        "width": {
            "type": IntType,
            "display_name": "Width",
            "short_desc": "Image width in pixels",
            "desc": "Width of the output image in pixels",
        },
        "height": {
            "type": IntType,
            "display_name": "Height",
            "short_desc": "Image height in pixels",
            "desc": "Height of the output image in pixels",
        },
        "format": {
            "type": StringType,
            "display_name": "Format",
            "short_desc": "Image format",
            "desc": "Format of the output image (e.g. PNG, JPEG)",
        },
        "size": {
            "type": IntType,
            "display_name": "Size",
            "short_desc": "Image data size in bytes",
            "desc": "Size of the output image data in bytes",
        },
        "output_path": {
            "type": StringOrNothingType,
            "display_name": "Output File Path",
            "short_desc": "Path the output image was written to",
            "desc": "File path where the output image was written, if 'output_path' was specified in "
                "the request",
        },
        "output_location": {
            "type": StringOrNothingType,
            "display_name": "Output Location",
            "short_desc": "Location the output image was written to",
            "desc": "File location where the output image was written, if 'output_location' was "
                "specified in the request",
        },
    };

    #! Common image input fields (data, input_path, or location - required group)
    const ImageInputFields = {
        "input_path": {
            "type": StringOrNothingType,
            "display_name": "Input File Path",
            "short_desc": "Path to input image file",
            "desc": "File path to read the input image from. One of 'input_path', 'data', or "
                "'location' must be provided.",
            "example_value": "/path/to/image.png",
        },
        "data": {
            "type": BinaryOrNothingType,
            "display_name": "Input Image Data",
            "short_desc": "Binary image data",
            "desc": "Binary data of the input image. One of 'input_path', 'data', or 'location' "
                "must be provided.",
        },
        "location": {
            "type": StringOrNothingType,
            "display_name": "Input Location",
            "short_desc": "Location of input image",
            "desc": "A file location string as supported by the "
                "[FileLocationHandler](https://docs.qore.org/current/modules/FileLocationHandler/html/index.html) "
                "module (e.g. `file:///path/to/image.png`, `https://example.com/image.jpg`). "
                "One of 'input_path', 'data', or 'location' must be provided.",
            "example_value": "file:///path/to/image.png",
        },
        "output_format": {
            "type": StringOrNothingType,
            "display_name": "Output Format",
            "short_desc": "Output image format",
            "desc": "Format for the output image. If not specified, the input format is preserved.",
            "allowed_values": ImageFormatAllowedValues,
        },
        "output_path": {
            "type": StringOrNothingType,
            "display_name": "Output File Path",
            "short_desc": "Path to write the output image to",
            "desc": "File path to write the processed output image to. If neither 'output_path' nor "
                "'output_location' is provided, the image data is returned in the response only.",
            "example_value": "/path/to/output.png",
        },
        "output_location": {
            "type": StringOrNothingType,
            "display_name": "Output Location",
            "short_desc": "Location to write the output image to",
            "desc": "A file location string as supported by the "
                "[FileLocationHandler](https://docs.qore.org/current/modules/FileLocationHandler/html/index.html) "
                "module (e.g. `file:///path/to/output.png`, `sftp://host/path/image.jpg`). "
                "If neither 'output_path' nor 'output_location' is provided, the image data is returned "
                "in the response only.",
            "example_value": "file:///path/to/output.png",
        },
        "return_data": {
            "type": BoolOrNothingType,
            "display_name": "Return Data",
            "short_desc": "Whether to include image data in the response",
            "desc": "Whether to include binary image data in the response hash. Defaults to `True` when "
                "no output path or location is specified, `False` when an output destination is provided.",
        },
    };
}

#! Common image response type
public class ImageMagickImageResponseDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageResponseFields);
    }
}

#! Resize request type
public class ImageMagickResizeRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "width": {
                "type": IntType,
                "display_name": "Width",
                "short_desc": "Target width in pixels",
                "desc": "Target width for the resized image in pixels",
                "example_value": 800,
            },
            "height": {
                "type": IntType,
                "display_name": "Height",
                "short_desc": "Target height in pixels",
                "desc": "Target height for the resized image in pixels",
                "example_value": 600,
            },
            "filter": {
                "type": StringOrNothingType,
                "display_name": "Filter",
                "short_desc": "Resize filter algorithm",
                "desc": "Filter algorithm to use for resizing. Default: Lanczos",
                "default_value": "Lanczos",
                "allowed_values": FilterTypeAllowedValues,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Crop request type
public class ImageMagickCropRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "width": {
                "type": IntType,
                "display_name": "Width",
                "short_desc": "Crop width in pixels",
                "desc": "Width of the cropped region in pixels",
            },
            "height": {
                "type": IntType,
                "display_name": "Height",
                "short_desc": "Crop height in pixels",
                "desc": "Height of the cropped region in pixels",
            },
            "x": {
                "type": IntType,
                "display_name": "X Offset",
                "short_desc": "Left offset in pixels",
                "desc": "Horizontal offset from the left edge",
                "default_value": 0,
            },
            "y": {
                "type": IntType,
                "display_name": "Y Offset",
                "short_desc": "Top offset in pixels",
                "desc": "Vertical offset from the top edge",
                "default_value": 0,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Rotate request type
public class ImageMagickRotateRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "degrees": {
                "type": FloatType,
                "display_name": "Degrees",
                "short_desc": "Rotation angle in degrees",
                "desc": "Clockwise rotation angle in degrees",
                "example_value": 90.0,
            },
            "background": {
                "type": StringOrNothingType,
                "display_name": "Background Color",
                "short_desc": "Background color for exposed areas",
                "desc": "Color for areas exposed by rotation. Default: white",
                "default_value": "white",
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Flip request type
public class ImageMagickFlipRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "direction": {
                "type": StringType,
                "display_name": "Direction",
                "short_desc": "Flip direction",
                "desc": "Direction to flip: 'vertical' (top to bottom) or 'horizontal' (left to right)",
                "default_value": "vertical",
                "allowed_values": FlipDirectionAllowedValues,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Blur request type
public class ImageMagickBlurRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Blur radius (0 for auto)",
                "desc": "Blur radius in pixels. Use 0 for automatic selection.",
                "default_value": 0.0,
            },
            "sigma": {
                "type": FloatType,
                "display_name": "Sigma",
                "short_desc": "Blur standard deviation",
                "desc": "Standard deviation of the Gaussian blur",
                "example_value": 2.0,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Sharpen request type
public class ImageMagickSharpenRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Sharpen radius (0 for auto)",
                "desc": "Sharpening radius in pixels. Use 0 for automatic selection.",
                "default_value": 0.0,
            },
            "sigma": {
                "type": FloatType,
                "display_name": "Sigma",
                "short_desc": "Sharpen standard deviation",
                "desc": "Standard deviation of the sharpening",
                "example_value": 1.0,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Brightness/contrast request type
public class ImageMagickBrightnessContrastRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "brightness": {
                "type": FloatType,
                "display_name": "Brightness",
                "short_desc": "Brightness adjustment (-100 to 100)",
                "desc": "Brightness adjustment value. Negative values darken, positive values brighten.",
                "default_value": 0.0,
            },
            "contrast": {
                "type": FloatType,
                "display_name": "Contrast",
                "short_desc": "Contrast adjustment (-100 to 100)",
                "desc": "Contrast adjustment value. Negative values reduce, positive values increase contrast.",
                "default_value": 0.0,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Thumbnail request type
public class ImageMagickThumbnailRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "width": {
                "type": IntType,
                "display_name": "Width",
                "short_desc": "Thumbnail width in pixels",
                "desc": "Target width for the thumbnail in pixels",
                "example_value": 200,
            },
            "height": {
                "type": IntType,
                "display_name": "Height",
                "short_desc": "Thumbnail height in pixels",
                "desc": "Target height for the thumbnail in pixels",
                "example_value": 200,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Auto orient request type
public class ImageMagickAutoOrientRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Strip request type
public class ImageMagickStripRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Charcoal effect request type
public class ImageMagickCharcoalRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Effect radius",
                "desc": "Charcoal effect radius in pixels",
                "default_value": 1.0,
            },
            "sigma": {
                "type": FloatOrNothingType,
                "display_name": "Sigma",
                "short_desc": "Standard deviation",
                "desc": "Standard deviation for the charcoal effect",
                "default_value": 1.0,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Sepia effect request type
public class ImageMagickSepiaRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "threshold": {
                "type": FloatOrNothingType,
                "display_name": "Threshold",
                "short_desc": "Sepia threshold",
                "desc": "Sepia tone threshold (try 80% of quantum range, e.g. 52000)",
                "default_value": 52000.0,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Oil paint effect request type
public class ImageMagickOilPaintRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Effect radius",
                "desc": "Oil painting effect radius",
                "default_value": 3.0,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Edge effect request type
public class ImageMagickEdgeRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Edge detection radius",
                "desc": "Edge detection radius in pixels",
                "default_value": 1.0,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Emboss effect request type
public class ImageMagickEmbossRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Effect radius",
                "desc": "Emboss effect radius",
                "default_value": 0.0,
            },
            "sigma": {
                "type": FloatOrNothingType,
                "display_name": "Sigma",
                "short_desc": "Standard deviation",
                "desc": "Standard deviation for the emboss effect",
                "default_value": 1.0,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Convert format request type
public class ImageMagickConvertFormatRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "format": {
                "type": StringType,
                "display_name": "Target Format",
                "short_desc": "Target image format",
                "desc": "Format to convert the image to",
                "allowed_values": ImageFormatAllowedValues,
                "example_value": "JPEG",
            },
            "quality": {
                "type": IntOrNothingType,
                "display_name": "Quality",
                "short_desc": "Compression quality (0-100)",
                "desc": "Compression quality for lossy formats (0-100). Higher is better quality.",
                "default_value": 85,
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Composite request type
public class ImageMagickCompositeRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "overlay_path": {
                "type": StringOrNothingType,
                "display_name": "Overlay File Path",
                "short_desc": "Path to overlay image file",
                "desc": "File path to read the overlay image from. One of 'overlay_path', "
                    "'overlay_data', or 'overlay_location' must be provided.",
            },
            "overlay_data": {
                "type": BinaryOrNothingType,
                "display_name": "Overlay Image Data",
                "short_desc": "Binary overlay image data",
                "desc": "Binary data of the overlay image. One of 'overlay_path', 'overlay_data', "
                    "or 'overlay_location' must be provided.",
            },
            "overlay_location": {
                "type": StringOrNothingType,
                "display_name": "Overlay Location",
                "short_desc": "Location of overlay image",
                "desc": "A file location string for the overlay image as supported by the "
                    "[FileLocationHandler](https://docs.qore.org/current/modules/FileLocationHandler/html/index.html) "
                    "module. One of 'overlay_path', 'overlay_data', or 'overlay_location' must be provided.",
                "example_value": "file:///path/to/overlay.png",
            },
            "operator": {
                "type": StringOrNothingType,
                "display_name": "Composite Operator",
                "short_desc": "Blending operator",
                "desc": "Composite blending operator. Default: Over",
                "default_value": "Over",
                "allowed_values": CompositeOpAllowedValues,
            },
            "gravity": {
                "type": StringOrNothingType,
                "display_name": "Gravity",
                "short_desc": "Placement gravity",
                "desc": "Gravity for placing the overlay image. If specified, x and y offsets are ignored.",
                "allowed_values": GravityTypeAllowedValues,
            },
            "x": {
                "type": IntOrNothingType,
                "display_name": "X Offset",
                "short_desc": "Horizontal offset in pixels",
                "desc": "Horizontal offset for overlay placement (used when gravity is not specified)",
                "default_value": 0,
            },
            "y": {
                "type": IntOrNothingType,
                "display_name": "Y Offset",
                "short_desc": "Vertical offset in pixels",
                "desc": "Vertical offset for overlay placement (used when gravity is not specified)",
                "default_value": 0,
            },
        });
        setRequiredGroups((("input_path", "data", "location"), ("overlay_path", "overlay_data", "overlay_location")));
    }
}

#! Annotate request type
public class ImageMagickAnnotateRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageInputFields);
        addQoreFields({
            "text": {
                "type": StringType,
                "display_name": "Text",
                "short_desc": "Text to draw on the image",
                "desc": "The text string to annotate on the image",
                "example_value": "Copyright 2026",
            },
            "x": {
                "type": FloatType,
                "display_name": "X Position",
                "short_desc": "Horizontal text position",
                "desc": "Horizontal position for the text in pixels",
                "example_value": 10.0,
            },
            "y": {
                "type": FloatType,
                "display_name": "Y Position",
                "short_desc": "Vertical text position",
                "desc": "Vertical position for the text in pixels",
                "example_value": 30.0,
            },
            "angle": {
                "type": FloatOrNothingType,
                "display_name": "Angle",
                "short_desc": "Text rotation angle",
                "desc": "Rotation angle for the text in degrees",
                "default_value": 0.0,
            },
            "font": {
                "type": StringOrNothingType,
                "display_name": "Font",
                "short_desc": "Font name",
                "desc": "Font name or path to font file",
                "default_value": "DejaVu-Sans",
            },
            "font_size": {
                "type": FloatOrNothingType,
                "display_name": "Font Size",
                "short_desc": "Font size in points",
                "desc": "Font size in points",
                "default_value": 12.0,
            },
            "fill_color": {
                "type": StringOrNothingType,
                "display_name": "Fill Color",
                "short_desc": "Text fill color",
                "desc": "Fill color for the text (e.g. 'black', 'white', '#FF0000')",
                "default_value": "black",
            },
            "stroke_color": {
                "type": StringOrNothingType,
                "display_name": "Stroke Color",
                "short_desc": "Text stroke color",
                "desc": "Stroke (outline) color for the text",
            },
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Get info request type
public class ImageMagickGetInfoRequestDataType inherits HashDataType {
    constructor() {
        addQoreFields({
            "input_path": ImageInputFields."input_path",
            "data": ImageInputFields."data",
            "location": ImageInputFields."location",
        });
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Get info response type
public class ImageMagickGetInfoResponseDataType inherits HashDataType {
    constructor() {
        addQoreFields({
            "width": {
                "type": IntType,
                "display_name": "Width",
                "short_desc": "Image width in pixels",
                "desc": "Width of the image in pixels",
            },
            "height": {
                "type": IntType,
                "display_name": "Height",
                "short_desc": "Image height in pixels",
                "desc": "Height of the image in pixels",
            },
            "format": {
                "type": StringType,
                "display_name": "Format",
                "short_desc": "Image format",
                "desc": "Image format (e.g. PNG, JPEG, GIF)",
            },
            "depth": {
                "type": IntType,
                "display_name": "Depth",
                "short_desc": "Bits per channel",
                "desc": "Image depth in bits per channel",
            },
            "colorspace": {
                "type": StringType,
                "display_name": "Colorspace",
                "short_desc": "Image colorspace",
                "desc": "Image colorspace (e.g. sRGB, CMYK, Gray)",
            },
            "image_type": {
                "type": StringType,
                "display_name": "Image Type",
                "short_desc": "Image type classification",
                "desc": "Image type (e.g. TrueColor, Palette, Grayscale)",
            },
            "compression_quality": {
                "type": IntType,
                "display_name": "Compression Quality",
                "short_desc": "Compression quality",
                "desc": "Compression quality (0-100, format dependent)",
            },
            "number_images": {
                "type": IntType,
                "display_name": "Number of Images",
                "short_desc": "Number of images in the file",
                "desc": "Number of images (for multi-image formats like animated GIF)",
            },
            "colors": {
                "type": IntType,
                "display_name": "Colors",
                "short_desc": "Number of unique colors",
                "desc": "Number of unique colors in the image",
            },
            "signature": {
                "type": StringType,
                "display_name": "Signature",
                "short_desc": "Cryptographic signature",
                "desc": "Cryptographic hash signature of the image",
            },
        });
    }
}
