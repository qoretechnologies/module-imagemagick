/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file QoreMagickImage.cpp QoreMagickImage implementation */
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

#include "QoreMagickImage.h"

void QoreMagickImage::setupProgressMonitor() {
    if (smh) {
        MagickSetImageProgressMonitor(wand, progressMonitor, this);
    }
}

MagickBooleanType QoreMagickImage::progressMonitor(const char* tag, const MagickOffsetType offset,
                                                    const MagickSizeType size, void* client_data) {
    QoreMagickImage* self = static_cast<QoreMagickImage*>(client_data);
    if (self->smh && self->smh->isInterruptRequested()) {
        self->interrupted.store(true, std::memory_order_release);
        return MagickFalse;  // Cancel the operation
    }
    return MagickTrue;
}

bool QoreMagickImage::checkInterrupted(ExceptionSink* xsink) {
    if (interrupted.load(std::memory_order_acquire)) {
        interrupted.store(false, std::memory_order_relaxed);
        // Clear the MagickWand exception set by the cancelled operation
        MagickClearException(wand);
        xsink->raiseException("PROGRAM-INTERRUPTED", "ImageMagick operation interrupted");
        return true;
    }
    return false;
}

QoreMagickImage::QoreMagickImage(const char* path, ExceptionSink* xsink) : wand(NewMagickWand()) {
    setupProgressMonitor();
    // Check filesystem sandbox access
    if (smh && !smh->checkFilesystemAccess(path, QSEC_READ, xsink)) {
        return;
    }
    // Check for I/O interrupt before file operation
    if (qore_check_io_interrupt(xsink, "reading image file")) {
        return;
    }
    if (MagickReadImage(wand, path) == MagickFalse) {
        if (!checkInterrupted(xsink)) {
            checkMagickError(wand, "error reading image file", xsink);
        }
    }
}

QoreMagickImage::QoreMagickImage(const BinaryNode* data, ExceptionSink* xsink) : wand(NewMagickWand()) {
    setupProgressMonitor();
    if (MagickReadImageBlob(wand, data->getPtr(), data->size()) == MagickFalse) {
        if (!checkInterrupted(xsink)) {
            checkMagickError(wand, "error reading image from binary data", xsink);
        }
    }
}

QoreMagickImage::QoreMagickImage(const BinaryNode* data, const char* format,
                                 ExceptionSink* xsink) : wand(NewMagickWand()) {
    setupProgressMonitor();
    MagickSetFormat(wand, format);
    if (MagickReadImageBlob(wand, data->getPtr(), data->size()) == MagickFalse) {
        if (!checkInterrupted(xsink)) {
            checkMagickError(wand, "error reading image from binary data with format", xsink);
        }
    }
}

QoreMagickImage::QoreMagickImage(size_t width, size_t height, const char* background,
                                 ExceptionSink* xsink) : wand(NewMagickWand()) {
    setupProgressMonitor();
    PixelWandHelper pw(background);
    if (MagickNewImage(wand, width, height, pw) == MagickFalse) {
        checkMagickError(wand, "error creating new image", xsink);
    }
}

QoreMagickImage::QoreMagickImage(const QoreMagickImage& old,
                                 ExceptionSink* xsink) : wand(CloneMagickWand(old.wand)) {
    if (!wand) {
        xsink->raiseException("IMAGEMAGICK-ERROR", "failed to clone MagickWand");
    } else {
        setupProgressMonitor();
    }
}

QoreMagickImage::~QoreMagickImage() {
    if (wand) {
        DestroyMagickWand(wand);
    }
}

// --- I/O ---

void QoreMagickImage::readFile(const char* path, ExceptionSink* xsink) {
    // Check filesystem sandbox access
    if (smh && !smh->checkFilesystemAccess(path, QSEC_READ, xsink)) {
        return;
    }
    // Check for I/O interrupt before file operation
    if (qore_check_io_interrupt(xsink, "reading image file")) {
        return;
    }
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickReadImage(wand, path) == MagickFalse) {
        if (!checkInterrupted(xsink)) {
            checkMagickError(wand, "error reading image file", xsink);
        }
    }
}

void QoreMagickImage::readData(const BinaryNode* data, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickReadImageBlob(wand, data->getPtr(), data->size()) == MagickFalse) {
        if (!checkInterrupted(xsink)) {
            checkMagickError(wand, "error reading image from binary data", xsink);
        }
    }
}

void QoreMagickImage::readDataWithFormat(const BinaryNode* data, const char* format,
                                         ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    MagickSetFormat(wand, format);
    if (MagickReadImageBlob(wand, data->getPtr(), data->size()) == MagickFalse) {
        if (!checkInterrupted(xsink)) {
            checkMagickError(wand, "error reading image from binary data with format", xsink);
        }
    }
}

