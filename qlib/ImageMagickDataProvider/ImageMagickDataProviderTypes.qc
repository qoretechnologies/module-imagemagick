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

#! Common image response type
public class ImageMagickImageResponseDataType inherits HashDataType {
    constructor() {
        addQoreFields(ImageResponseFields);
    }
}

#! Resize request type
public class ImageMagickResizeRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
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
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Crop request type
public class ImageMagickCropRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "width": {
                "type": IntType,
                "display_name": "Width",
                "short_desc": "Crop width in pixels",
                "desc": "Width of the cropped region in pixels",
                "example_value": 400,
            },
            "height": {
                "type": IntType,
                "display_name": "Height",
                "short_desc": "Crop height in pixels",
                "desc": "Height of the cropped region in pixels",
                "example_value": 300,
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
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Rotate request type
public class ImageMagickRotateRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
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
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Flip request type
public class ImageMagickFlipRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "direction": {
                "type": StringType,
                "display_name": "Direction",
                "short_desc": "Flip direction",
                "desc": "Direction to flip: 'vertical' (top to bottom) or 'horizontal' (left to right)",
                "default_value": "vertical",
                "allowed_values": FlipDirectionAllowedValues,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Blur request type
public class ImageMagickBlurRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
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
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Sharpen request type
public class ImageMagickSharpenRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
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
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Brightness/contrast request type
public class ImageMagickBrightnessContrastRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "brightness": {
                "type": FloatType,
                "display_name": "Brightness",
                "short_desc": "Brightness adjustment (-100 to 100)",
                "desc": "Brightness adjustment value. Negative values darken, positive values brighten.",
                "default_value": 0.0,
                "example_value": 10.0,
            },
            "contrast": {
                "type": FloatType,
                "display_name": "Contrast",
                "short_desc": "Contrast adjustment (-100 to 100)",
                "desc": "Contrast adjustment value. Negative values reduce, positive values increase contrast.",
                "default_value": 0.0,
                "example_value": 10.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Thumbnail request type
public class ImageMagickThumbnailRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
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
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Auto orient request type
public class ImageMagickAutoOrientRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields;
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Strip request type
public class ImageMagickStripRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields;
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Charcoal effect request type
public class ImageMagickCharcoalRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Effect radius",
                "desc": "Charcoal effect radius in pixels",
                "default_value": 1.0,
                "example_value": 1.0,
            },
            "sigma": {
                "type": FloatOrNothingType,
                "display_name": "Sigma",
                "short_desc": "Standard deviation",
                "desc": "Standard deviation for the charcoal effect",
                "default_value": 1.0,
                "example_value": 1.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Sepia effect request type
public class ImageMagickSepiaRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "threshold": {
                "type": FloatOrNothingType,
                "display_name": "Threshold",
                "short_desc": "Sepia threshold",
                "desc": "Sepia tone threshold as a fraction of the quantum range. Higher values "
                    "produce a stronger sepia effect. A value of approximately 80% of the quantum "
                    "range (e.g. 52000 for a 16-bit quantum depth) gives a classic warm sepia tone.",
                "default_value": 52000.0,
                "example_value": 52000.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Oil paint effect request type
public class ImageMagickOilPaintRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Effect radius",
                "desc": "Oil painting effect radius",
                "default_value": 3.0,
                "example_value": 3.0,
            },
            "sigma": {
                "type": FloatOrNothingType,
                "display_name": "Sigma",
                "short_desc": "Standard deviation",
                "desc": "Standard deviation for the oil painting effect",
                "default_value": 1.0,
                "example_value": 1.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Edge effect request type
public class ImageMagickEdgeRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Edge detection radius",
                "desc": "Edge detection radius in pixels",
                "default_value": 1.0,
                "example_value": 1.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Emboss effect request type
public class ImageMagickEmbossRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Effect radius",
                "desc": "Emboss effect radius",
                "default_value": 0.0,
                "example_value": 0.0,
            },
            "sigma": {
                "type": FloatOrNothingType,
                "display_name": "Sigma",
                "short_desc": "Standard deviation",
                "desc": "Standard deviation for the emboss effect",
                "default_value": 1.0,
                "example_value": 1.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Convert format request type
public class ImageMagickConvertFormatRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
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
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Composite request type
public class ImageMagickCompositeRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "overlay_path": {
                "type": StringOrNothingType,
                "display_name": "Overlay File Path",
                "short_desc": "Path to overlay image file",
                "desc": "File path to read the overlay image from. One of 'overlay_path', "
                    "'overlay_data', or 'overlay_location' must be provided.",
                "example_value": "/path/to/overlay.png",
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
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"), ("overlay_path", "overlay_data", "overlay_location")));
    }
}

#! Annotate request type
public class ImageMagickAnnotateRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
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
                "example_value": "white",
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Get info request type
public class ImageMagickGetInfoRequestDataType inherits HashDataType {
    public {
        const Fields = {
            "input_path": ImageInputFields."input_path",
            "data": ImageInputFields."data",
            "location": ImageInputFields."location",
        };
    }

    constructor() {
        addQoreFields(Fields);
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
                "example_value": 1920,
            },
            "height": {
                "type": IntType,
                "display_name": "Height",
                "short_desc": "Image height in pixels",
                "desc": "Height of the image in pixels",
                "example_value": 1080,
            },
            "format": {
                "type": StringType,
                "display_name": "Format",
                "short_desc": "Image format",
                "desc": "Image format (e.g. PNG, JPEG, GIF)",
                "example_value": "PNG",
            },
            "depth": {
                "type": IntType,
                "display_name": "Depth",
                "short_desc": "Bits per channel",
                "desc": "Image depth in bits per channel",
                "example_value": 8,
            },
            "colorspace": {
                "type": StringType,
                "display_name": "Colorspace",
                "short_desc": "Image colorspace",
                "desc": "Image colorspace (e.g. sRGB, CMYK, Gray)",
                "example_value": "sRGB",
            },
            "image_type": {
                "type": StringType,
                "display_name": "Image Type",
                "short_desc": "Image type classification",
                "desc": "Image type (e.g. TrueColor, Palette, Grayscale)",
                "example_value": "TrueColor",
            },
            "compression_quality": {
                "type": IntType,
                "display_name": "Compression Quality",
                "short_desc": "Compression quality",
                "desc": "Compression quality (0-100, format dependent)",
                "example_value": 85,
            },
            "number_images": {
                "type": IntType,
                "display_name": "Number of Images",
                "short_desc": "Number of images in the file",
                "desc": "Number of images (for multi-image formats like animated GIF)",
                "example_value": 1,
            },
            "colors": {
                "type": IntType,
                "display_name": "Colors",
                "short_desc": "Number of unique colors",
                "desc": "Number of unique colors in the image",
                "example_value": 65536,
            },
            "signature": {
                "type": StringType,
                "display_name": "Signature",
                "short_desc": "Cryptographic signature",
                "desc": "Cryptographic hash signature of the image",
                "example_value": "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
            },
        });
    }
}

