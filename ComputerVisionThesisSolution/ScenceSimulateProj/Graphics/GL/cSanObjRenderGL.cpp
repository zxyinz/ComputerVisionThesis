#include"cSanObjRenderGL.h"
using namespace std;
using namespace San;
using namespace San::Mathematics;
using namespace San::Graphics;
#ifdef _OPENGL
void cSanObjRenderGL::iDrawPix(const SPOINT3 &PixPos,SANCOLOR Color,const lpSANRENDERDESC pRenderDesc)
{
	sfloat PosX = PixPos.x;
	sfloat PosY = PixPos.y;
	if (pRenderDesc != nullptr)
	{
		if (pRenderDesc->window_width != 0)
		{
			PosX = (sfloat) (PixPos.x - pRenderDesc->window_width / 2) / (sfloat) (pRenderDesc->window_width / 2);
		}
		if (pRenderDesc->window_hight != 0)
		{
			PosY = (sfloat) ((pRenderDesc->window_hight / 2) - PixPos.y) / (sfloat) (pRenderDesc->window_hight / 2);
		}
	}
	::glRasterPos2f(PosX,PosY);
	SANCOLOR Col = Color;
	if(Color.a!=0xff)
	{
		sfloat rate=(sfloat)Color.a/0xff;
		SANCOLOR SrcColor;
		::glReadPixels(PixPos.x,PixPos.y,1,1,GL_RGBA,GL_UNSIGNED_BYTE,SrcColor.color_a);
		Color.r=Color.r*rate+SrcColor.r*(1-rate);
		Color.g=Color.g*rate+SrcColor.g*(1-rate);
		Color.b=Color.b*rate+SrcColor.b*(1-rate);
	}
	::glDrawPixels(1,1,GL_RGBA,GL_UNSIGNED_BYTE,Color.color_a);
}
void cSanObjRenderGL::iDrawPoint(const SPOINT3 Pos,const SANCOLORF Color,const lpSANRENDERDESC pRenderDesc)
{
	sfloat size[2]={1.0,1.0};
	sfloat PointSize=size[0];
	::glGetFloatv(GL_POINT_SIZE_RANGE,size);
	if(pRenderDesc!=nullptr)
	{
		if(pRenderDesc->point_size<size[0])
		{
			PointSize=size[0];
		}
		else
		{
			if(pRenderDesc->point_size>size[1])
			{
				PointSize=size[1];
			}
			else
			{
				PointSize=pRenderDesc->point_size;
			}
		}
	}
	::glPushMatrix();
	::glDisable(GL_TEXTURE_1D);
	::glDisable(GL_TEXTURE_2D);
	::glDisable(GL_TEXTURE_3D);
	::glEnable(GL_COLOR_MATERIAL);
	::glColor3f(Color.r,Color.g,Color.b);
	::glPointSize(PointSize);
	::glBegin(GL_POINTS);
	::glVertex3fv(Pos.p);
	::glEnd();
	::glDisable(GL_COLOR_MATERIAL);
	::glPopMatrix();
}
void cSanObjRenderGL::iDrawLine(const SPOINT3 SrcPos,const SPOINT3 DestPos,const SANCOLORF Color,const lpSANRENDERDESC pRenderDesc)
{
	sfloat size[2]={1.0,1.0};
	sfloat LineSize=size[0];
	::glGetFloatv(GL_LINE_WIDTH_RANGE,size);
	if(pRenderDesc!=nullptr)
	{
		if(pRenderDesc->line_size<size[0])
		{
			LineSize=size[0];
		}
		else
		{
			if(pRenderDesc->line_size>size[1])
			{
				LineSize=size[1];
			}
			else
			{
				LineSize=pRenderDesc->line_size;
			}
		}
	}
	::glPushMatrix();
	::glDisable(GL_TEXTURE_1D);
	::glDisable(GL_TEXTURE_2D);
	::glDisable(GL_TEXTURE_3D);
	::glEnable(GL_COLOR_MATERIAL);
	::glColor3f(Color.r,Color.g,Color.b);
	::glLineWidth(LineSize);
	::glBegin(GL_LINES);
	::glVertex3fv(SrcPos.p);
	::glVertex3fv(DestPos.p);
	::glEnd();
	::glDisable(GL_COLOR_MATERIAL);
	::glPopMatrix();
}
void cSanObjRenderGL::iDrawShape(const SANSHAPE Shape,const lpSANRENDERDESC pRenderDesc)
{
	//SANRENDERDESC Desc;
	//if(pRenderDesc!=nullptr)
	//{
	//	Desc = *pRenderDesc;
	//}
	//::glMatrixMode(GL_MODELVIEW);
	//::glPushMatrix();
	//::glDisable(GL_TEXTURE_1D);
	//::glDisable(GL_TEXTURE_2D);
	//::glDisable(GL_TEXTURE_3D);
	//::glEnable(GL_COLOR_MATERIAL);
	//::glTranslated(Shape.paramlist[0].x,Shape.paramlist[0].y,Shape.paramlist[0].z);
	//::glRotated(Desc.rotation.x, 1.0, 0.0, 0.0);
	//::glRotated(Desc.rotation.y, 0.0, 1.0, 0.0);
	//::glRotated(Desc.rotation.z, 0.0, 0.0, 1.0);
	//::glScaled(Desc.scale.x, Desc.scale.y, Desc.scale.z);
	////::glBegin();
	//::glColor3f(Desc.obj_color.r / 255, Desc.obj_color.g / 255, Desc.obj_color.b / 255);
	//switch(Shape.type)
	//{
	//case San::Mathematics::SST_BALL:
	//	if (Desc.ObjRenderStyle == San::Graphics::SORS_LINE)
	//	{
	//		::glutWireSphere(Shape.paramlist[1].x,32,32);
	//	}
	//	if (Desc.ObjRenderStyle == San::Graphics::SORS_TRANGLE)
	//	{
	//		::glutSolidSphere(Shape.paramlist[1].x,32,32);
	//	}
	//	break;
	//case San::Mathematics::SST_PLANE:
	//	break;
	//case San::Mathematics::SST_CUBOID:
	//	if (Desc.ObjRenderStyle == San::Graphics::SORS_LINE)
	//	{
	//		::glutWireCube((sfloat)Shape.paramlist[1].x*2);
	//	}
	//	if (Desc.ObjRenderStyle == San::Graphics::SORS_TRANGLE)
	//	{
	//		::glutSolidCube((sfloat)Shape.paramlist[1].x*2);
	//	}
	//	break;
	//case San::Mathematics::SST_CYLINDER:
	//	break;
	//default:
	//	break;
	//}
	////::glEnd();
	//::glDisable(GL_COLOR_MATERIAL);
	//::glPopMatrix();
}
void cSanObjRenderGL::iDrawObject(const SPOINT3 Pos,const lpSANPIXMAP2D pPixMap2D,const eSANPIXTYPE Format,const LPRECT pCutRect,const lpSANRENDERDESC pRenderDesc)
{
	if(pPixMap2D==nullptr)
	{
		return;
	}
}
void cSanObjRenderGL::iDrawObject(const lpSANMESH pMesh,const lpSPOINT3 pCoord,const lpSANRENDERDESC pRenderDesc)
{
	SHANDLE pDeviceResManager = nullptr;
	SPOINT3 Coord(0.0f, 0.0f, 0.0f);
	if(pMesh==nullptr)
	{
		return;
	}
	if(pMesh->value.empty())
	{
		return;
	}
	if (pCoord != nullptr)
	{
		Coord = *pCoord;
	}
	if(pRenderDesc!=nullptr)
	{
		pDeviceResManager=pRenderDesc->pDeviceResManager;
		if(pRenderDesc->pDeviceResManager==nullptr)
		{
			pRenderDesc->pDeviceResManager=::gloFoundVector(_SSTR("Sys_SanGLBufferManager"));
		}
		if(pRenderDesc->pDeviceResManager==nullptr)
		{
			pRenderDesc->pDeviceResManager=pDeviceResManager;
			return;
		}
	}
	SANOBJECTLIST::iterator pObj=pMesh->value.begin();
	while(pObj!=pMesh->value.end())
	{
		cSanObjRenderGL::iDrawObject(&(*pObj),&Coord,pRenderDesc);
		pObj++;
	}
	if(pRenderDesc!=nullptr)
	{
		pRenderDesc->pDeviceResManager=pDeviceResManager;
	}
}
void cSanObjRenderGL::iDrawObject(const lpSANOBJECT pObject, const lpSPOINT3 pCoord, const lpSANRENDERDESC pRenderDesc)
{
	if (pObject == nullptr)
	{
		return;
	}
	SPOINT3 Coord(0.0f, 0.0f, 0.0f);
	SANRENDERDESC RenderDesc;
	cSanGLBufferManager* pBufferManager = nullptr;
	cSanShaderManager* pShaderManager = nullptr;
	Coord = (pCoord == nullptr) ? Coord : (*pCoord);
	RenderDesc = (pRenderDesc == nullptr) ? pObject->Material.ObjectRenderDesc : *pRenderDesc;
	pBufferManager = (RenderDesc.pDeviceResManager == nullptr) ? (cSanGLBufferManager*)::gloFoundVector(_SSTR("Sys_SanGLBufferManager")) : (cSanGLBufferManager*) RenderDesc.pDeviceResManager;
	pShaderManager = (RenderDesc.pShaderManager == nullptr) ? (cSanShaderManager*)::gloFoundVector(_SSTR("Sys_SanShaderManager")) : (cSanShaderManager*) RenderDesc.pShaderManager;
	if ((pBufferManager == nullptr) || (pShaderManager == nullptr))
	{
		return;
	}
	if (RenderDesc.bUseShaderProgram)
	{
		pShaderManager->iEnableProgram(RenderDesc.strShaderProgramName);
	}
	else
	{
		for (uint32 seek = 0; seek < SST_SHADER_COUNT; seek = seek + 1)
		{
			pShaderManager->iEnableShaderObj(RenderDesc.ShaderIDArray[seek]);
		}
	}
	RenderDesc.bEnableDepthTest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	SANMATRIX4X4 ModelViewMatrix;
	ModelViewMatrix.iIdentity();
	if (RenderDesc.bUseMatrix)
	{
		ModelViewMatrix = RenderDesc.matrix;
	}
	else
	{
		::gloSetTranslateMatrix(ModelViewMatrix, Coord + RenderDesc.translation);
		::gloSetRotateMatrix(ModelViewMatrix, RenderDesc.rotation, SVECTOR3(1.0, 1.0, 1.0));
		::gloSetScaleMatrix(ModelViewMatrix, RenderDesc.scale);
	}
	GLenum GLFaceType = GL_FRONT;
	if (RenderDesc.bShowBackFaces)
	{
		::glEnable(GL_CULL_FACE);
		GLFaceType = GL_FRONT_AND_BACK;
	}
	else
	{
		::glDisable(GL_CULL_FACE);
		GLFaceType = GL_FRONT;
	}
	SANCOLORF DiffuseColor = pObject->Material.Diffuse;
	if (RenderDesc.bUseObjColor)
	{
		DiffuseColor = RenderDesc.obj_color;
	}
#ifdef _FLOAT_TYPE_DOUBLE
	sfloat MaterialColor[4];///////////////////////////////////////////////////////////gl material enable
	for(uint8 seek_mat=0;seek_mat<4;seek_mat=seek_mat+1)
	{
		MaterialColor[seek_mat]=(sfloat)DiffuseColor.color_a[seek_mat];
	}
	::glMaterialfv(GLFaceType,GL_DIFFUSE,MaterialColor);
	for(uint8 seek_mat=0;seek_mat<4;seek_mat=seek_mat+1)
	{
		MaterialColor[seek_mat]=(sfloat)pObject->Material.Ambient.color_a[seek_mat];
	}
	::glMaterialfv(GLFaceType,GL_AMBIENT,MaterialColor);
	for(uint8 seek_mat=0;seek_mat<4;seek_mat=seek_mat+1)
	{
		MaterialColor[seek_mat]=(sfloat)pObject->Material.Specular.color_a[seek_mat];
	}
	::glMaterialfv(GLFaceType,GL_SPECULAR,MaterialColor);
	for(uint8 seek_mat=0;seek_mat<4;seek_mat=seek_mat+1)
	{
		MaterialColor[seek_mat]=(sfloat)pObject->Material.Emissive.color_a[seek_mat];
	}
	::glMaterialfv(GLFaceType,GL_EMISSION,MaterialColor);
	::glMaterialf(GLFaceType,GL_SHININESS,(sfloat)(pObject->Material.Shininess));
#else
	::glMaterialfv(GLFaceType, GL_DIFFUSE, DiffuseColor.color_a);
	::glMaterialfv(GLFaceType, GL_AMBIENT, pObject->Material.Ambient.color_a);
	::glMaterialfv(GLFaceType, GL_SPECULAR, pObject->Material.Specular.color_a);
	::glMaterialfv(GLFaceType, GL_EMISSION, pObject->Material.Emissive.color_a);
	::glMaterialf(GLFaceType, GL_SHININESS, pObject->Material.Shininess);
#endif
	//::glDisable(GL_TEXTURE_2D);
	if (RenderDesc.bUseTexture)
	{
		//::glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		uint32 TextureListSize = pObject->Material.TexList.size();
		if (TextureListSize != 0)
		{
			lpSANTEXTURE pTexture = nullptr;
			SVALUE CurrentTexHandle = 0;
			SANMATERIALTEXTURELIST::iterator pTex = pObject->Material.TexList.begin();
			for (int seekt = 0; seekt < TextureListSize; seekt = seekt + 1)
			{
				pTexture = ((lpSANRESPTRC) (pTex->value))->iGetTexturePtr();
				//pTexture->iUpdate();
				//::glEnable(GL_TEXTURE_2D);
				if (pTexture->CurrentParam == 0)
				{
					CurrentTexHandle = pBufferManager->iCreateTBOObject(pTexture); // release
				}
				else
				{
					CurrentTexHandle = pTexture->CurrentParam;
				}
				if (pTex->description.RenderType == STRT_USERDEF)
				{
					if (CurrentTexHandle != 0)
					{
						pBufferManager->iEnableTBOObject(CurrentTexHandle, pTex->description.RenderType + pTex->description.UserDefOffsetID, *pRenderDesc);
					}
				}
				else
				{
					if (CurrentTexHandle != 0)
					{
						pBufferManager->iEnableTBOObject(CurrentTexHandle, pTex->description.RenderType, *pRenderDesc);
					}
				}
				pTex++;
			}
		}
	}
	sfloat ObjSize[2] = { 1.0, 1.0 };
	sfloat DrawSize = ObjSize[0];
	uint32 GLRenderMode = GL_TRIANGLES;
	switch (RenderDesc.ObjRenderStyle)
	{
	case SORS_POINT:
		::glGetFloatv(GL_POINT_SIZE_RANGE, ObjSize);
		if (RenderDesc.point_size < ObjSize[0])
		{
			DrawSize = ObjSize[0];
		}
		else
		{
			if (RenderDesc.point_size > ObjSize[1])
			{
				DrawSize = ObjSize[1];
			}
			else
			{
				DrawSize = RenderDesc.point_size;
			}
		}
		::glPointSize(DrawSize);
		GLRenderMode = GL_POINTS;
		break;
	case SORS_LINE:
		::glGetFloatv(GL_LINE_WIDTH_RANGE, ObjSize);
		if (RenderDesc.line_size < ObjSize[0])
		{
			DrawSize = ObjSize[0];
		}
		else
		{
			if (RenderDesc.line_size > ObjSize[1])
			{
				DrawSize = ObjSize[1];
			}
			else
			{
				DrawSize = RenderDesc.line_size;
			}
		}
		::glLineWidth(DrawSize);
		GLRenderMode = GL_LINE_STRIP;
		break;
	case SORS_TRANGLE:
	default:
		GLRenderMode = GL_TRIANGLES;
		break;
	}
	if (pObject->ObjParam != nullptr)
	{
		pBufferManager->iEnableVAOObject((uint32) (pObject->ObjParam), false);
		::glDrawArrays(GLRenderMode, 0, pObject->VertexListSize);
		pBufferManager->iEnableVAOObject(0, false);
	}
	else
	{
		::glBegin(GLRenderMode);
		uint8 FaceCount = 0;
		for (uint32 seek = 0; seek < pObject->VertexListSize; seek = seek + 1)
		{
			::glColor4ubv((pObject->pVertexList[seek].vc.color_a));
			if (FaceCount == 0)
			{
				::glNormal3fv(pObject->pVertexList[seek].vn.p);
			}
			else
			{
				if (FaceCount < 3)
				{
					FaceCount = FaceCount + 1;
				}
				else
				{
					FaceCount = 0;
				}
			}
			if (pObject->pVertexList[seek].vt_size != 0)
			{
				uint32 TextureID = GL_TEXTURE0;/////////////////////////////////////////////////////////////////////max hardware support texturenumber
				for (uint32 seektex = 0; seektex < pObject->pVertexList[seek].vt_size; seektex = seektex + 1)
				{
					::glMultiTexCoord2f(TextureID, pObject->pVertexList[seek].p_vt[0].x, pObject->pVertexList[seek].p_vt[0].y);////////////////////////////////////////////////
					TextureID = TextureID + 1;
				}
			}
			::glVertex3fv((pObject->pVertexList[seek].v.p));
		}
		::glEnd();
	}
	::glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SINGLE_COLOR);
	::glPopMatrix();
	//RenderDesc.pDeviceResManager = pDeviceResManager;
}
#endif