QoreHashNode* QoreMagickImage::pingFile(const char* path, ExceptionSink* xsink) {
    // Check filesystem sandbox access
    if (smh && !smh->checkFilesystemAccess(path, QSEC_READ, xsink)) {
        return nullptr;
    }
    // Check for I/O interrupt before file operation
    if (qore_check_io_interrupt(xsink, "pinging image file")) {
        return nullptr;
    }
    QoreAutoRWWriteLocker al(rwlock);
    MagickWand* pw = NewMagickWand();
    if (MagickPingImage(pw, path) == MagickFalse) {
        checkMagickError(pw, "error pinging image file", xsink);
        DestroyMagickWand(pw);
        return nullptr;
    }
    ReferenceHolder<QoreHashNode> h(new QoreHashNode(hashdeclImageInfo, xsink), xsink);
    h->setKeyValue("width", static_cast<int64>(MagickGetImageWidth(pw)), xsink);
    h->setKeyValue("height", static_cast<int64>(MagickGetImageHeight(pw)), xsink);
    char* fmt = MagickGetImageFormat(pw);
    h->setKeyValue("format", new QoreStringNode(fmt ? fmt : ""), xsink);
    if (fmt) {
        MagickRelinquishMemory(fmt);
    }
    h->setKeyValue("depth", static_cast<int64>(MagickGetImageDepth(pw)), xsink);
    h->setKeyValue("colorspace", new QoreStringNode(colorspaceTypeToString(MagickGetImageColorspace(pw))), xsink);
    h->setKeyValue("image_type", new QoreStringNode(imageTypeToString(MagickGetImageType(pw))), xsink);
    double x_res, y_res;
    MagickGetImageResolution(pw, &x_res, &y_res);
    ReferenceHolder<QoreHashNode> res_h(new QoreHashNode(hashdeclImageResolutionInfo, xsink), xsink);
    res_h->setKeyValue("x", x_res, xsink);
    res_h->setKeyValue("y", y_res, xsink);
    h->setKeyValue("resolution", res_h.release(), xsink);
    h->setKeyValue("compression_quality", static_cast<int64>(MagickGetImageCompressionQuality(pw)), xsink);
    h->setKeyValue("number_images", static_cast<int64>(MagickGetNumberImages(pw)), xsink);
    h->setKeyValue("colors", static_cast<int64>(MagickGetImageColors(pw)), xsink);
    char* sig = MagickGetImageSignature(pw);
    h->setKeyValue("signature", new QoreStringNode(sig ? sig : ""), xsink);
    if (sig) {
        MagickRelinquishMemory(sig);
    }
    DestroyMagickWand(pw);
    return h.release();
}

QoreHashNode* QoreMagickImage::pingData(const BinaryNode* data, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    MagickWand* pw = NewMagickWand();
    if (MagickPingImageBlob(pw, data->getPtr(), data->size()) == MagickFalse) {
        checkMagickError(pw, "error pinging image data", xsink);
        DestroyMagickWand(pw);
        return nullptr;
    }
    ReferenceHolder<QoreHashNode> h(new QoreHashNode(hashdeclImageInfo, xsink), xsink);
    h->setKeyValue("width", static_cast<int64>(MagickGetImageWidth(pw)), xsink);
    h->setKeyValue("height", static_cast<int64>(MagickGetImageHeight(pw)), xsink);
    char* fmt = MagickGetImageFormat(pw);
    h->setKeyValue("format", new QoreStringNode(fmt ? fmt : ""), xsink);
    if (fmt) {
        MagickRelinquishMemory(fmt);
    }
    h->setKeyValue("depth", static_cast<int64>(MagickGetImageDepth(pw)), xsink);
    h->setKeyValue("colorspace", new QoreStringNode(colorspaceTypeToString(MagickGetImageColorspace(pw))), xsink);
    h->setKeyValue("image_type", new QoreStringNode(imageTypeToString(MagickGetImageType(pw))), xsink);
    double x_res, y_res;
    MagickGetImageResolution(pw, &x_res, &y_res);
    ReferenceHolder<QoreHashNode> res_h(new QoreHashNode(hashdeclImageResolutionInfo, xsink), xsink);
    res_h->setKeyValue("x", x_res, xsink);
    res_h->setKeyValue("y", y_res, xsink);
    h->setKeyValue("resolution", res_h.release(), xsink);
    h->setKeyValue("compression_quality", static_cast<int64>(MagickGetImageCompressionQuality(pw)), xsink);
    h->setKeyValue("number_images", static_cast<int64>(MagickGetNumberImages(pw)), xsink);
    h->setKeyValue("colors", static_cast<int64>(MagickGetImageColors(pw)), xsink);
    char* sig = MagickGetImageSignature(pw);
    h->setKeyValue("signature", new QoreStringNode(sig ? sig : ""), xsink);
    if (sig) {
        MagickRelinquishMemory(sig);
    }
    DestroyMagickWand(pw);
    return h.release();
}

void QoreMagickImage::writeFile(const char* path, ExceptionSink* xsink) {
    // Check filesystem sandbox access
    if (smh && !smh->checkFilesystemAccess(path, QSEC_WRITE | QSEC_CREATE, xsink)) {
        return;
    }
    // Check for I/O interrupt before file operation
    if (qore_check_io_interrupt(xsink, "writing image file")) {
        return;
    }
    QoreAutoRWReadLocker al(rwlock);
    if (MagickWriteImage(wand, path) == MagickFalse) {
        if (!checkInterrupted(xsink)) {
            checkMagickError(wand, "error writing image file", xsink);
        }
    }
}

