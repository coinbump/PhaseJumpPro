#ifndef ExpBitmap_hpp
#define ExpBitmap_hpp

enum class ScanLine {
    ScanFromLeft,
    ScanFromTop,
    ScanFromRight,
    ScanFromBottom

};

enum class BitmapAlpha {
    None,               /* For example, RGB. */
    PremultipliedLast,  /* For example, premultiplied RGBA */
    PremultipliedFirst, /* For example, premultiplied ARGB */
    Last,               /* For example, non-premultiplied RGBA */
    First,              /* For example, non-premultiplied ARGB */
    Only                /* No color data, alpha data only */
};

class Image;

/*
    Bitmap

    Stores pixel data offscreen as a series of pixels.

    Currently this only supports RGBA, 32-bit bitmaps.

    FUTURE: support multiple pixel formats.

    NOTE: this assumes reading, not Cartesian coordinates.

    FUTURE: evaluate use of Vigra graphics library.
    http://hci.iwr.uni-heidelberg.de/vigra/

    FUTURE: evaluate hq3x, etc. algorithms:
    http://en.wikipedia.org/wiki/2xSaI#2xSaI

    TODO: evaluate libpng, NSBitmapRep for image conversions.
    TODO: support 16 bit bitmaps, 16 bit to 32 bit color conversion.

    FUTURE: Evaluate:
    Convolution Filters:
    http://www.roborealm.com/help/Convolution.php
    http://www.fho-emden.de/~hoffmann/bicubic03042002.pdf
    Resampling Kernel:
   http://docs.google.com/viewer?a=v&q=cache:ILj2E1xoIxoJ:ftp2.bmtmicro.com/dlc/Resampling.pdf+bicubic+resampling+algorithm&hl=en&gl=us&pid=bl&srcid=ADGEEShHeAfwCSdpgzcpOAt3r1jvP8BcfekIpcOfFdJchMejQiNaeJxzfzS5-zNWidAXGvp00HqDY7Vva8dForCSd1jZrDh14pEZbsiXRrzzYnBHrgbJ6uVzbyKDLjvwAkIW6XbRR1Ke&sig=AHIEtbQWg08eTxsHW9sHIZ5xwY0QoUyY8w

 */
class ExpBitmap : public RetainObject {
protected:
    void* get_pixel_data(int x, int y) const;

protected:
    // RESAMPLING: (in progress)
    int GetIntResamplePixelPos(float width, float x, float destWidth);
    float GetFResamplePixelPos(float width, float x, float destWidth);
    FColor GetUpsampleBicubicDestColor(Vector2Int sourcePt, Vector2Int destSize);
    float BicubicWeightFoo(float x);
    float BicubicP(float x);
    FColor GetScaledBoxAverageColor(float x, float y, float scale);

public:
    PixelFormat mPixelFormat;
    BitmapAlpha mAlphaFormat;
    Vector2Int mSize;

    // OPTIONAL:
    String mName; // Use for identifying bitmaps.

    Bitmap();
    virtual ~Bitmap();

    virtual void Flush();

    int GetNumBitsForColorComponent(ColorComponent c) const;
    bool HasAlpha() const;
    bool IsAlphaBinary() const;

    virtual void Build(int width, int height, PixelFormat pf);

    virtual void RenderBitmap(Bitmap* bitmap, int x, int y);
    void ReplaceData(void* data, int width, int height);

    Bitmap* Copy();

    virtual Bitmap* New() const {
        return NewBitmap();
    }

    virtual Bitmap* NewSubBitmap(VecRect2Int bounds);
    Bitmap* CreateUpsampleBicubic(int destWidth, int destHeight);
    Bitmap* CreateDownsampleBoxAverage(float scale);

    virtual void CopyInto(Bitmap* into) const;

    Color GetPixelColor(uint32_t x, uint32_t y) const;
    Color GetPixelIndexColor(uint32_t i) const;
    void SetPixelColor(uint32_t x, uint32_t y, Color color);
    void SetPixelIndexColor(uint32_t i, Color color);
    bool AnyPixelsOnVScanLine(int x, bool test = false, Color testColor = ColorNone) const;
    bool AnyPixelsOnScanLine(int y, bool test = false, Color testColor = ColorNone) const;
    int GetFirstPixelScanlineLToR(int y) const;
    int GetFirstPixelScanlineRToL(int y) const;
    int GetFirstPixelScanlineTToB(int x) const;
    int GetFirstPixelScanlineBToT(int x) const;
    int GetFirstPixelYOnVScanLine(int x) const;
    int ScanToAnyPixels(ScanLine scan, bool test = false, Color testColor = ColorNone) const;

    static Bitmap* NewBitmap(); // Use to get platform-correct class

    // RENDERS:
    void EraseRect(VecRect2Int const& rect);

    // UTILITIES:
    virtual VecRect2Int TrimToAnyPixels(int border);
    virtual uint32_t
    FloodFillAnyAlpha(Vector2Int firstPixel, FColor fillColor, bool alter, VecRect2Int& fillBounds);

    virtual Image* NewImage() {
        return NULL;
    }

    virtual void ImportImage(Image* image) {
        PJLog("UNIMPLEMENTED. Bitmap.ImportImage not supported on this platform.");
    }

    virtual bool IsPremultAlpha() const {
        return true;
    } // FUTURE: support test for this.

    bool empty() const {
        return Width() <= 0 || Height() <= 0;
    }

    bool SearchIsOpaque() const;

    uint32_t GetPixelDataSize() const {
        return BytesPerPixel();
    } // FUTURE: support more pixel formats

    uint32_t BytesPerPixel() const;

    // DATA:
    void* GetPixelData(int x, int y) const {
        return get_pixel_data(x, y);
    }

    char* NewVPixelLine(int x) const;
    char* NewHPixelLine(int y) const;

    // GO:
    void Stop() {
        Flush();
    }

    uint32_t Width() const {
        return mSize.x();
    }

    uint32_t Height() const {
        return mSize.y();
    }

    uint32_t GetNumPixels() const {
        return Width() * Height();
    }
};

#endif /* ExpBitmap_hpp */
