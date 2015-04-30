#include"FileLoader.h"
#include"SanFileIO.h"
using namespace std;
namespace San
{
	namespace FileIOStream
	{
#ifndef __CBMPREADER_H__
#define __CBMPREADER_H__
#define BITMAPTYPE 0x4d42
		class cBMPLoader
		{
		private:
			BITMAPFILEHEADER *m_pBMPHeader;
			BITMAPINFOHEADER *m_pBMPInfo;
			int32 m_BMPImageSize;
			uint8 *m_image;
			FILEROAD m_pBMPRoad;
			uint8 m_RGBchange;
			bool m_BMPFileLoadType;
			MAPTYPE m_BitmapType;
		public:
			cBMPLoader(FILEROAD road)
			{
				this->m_pBMPHeader = new BITMAPFILEHEADER;
				this->m_pBMPInfo = new BITMAPINFOHEADER;
				this->m_pBMPRoad = road;
				this->m_BMPImageSize = 0;
				this->m_image = nullptr;
				this->m_RGBchange = 0;
				this->m_BMPFileLoadType = false;
				this->m_BitmapType = BMP_RGB;
			};
			~cBMPLoader()
			{
				delete[] m_pBMPHeader;
				delete[] m_pBMPInfo;
				this->m_image = nullptr;
			};
			uint32 iBMPGetHigh() const;
			uint32 iBMPGetWeight() const;
			DWORD iBMPGetSize() const;
			FILEROAD iBMPGetRoad() const;
			bool iBMPLoad();
			bool iFreeImage();
			uint8* iBMPGetImage() const;
			MAPTYPE iGetBitmapType() const;
		};
#endif
	}
}