void QoreMagickImage::writeFiles(const char* path, bool adjoin, ExceptionSink* xsink) {
    // Check filesystem sandbox access
    if (smh && !smh->checkFilesystemAccess(path, QSEC_WRITE | QSEC_CREATE, xsink)) {
        return;
    }
    // Check for I/O interrupt before file operation
    if (qore_check_io_interrupt(xsink, "writing image files")) {
        return;
    }
    QoreAutoRWReadLocker al(rwlock);
    if (MagickWriteImages(wand, path, adjoin ? MagickTrue : MagickFalse) == MagickFalse) {
        if (!checkInterrupted(xsink)) {
            checkMagickError(wand, "error writing image files", xsink);
        }
    }
}

BinaryNode* QoreMagickImage::toData(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    size_t length;
    unsigned char* data = MagickGetImageBlob(wand, &length);
    if (!data) {
        checkMagickError(wand, "error getting image data", xsink);
        return nullptr;
    }
    SimpleRefHolder<BinaryNode> rv(new BinaryNode());
    rv->append(data, length);
    MagickRelinquishMemory(data);
    return rv.release();
}

BinaryNode* QoreMagickImage::toDataWithFormat(const char* format, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    // Save current format
    char* old_format = MagickGetImageFormat(wand);
    MagickSetImageFormat(wand, format);
    size_t length;
    unsigned char* data = MagickGetImageBlob(wand, &length);
    // Restore format
    if (old_format) {
        MagickSetImageFormat(wand, old_format);
        MagickRelinquishMemory(old_format);
    }
    if (!data) {
        checkMagickError(wand, "error getting image data in format", xsink);
        return nullptr;
    }
    SimpleRefHolder<BinaryNode> rv(new BinaryNode());
    rv->append(data, length);
    MagickRelinquishMemory(data);
    return rv.release();
}

// --- Info/Metadata ---

int64 QoreMagickImage::getWidth(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    return static_cast<int64>(MagickGetImageWidth(wand));
}

int64 QoreMagickImage::getHeight(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    return static_cast<int64>(MagickGetImageHeight(wand));
}

QoreStringNode* QoreMagickImage::getFormat(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    char* fmt = MagickGetImageFormat(wand);
    QoreStringNode* rv = new QoreStringNode(fmt ? fmt : "");
    if (fmt) {
        MagickRelinquishMemory(fmt);
    }
    return rv;
}

void QoreMagickImage::setFormat(const char* format, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSetImageFormat(wand, format) == MagickFalse) {
        checkMagickError(wand, "error setting image format", xsink);
    }
}

int64 QoreMagickImage::getDepth(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    return static_cast<int64>(MagickGetImageDepth(wand));
}

void QoreMagickImage::setDepth(int64 depth, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSetImageDepth(wand, static_cast<size_t>(depth)) == MagickFalse) {
        checkMagickError(wand, "error setting image depth", xsink);
    }
}

QoreStringNode* QoreMagickImage::getColorspace(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    return new QoreStringNode(colorspaceTypeToString(MagickGetImageColorspace(wand)));
}

void QoreMagickImage::setColorspace(const char* cs, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    ColorspaceType cst = stringToColorspaceType(cs, xsink);
    if (*xsink) {
        return;
    }
    if (MagickSetImageColorspace(wand, cst) == MagickFalse) {
        checkMagickError(wand, "error setting colorspace", xsink);
    }
}

QoreStringNode* QoreMagickImage::getImageType(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    return new QoreStringNode(imageTypeToString(MagickGetImageType(wand)));
}

QoreHashNode* QoreMagickImage::getResolution(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    double x, y;
    MagickGetImageResolution(wand, &x, &y);
    ReferenceHolder<QoreHashNode> h(new QoreHashNode(hashdeclImageResolutionInfo, xsink), xsink);
    h->setKeyValue("x", x, xsink);
    h->setKeyValue("y", y, xsink);
    return h.release();
}

void QoreMagickImage::setResolution(double x, double y, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSetImageResolution(wand, x, y) == MagickFalse) {
        checkMagickError(wand, "error setting resolution", xsink);
    }
}

int64 QoreMagickImage::getCompressionQuality(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    return static_cast<int64>(MagickGetImageCompressionQuality(wand));
}

void QoreMagickImage::setCompressionQuality(int64 quality, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSetImageCompressionQuality(wand, static_cast<size_t>(quality)) == MagickFalse) {
        checkMagickError(wand, "error setting compression quality", xsink);
    }
}

QoreStringNode* QoreMagickImage::getSignature(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    char* sig = MagickGetImageSignature(wand);
    QoreStringNode* rv = new QoreStringNode(sig ? sig : "");
    if (sig) {
        MagickRelinquishMemory(sig);
    }
    return rv;
}

QoreStringNode* QoreMagickImage::identify(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    char* id = MagickIdentifyImage(wand);
    QoreStringNode* rv = new QoreStringNode(id ? id : "");
    if (id) {
        MagickRelinquishMemory(id);
    }
    return rv;
}

