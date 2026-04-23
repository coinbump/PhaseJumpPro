#include "MacMovie.h"

#include "Bitmap.h"
#include "GLTexture.h"

#import <AVFoundation/AVFoundation.h>
#import <CoreVideo/CoreVideo.h>

using namespace PJ;

struct MacMovie::Impl {
    AVPlayer* player{};
    AVPlayerItem* item{};
    AVPlayerItemVideoOutput* output{};
};

MacMovie::MacMovie(Config const& config) :
    impl(std::make_unique<Impl>()) {
    @autoreleasepool {
        NSString* nsPath = [NSString stringWithUTF8String:config.filePath.string().c_str()];
        NSURL* url = [NSURL fileURLWithPath:nsPath];

        AVURLAsset* asset = [AVURLAsset URLAssetWithURL:url options:nil];
        impl->item = [AVPlayerItem playerItemWithAsset:asset];

        // Request BGRA frames
        NSDictionary* attrs = @{
            (NSString*)kCVPixelBufferPixelFormatTypeKey : @(kCVPixelFormatType_32BGRA)
        };
        impl->output = [[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:attrs];
        [impl->item addOutput:impl->output];

        impl->player = [AVPlayer playerWithPlayerItem:impl->item];
    }
}

MacMovie::~MacMovie() {
    @autoreleasepool {
        if (impl && impl->player) {
            [impl->player pause];
        }
        impl.reset();
    }
}

void MacMovie::Play() {
    @autoreleasepool {
        GUARD(impl && impl->player)
        [impl->player play];
    }
}

void MacMovie::Pause() {
    @autoreleasepool {
        GUARD(impl && impl->player)
        [impl->player pause];
    }
}

SP<Texture> MacMovie::MakeTexture() {
    @autoreleasepool {
        GUARDR(impl && impl->output && impl->player, texture)

        CMTime const time = [impl->player currentTime];
        GUARDR(CMTIME_IS_VALID(time), texture)

        // Return latest cached frame
        if (![impl->output hasNewPixelBufferForItemTime:time]) {
            return texture;
        }

        CVPixelBufferRef pixelBuffer =
            [impl->output copyPixelBufferForItemTime:time itemTimeForDisplay:NULL];
        GUARDR(pixelBuffer, texture)

        CVPixelBufferLockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);

        size_t const width = CVPixelBufferGetWidth(pixelBuffer);
        size_t const height = CVPixelBufferGetHeight(pixelBuffer);
        size_t const rowBytes = CVPixelBufferGetBytesPerRow(pixelBuffer);
        auto* source = static_cast<uint8_t const*>(CVPixelBufferGetBaseAddress(pixelBuffer));

        if (source && width > 0 && height > 0) {
            // Pack BGRA → RGBA into a contiguous buffer for GL upload. AVFoundation's row
            // stride often exceeds width*4 for alignment, so we can't feed pixels directly.
            RGBABitmap::Config bitmapConfig{ .size = Vector2Int((int)width, (int)height) };
            auto bitmap = NEW<RGBABitmap>(bitmapConfig);
            auto& pixels = bitmap->Pixels();

            for (size_t y = 0; y < height; y++) {
                uint8_t const* srcRow = source + y * rowBytes;
                auto* dstRow = &pixels[y * width];
                for (size_t x = 0; x < width; x++) {
                    uint8_t const* srcPixel = srcRow + x * 4;
                    dstRow[x] = RGBAColor(
                        srcPixel[2], srcPixel[1], srcPixel[0], srcPixel[3]
                    );
                }
            }

            texture = MAKE<GLTexture>(GLTexture::Config{ .bitmap = bitmap.get() });
        }

        CVPixelBufferUnlockBaseAddress(pixelBuffer, kCVPixelBufferLock_ReadOnly);
        CVBufferRelease(pixelBuffer);

        return texture;
    }
}
