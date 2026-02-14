/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file imagemagick-module.cpp imagemagick module implementation */
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

#include "imagemagick-module.h"
#include "QC_MagickImage.h"
#include "QC_MagickDrawing.h"

static void imagemagick_module_init(QoreModuleInitContext& ctx, ExceptionSink& xsink);
static void imagemagick_module_ns_init(QoreNamespace* rns, QoreNamespace* qns, ExceptionSink& xsink);
static void imagemagick_module_delete();

extern "C" DLLEXPORT void imagemagick_qore_module_desc(QoreModuleInfo& mod_info) {
    mod_info.name = "imagemagick";
    mod_info.version = "1.0.0";
    mod_info.desc = "Qore ImageMagick module for image processing and manipulation";
    mod_info.author = "Qore Technologies, s.r.o.";
    mod_info.url = "https://github.com/qoretechnologies/module-imagemagick";
    mod_info.api_major = QORE_MODULE_API_MAJOR;
    mod_info.api_minor = QORE_MODULE_API_MINOR;
    mod_info.init = imagemagick_module_init;
    mod_info.ns_init = imagemagick_module_ns_init;
    mod_info.del = imagemagick_module_delete;
    mod_info.license = QL_MIT;
    mod_info.license_str = "MIT";
}

// Global hashdecl pointers
const TypedHashDecl* hashdeclImageInfo = nullptr;
const TypedHashDecl* hashdeclImageResolutionInfo = nullptr;
const TypedHashDecl* hashdeclPointInfo = nullptr;

QoreNamespace ImageMagickNs("Qore::ImageMagick");

static void imagemagick_module_init(QoreModuleInitContext& ctx, ExceptionSink& xsink) {
    // Initialize MagickWand library
    MagickWandGenesis();

    // Initialize hashdecls (order matters: referenced hashdecls must be initialized first)
    hashdeclImageResolutionInfo = init_hashdecl_ImageResolutionInfo(ImageMagickNs);
    hashdeclPointInfo = init_hashdecl_PointInfo(ImageMagickNs);
    hashdeclImageInfo = init_hashdecl_ImageInfo(ImageMagickNs);

    // Initialize classes - MagickDrawing must be initialized before MagickImage
    // because MagickImage references it as a parameter type
    ImageMagickNs.addSystemClass(initMagickDrawingClass(ImageMagickNs));
    ImageMagickNs.addSystemClass(initMagickImageClass(ImageMagickNs));
}

static void imagemagick_module_ns_init(QoreNamespace* rns, QoreNamespace* qns, ExceptionSink& xsink) {
    qns->addNamespace(ImageMagickNs.copy());
}

static void imagemagick_module_delete() {
    MagickWandTerminus();
}