int64 QoreMagickImage::getColors(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    return static_cast<int64>(MagickGetImageColors(wand));
}

int64 QoreMagickImage::getNumberImages(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    return static_cast<int64>(MagickGetNumberImages(wand));
}

QoreHashNode* QoreMagickImage::getInfo(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    ReferenceHolder<QoreHashNode> h(new QoreHashNode(hashdeclImageInfo, xsink), xsink);
    h->setKeyValue("width", static_cast<int64>(MagickGetImageWidth(wand)), xsink);
    h->setKeyValue("height", static_cast<int64>(MagickGetImageHeight(wand)), xsink);
    char* fmt = MagickGetImageFormat(wand);
    h->setKeyValue("format", new QoreStringNode(fmt ? fmt : ""), xsink);
    if (fmt) {
        MagickRelinquishMemory(fmt);
    }
    h->setKeyValue("depth", static_cast<int64>(MagickGetImageDepth(wand)), xsink);
    h->setKeyValue("colorspace", new QoreStringNode(colorspaceTypeToString(MagickGetImageColorspace(wand))), xsink);
    h->setKeyValue("image_type", new QoreStringNode(imageTypeToString(MagickGetImageType(wand))), xsink);
    double x_res, y_res;
    MagickGetImageResolution(wand, &x_res, &y_res);
    ReferenceHolder<QoreHashNode> res_h(new QoreHashNode(hashdeclImageResolutionInfo, xsink), xsink);
    res_h->setKeyValue("x", x_res, xsink);
    res_h->setKeyValue("y", y_res, xsink);
    h->setKeyValue("resolution", res_h.release(), xsink);
    h->setKeyValue("compression_quality", static_cast<int64>(MagickGetImageCompressionQuality(wand)), xsink);
    h->setKeyValue("number_images", static_cast<int64>(MagickGetNumberImages(wand)), xsink);
    h->setKeyValue("colors", static_cast<int64>(MagickGetImageColors(wand)), xsink);
    char* sig = MagickGetImageSignature(wand);
    h->setKeyValue("signature", new QoreStringNode(sig ? sig : ""), xsink);
    if (sig) {
        MagickRelinquishMemory(sig);
    }
    return h.release();
}

// --- Properties/Profiles ---

QoreStringNode* QoreMagickImage::getProperty(const char* name, ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    char* value = MagickGetImageProperty(wand, name);
    if (!value) {
        return nullptr;
    }
    QoreStringNode* rv = new QoreStringNode(value);
    MagickRelinquishMemory(value);
    return rv;
}

QoreHashNode* QoreMagickImage::getProperties(const char* pattern, ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    size_t count;
    char** props = MagickGetImageProperties(wand, pattern, &count);
    if (!props) {
        return new QoreHashNode(autoTypeInfo);
    }
    ReferenceHolder<QoreHashNode> h(new QoreHashNode(autoTypeInfo), xsink);
    for (size_t i = 0; i < count; ++i) {
        char* value = MagickGetImageProperty(wand, props[i]);
        h->setKeyValue(props[i], value ? new QoreStringNode(value) : QoreValue(), xsink);
        if (value) {
            MagickRelinquishMemory(value);
        }
        MagickRelinquishMemory(props[i]);
    }
    MagickRelinquishMemory(props);
    return h.release();
}

void QoreMagickImage::setProperty(const char* name, const char* value, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSetImageProperty(wand, name, value) == MagickFalse) {
        checkMagickError(wand, "error setting image property", xsink);
    }
}

void QoreMagickImage::deleteProperty(const char* name, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickDeleteImageProperty(wand, name) == MagickFalse) {
        checkMagickError(wand, "error deleting image property", xsink);
    }
}

BinaryNode* QoreMagickImage::getProfile(const char* name, ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    size_t length;
    unsigned char* data = MagickGetImageProfile(wand, name, &length);
    if (!data) {
        return nullptr;
    }
    SimpleRefHolder<BinaryNode> rv(new BinaryNode());
    rv->append(data, length);
    MagickRelinquishMemory(data);
    return rv.release();
}

void QoreMagickImage::setProfile(const char* name, const BinaryNode* data, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSetImageProfile(wand, name, data->getPtr(), data->size()) == MagickFalse) {
        checkMagickError(wand, "error setting image profile", xsink);
    }
}

QoreListNode* QoreMagickImage::getProfileNames(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    size_t count;
    char** profiles = MagickGetImageProfiles(wand, "*", &count);
    if (!profiles) {
        return new QoreListNode(stringTypeInfo);
    }
    ReferenceHolder<QoreListNode> l(new QoreListNode(stringTypeInfo), xsink);
    for (size_t i = 0; i < count; ++i) {
        l->push(new QoreStringNode(profiles[i]), xsink);
        MagickRelinquishMemory(profiles[i]);
    }
    MagickRelinquishMemory(profiles);
    return l.release();
}

void QoreMagickImage::strip(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickStripImage(wand) == MagickFalse) {
        checkMagickError(wand, "error stripping image", xsink);
    }
}

