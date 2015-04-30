#include"cSanVisualMouse.h"
using namespace std;
using namespace San;
using namespace San::Device;
#ifndef _DIRECTX
bool cSanVisualMouse::iUpdate()
{
	DWORD DataNumber=1;
	::memset((this->m_MDBuffer),0,sizeof(this->m_MDBuffer));
	for(int seek=0;seek<this->m_SetCount;seek=seek+1)
	{
		stSPOINT3EX<long> PosOffset;
		::memset(&(PosOffset),0,sizeof(PosOffset));
		if(this->m_MDBuffer[seek].dwOfs==DIMOFS_X)
		{
			PosOffset.x=PosOffset.x+this->m_MDBuffer[seek].dwData;
		}
		if(this->m_MDBuffer[seek].dwOfs==DIMOFS_Y)
		{
			PosOffset.y=PosOffset.y+this->m_MDBuffer[seek].dwData;
		}
		if(this->m_MDBuffer[seek].dwOfs==DIMOFS_Z)
		{
			PosOffset.z=PosOffset.z+this->m_MDBuffer[seek].dwData;
		}
		if((PosOffset.x!=0)||(PosOffset.y!=0)||(PosOffset.z!=0))
		{
			this->m_bMouseMove=true;
		}
		this->m_MousePos.x=this->m_MousePos.x+PosOffset.x;
		this->m_MousePos.y=this->m_MousePos.y+PosOffset.y;
		this->m_MousePos.z=this->m_MousePos.z+PosOffset.z;
	}
	this->m_SetCount=0;
	//::cout<<this->m_MousePos.x<<"	"<<this->m_MousePos.y<<"\n";
	//::SetCursorPos(this->m_MousePos.x,this->m_MousePos.y);
	return true;
}
void cSanVisualMouse::iClear()
{
	::memset((this->m_MDBuffer),0,sizeof(this->m_MDBuffer));
	::memset(&(this->m_MousePos),0,sizeof(this->m_MousePos));
}
void cSanVisualMouse::iSetMousePos(stSPOINT3EX<long> Pos)
{
	this->m_MousePos.x=Pos.x;
	this->m_MousePos.y=Pos.y;
	this->m_MousePos.z=Pos.z;
	//::SetCursorPos(this->m_MousePos.x,this->m_MousePos.y);/////////////////////////////////////////////////////
}
void cSanVisualMouse::iSetMousePos(long x,long y,long z)
{
	this->m_MousePos.x=x;
	this->m_MousePos.y=y;
	this->m_MousePos.z=z;
	//::SetCursorPos(this->m_MousePos.x,this->m_MousePos.y);/////////////////////////////////////////////////////
}
stSPOINT3EX<long> cSanVisualMouse::iGetMousePos()
{
	return this->m_MousePos;
}
DWORD cSanVisualMouse::iGetMouseState()
{
	DWORD State=NULL;
	for(int seek=0;seek<MD_SIZE;seek=seek+1)
	{
		if((this->m_MDBuffer[seek].dwOfs==DIMOFS_BUTTON0)&&(this->m_MDBuffer[seek].dwData&0x80))
		{
			State=State|MDS_LBUTTON|MDS_BUTTONDOWN;
		}
		if((this->m_MDBuffer[seek].dwOfs==DIMOFS_BUTTON0)&&!(this->m_MDBuffer[seek].dwData&0x80))
		{
			State=State|MDS_LBUTTON|MDS_BUTTONUP;
		}
		if((this->m_MDBuffer[seek].dwOfs==DIMOFS_BUTTON1)&&(this->m_MDBuffer[seek].dwData&0x80))
		{
			State=State|MDS_RBUTTON|MDS_BUTTONDOWN;
		}
		if((this->m_MDBuffer[seek].dwOfs==DIMOFS_BUTTON1)&&!(this->m_MDBuffer[seek].dwData&0x80))
		{
			State=State|MDS_RBUTTON|MDS_BUTTONUP;
		}
		if((this->m_MDBuffer[seek].dwOfs==DIMOFS_BUTTON2)&&(this->m_MDBuffer[seek].dwData&0x80))
		{
			State=State|MDS_MBUTTON|MDS_BUTTONUP;
		}
		if((this->m_MDBuffer[seek].dwOfs==DIMOFS_BUTTON2)&&!(this->m_MDBuffer[seek].dwData&0x80))
		{
			State=State|MDS_MBUTTON|MDS_BUTTONDOWN;
		}
	}
	if(this->m_bMouseMove)
	{
		State=State|MDS_MOVE;
	}
	this->m_bMouseMove=false;
	return State;
}
#endif