#include"SanGraphics.h"
using namespace std;
using namespace San;
using namespace San::Mathematics;
using namespace San::Graphics;
stSANCOLOR& stSANCOLOR::operator+(const stSANCOLOR& Color)
{
	int16 value=this->r+Color.r;
	this->r=value>255?255:value;
	value=this->g+Color.g;
	this->g=value>255?255:value;
	value=this->b+Color.b;
	this->b=value>255?255:value;
	value=this->a+Color.a;
	this->a=value>255?255:value;
	return *this;
}
stSANCOLOR& stSANCOLOR::operator-(const stSANCOLOR& Color)
{
	int16 value=this->r-Color.r;
	this->r=value<0?0:value;
	value=this->g-Color.g;
	this->g=value<0?0:value;
	value=this->b-Color.b;
	this->b=value<0?0:value;
	value=this->a-Color.a;
	this->a=value<0?0:value;
	return *this;
}
stSANCOLOR& stSANCOLOR::operator=(const stSANCOLORF& Color)
{
	this->r=Color.r*255;
	this->g=Color.g*255;
	this->b=Color.b*255;
	this->a=Color.a*255;
	return *this;
}
/*Color Float*/
stSANCOLORF& stSANCOLORF::operator+(const stSANCOLORF& Color)
{
	sfloat value=this->r+Color.r;
	//this->r=value>1.0f?1.0f:value;
	value=this->g+Color.g;
	//this->g=value>1.0f?1.0f:value;
	value=this->b+Color.b;
	//this->b=value>1.0f?1.0f:value;
	value=this->a+Color.a;
	//this->a=value>1.0f?1.0f:value;
	return *this;
}
stSANCOLORF& stSANCOLORF::operator-(const stSANCOLORF& Color)
{
	sfloat value=this->r-Color.r;
	//this->r=value<0.0f?0.0f:value;
	value=this->g-Color.g;
	//this->g=value<0.0f?0.0f:value;
	value=this->b-Color.b;
	//this->b=value<0.0f?0.0f:value;
	value=this->a-Color.a;
	//this->a=value<0.0f?0.0f:value;
	return *this;
}
stSANCOLORF& stSANCOLORF::operator=(const stSANCOLOR& Color)
{
	this->r = Color.r;// / 255;
	this->g = Color.g;// / 255;
	this->b = Color.b;// / 255;
	this->a = Color.a;// / 255;
	return *this;
}
/*Graphics Vector*/
stSANGRAPHICSPOINT::stSANGRAPHICSPOINT()
	:p_vt(nullptr),
	vt_size(0)
{}
stSANGRAPHICSPOINT::stSANGRAPHICSPOINT(const stSANGRAPHICSPOINT& P)
{
	v=P.v;
	vn=P.vn;
	vc=P.vc;
	if(P.vt_size!=0)
	{
		vt_size=P.vt_size;
		p_vt=new SVECTOR3[vt_size];
		::memcpy(p_vt,P.p_vt,sizeof(SVECTOR3)*vt_size);
	}
}
stSANGRAPHICSPOINT::~stSANGRAPHICSPOINT()
{
	if(p_vt!=nullptr)
	{
		delete[] p_vt;
		p_vt=nullptr;
		vt_size=0;
	}
}
stSANGRAPHICSPOINT& stSANGRAPHICSPOINT::operator=(const stSANGRAPHICSPOINT& P)
{
	lpSVECTOR3 buffer=nullptr;
	this->v=P.v;
	this->vn=P.vn;
	this->vc=P.vc;
	if(P.vt_size!=0)
	{
		buffer=new SVECTOR3[this->vt_size];
		::memcpy(buffer,P.p_vt,vt_size*sizeof(SVECTOR3));
	}
	if(this->vt_size!=0)
	{
		delete[] this->p_vt;// may cause problem when vt_size equal 1
	}
	this->vt_size=P.vt_size;
	this->p_vt=buffer;
	buffer=nullptr;
	return *this;
}
/*Skelecton Desc*/
stSANSKELETONPOINTDESC::stSANSKELETONPOINTDESC()
	:skeleton_list_ptr(nullptr),
	weight_list_ptr(nullptr),
	list_size(0)
{}
stSANSKELETONPOINTDESC::stSANSKELETONPOINTDESC(const stSANSKELETONPOINTDESC& P)
{
	if(P.list_size!=0)
	{
		this->skeleton_list_ptr=new uint32[P.list_size];
		this->weight_list_ptr=new sfloat[P.list_size];
		for(uint32 seek=0;seek<P.list_size;seek=seek+1)
		{
			this->skeleton_list_ptr[seek]=0;
			this->weight_list_ptr[seek]=0.0f;
		}
		this->list_size=P.list_size;
	}
}
stSANSKELETONPOINTDESC::~stSANSKELETONPOINTDESC()
{
	if(this->list_size!=0)
	{
		delete[] this->skeleton_list_ptr;
		this->skeleton_list_ptr=nullptr;
		delete[] this->weight_list_ptr;
		this->weight_list_ptr=nullptr;
		this->list_size=0;
	}
}
stSANSKELETONPOINTDESC& stSANSKELETONPOINTDESC::operator=(const stSANSKELETONPOINTDESC& P)
{
	uint32* p_skeletonbuffer=nullptr;
	sfloat* p_weightbuffer=nullptr;
	if(P.list_size!=0)
	{
		p_skeletonbuffer=new uint32[P.list_size];
		::memcpy(p_skeletonbuffer,P.skeleton_list_ptr,P.list_size*sizeof(uint32));
		p_weightbuffer=new sfloat[P.list_size];
		::memcpy(p_weightbuffer,P.weight_list_ptr,P.list_size*sizeof(sfloat));
	}
	if(this->list_size!=0)
	{
		delete[] this->skeleton_list_ptr;
		delete[] this->weight_list_ptr;
	}
	this->list_size=P.list_size;
	this->skeleton_list_ptr=p_skeletonbuffer;
	this->weight_list_ptr=p_weightbuffer;
	p_skeletonbuffer=nullptr;
	p_weightbuffer=nullptr;
	return *this;
}
#ifdef _DIRECTX
void stSANXFRAMEELEMENT::UpdateMatrices(const D3DXFRAME *inFrame,const D3DXMATRIX *parentMatrix)
{
	stD3DFRAMESV *pFrame=(stD3DFRAMESV*)inFrame;
	if(parentMatrix!=NULL)
	{
		::D3DXMatrixMultiply(&(pFrame->Matrix),&(pFrame->TransformationMatrix),parentMatrix);
	}
	else
	{
		pFrame->Matrix=pFrame->TransformationMatrix;
	}
	if(pFrame->pFrameSibling!=NULL)
	{
		this->UpdateMatrices(pFrame->pFrameSibling,parentMatrix);
	}
	if(pFrame->pFrameFirstChild!=NULL)
	{
		this->UpdateMatrices(pFrame->pFrameFirstChild,&(pFrame->Matrix));
	}
}
void stSANXFRAMEELEMENT::SetAnimation(unsigned int index,float Time,float Duration)
{
	if((index>=this->NumAnimations)||(index==this->CurrentAnimation))
	{
		return;
	}
	this->CurrentAnimation=index;
	LPD3DXANIMATIONSET AnimationSet;
	this->AnimationControler->GetAnimationSet(this->CurrentAnimation,&AnimationSet);
	unsigned long NextTrack=(this->CurrentAnimation==0?1:0);
	this->AnimationControler->SetTrackAnimationSet(NextTrack,AnimationSet);
	AnimationSet->Release();
	this->AnimationControler->UnkeyAllTrackEvents(this->CurrentTrack);
	this->AnimationControler->UnkeyAllTrackEvents(NextTrack);
	this->AnimationControler->KeyTrackEnable(this->CurrentTrack,FALSE,Time+Duration);
	this->AnimationControler->KeyTrackSpeed(this->CurrentTrack,0.0,Time,Duration,D3DXTRANSITION_LINEAR);
	this->AnimationControler->KeyTrackWeight(this->CurrentTrack,0.0,Time,Duration,D3DXTRANSITION_LINEAR);
	this->AnimationControler->SetTrackEnable(NextTrack,TRUE);
	this->AnimationControler->KeyTrackSpeed(NextTrack,0.0,Time,Duration,D3DXTRANSITION_LINEAR);
	this->AnimationControler->KeyTrackWeight(NextTrack,0.0,Time,Duration,D3DXTRANSITION_LINEAR);
	this->CurrentTrack=NextTrack;
}
void stSANXFRAMEELEMENT::SetNextAnimation(float Time,float Duration)
{
	unsigned int newAnimationSet=this->CurrentAnimation+1;
	if(newAnimationSet>=this->NumAnimations)
	{
		newAnimationSet=0;
	}
	this->SetAnimation(newAnimationSet,Time,Duration);
}
#endif