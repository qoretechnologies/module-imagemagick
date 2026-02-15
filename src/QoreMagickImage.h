/* -*- mode: c++; indent-tabs-mode: nil -*- */
/** @file QoreMagickImage.h QoreMagickImage class header */
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

#ifndef _QORE_IMAGEMAGICK_QOREMAGICKIMAGE_H
#define _QORE_IMAGEMAGICK_QOREMAGICKIMAGE_H

#include "imagemagick-module.h"
#include "ImageMagickHelper.h"

#include <atomic>

//! QoreMagickImage - private data class for MagickImage Qore class
/** This class wraps a MagickWand* and is thread-safe via QoreRWLock.
    Each instance owns its own MagickWand.
*/
class QoreMagickImage : public AbstractPrivateData {
public:
    //! Constructor from file path
    DLLLOCAL QoreMagickImage(const char* path, ExceptionSink* xsink);

    //! Constructor from binary data
    DLLLOCAL QoreMagickImage(const BinaryNode* data, ExceptionSink* xsink);

    //! Constructor from binary data with explicit format
    DLLLOCAL QoreMagickImage(const BinaryNode* data, const char* format, ExceptionSink* xsink);

    //! Constructor for blank image
    DLLLOCAL QoreMagickImage(size_t width, size_t height, const char* background, ExceptionSink* xsink);

    //! Copy constructor (clone wand)
    DLLLOCAL QoreMagickImage(const QoreMagickImage& old, ExceptionSink* xsink);

    //! Destructor
    DLLLOCAL virtual ~QoreMagickImage();

    //! Check if the last operation was interrupted and raise PROGRAM-INTERRUPTED if so
    /** @return true if interrupted (exception raised), false otherwise
    */
    DLLLOCAL bool checkInterrupted(ExceptionSink* xsink);

    // --- I/O ---
    DLLLOCAL void readFile(const char* path, ExceptionSink* xsink);
    DLLLOCAL void readData(const BinaryNode* data, ExceptionSink* xsink);
    DLLLOCAL void readDataWithFormat(const BinaryNode* data, const char* format, ExceptionSink* xsink);
    DLLLOCAL QoreHashNode* pingFile(const char* path, ExceptionSink* xsink);
    DLLLOCAL QoreHashNode* pingData(const BinaryNode* data, ExceptionSink* xsink);
    DLLLOCAL void writeFile(const char* path, ExceptionSink* xsink);
    DLLLOCAL void writeFiles(const char* path, bool adjoin, ExceptionSink* xsink);
    DLLLOCAL BinaryNode* toData(ExceptionSink* xsink);
    DLLLOCAL BinaryNode* toDataWithFormat(const char* format, ExceptionSink* xsink);

    // --- Info/Metadata ---
    DLLLOCAL int64 getWidth(ExceptionSink* xsink);
    DLLLOCAL int64 getHeight(ExceptionSink* xsink);
    DLLLOCAL QoreStringNode* getFormat(ExceptionSink* xsink);
    DLLLOCAL void setFormat(const char* format, ExceptionSink* xsink);
    DLLLOCAL int64 getDepth(ExceptionSink* xsink);
    DLLLOCAL void setDepth(int64 depth, ExceptionSink* xsink);
    DLLLOCAL QoreStringNode* getColorspace(ExceptionSink* xsink);
    DLLLOCAL void setColorspace(const char* cs, ExceptionSink* xsink);
    DLLLOCAL QoreStringNode* getImageType(ExceptionSink* xsink);
    DLLLOCAL QoreHashNode* getResolution(ExceptionSink* xsink);
    DLLLOCAL void setResolution(double x, double y, ExceptionSink* xsink);
    DLLLOCAL int64 getCompressionQuality(ExceptionSink* xsink);
    DLLLOCAL void setCompressionQuality(int64 quality, ExceptionSink* xsink);
    DLLLOCAL QoreStringNode* getSignature(ExceptionSink* xsink);
    DLLLOCAL QoreStringNode* identify(ExceptionSink* xsink);
    DLLLOCAL int64 getColors(ExceptionSink* xsink);
    DLLLOCAL int64 getNumberImages(ExceptionSink* xsink);
    DLLLOCAL QoreHashNode* getInfo(ExceptionSink* xsink);

