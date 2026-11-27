/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file ImageMagickHelper.h ImageMagick helper utilities */
/*
    Qore imagemagick module

    Copyright (C) 2026 Qore Technologies, s.r.o.

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

#ifndef _QORE_IMAGEMAGICK_HELPER_H
#define _QORE_IMAGEMAGICK_HELPER_H

#include "imagemagick-module.h"

#include <memory>
#include <string>
#include <vector>

//! RAII wrapper for temporary PixelWand objects
class PixelWandHelper {
public:
    DLLLOCAL PixelWandHelper() : pw(NewPixelWand()) {}

    DLLLOCAL PixelWandHelper(const char* color) : pw(NewPixelWand()) {
        if (pw) {
            PixelSetColor(pw, color);
        }
    }

    DLLLOCAL ~PixelWandHelper() {
        if (pw) {
            DestroyPixelWand(pw);
        }
    }

    DLLLOCAL PixelWand* get() const { return pw; }
    DLLLOCAL operator PixelWand*() const { return pw; }

    // Non-copyable
    PixelWandHelper(const PixelWandHelper&) = delete;
    PixelWandHelper& operator=(const PixelWandHelper&) = delete;

private:
    PixelWand* pw;
};

//! Check for MagickWand errors and raise Qore exceptions
/** @param wand the MagickWand to check
    @param context description of the operation for error messages
    @param xsink exception sink
    @return true if an error was found
*/
DLLLOCAL bool checkMagickError(MagickWand* wand, const char* context, ExceptionSink* xsink);

//! Check for DrawingWand errors and raise Qore exceptions
DLLLOCAL bool checkDrawingError(DrawingWand* wand, const char* context, ExceptionSink* xsink);

//! Convert string to FilterType
DLLLOCAL FilterType stringToFilterType(const char* str, ExceptionSink* xsink);

//! Convert FilterType to string
DLLLOCAL const char* filterTypeToString(FilterType ft);

//! Convert string to GravityType
DLLLOCAL GravityType stringToGravityType(const char* str, ExceptionSink* xsink);

//! Convert GravityType to string
DLLLOCAL const char* gravityTypeToString(GravityType gt);

//! Convert string to ColorspaceType
DLLLOCAL ColorspaceType stringToColorspaceType(const char* str, ExceptionSink* xsink);

//! Convert ColorspaceType to string
DLLLOCAL const char* colorspaceTypeToString(ColorspaceType cs);

//! Convert string to CompositeOperator
DLLLOCAL CompositeOperator stringToCompositeOp(const char* str, ExceptionSink* xsink);

//! Convert CompositeOperator to string
DLLLOCAL const char* compositeOpToString(CompositeOperator op);

//! Convert string to NoiseType
DLLLOCAL NoiseType stringToNoiseType(const char* str, ExceptionSink* xsink);

//! Convert NoiseType to string
DLLLOCAL const char* noiseTypeToString(NoiseType nt);

//! Convert ImageType to string
DLLLOCAL const char* imageTypeToString(ImageType it);

//! Get a color string from a PixelWand
DLLLOCAL QoreStringNode* getPixelColor(PixelWand* pw);

#ifdef QORE_HAVE_CALLER_AMBIENT_PREDICATES
//! the paths, fonts and data that a lent object gives are decided with the code that it was lent to
#define IM_LENT_PATH_CHECKS_VAL true
#else
#define IM_LENT_PATH_CHECKS_VAL false
#endif

//! Returns true if the calling code is restricted from any of the given functional domains
/** A method of an object that unrestricted code (the host) created and lent to restricted code runs with what the
    object lends (a capability frame): the code that the object was lent to decides
    (qore_caller_lent_to_restricts_domain(); module-imagemagick#10).  With a %Qore without that function
    (\c QORE_HAVE_CALLER_AMBIENT_PREDICATES is not defined), only the calling code decides
    (qore_caller_restricts_domain()) if \a always is true, and false is returned otherwise: the methods with the
    \c FILESYSTEM functional domain are refused for restricted calling code when they are called

    @param domain the functional domains (\c QDOM_*)
    @param always true for a value that a method without the domain takes (such as a font that names a file)
*/
DLLLOCAL bool callerRestricts(int64 domain, bool always);

//! Refuses a file path that a method with the \c FILESYSTEM functional domain is given, for restricted code
/** For the code that a host-created object was lent to (see callerRestricts()), as such code cannot call the method
    itself

    @return 0 if the path can be used, -1 if \c PERMISSION-DENIED was raised
*/
DLLLOCAL int checkCallerPath(const char* path, ExceptionSink* xsink);

//! Refuses a font or font family name that would break out of the quoted name in an ImageMagick drawing script
/** @return 0 if the name can be used, -1 if \c IMAGEMAGICK-ERROR was raised
*/
DLLLOCAL int checkFontName(const char* font, const char* what, ExceptionSink* xsink);

//! Refuses a font that names a file if the calling code may not read the file
/** ImageMagick reads a font that names an existing file as that file, before it looks the name up among the
    registered fonts, and \c "@" introduces a file: a font that starts with \c "@" or \c "~", contains a path
    separator or names an existing file is the calling code's input, so it is refused if that code is restricted from
    the \c FILESYSTEM functional domain, and it is checked with the file-system policy of a sandbox that governs the
    code; a font with a quote, a backslash or a control character is refused for all callers (checkFontName())

    @return 0 if the font can be used, -1 if an exception was raised
*/
DLLLOCAL int checkFontAccess(const char* font, ExceptionSink* xsink);