// --- Resize ---

void QoreMagickImage::resize(int64 width, int64 height, const char* filter, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    FilterType ft = stringToFilterType(filter, xsink);
    if (*xsink) {
        return;
    }
    if (MagickResizeImage(wand, static_cast<size_t>(width), static_cast<size_t>(height), ft) == MagickFalse) {
        checkMagickError(wand, "error resizing image", xsink);
    }
}

void QoreMagickImage::scale(int64 width, int64 height, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickScaleImage(wand, static_cast<size_t>(width), static_cast<size_t>(height)) == MagickFalse) {
        checkMagickError(wand, "error scaling image", xsink);
    }
}

void QoreMagickImage::thumbnail(int64 width, int64 height, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickThumbnailImage(wand, static_cast<size_t>(width), static_cast<size_t>(height)) == MagickFalse) {
        checkMagickError(wand, "error creating thumbnail", xsink);
    }
}

void QoreMagickImage::adaptiveResize(int64 width, int64 height, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickAdaptiveResizeImage(wand, static_cast<size_t>(width), static_cast<size_t>(height)) == MagickFalse) {
        checkMagickError(wand, "error adaptive resizing image", xsink);
    }
}

void QoreMagickImage::sample(int64 width, int64 height, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSampleImage(wand, static_cast<size_t>(width), static_cast<size_t>(height)) == MagickFalse) {
        checkMagickError(wand, "error sampling image", xsink);
    }
}

void QoreMagickImage::resample(double x_res, double y_res, const char* filter, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    FilterType ft = stringToFilterType(filter, xsink);
    if (*xsink) {
        return;
    }
    if (MagickResampleImage(wand, x_res, y_res, ft) == MagickFalse) {
        checkMagickError(wand, "error resampling image", xsink);
    }
}

void QoreMagickImage::liquidRescale(int64 width, int64 height, double delta_x, double rigidity,
                                    ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickLiquidRescaleImage(wand, static_cast<size_t>(width), static_cast<size_t>(height),
                                 delta_x, rigidity) == MagickFalse) {
        checkMagickError(wand, "error liquid rescaling image", xsink);
    }
}

// --- Crop ---

void QoreMagickImage::crop(int64 width, int64 height, int64 x, int64 y, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickCropImage(wand, static_cast<size_t>(width), static_cast<size_t>(height),
                        static_cast<ssize_t>(x), static_cast<ssize_t>(y)) == MagickFalse) {
        checkMagickError(wand, "error cropping image", xsink);
    }
}

void QoreMagickImage::trim(double fuzz, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickTrimImage(wand, fuzz) == MagickFalse) {
        checkMagickError(wand, "error trimming image", xsink);
    }
}

void QoreMagickImage::extent(int64 width, int64 height, int64 x, int64 y, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickExtentImage(wand, static_cast<size_t>(width), static_cast<size_t>(height),
                          static_cast<ssize_t>(x), static_cast<ssize_t>(y)) == MagickFalse) {
        checkMagickError(wand, "error extending image", xsink);
    }
}

void QoreMagickImage::chop(int64 width, int64 height, int64 x, int64 y, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickChopImage(wand, static_cast<size_t>(width), static_cast<size_t>(height),
                        static_cast<ssize_t>(x), static_cast<ssize_t>(y)) == MagickFalse) {
        checkMagickError(wand, "error chopping image", xsink);
    }
}

void QoreMagickImage::shave(int64 columns, int64 rows, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickShaveImage(wand, static_cast<size_t>(columns), static_cast<size_t>(rows)) == MagickFalse) {
        checkMagickError(wand, "error shaving image", xsink);
    }
}

void QoreMagickImage::splice(int64 width, int64 height, int64 x, int64 y, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSpliceImage(wand, static_cast<size_t>(width), static_cast<size_t>(height),
                          static_cast<ssize_t>(x), static_cast<ssize_t>(y)) == MagickFalse) {
        checkMagickError(wand, "error splicing image", xsink);
    }
}

// --- Transform ---

void QoreMagickImage::rotate(double degrees, const char* background, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    PixelWandHelper pw(background);
    if (MagickRotateImage(wand, pw, degrees) == MagickFalse) {
        checkMagickError(wand, "error rotating image", xsink);
    }
}

void QoreMagickImage::flip(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickFlipImage(wand) == MagickFalse) {
        checkMagickError(wand, "error flipping image", xsink);
    }
}

void QoreMagickImage::flop(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickFlopImage(wand) == MagickFalse) {
        checkMagickError(wand, "error flopping image", xsink);
    }
}

void QoreMagickImage::transpose(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickTransposeImage(wand) == MagickFalse) {
        checkMagickError(wand, "error transposing image", xsink);
    }
}

void QoreMagickImage::transverse(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickTransverseImage(wand) == MagickFalse) {
        checkMagickError(wand, "error transversing image", xsink);
    }
}

void QoreMagickImage::shear(double x, double y, const char* background, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    PixelWandHelper pw(background);
    if (MagickShearImage(wand, pw, x, y) == MagickFalse) {
        checkMagickError(wand, "error shearing image", xsink);
    }
}

