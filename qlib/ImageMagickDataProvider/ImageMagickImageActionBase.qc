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

public namespace ImageMagickDataProvider {

#! Base class for ImageMagick image action providers
/** Provides common functionality for loading images and returning results.
*/
public class ImageMagickImageActionBase inherits AbstractDataProvider {
    #! Returns font reference data as AllowedValueInfo list
    static *list<hash<AllowedValueInfo>> getFontReferenceData() {
        list<string> fonts = MagickImage::queryFonts();
        return map <AllowedValueInfo>{"value": $1, "display_name": $1}, fonts;
    }

    #! Loads an image from a request hash (input_path, data, or location)
    /** The paths and locations of the request are checked first (checkRequestPaths()), so an action that calls this
        method first refuses a restricted caller before anything is read or written

        @throw PERMISSION-DENIED the calling code (also the code that a host-created provider was lent to) is restricted
        from the \c FILESYSTEM functional domain, or from a functional domain of the handler of a location; see
        checkRequestPaths()
    */
    static MagickImage loadImage(hash<auto> req) {
        ImageMagickImageActionBase::checkRequestPaths(req);
        if (req.input_path) {
            return new MagickImage(req.input_path);
        }
        if (req.location) {
            return new MagickImage(FileLocationHandler::getBinaryFileFromLocation(req.location));
        }
        if (!req.data) {
            throw "IMAGEMAGICK-ERROR", "one of 'input_path', 'data', or 'location' must be provided";
        }
        return new MagickImage(req.data);
    }

