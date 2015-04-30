#include"FileLoader.h"
#include"SanFileIO.h"
using namespace std;
namespace San
{
	namespace FileIOStream
	{
#ifndef __CTGAREADER_H__
#define __CTGAREADER_H__
		struct stTGAFILEHEADER
		{
			uint8 tgaInfo;
			bool tgaClrTab;
			uint8 tgaType;
			uint16 tgaClrTabHead;
			uint16 tgaClrTabSize;
			//uint8 tgaClrTabBit;
			uint16 tgaPixX;
			uint16 tgaPixY;
			uint16 tgaWidth;
			uint16 tgaHeight;
			uint8 tgaBitCount;
			uint8 tgaMapDis;
		};
		class cTGALoader
		{
		private:
			stTGAFILEHEADER *m_pTGAHeader;
			uint8 m_RGBChange;
			FILEROAD m_pRoad;
			uint8 *m_image;
			int32 m_TGAImageSize;
			bool m_TGAFileLoadType;
			MAPTYPE m_TGAType;
		public:
			cTGALoader(FILEROAD road)
			{
				this->m_pTGAHeader = new stTGAFILEHEADER;
				this->m_pRoad = road;
				this->m_image = nullptr;
				this->m_TGAImageSize = 0;
				this->m_RGBChange = 0;
				this->m_TGAFileLoadType = false;
				this->m_TGAType = BMP_RGBA;
			};
			~cTGALoader()
			{
				delete this->m_pTGAHeader;
				this->m_image = nullptr;
			};
			int32 iTGAGetHigh() const;
			int32 iTGAGetWeight() const;
			int32 iTGAGetSize() const;
			bool iTGALoad();
			bool iFreeImage();
			FILEROAD iGetRoad() const;
			uint8* iGetImage() const;
			MAPTYPE iGetTGAType() const;
		};
#endif
	}
}