    // --- Properties/Profiles ---
    DLLLOCAL QoreStringNode* getProperty(const char* name, ExceptionSink* xsink);
    DLLLOCAL QoreHashNode* getProperties(const char* pattern, ExceptionSink* xsink);
    DLLLOCAL void setProperty(const char* name, const char* value, ExceptionSink* xsink);
    DLLLOCAL void deleteProperty(const char* name, ExceptionSink* xsink);
    DLLLOCAL BinaryNode* getProfile(const char* name, ExceptionSink* xsink);
    DLLLOCAL void setProfile(const char* name, const BinaryNode* data, ExceptionSink* xsink);
    DLLLOCAL QoreListNode* getProfileNames(ExceptionSink* xsink);
    DLLLOCAL void strip(ExceptionSink* xsink);

    // --- Resize ---
    DLLLOCAL void resize(int64 width, int64 height, const char* filter, ExceptionSink* xsink);
    DLLLOCAL void scale(int64 width, int64 height, ExceptionSink* xsink);
    DLLLOCAL void thumbnail(int64 width, int64 height, ExceptionSink* xsink);
    DLLLOCAL void adaptiveResize(int64 width, int64 height, ExceptionSink* xsink);
    DLLLOCAL void sample(int64 width, int64 height, ExceptionSink* xsink);
    DLLLOCAL void resample(double x_res, double y_res, const char* filter, ExceptionSink* xsink);
    DLLLOCAL void liquidRescale(int64 width, int64 height, double delta_x, double rigidity,
                                ExceptionSink* xsink);

    // --- Crop ---
    DLLLOCAL void crop(int64 width, int64 height, int64 x, int64 y, ExceptionSink* xsink);
    DLLLOCAL void trim(double fuzz, ExceptionSink* xsink);
    DLLLOCAL void extent(int64 width, int64 height, int64 x, int64 y, ExceptionSink* xsink);
    DLLLOCAL void chop(int64 width, int64 height, int64 x, int64 y, ExceptionSink* xsink);
    DLLLOCAL void shave(int64 columns, int64 rows, ExceptionSink* xsink);
    DLLLOCAL void splice(int64 width, int64 height, int64 x, int64 y, ExceptionSink* xsink);

    // --- Transform ---
    DLLLOCAL void rotate(double degrees, const char* background, ExceptionSink* xsink);
    DLLLOCAL void flip(ExceptionSink* xsink);
    DLLLOCAL void flop(ExceptionSink* xsink);
    DLLLOCAL void transpose(ExceptionSink* xsink);
    DLLLOCAL void transverse(ExceptionSink* xsink);
    DLLLOCAL void shear(double x, double y, const char* background, ExceptionSink* xsink);
    DLLLOCAL void deskew(double threshold, ExceptionSink* xsink);
    DLLLOCAL void autoOrient(ExceptionSink* xsink);

    // --- Blur/Sharpen ---
    DLLLOCAL void blur(double radius, double sigma, ExceptionSink* xsink);
    DLLLOCAL void gaussianBlur(double radius, double sigma, ExceptionSink* xsink);
    DLLLOCAL void adaptiveBlur(double radius, double sigma, ExceptionSink* xsink);
    DLLLOCAL void sharpen(double radius, double sigma, ExceptionSink* xsink);
    DLLLOCAL void adaptiveSharpen(double radius, double sigma, ExceptionSink* xsink);
    DLLLOCAL void unsharpMask(double radius, double sigma, double gain, double threshold,
                              ExceptionSink* xsink);
    DLLLOCAL void motionBlur(double radius, double sigma, double angle, ExceptionSink* xsink);
    DLLLOCAL void selectiveBlur(double radius, double sigma, double threshold, ExceptionSink* xsink);
    DLLLOCAL void rotationalBlur(double angle, ExceptionSink* xsink);

    // --- Color ---
    DLLLOCAL void brightnessContrast(double brightness, double contrast, ExceptionSink* xsink);
    DLLLOCAL void gamma(double value, ExceptionSink* xsink);
    DLLLOCAL void level(double black, double gamma_val, double white, ExceptionSink* xsink);
    DLLLOCAL void modulate(double brightness, double saturation, double hue, ExceptionSink* xsink);
    DLLLOCAL void negate(bool gray_only, ExceptionSink* xsink);
    DLLLOCAL void normalize(ExceptionSink* xsink);
    DLLLOCAL void equalize(ExceptionSink* xsink);
    DLLLOCAL void autoGamma(ExceptionSink* xsink);
    DLLLOCAL void autoLevel(ExceptionSink* xsink);
    DLLLOCAL void sigmoidalContrast(bool sharpen, double strength, double midpoint,
                                    ExceptionSink* xsink);
    DLLLOCAL void contrastStretch(double black, double white, ExceptionSink* xsink);
    DLLLOCAL void linearStretch(double black, double white, ExceptionSink* xsink);
    DLLLOCAL void enhance(ExceptionSink* xsink);
    DLLLOCAL void whiteBalance(ExceptionSink* xsink);
    DLLLOCAL void transformColorspace(const char* cs, ExceptionSink* xsink);