//! Refuses, for restricted calling code, a file name whose ImageMagick syntax selects a format or opens another file
/** Called for code that a sandbox governs or that is restricted from formats (callerRestrictsFormats()): a coder
    prefix (\c "png:file"), a list file (\c "@file") and a scene or size suffix (\c "file[0]")

    @return 0 if the name can be used, -1 if \c FILESYSTEM-ACCESS-DENIED was raised
*/
DLLLOCAL int checkMagickFileName(const char* path, ExceptionSink* xsink);

//! the functional domains whose restriction makes the calling code restricted from formats that ImageMagick decodes
//! with external programs or whose data names other files (module-imagemagick#13)
#define IM_DECODE_RESTRICTED_DOMAINS (QDOM_FILESYSTEM | QDOM_NETWORK | QDOM_EXTERNAL_PROCESS | QDOM_PROCESS)

//! Returns true if the calling code may not use formats that ImageMagick decodes or encodes with an external program
//! or whose data names other files
/** True if a sandbox governs the calling code or if that code (also the code that a host-created object was lent
    to; see callerRestricts()) is restricted from the \c FILESYSTEM, \c NETWORK, \c EXTERNAL_PROCESS or
    \c PROCESS_CONTROL functional domain

    @param sandboxed true if a sandbox governs the calling code
*/
DLLLOCAL bool callerRestrictsFormats(bool sandboxed);

//! Returns the format that ImageMagick decodes the given data in
/** The format is decided as ImageMagick decides it: an explicit format that ImageMagick knows is used as it is;
    without one, or with one that it does not know (which ImageMagick ignores), the format is detected from the data
    and, for data read from a file, from the file name's extension (SetImageInfo())

    @param data the data, which must not be empty
    @param len the size of the data
    @param format the explicit format, if any
    @param path the path of the file that the data was read from, if any; it must have no ImageMagick file name
    syntax (checkMagickFileName())

    @return the name of the format, or an empty string if it cannot be determined
*/
DLLLOCAL std::string getDecodeFormat(const void* data, size_t len, const char* format, const char* path);

//! Refuses, for restricted calling code, a format that ImageMagick decodes with an external program or whose data
//! names other files
/** A format is refused if ImageMagick has no decoder of its own for it (it would decode it with a delegate), if a
    delegate is configured for its coder (such as Ghostscript for PostScript and PDF), if it or its coder is one of the
    formats decoded by Ghostscript or another external program, or if its data names other files or URLs that
    ImageMagick reads, or embeds data in a format that ImageMagick detects (such as MVG, MSL, SVG, TEXT or WPG)

    @param magick the format, as returned by getDecodeFormat()

    @return 0 if the format can be decoded, -1 if \c PERMISSION-DENIED or \c IMAGEMAGICK-ERROR (no format) was raised
*/
DLLLOCAL int checkDecodeFormat(const char* magick, ExceptionSink* xsink);

//! Refuses, for restricted calling code, an output format that ImageMagick encodes with an external program
/** @param magick the format; an empty format is not checked

    @return 0 if the format can be used, -1 if \c PERMISSION-DENIED was raised
*/
DLLLOCAL int checkEncodeFormat(const char* magick, ExceptionSink* xsink);

//! Reads a regular file into memory
/** @return 0 if the file was read, -1 if an exception was raised
*/
DLLLOCAL int readRegularFile(const char* path, std::vector<unsigned char>& buf, ExceptionSink* xsink);

//! Destroys an ImageInfo when it goes out of scope (the blob is not freed)
class ImageInfoHolder {
public:
    DLLLOCAL ImageInfoHolder() : info(AcquireImageInfo()) {
    }

    DLLLOCAL ~ImageInfoHolder() {
        if (info) {
            // the blob is the caller's
            SetImageInfoBlob(info, nullptr, 0);
            DestroyImageInfo(info);
        }
    }

    DLLLOCAL ImageInfo* operator->() const {
        return info;
    }

    DLLLOCAL ImageInfo* get() const {
        return info;
    }

    ImageInfoHolder(const ImageInfoHolder&) = delete;
    ImageInfoHolder& operator=(const ImageInfoHolder&) = delete;

private:
    ImageInfo* info;
};

//! Destroys an ExceptionInfo when it goes out of scope
class ExceptionInfoHolder {
public:
    DLLLOCAL ExceptionInfoHolder() : ex(AcquireExceptionInfo()) {
    }

    DLLLOCAL ~ExceptionInfoHolder() {
        if (ex) {
            DestroyExceptionInfo(ex);
        }
    }

    DLLLOCAL ExceptionInfo* get() const {
        return ex;
    }

    ExceptionInfoHolder(const ExceptionInfoHolder&) = delete;
    ExceptionInfoHolder& operator=(const ExceptionInfoHolder&) = delete;

private:
    ExceptionInfo* ex;
};

//! Returns true if ImageMagick was built with FreeType
DLLLOCAL bool haveFreetype();

//! Refuses, for restricted calling code, a font that ImageMagick renders with Ghostscript or an X server
/** A font whose name starts with \c "^" (Ghostscript) or \c "-" (X server), or any text if ImageMagick was built
    without FreeType (it then renders all text with Ghostscript); the calling code is restricted as for
    callerRestrictsFormats()

    @param font the font, if any
    @param text true if text is rendered (else only the font's name is checked)

    @return 0 if the font can be used, -1 if \c PERMISSION-DENIED was raised
*/
DLLLOCAL int checkFontRenderer(const char* font, bool text, ExceptionSink* xsink);

//! Destroys a MagickWand when it goes out of scope
typedef std::unique_ptr<MagickWand, MagickWand* (*)(MagickWand*)> MagickWandHolder;

#endif // _QORE_IMAGEMAGICK_HELPER_H
