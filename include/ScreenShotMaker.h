#ifndef _ScreenShotMaker_h__
#define _ScreenShotMaker_h__
#else
#error reinclude ScreenShotMaker.h
#endif

namespace TheHeaven
{
  namespace Graphics
  {
    enum class ScreenshotFormat
    {
      BMP,
      JPEG,
      GIF,
      TIFF,
      PNG,
    };

    // func void MakeScreenShot(PALETTEENTRY palette[256], int width, int height, size_t pitch const unsigned char* rawData)
    //
    // Make screenshot using 256-colors palette and pointer to raw screen surface data
    //
    // @param palette Array of 256 PALETTEENTRY. You can get it using DirectDrawPalettePtr->GetEntries()
    // @param width Screen width
    // @param height Screen height
    // @param pitch Pitch size of screen surface - distance to start of next line
    // @param rawData Pointer to raw screen surface data
    void MakeScreenShot(
      PALETTEENTRY* palette, 
      int width, 
      int height, 
      size_t pitch, 
      const unsigned char* rawData,
      ScreenshotFormat format)/* throw(std::runtime_error, std::invalid_argument, std::bad_alloc)*/;

    void MakeScreenShot(IDirectDrawSurface7* surface, const ScreenshotFormat format);

  }// namespace Graphics
}// namespace TheHeaven