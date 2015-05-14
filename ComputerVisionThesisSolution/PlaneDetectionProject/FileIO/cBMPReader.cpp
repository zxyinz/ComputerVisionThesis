#include"cBMPReader.h"
using namespace std;
using namespace San;
using namespace San::FileIOStream;
bool cBMPLoader::iBMPLoad()
{
	fstream *file;
	int32 PixSize=3;
	file=new fstream;
	file->open(this->m_pBMPRoad,ios::binary|ios::in,0x40);
	if(file==nullptr)
	{
		return false;
	}
	file->read((char *)this->m_pBMPHeader,sizeof(BITMAPFILEHEADER));
	if(this->m_pBMPHeader->bfType!=BITMAPTYPE)
	{
		return false;
	}
	file->seekg(sizeof(BITMAPFILEHEADER),ios::beg);
	file->read((char *)this->m_pBMPInfo,sizeof(BITMAPINFOHEADER));
	if(m_pBMPInfo->biBitCount==32)
	{
		this->m_BitmapType=BMP_RGBA;
		PixSize=4;
	}
	this->m_BMPImageSize=this->m_pBMPInfo->biHeight*this->m_pBMPInfo->biWidth*PixSize;
	this->m_image=new uint8[this->m_BMPImageSize];
	file->seekg(this->m_pBMPHeader->bfOffBits,ios::beg);
	this->m_image=new uint8[this->m_BMPImageSize];
	if(this->m_image==nullptr)
	{
		file->close();
		return false;
	}
	file->read((char*)this->m_image,this->m_BMPImageSize);
	for(int32 RGBpointer=0;RGBpointer<this->m_BMPImageSize;RGBpointer=RGBpointer+PixSize)
	{
		this->m_RGBchange=this->m_image[RGBpointer];
		this->m_image[RGBpointer]=this->m_image[RGBpointer+2];
		this->m_image[RGBpointer+2]=this->m_RGBchange;
	}
	file->close();
	this->m_BMPFileLoadType=true;
	return true;
}
bool cBMPLoader::iFreeImage()
{
	if(m_image==nullptr)
	{
		return true;
	}
	else
	{
		delete[] m_image;
		if(m_image==nullptr)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
uint32 cBMPLoader::iBMPGetHigh() const
{
	if(!this->m_BMPFileLoadType)
	{
		return 0;
	}
	return this->m_pBMPInfo->biHeight;
}
uint32 cBMPLoader::iBMPGetWeight() const
{
	if(!this->m_BMPFileLoadType)
	{
		return 0;
	}
	return this->m_pBMPInfo->biWidth;
}
DWORD cBMPLoader::iBMPGetSize() const
{
	if(!this->m_BMPFileLoadType)
	{
		return 0;
	}
	return this->m_pBMPHeader->bfSize;
}
FILEROAD cBMPLoader::iBMPGetRoad() const
{
	return this->m_pBMPRoad;
}
uint8* cBMPLoader::iBMPGetImage() const
{
	return this->m_image;
}
MAPTYPE cBMPLoader::iGetBitmapType() const
{
	return this->m_BitmapType;
}