    // --- Effects ---
    DLLLOCAL void charcoal(double radius, double sigma, ExceptionSink* xsink);
    DLLLOCAL void edge(double radius, ExceptionSink* xsink);
    DLLLOCAL void emboss(double radius, double sigma, ExceptionSink* xsink);
    DLLLOCAL void oilPaint(double radius, double sigma, ExceptionSink* xsink);
    DLLLOCAL void sketch(double radius, double sigma, double angle, ExceptionSink* xsink);
    DLLLOCAL void solarize(double threshold, ExceptionSink* xsink);
    DLLLOCAL void sepiaTone(double threshold, ExceptionSink* xsink);
    DLLLOCAL void vignette(double radius, double sigma, int64 x, int64 y, ExceptionSink* xsink);
    DLLLOCAL void shadow(double alpha, double sigma, int64 x, int64 y, ExceptionSink* xsink);
    DLLLOCAL void shade(bool gray, double azimuth, double elevation, ExceptionSink* xsink);
    DLLLOCAL void wave(double amplitude, double wavelength, ExceptionSink* xsink);
    DLLLOCAL void swirl(double degrees, ExceptionSink* xsink);
    DLLLOCAL void implode(double amount, ExceptionSink* xsink);
    DLLLOCAL void despeckle(ExceptionSink* xsink);
    DLLLOCAL void posterize(int64 levels, bool dither, ExceptionSink* xsink);
    DLLLOCAL void threshold(double value, ExceptionSink* xsink);
    DLLLOCAL void addNoise(const char* noise_type, double attenuate, ExceptionSink* xsink);
    DLLLOCAL void blueShift(double factor, ExceptionSink* xsink);
    DLLLOCAL void reduceNoise(double radius, ExceptionSink* xsink);
    DLLLOCAL void waveletDenoise(double threshold, double softness, ExceptionSink* xsink);

    // --- Composite ---
    DLLLOCAL void composite(QoreMagickImage* source, const char* op, int64 x, int64 y,
                            ExceptionSink* xsink);
    DLLLOCAL void compositeGravity(QoreMagickImage* source, const char* op, const char* gravity,
                                   ExceptionSink* xsink);

    // --- Drawing ---
    DLLLOCAL void annotate(const char* text, double x, double y, double angle, const char* font,
                           double font_size, const char* fill_color, const char* stroke_color,
                           ExceptionSink* xsink);
    DLLLOCAL void draw(DrawingWand* dw, ExceptionSink* xsink);
    DLLLOCAL void border(const char* color, int64 width, int64 height, ExceptionSink* xsink);
    DLLLOCAL void frame(const char* color, int64 width, int64 height, int64 inner_bevel,
                        int64 outer_bevel, ExceptionSink* xsink);

    // --- Multi-image ---
    DLLLOCAL bool hasNextImage(ExceptionSink* xsink);
    DLLLOCAL bool hasPreviousImage(ExceptionSink* xsink);
    DLLLOCAL bool nextImage(ExceptionSink* xsink);
    DLLLOCAL bool previousImage(ExceptionSink* xsink);
    DLLLOCAL void setIteratorIndex(int64 index, ExceptionSink* xsink);
    DLLLOCAL int64 getIteratorIndex(ExceptionSink* xsink);
    DLLLOCAL void resetIterator(ExceptionSink* xsink);

    // --- Pixel ---
    DLLLOCAL QoreStringNode* getPixelColor(int64 x, int64 y, ExceptionSink* xsink);
    DLLLOCAL void setPixelColor(int64 x, int64 y, const char* color, ExceptionSink* xsink);

    //! Get internal wand (for read-only operations)
    DLLLOCAL MagickWand* getWand() const { return wand; }

private:
    MagickWand* wand;
    mutable QoreRWLock rwlock;
    //! Sandbox manager helper for interrupt checking (acquired at construction)
    QoreSandboxManagerHelper smh;
    //! Flag set by progress monitor when interrupt is detected
    std::atomic<bool> interrupted{false};

    //! Set up progress monitor for interruptible operations
    DLLLOCAL void setupProgressMonitor();

    //! Progress monitor callback - checks for sandbox interrupt
    static MagickBooleanType progressMonitor(const char* tag, const MagickOffsetType offset,
                                              const MagickSizeType size, void* client_data);
};

#endif // _QORE_IMAGEMAGICK_QOREMAGICKIMAGE_H