    #! Returns the image as a response hash with optional output writing
    /**
        @throw PERMISSION-DENIED the calling code (also the code that a host-created provider was lent to) is restricted
        from the \c FILESYSTEM functional domain, or from a functional domain of the handler of a location; see
        checkRequestPaths()
    */
    static hash<auto> makeResponse(MagickImage img, hash<auto> req) {
        # the output path and location are the caller's input (also when this public method is called directly)
        ImageMagickImageActionBase::checkCallerPaths(req, ("output_path",), ("output_location",));
        if (req.output_format) {
            img.setFormat(req.output_format);
        }
        # Write to output_path if specified (uses C++ writeFile which enforces sandbox checks)
        if (req.output_path) {
            img.writeFile(req.output_path);
        }
        bool has_output = exists req.output_path || exists req.output_location;
        bool do_return_data = exists req.return_data ? req.return_data : !has_output;
        # Only skip serialization when output_path was written and data is not needed elsewhere
        bool needs_data = do_return_data || exists req.output_location || !exists req.output_path;
        *binary data;
        if (needs_data) {
            data = img.toData();
            # Write to output_location if specified
            if (req.output_location) {
                FileLocationHandler::writeFileToLocation(req.output_location, data);
            }
        }
        hash<auto> response = {
            "width": img.width(),
            "height": img.height(),
            "format": img.format(),
        };
        if (data) {
            response."size" = data.size();
        } else if (req.output_path) {
            response."size" = hstat(req.output_path).size;
        }
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

    #! Refuses the paths, locations and font files of an image request if the calling code is restricted from them
    /** Called by loadImage() and so before any action reads or writes anything: the \c input_path, \c overlay_path
        and \c output_path paths, the \c location, \c overlay_location and \c output_location locations, and a
        \c font that names a file (see isFontFile()); see checkCallerPaths()

        @param req the request

        @throw LOCATION-ERROR the options of a location cannot be parsed
        @throw PERMISSION-DENIED the calling code (also the code that a host-created provider was lent to) is restricted
        from the \c FILESYSTEM functional domain, or from a functional domain of the handler of a location
    */
    static checkRequestPaths(hash<auto> req) {
        ImageMagickImageActionBase::checkCallerPaths(req, ("input_path", "overlay_path", "output_path"),
            ("location", "overlay_location", "output_location"));
        if (exists req.font && ImageMagickImageActionBase::isFontFile(req.font)) {
            ImageMagickImageActionBase::checkCallerPaths(req, ("font",));
        }
    }

    #! Returns @ref True if ImageMagick reads the given font as a file
    /** ImageMagick reads a font that names an existing file as that file, before it looks the name up among the
        registered fonts, and \c "@" introduces a file: a font names a path if it starts with \c "@" or \c "~",
        contains a path separator, or a file with its name exists
    */
    static bool isFontFile(auto font) {
        string f = font;
        if (!f.val()) {
            return False;
        }
        if (f =~ /[\/\\]/ || f =~ /^[@~]/) {
            return True;
        }
        try {
            return exists hstat(f);
        } catch (hash<ExceptionInfo> ex) {
            # the file-system policy of a sandbox that governs the calling code does not allow the name to be looked
            # up: it is handled as a file, which the policy then decides on when the font is used
            return True;
        }
    }

    #! Refuses the filesystem paths and locations of a request if the calling code is restricted from them
    /** Library code runs with its own %Program's options, so the paths and locations that the calling code gives are
        refused before anything is read or written if that code is restricted from the \c FILESYSTEM functional
        domain (for example with \c PO_NO_FILESYSTEM), also when the provider was lent to that code by the host (see
        @ref Qore::SandboxManager::isLentToRestricted() "SandboxManager::isLentToRestricted()"); a location is refused
        if that code is restricted from a functional domain that the handler of the location's scheme uses for its
        caller (see @ref FileLocationHandler::AbstractFileLocationHandler::getCallerDomains()
        "AbstractFileLocationHandler::getCallerDomains()").

        @param req the request
        @param paths the fields of the request that are filesystem paths; a field of a nested hash is given with
        \c "." (ex: \c "watermark.font.font_path"), and \c "[]" after the name of a field that holds a list stands
        for each of its elements (ex: \c "placements[].image_path", \c "inputs[]")
        @param locations the fields of the request that are FileLocationHandler locations

        @throw LOCATION-ERROR the options of a location cannot be parsed
        @throw PERMISSION-DENIED the calling code is restricted from the \c FILESYSTEM functional domain or from a
        functional domain of the handler of a location
    */
    static checkCallerPaths(hash<auto> req, list<string> paths, *list<string> locations) {
        foreach string field in (paths) {
            foreach auto v in (ImageMagickImageActionBase::getFieldValues(req, field.split("."))) {
                if (exists v && SandboxManager::isLentToRestricted(DOM_FILESYSTEM)) {
                    throw "PERMISSION-DENIED", sprintf("the %y field of the request cannot be used: the calling code "
                        "is restricted from the FILESYSTEM functional domain", field);
                }
            }
        }
        foreach string field in (locations) {
            if (exists req{field}) {
                ImageMagickImageActionBase::checkCallerLocation(field, req{field});
            }
        }
    }

    #! Returns the values of the given field of a request
    /** @param v the value to search
        @param path the components of the field's name; a component that ends in \c "[]" stands for each element of
        a list

        @return the values found; a value that is not a hash where a hash is expected (or not a list where a list is
        expected) has no fields
    */
    private static list<auto> getFieldValues(auto v, list<string> path) {
        if (!path) {
            return (v,);
        }
        string key = shift path;
        bool each = key =~ /\[\]$/;
        if (each) {
            splice key, -2;
        }
        if (v.typeCode() != NT_HASH) {
            return ();
        }
        auto fv = v{key};
        if (!each) {
            return ImageMagickImageActionBase::getFieldValues(fv, path);
        }
        list<auto> rv = ();
        if (fv.typeCode() == NT_LIST) {
            foreach auto e in (fv) {
                rv += ImageMagickImageActionBase::getFieldValues(e, path);
            }
        }
        return rv;
    }

    #! Refuses a location given in a request if the calling code is restricted from a functional domain of its handler
    /** @param field the name of the request field, for the error message
        @param location the location

        @throw LOCATION-ERROR the options of the location cannot be parsed
        @throw PERMISSION-DENIED the calling code is restricted from a functional domain of the location's handler
    */
    private static checkCallerLocation(string field, string location) {
        string scheme = (location =~ x/^(\w+):\/\//)[0] ?? "file";
        AbstractFileLocationHandler handler;
        try {
            handler = FileLocationHandler::getHandler(scheme);
        } catch (hash<ExceptionInfo> ex) {
            if (ex.err == "SCHEME-ERROR") {
                # an unknown scheme is refused when the location is used
                return;
            }
            rethrow;
        }
        # the options at the end of the location, as FileLocationHandler takes them
        *hash<auto> opts;
        *string opt_str = (location =~ x/((?:{).*(?:}))$/)[0];
        if (opt_str) {
            auto v = parse_to_qore_value(opt_str);
            if (v.typeCode() == NT_HASH) {
                opts = v;
            } else if (v.strp()) {
                opts = {v: True};
            } else {
                throw "LOCATION-ERROR", sprintf("the options of the location in the %y field of the request cannot be "
                    "parsed", field);
            }
        }
        int dom = handler.getCallerDomains(scheme, opts);
        if (dom && SandboxManager::isLentToRestricted(dom)) {
            throw "PERMISSION-DENIED", sprintf("the %y field of the request cannot be used: the calling code is "
                "restricted from a functional domain of the handler of %y locations", field, scheme);
        }
        # the handler of conn locations has no domains of its own and gives the options that the caller adds to the
        # handler that it resolves the location to, which judges the calling code as
        # SandboxManager::isCallerRestricted() does; for the code that a host-created provider was lent to, which that
        # does not see, the options are refused here if that code is restricted from the file system or the network
        if (!dom && opts && (scheme == "conn" || scheme == "connection")
                && ((SandboxManager::isLentToRestricted(DOM_FILESYSTEM)
                        && !SandboxManager::isCallerRestricted(DOM_FILESYSTEM))
                    || (SandboxManager::isLentToRestricted(DOM_NETWORK)
                        && !SandboxManager::isCallerRestricted(DOM_NETWORK)))) {
            throw "PERMISSION-DENIED", sprintf("the options of the location in the %y field of the request cannot be "
                "used: the calling code is restricted from the FILESYSTEM or NETWORK functional domain", field);
        }
    }
}
}
