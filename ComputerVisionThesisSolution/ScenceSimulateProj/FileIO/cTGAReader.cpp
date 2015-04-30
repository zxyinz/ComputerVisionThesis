#include"cTGAReader.h"
using namespace San;
using namespace San::FileIOStream;
bool cTGALoader::iTGALoad()
{
	fstream *file;
	file=new fstream;
	int32 PixSize=4;
	file->open(this->m_pRoad,ios::binary|ios::in,0x40);
	if(file==nullptr)
	{
		return false;
	}
	file->read((char*)this->m_pTGAHeader,sizeof(stTGAFILEHEADER));
	if((this->m_pTGAHeader->tgaType!=2)||(this->m_pTGAHeader->tgaClrTab==1))
	{
		return false;
	}
	this->m_TGAImageSize=this->m_pTGAHeader->tgaWidth*this->m_pTGAHeader->tgaHeight*PixSize;
	this->m_image=new uint8[this->m_TGAImageSize];
	if(this->m_image==nullptr)
	{
		file->close();
		return false;
	}
	file->read((char*)this->m_image,this->m_TGAImageSize);
	for(int32 seek=0;seek<this->m_TGAImageSize;seek=seek+PixSize)
	{
		this->m_RGBChange=m_image[seek];
		m_image[seek]=m_image[seek+2];
		m_image[seek+2]=this->m_RGBChange;
	}
	file->close();
	this->m_TGAFileLoadType=true;
	return true;
}
int32 cTGALoader::iTGAGetHigh() const
{
	if(!this->m_TGAFileLoadType)
	{
		return 0;
	}
	return this->m_pTGAHeader->tgaHeight;
}
int32 cTGALoader::iTGAGetWeight() const
{
	if(!this->m_TGAFileLoadType)
	{
		return 0;
	}
	return this->m_pTGAHeader->tgaWidth;
}
int32 cTGALoader::iTGAGetSize() const
{
	if(!this->m_TGAFileLoadType)
	{
		return 0;
	}
	return this->m_TGAImageSize;
}
bool cTGALoader::iFreeImage()
{
	if(this->m_image==nullptr)
	{
		return true;
	}
	else
	{
		delete[] this->m_image;
		this->m_image=nullptr;
	}
	return true;
}
FILEROAD cTGALoader::iGetRoad() const
{
	return this->m_pRoad;
}
uint8* cTGALoader::iGetImage() const
{
	if(!this->m_TGAFileLoadType)
	{
		return nullptr;
	}
	return this->m_image;
}
MAPTYPE cTGALoader::iGetTGAType() const
{
	return this->m_TGAType;
}