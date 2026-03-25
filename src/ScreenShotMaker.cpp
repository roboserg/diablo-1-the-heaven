#include "stdafx.h"
#include "ScreenShotMaker.h"
#include <gdiplus.h>
#include <memory>
#include <limits>

#pragma comment(lib, "Gdiplus.lib")

// а тут зачем namespace'ы ?
namespace TheHeaven
{
  namespace Graphics
  {

    class GdiPlusHelper
    {
    public:
      GdiPlusHelper()
      {
        Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
      }
      ~GdiPlusHelper()
      {
        Gdiplus::GdiplusShutdown(gdiplusToken);
      }
    
      const wchar_t* ScreenshotFormatToString(const ScreenshotFormat format)
      {
        switch(format)
        {
        case ScreenshotFormat::BMP:  return L"image/bmp";
        case ScreenshotFormat::JPEG: return L"image/jpeg";
        case ScreenshotFormat::GIF:  return L"image/gif";
        case ScreenshotFormat::TIFF: return L"image/tiff";
        case ScreenshotFormat::PNG:  return L"image/png";
        default:
          throw std::invalid_argument("Invalid ScreenshotFormat value");
        }
      }

      int GetEncoderCLSID(const ScreenshotFormat format, CLSID* pClsid)
      {
        const wchar_t* szFormat = ScreenshotFormatToString(format);
        UINT num = 0;          // number of image encoders
        UINT size = 0;         // size of the image encoder array in bytes

        std::unique_ptr<Gdiplus::ImageCodecInfo[]> pImageCodecInfo;

        Gdiplus::GetImageEncodersSize(&num, &size);
        if( size == 0 )
          return -1;  // Failure

        pImageCodecInfo.reset(reinterpret_cast<Gdiplus::ImageCodecInfo*>(new char[size]));

        Gdiplus::GetImageEncoders(num, size, &pImageCodecInfo[0]);

        for( UINT j = 0; j < num; ++j ){
          if( wcscmp(pImageCodecInfo[j].MimeType, szFormat) == 0 ){
            *pClsid = pImageCodecInfo[j].Clsid;
            return j;  // Success
          }    
        }
        return -1;  // Failure
      }

    private:
      Gdiplus::GdiplusStartupInput gdiplusStartupInput;
      ULONG_PTR gdiplusToken;
    };

    // func std::wstring GetNextFileName(const wchar_t* initialFileName)
    // Generates incremental filenames
    // @param initialFileName File name template. Should contain exact one %d in its body. Ex: "screen%d.bmp".
    // @returns First 'empty' file name.
    std::wstring GetNextFileName(const wchar_t* initialFileName)
    {
      std::vector<wchar_t> buffer(MAX_PATH+10);
      for( size_t i = 0; i < numeric_limits<size_t>::max(); ++i ){
        swprintf(&buffer[0], MAX_PATH, initialFileName, i);
        _wfinddata32_t findData;
        intptr_t findHandle = _wfindfirst32(&buffer[0], &findData);
        if( -1 != findHandle ){
          _findclose(findHandle);
        }
        else
        {
          return std::wstring(&buffer[0]);
        }
      }
      throw std::runtime_error("To many screenshot files in a current directory");
    }

    std::wstring ScreenshotFileName(const ScreenshotFormat format)
    {
      //
      // @todo Autoincrement filename in case of file existance
      //
      switch( format ){
      case ScreenshotFormat::BMP:  return GetNextFileName(L"screenshot%.5d.bmp");
      case ScreenshotFormat::GIF:  return GetNextFileName(L"screenshot%.5d.gif");
      case ScreenshotFormat::JPEG: return GetNextFileName(L"screenshot%.5d.jpg");
      case ScreenshotFormat::PNG:  return GetNextFileName(L"screenshot%.5d.png");
      case ScreenshotFormat::TIFF: return GetNextFileName(L"screenshot%.5d.tif");
      }
      throw std::invalid_argument("Invalid screenshot format");
    }

    void MakeScreenShot(IDirectDrawSurface7* surface, const ScreenshotFormat format)
    {
      GdiPlusHelper gph;
      CLSID encoderID;
      if( -1 == gph.GetEncoderCLSID(format, &encoderID) ){
        throw std::runtime_error("Unable to find encoder ID");
      }

      std::unique_ptr<Gdiplus::Bitmap> bmp(Gdiplus::Bitmap::FromDirectDrawSurface7(surface));

      bmp->Save(ScreenshotFileName(format).c_str(), &encoderID);
    }
    
	// Новая фукнция для сохрания скриншотов в PNG 
	void MakeScreenShot(
      PALETTEENTRY* palette, 
      int width, 
      int height, 
      size_t pitch, 
      const unsigned char* rawData,
      ScreenshotFormat format)
    {
      std::vector<COLORREF> buffer(width*height);
      size_t pixelIdx = 0;

      const unsigned char* rawDataPtr = rawData;

      //
      // Convert each raw 
      //
      for( int i = 0; i < height; ++i ){
        for( int j = 0; j < width; ++j ){
          PALETTEENTRY *pal = &palette[rawDataPtr[j]];
          auto r = pal->peRed;
          auto g = pal->peGreen;
          auto b = pal->peBlue;
          buffer[pixelIdx++] = RGB(b, g, r); // Something strange with the palette. I had to swap red and blue components
        }
        rawDataPtr += pitch;
      }

      BITMAP bmp = {0};
      bmp.bmWidth = width;
      bmp.bmHeight = height;
      bmp.bmBitsPixel = 32;
      bmp.bmWidthBytes = width*sizeof(COLORREF);
      bmp.bmBits = &buffer[0];
      bmp.bmPlanes = 1;
      HBITMAP hBm = CreateBitmapIndirect(&bmp);

      GdiPlusHelper gph;
      CLSID encid;
      if( -1 == gph.GetEncoderCLSID(format, &encid) ){
        DeleteObject(hBm);
        throw std::runtime_error("Unable to find encoder ID");
      }
      std::unique_ptr<Gdiplus::Bitmap> gpbmp(Gdiplus::Bitmap::FromHBITMAP(hBm, NULL));
      if( gpbmp ){
        gpbmp->Save(ScreenshotFileName(format).c_str(), &encid);
        gpbmp.reset();
      }
      DeleteObject(hBm);
    }

  }// namespace Graphics
}// namespace TheHeaven