void QoreMagickImage::deskew(double threshold, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickDeskewImage(wand, threshold) == MagickFalse) {
        checkMagickError(wand, "error deskewing image", xsink);
    }
}

void QoreMagickImage::autoOrient(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickAutoOrientImage(wand) == MagickFalse) {
        checkMagickError(wand, "error auto-orienting image", xsink);
    }
}

// --- Blur/Sharpen ---

void QoreMagickImage::blur(double radius, double sigma, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickBlurImage(wand, radius, sigma) == MagickFalse) {
        checkMagickError(wand, "error blurring image", xsink);
    }
}

void QoreMagickImage::gaussianBlur(double radius, double sigma, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickGaussianBlurImage(wand, radius, sigma) == MagickFalse) {
        checkMagickError(wand, "error gaussian blurring image", xsink);
    }
}

void QoreMagickImage::adaptiveBlur(double radius, double sigma, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickAdaptiveBlurImage(wand, radius, sigma) == MagickFalse) {
        checkMagickError(wand, "error adaptive blurring image", xsink);
    }
}

void QoreMagickImage::sharpen(double radius, double sigma, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSharpenImage(wand, radius, sigma) == MagickFalse) {
        checkMagickError(wand, "error sharpening image", xsink);
    }
}

void QoreMagickImage::adaptiveSharpen(double radius, double sigma, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickAdaptiveSharpenImage(wand, radius, sigma) == MagickFalse) {
        checkMagickError(wand, "error adaptive sharpening image", xsink);
    }
}

void QoreMagickImage::unsharpMask(double radius, double sigma, double gain, double threshold,
                                  ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickUnsharpMaskImage(wand, radius, sigma, gain, threshold) == MagickFalse) {
        checkMagickError(wand, "error unsharp masking image", xsink);
    }
}

void QoreMagickImage::motionBlur(double radius, double sigma, double angle, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickMotionBlurImage(wand, radius, sigma, angle) == MagickFalse) {
        checkMagickError(wand, "error motion blurring image", xsink);
    }
}

void QoreMagickImage::selectiveBlur(double radius, double sigma, double threshold,
                                    ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSelectiveBlurImage(wand, radius, sigma, threshold) == MagickFalse) {
        checkMagickError(wand, "error selective blurring image", xsink);
    }
}

void QoreMagickImage::rotationalBlur(double angle, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickRotationalBlurImage(wand, angle) == MagickFalse) {
        checkMagickError(wand, "error rotational blurring image", xsink);
    }
}

// --- Color ---

void QoreMagickImage::brightnessContrast(double brightness, double contrast, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickBrightnessContrastImage(wand, brightness, contrast) == MagickFalse) {
        checkMagickError(wand, "error adjusting brightness/contrast", xsink);
    }
}

void QoreMagickImage::gamma(double value, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickGammaImage(wand, value) == MagickFalse) {
        checkMagickError(wand, "error adjusting gamma", xsink);
    }
}

void QoreMagickImage::level(double black, double gamma_val, double white, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickLevelImage(wand, black, gamma_val, white) == MagickFalse) {
        checkMagickError(wand, "error adjusting levels", xsink);
    }
}

void QoreMagickImage::modulate(double brightness, double saturation, double hue,
                               ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickModulateImage(wand, brightness, saturation, hue) == MagickFalse) {
        checkMagickError(wand, "error modulating image", xsink);
    }
}

void QoreMagickImage::negate(bool gray_only, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickNegateImage(wand, gray_only ? MagickTrue : MagickFalse) == MagickFalse) {
        checkMagickError(wand, "error negating image", xsink);
    }
}

void QoreMagickImage::normalize(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickNormalizeImage(wand) == MagickFalse) {
        checkMagickError(wand, "error normalizing image", xsink);
    }
}

void QoreMagickImage::equalize(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickEqualizeImage(wand) == MagickFalse) {
        checkMagickError(wand, "error equalizing image", xsink);
    }
}

void QoreMagickImage::autoGamma(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickAutoGammaImage(wand) == MagickFalse) {
        checkMagickError(wand, "error auto gamma", xsink);
    }
}

void QoreMagickImage::autoLevel(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickAutoLevelImage(wand) == MagickFalse) {
        checkMagickError(wand, "error auto level", xsink);
    }
}

void QoreMagickImage::sigmoidalContrast(bool sharpen_flag, double strength, double midpoint,
                                        ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSigmoidalContrastImage(wand, sharpen_flag ? MagickTrue : MagickFalse,
                                     strength, midpoint) == MagickFalse) {
        checkMagickError(wand, "error adjusting sigmoidal contrast", xsink);
    }
}

void QoreMagickImage::contrastStretch(double black, double white, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickContrastStretchImage(wand, black, white) == MagickFalse) {
        checkMagickError(wand, "error stretching contrast", xsink);
    }
}

void QoreMagickImage::linearStretch(double black, double white, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickLinearStretchImage(wand, black, white) == MagickFalse) {
        checkMagickError(wand, "error linear stretching", xsink);
    }
}