# ==================== Color Adjustment Request Types ====================

#! Negate request type
public class ImageMagickNegateRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "gray_only": {
                "type": BoolOrNothingType,
                "display_name": "Gray Only",
                "short_desc": "Only negate grayscale values",
                "desc": "When true, only negate the grayscale values in the image",
                "default_value": False,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Normalize request type (no extra params)
public class ImageMagickNormalizeRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields;
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Equalize request type (no extra params)
public class ImageMagickEqualizeRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields;
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Auto gamma request type (no extra params)
public class ImageMagickAutoGammaRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields;
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Auto level request type (no extra params)
public class ImageMagickAutoLevelRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields;
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Enhance request type (no extra params)
public class ImageMagickEnhanceRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields;
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! White balance request type (no extra params)
public class ImageMagickWhiteBalanceRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields;
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Gamma request type
public class ImageMagickGammaRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "gamma_value": {
                "type": FloatType,
                "display_name": "Gamma Value",
                "short_desc": "Gamma correction value",
                "desc": "Gamma correction value. Values less than 1.0 darken the image, "
                    "values greater than 1.0 brighten the image.",
                "example_value": 1.5,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Modulate request type
public class ImageMagickModulateRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "brightness": {
                "type": FloatOrNothingType,
                "display_name": "Brightness",
                "short_desc": "Brightness percentage (100 = no change)",
                "desc": "Brightness as a percentage. 100.0 means no change, less than 100 darkens, "
                    "greater than 100 brightens.",
                "default_value": 100.0,
                "example_value": 110.0,
            },
            "saturation": {
                "type": FloatOrNothingType,
                "display_name": "Saturation",
                "short_desc": "Saturation percentage (100 = no change)",
                "desc": "Saturation as a percentage. 100.0 means no change, 0 is fully desaturated, "
                    "greater than 100 increases saturation.",
                "default_value": 100.0,
                "example_value": 120.0,
            },
            "hue": {
                "type": FloatOrNothingType,
                "display_name": "Hue",
                "short_desc": "Hue rotation percentage (100 = no change)",
                "desc": "Hue rotation as a percentage. 100.0 means no change.",
                "default_value": 100.0,
                "example_value": 100.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Level request type
public class ImageMagickLevelRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "black_point": {
                "type": FloatType,
                "display_name": "Black Point",
                "short_desc": "Black point level",
                "desc": "Black point level for input mapping",
                "example_value": 0.0,
            },
            "gamma_value": {
                "type": FloatType,
                "display_name": "Gamma",
                "short_desc": "Gamma correction value",
                "desc": "Gamma correction value for the level adjustment",
                "example_value": 1.0,
            },
            "white_point": {
                "type": FloatType,
                "display_name": "White Point",
                "short_desc": "White point level",
                "desc": "White point level for input mapping",
                "example_value": 65535.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Contrast stretch request type
public class ImageMagickContrastStretchRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "black_point": {
                "type": FloatType,
                "display_name": "Black Point",
                "short_desc": "Percentage of pixels for black point",
                "desc": "Percentage of pixels to map to black (0.0-100.0)",
                "example_value": 1.0,
            },
            "white_point": {
                "type": FloatType,
                "display_name": "White Point",
                "short_desc": "Percentage of pixels for white point",
                "desc": "Percentage of pixels to map to white (0.0-100.0)",
                "example_value": 1.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Linear stretch request type
public class ImageMagickLinearStretchRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "black_point": {
                "type": FloatType,
                "display_name": "Black Point",
                "short_desc": "Percentage of pixels for black point",
                "desc": "Percentage of pixels to map to black (0.0-100.0)",
                "example_value": 2.0,
            },
            "white_point": {
                "type": FloatType,
                "display_name": "White Point",
                "short_desc": "Percentage of pixels for white point",
                "desc": "Percentage of pixels to map to white (0.0-100.0)",
                "example_value": 2.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Sigmoidal contrast request type
public class ImageMagickSigmoidalContrastRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "sharpen": {
                "type": BoolOrNothingType,
                "display_name": "Sharpen",
                "short_desc": "Increase contrast when true",
                "desc": "When true, increases contrast; when false, decreases contrast",
                "default_value": True,
            },
            "strength": {
                "type": FloatType,
                "display_name": "Strength",
                "short_desc": "Contrast strength",
                "desc": "Strength of the contrast adjustment (typically 3.0-20.0)",
                "example_value": 5.0,
            },
            "midpoint": {
                "type": FloatType,
                "display_name": "Midpoint",
                "short_desc": "Midpoint of the contrast curve",
                "desc": "Midpoint of the sigmoidal contrast curve as a percentage of the quantum range",
                "example_value": 50.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Transform colorspace request type
public class ImageMagickTransformColorspaceRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "colorspace": {
                "type": StringType,
                "display_name": "Colorspace",
                "short_desc": "Target colorspace",
                "desc": "Target colorspace to transform the image to",
                "allowed_values": ColorspaceTypeAllowedValues,
                "example_value": "Gray",
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Threshold request type
public class ImageMagickThresholdRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "threshold_value": {
                "type": FloatType,
                "display_name": "Threshold Value",
                "short_desc": "Binary threshold value",
                "desc": "Threshold value for binary black/white conversion. Pixels above this value "
                    "become white, pixels below become black.",
                "example_value": 32768.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

# ==================== Effects Request Types ====================

#! Solarize request type
public class ImageMagickSolarizeRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "threshold": {
                "type": FloatType,
                "display_name": "Threshold",
                "short_desc": "Solarize threshold",
                "desc": "Threshold for the solarize effect. Pixels above this value are negated.",
                "example_value": 32768.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Posterize request type
public class ImageMagickPosterizeRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "levels": {
                "type": IntType,
                "display_name": "Levels",
                "short_desc": "Number of color levels",
                "desc": "Number of color levels per channel (e.g. 4 reduces to 4 levels per channel)",
                "example_value": 4,
            },
            "dither": {
                "type": BoolOrNothingType,
                "display_name": "Dither",
                "short_desc": "Apply dithering",
                "desc": "Apply dithering to reduce banding artifacts",
                "default_value": False,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Vignette request type
public class ImageMagickVignetteRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "radius": {
                "type": FloatType,
                "display_name": "Radius",
                "short_desc": "Vignette radius",
                "desc": "Radius of the vignette effect in pixels",
                "example_value": 10.0,
            },
            "sigma": {
                "type": FloatType,
                "display_name": "Sigma",
                "short_desc": "Standard deviation",
                "desc": "Standard deviation for the vignette blur",
                "example_value": 5.0,
            },
            "x": {
                "type": IntOrNothingType,
                "display_name": "X Offset",
                "short_desc": "Horizontal offset",
                "desc": "Horizontal offset for the vignette center",
                "default_value": 0,
            },
            "y": {
                "type": IntOrNothingType,
                "display_name": "Y Offset",
                "short_desc": "Vertical offset",
                "desc": "Vertical offset for the vignette center",
                "default_value": 0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Sketch request type
public class ImageMagickSketchRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "radius": {
                "type": FloatType,
                "display_name": "Radius",
                "short_desc": "Sketch radius",
                "desc": "Radius of the pencil sketch effect in pixels",
                "example_value": 0.0,
            },
            "sigma": {
                "type": FloatType,
                "display_name": "Sigma",
                "short_desc": "Standard deviation",
                "desc": "Standard deviation for the sketch effect",
                "example_value": 1.0,
            },
            "angle": {
                "type": FloatType,
                "display_name": "Angle",
                "short_desc": "Sketch angle in degrees",
                "desc": "Angle of the pencil strokes in degrees",
                "example_value": 45.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Wave request type
public class ImageMagickWaveRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "amplitude": {
                "type": FloatType,
                "display_name": "Amplitude",
                "short_desc": "Wave amplitude in pixels",
                "desc": "Amplitude of the wave distortion in pixels",
                "example_value": 5.0,
            },
            "wavelength": {
                "type": FloatType,
                "display_name": "Wavelength",
                "short_desc": "Wave wavelength in pixels",
                "desc": "Wavelength of the wave distortion in pixels",
                "example_value": 25.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Swirl request type
public class ImageMagickSwirlRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "degrees": {
                "type": FloatType,
                "display_name": "Degrees",
                "short_desc": "Swirl angle in degrees",
                "desc": "Angle of the swirl distortion in degrees",
                "example_value": 90.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Implode request type
public class ImageMagickImplodeRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "amount": {
                "type": FloatType,
                "display_name": "Amount",
                "short_desc": "Implode amount",
                "desc": "Amount of implosion. Positive values implode toward the center, "
                    "negative values explode outward.",
                "example_value": 0.5,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Shadow request type
public class ImageMagickShadowRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "alpha": {
                "type": FloatType,
                "display_name": "Alpha",
                "short_desc": "Shadow opacity (0.0-1.0)",
                "desc": "Opacity of the shadow (0.0 = fully transparent, 1.0 = fully opaque)",
                "example_value": 0.5,
            },
            "sigma": {
                "type": FloatType,
                "display_name": "Sigma",
                "short_desc": "Shadow blur standard deviation",
                "desc": "Standard deviation for the shadow blur",
                "example_value": 5.0,
            },
            "x": {
                "type": IntOrNothingType,
                "display_name": "X Offset",
                "short_desc": "Horizontal shadow offset",
                "desc": "Horizontal offset for the shadow in pixels",
                "default_value": 0,
                "example_value": 5,
            },
            "y": {
                "type": IntOrNothingType,
                "display_name": "Y Offset",
                "short_desc": "Vertical shadow offset",
                "desc": "Vertical offset for the shadow in pixels",
                "default_value": 0,
                "example_value": 5,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Shade request type
public class ImageMagickShadeRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "gray": {
                "type": BoolOrNothingType,
                "display_name": "Grayscale",
                "short_desc": "Output as grayscale",
                "desc": "When true, output is a grayscale shaded image; when false, shading is "
                    "applied to the color image",
                "default_value": True,
            },
            "azimuth": {
                "type": FloatType,
                "display_name": "Azimuth",
                "short_desc": "Light source azimuth in degrees",
                "desc": "Azimuth angle of the light source in degrees (0-360)",
                "example_value": 135.0,
            },
            "elevation": {
                "type": FloatType,
                "display_name": "Elevation",
                "short_desc": "Light source elevation in degrees",
                "desc": "Elevation angle of the light source in degrees (0-90)",
                "example_value": 30.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Add noise request type
public class ImageMagickAddNoiseRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "noise_type": {
                "type": StringType,
                "display_name": "Noise Type",
                "short_desc": "Type of noise to add",
                "desc": "The type of noise to add to the image",
                "allowed_values": NoiseTypeAllowedValues,
                "example_value": "Gaussian",
            },
            "attenuate": {
                "type": FloatOrNothingType,
                "display_name": "Attenuate",
                "short_desc": "Noise attenuation factor",
                "desc": "Attenuation factor for the noise. Lower values produce less noise.",
                "default_value": 1.0,
                "example_value": 1.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Blue shift request type
public class ImageMagickBlueShiftRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "factor": {
                "type": FloatOrNothingType,
                "display_name": "Factor",
                "short_desc": "Blue shift factor",
                "desc": "Factor for the moonlight/blue shift simulation. Higher values produce a "
                    "stronger effect.",
                "default_value": 1.5,
                "example_value": 1.5,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

# ==================== Blur/Sharpen/Denoise Request Types ====================

#! Gaussian blur request type
public class ImageMagickGaussianBlurRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Blur radius (0 for auto)",
                "desc": "Gaussian blur radius in pixels. Use 0 for automatic selection.",
                "default_value": 0.0,
            },
            "sigma": {
                "type": FloatType,
                "display_name": "Sigma",
                "short_desc": "Standard deviation",
                "desc": "Standard deviation of the Gaussian blur",
                "example_value": 2.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Motion blur request type
public class ImageMagickMotionBlurRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "radius": {
                "type": FloatType,
                "display_name": "Radius",
                "short_desc": "Blur radius",
                "desc": "Motion blur radius in pixels",
                "example_value": 0.0,
            },
            "sigma": {
                "type": FloatType,
                "display_name": "Sigma",
                "short_desc": "Standard deviation",
                "desc": "Standard deviation for the motion blur",
                "example_value": 5.0,
            },
            "angle": {
                "type": FloatType,
                "display_name": "Angle",
                "short_desc": "Motion angle in degrees",
                "desc": "Angle of the motion blur in degrees",
                "example_value": 45.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Adaptive blur request type
public class ImageMagickAdaptiveBlurRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Blur radius (0 for auto)",
                "desc": "Adaptive blur radius in pixels. Use 0 for automatic selection.",
                "default_value": 0.0,
            },
            "sigma": {
                "type": FloatType,
                "display_name": "Sigma",
                "short_desc": "Standard deviation",
                "desc": "Standard deviation of the adaptive blur",
                "example_value": 2.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Rotational blur request type
public class ImageMagickRotationalBlurRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "angle": {
                "type": FloatType,
                "display_name": "Angle",
                "short_desc": "Rotation angle in degrees",
                "desc": "Angle of the rotational blur in degrees",
                "example_value": 10.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Adaptive sharpen request type
public class ImageMagickAdaptiveSharpenRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "radius": {
                "type": FloatOrNothingType,
                "display_name": "Radius",
                "short_desc": "Sharpen radius (0 for auto)",
                "desc": "Adaptive sharpen radius in pixels. Use 0 for automatic selection.",
                "default_value": 0.0,
            },
            "sigma": {
                "type": FloatType,
                "display_name": "Sigma",
                "short_desc": "Standard deviation",
                "desc": "Standard deviation for the adaptive sharpening",
                "example_value": 1.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Unsharp mask request type
public class ImageMagickUnsharpMaskRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "radius": {
                "type": FloatType,
                "display_name": "Radius",
                "short_desc": "Blur radius",
                "desc": "Radius of the unsharp mask in pixels",
                "example_value": 0.0,
            },
            "sigma": {
                "type": FloatType,
                "display_name": "Sigma",
                "short_desc": "Standard deviation",
                "desc": "Standard deviation for the unsharp mask",
                "example_value": 1.0,
            },
            "gain": {
                "type": FloatType,
                "display_name": "Gain",
                "short_desc": "Sharpening gain",
                "desc": "Amount of sharpening to apply (0.0-1.0 for subtle, higher for aggressive)",
                "example_value": 1.0,
            },
            "threshold": {
                "type": FloatType,
                "display_name": "Threshold",
                "short_desc": "Sharpening threshold",
                "desc": "Threshold below which sharpening is not applied (helps avoid noise amplification)",
                "example_value": 0.05,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Despeckle request type (no extra params)
public class ImageMagickDespeckleRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields;
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Reduce noise request type
public class ImageMagickReduceNoiseRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "radius": {
                "type": FloatType,
                "display_name": "Radius",
                "short_desc": "Noise reduction radius",
                "desc": "Radius of the noise reduction filter in pixels",
                "example_value": 1.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Wavelet denoise request type
public class ImageMagickWaveletDenoiseRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "threshold": {
                "type": FloatType,
                "display_name": "Threshold",
                "short_desc": "Denoise threshold",
                "desc": "Denoising threshold. Higher values remove more noise but may lose detail.",
                "example_value": 10.0,
            },
            "softness": {
                "type": FloatOrNothingType,
                "display_name": "Softness",
                "short_desc": "Softness factor",
                "desc": "Softness attenuation factor for the denoising",
                "default_value": 0.0,
                "example_value": 0.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

# ==================== Transform Request Types ====================

#! Trim request type
public class ImageMagickTrimRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "fuzz": {
                "type": FloatOrNothingType,
                "display_name": "Fuzz",
                "short_desc": "Color distance fuzz factor",
                "desc": "Fuzz factor for color comparison when determining borders to trim. "
                    "Higher values trim more aggressively.",
                "default_value": 0.0,
                "example_value": 10.0,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Border request type
public class ImageMagickBorderRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "color": {
                "type": StringType,
                "display_name": "Border Color",
                "short_desc": "Color of the border",
                "desc": "Color for the border (e.g. 'black', 'white', '#FF0000')",
                "example_value": "black",
            },
            "width": {
                "type": IntType,
                "display_name": "Border Width",
                "short_desc": "Horizontal border width in pixels",
                "desc": "Width of the border to add on left and right sides in pixels. "
                    "Total image width increases by 2x this value.",
                "example_value": 10,
            },
            "height": {
                "type": IntType,
                "display_name": "Border Height",
                "short_desc": "Vertical border height in pixels",
                "desc": "Height of the border to add on top and bottom in pixels. "
                    "Total image height increases by 2x this value.",
                "example_value": 10,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Frame request type
public class ImageMagickFrameRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "color": {
                "type": StringType,
                "display_name": "Frame Color",
                "short_desc": "Color of the frame",
                "desc": "Color for the frame (e.g. 'gray', 'gold', '#C0C0C0')",
                "example_value": "gray",
            },
            "width": {
                "type": IntType,
                "display_name": "Frame Width",
                "short_desc": "Horizontal frame width in pixels",
                "desc": "Width of the frame border on each side in pixels",
                "example_value": 20,
            },
            "height": {
                "type": IntType,
                "display_name": "Frame Height",
                "short_desc": "Vertical frame height in pixels",
                "desc": "Height of the frame border on top and bottom in pixels",
                "example_value": 20,
            },
            "inner_bevel": {
                "type": IntType,
                "display_name": "Inner Bevel",
                "short_desc": "Inner bevel width in pixels",
                "desc": "Width of the inner bevel effect in pixels",
                "example_value": 3,
            },
            "outer_bevel": {
                "type": IntType,
                "display_name": "Outer Bevel",
                "short_desc": "Outer bevel width in pixels",
                "desc": "Width of the outer bevel effect in pixels",
                "example_value": 3,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Shear request type
public class ImageMagickShearRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "x": {
                "type": FloatType,
                "display_name": "X Shear",
                "short_desc": "Horizontal shear angle in degrees",
                "desc": "Horizontal shear angle in degrees",
                "example_value": 10.0,
            },
            "y": {
                "type": FloatType,
                "display_name": "Y Shear",
                "short_desc": "Vertical shear angle in degrees",
                "desc": "Vertical shear angle in degrees",
                "example_value": 0.0,
            },
            "background": {
                "type": StringOrNothingType,
                "display_name": "Background Color",
                "short_desc": "Background color for exposed areas",
                "desc": "Color for areas exposed by the shear transformation",
                "default_value": "white",
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

#! Deskew request type
public class ImageMagickDeskewRequestDataType inherits HashDataType {
    public {
        const Fields = ImageInputFields + {
            "threshold": {
                "type": FloatType,
                "display_name": "Threshold",
                "short_desc": "Deskew threshold",
                "desc": "Deskew threshold for detecting skew. Typical values are 0.4 (40%%) for "
                    "scanned documents.",
                "example_value": 0.4,
            },
        };
    }

    constructor() {
        addQoreFields(Fields);
        setRequiredGroups((("input_path", "data", "location"),));
    }
}

}