void QoreMagickImage::enhance(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickEnhanceImage(wand) == MagickFalse) {
        checkMagickError(wand, "error enhancing image", xsink);
    }
}

void QoreMagickImage::whiteBalance(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickWhiteBalanceImage(wand) == MagickFalse) {
        checkMagickError(wand, "error white balancing image", xsink);
    }
}

void QoreMagickImage::transformColorspace(const char* cs, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    ColorspaceType cst = stringToColorspaceType(cs, xsink);
    if (*xsink) {
        return;
    }
    if (MagickTransformImageColorspace(wand, cst) == MagickFalse) {
        checkMagickError(wand, "error transforming colorspace", xsink);
    }
}

// --- Effects ---

void QoreMagickImage::charcoal(double radius, double sigma, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickCharcoalImage(wand, radius, sigma) == MagickFalse) {
        checkMagickError(wand, "error applying charcoal effect", xsink);
    }
}

void QoreMagickImage::edge(double radius, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickEdgeImage(wand, radius) == MagickFalse) {
        checkMagickError(wand, "error applying edge effect", xsink);
    }
}

void QoreMagickImage::emboss(double radius, double sigma, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickEmbossImage(wand, radius, sigma) == MagickFalse) {
        checkMagickError(wand, "error applying emboss effect", xsink);
    }
}

void QoreMagickImage::oilPaint(double radius, double sigma, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickOilPaintImage(wand, radius, sigma) == MagickFalse) {
        checkMagickError(wand, "error applying oil paint effect", xsink);
    }
}

void QoreMagickImage::sketch(double radius, double sigma, double angle, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSketchImage(wand, radius, sigma, angle) == MagickFalse) {
        checkMagickError(wand, "error applying sketch effect", xsink);
    }
}

void QoreMagickImage::solarize(double threshold, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSolarizeImage(wand, threshold) == MagickFalse) {
        checkMagickError(wand, "error solarizing image", xsink);
    }
}

void QoreMagickImage::sepiaTone(double threshold, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSepiaToneImage(wand, threshold) == MagickFalse) {
        checkMagickError(wand, "error applying sepia tone", xsink);
    }
}

void QoreMagickImage::vignette(double radius, double sigma, int64 x, int64 y,
                               ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickVignetteImage(wand, radius, sigma, static_cast<ssize_t>(x),
                            static_cast<ssize_t>(y)) == MagickFalse) {
        checkMagickError(wand, "error applying vignette", xsink);
    }
}

void QoreMagickImage::shadow(double alpha, double sigma, int64 x, int64 y, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickShadowImage(wand, alpha, sigma, static_cast<ssize_t>(x),
                          static_cast<ssize_t>(y)) == MagickFalse) {
        checkMagickError(wand, "error applying shadow", xsink);
    }
}

void QoreMagickImage::shade(bool gray, double azimuth, double elevation, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickShadeImage(wand, gray ? MagickTrue : MagickFalse, azimuth, elevation) == MagickFalse) {
        checkMagickError(wand, "error applying shade", xsink);
    }
}

void QoreMagickImage::wave(double amplitude, double wavelength, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickWaveImage(wand, amplitude, wavelength, BilinearInterpolatePixel) == MagickFalse) {
        checkMagickError(wand, "error applying wave", xsink);
    }
}

void QoreMagickImage::swirl(double degrees, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSwirlImage(wand, degrees, BilinearInterpolatePixel) == MagickFalse) {
        checkMagickError(wand, "error applying swirl", xsink);
    }
}

void QoreMagickImage::implode(double amount, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickImplodeImage(wand, amount, BilinearInterpolatePixel) == MagickFalse) {
        checkMagickError(wand, "error applying implode", xsink);
    }
}

void QoreMagickImage::despeckle(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickDespeckleImage(wand) == MagickFalse) {
        checkMagickError(wand, "error despeckling image", xsink);
    }
}

void QoreMagickImage::posterize(int64 levels, bool dither, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickPosterizeImage(wand, static_cast<size_t>(levels),
                             dither ? RiemersmaDitherMethod : NoDitherMethod) == MagickFalse) {
        checkMagickError(wand, "error posterizing image", xsink);
    }
}

void QoreMagickImage::threshold(double value, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickThresholdImage(wand, value) == MagickFalse) {
        checkMagickError(wand, "error thresholding image", xsink);
    }
}

void QoreMagickImage::addNoise(const char* noise_type, double attenuate, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    NoiseType nt = stringToNoiseType(noise_type, xsink);
    if (*xsink) {
        return;
    }
    if (MagickAddNoiseImage(wand, nt, attenuate) == MagickFalse) {
        checkMagickError(wand, "error adding noise", xsink);
    }
}

void QoreMagickImage::blueShift(double factor, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickBlueShiftImage(wand, factor) == MagickFalse) {
        checkMagickError(wand, "error blue shifting image", xsink);
    }
}

void QoreMagickImage::reduceNoise(double radius, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickReduceNoiseImage(wand, radius) == MagickFalse) {
        checkMagickError(wand, "error reducing noise", xsink);
    }
}

void QoreMagickImage::waveletDenoise(double threshold, double softness, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickWaveletDenoiseImage(wand, threshold, softness) == MagickFalse) {
        checkMagickError(wand, "error wavelet denoising", xsink);
    }
}

// --- Composite ---

void QoreMagickImage::composite(QoreMagickImage* source, const char* op, int64 x, int64 y,
                                ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    CompositeOperator cop = stringToCompositeOp(op, xsink);
    if (*xsink) {
        return;
    }
    if (MagickCompositeImage(wand, source->getWand(), cop, MagickTrue,
                             static_cast<ssize_t>(x), static_cast<ssize_t>(y)) == MagickFalse) {
        checkMagickError(wand, "error compositing image", xsink);
    }
}

void QoreMagickImage::compositeGravity(QoreMagickImage* source, const char* op,
                                       const char* gravity, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    CompositeOperator cop = stringToCompositeOp(op, xsink);
    if (*xsink) {
        return;
    }
    GravityType gt = stringToGravityType(gravity, xsink);
    if (*xsink) {
        return;
    }
    if (MagickCompositeImageGravity(wand, source->getWand(), cop, gt) == MagickFalse) {
        checkMagickError(wand, "error compositing image with gravity", xsink);
    }
}

// --- Drawing ---

void QoreMagickImage::annotate(const char* text, double x, double y, double angle,
                               const char* font, double font_size, const char* fill_color,
                               const char* stroke_color, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    DrawingWand* dw = NewDrawingWand();
    if (font && font[0]) {
        DrawSetFont(dw, font);
    }
    if (font_size > 0) {
        DrawSetFontSize(dw, font_size);
    }
    if (fill_color && fill_color[0]) {
        PixelWandHelper pw(fill_color);
        DrawSetFillColor(dw, pw);
    }
    if (stroke_color && stroke_color[0]) {
        PixelWandHelper pw(stroke_color);
        DrawSetStrokeColor(dw, pw);
    }
    if (MagickAnnotateImage(wand, dw, x, y, angle, text) == MagickFalse) {
        checkMagickError(wand, "error annotating image", xsink);
    }
    DestroyDrawingWand(dw);
}

void QoreMagickImage::draw(DrawingWand* dw, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickDrawImage(wand, dw) == MagickFalse) {
        checkMagickError(wand, "error drawing on image", xsink);
    }
}

void QoreMagickImage::border(const char* color, int64 width, int64 height, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    PixelWandHelper pw(color);
    if (MagickBorderImage(wand, pw, static_cast<size_t>(width), static_cast<size_t>(height),
                          OverCompositeOp) == MagickFalse) {
        checkMagickError(wand, "error adding border", xsink);
    }
}

void QoreMagickImage::frame(const char* color, int64 width, int64 height, int64 inner_bevel,
                            int64 outer_bevel, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    PixelWandHelper pw(color);
    if (MagickFrameImage(wand, pw, static_cast<size_t>(width), static_cast<size_t>(height),
                         static_cast<ssize_t>(inner_bevel), static_cast<ssize_t>(outer_bevel),
                         OverCompositeOp) == MagickFalse) {
        checkMagickError(wand, "error adding frame", xsink);
    }
}

// --- Multi-image ---

bool QoreMagickImage::hasNextImage(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    return MagickHasNextImage(wand) == MagickTrue;
}

bool QoreMagickImage::hasPreviousImage(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    return MagickHasPreviousImage(wand) == MagickTrue;
}

bool QoreMagickImage::nextImage(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    return MagickNextImage(wand) == MagickTrue;
}

bool QoreMagickImage::previousImage(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    return MagickPreviousImage(wand) == MagickTrue;
}

void QoreMagickImage::setIteratorIndex(int64 index, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    if (MagickSetIteratorIndex(wand, static_cast<ssize_t>(index)) == MagickFalse) {
        checkMagickError(wand, "error setting iterator index", xsink);
    }
}

int64 QoreMagickImage::getIteratorIndex(ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    return static_cast<int64>(MagickGetIteratorIndex(wand));
}

void QoreMagickImage::resetIterator(ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    MagickResetIterator(wand);
}

// --- Pixel ---

QoreStringNode* QoreMagickImage::getPixelColor(int64 x, int64 y, ExceptionSink* xsink) {
    QoreAutoRWReadLocker al(rwlock);
    PixelWandHelper pw;
    if (MagickGetImagePixelColor(wand, static_cast<ssize_t>(x), static_cast<ssize_t>(y),
                                 pw) == MagickFalse) {
        checkMagickError(wand, "error getting pixel color", xsink);
        return nullptr;
    }
    return ::getPixelColor(pw);
}

void QoreMagickImage::setPixelColor(int64 x, int64 y, const char* color, ExceptionSink* xsink) {
    QoreAutoRWWriteLocker al(rwlock);
    PixelWandHelper pw(color);
    if (MagickSetImagePixelColor(wand, static_cast<ssize_t>(x), static_cast<ssize_t>(y),
                                 pw) == MagickFalse) {
        checkMagickError(wand, "error setting pixel color", xsink);
    }
}
