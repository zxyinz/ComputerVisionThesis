#include"cSanGLBufferManager.h"
using namespace std;
using namespace San;
using namespace San::Graphics;
#ifdef _OPENGL
bool cSanGLBufferManager::iCreateGLBufferManager()
{
	if (::gloFoundVector(this->m_strManagerName))
	{
		return false;
	}
	this->_Release();
	::gloRegisterVector(this->m_strManagerName, VT_SYS | VT_VAR, (SHANDLE)this);
	return true;
}
void cSanGLBufferManager::iReleaseGLBufferManager()
{
	this->_Release();
	if (::gloFoundVector(this->m_strManagerName))
	{
		::gloDestoryVector(this->m_strManagerName, VT_SYS | VT_VAR);
	}
}
uint32 cSanGLBufferManager::iCreateVBOObject(const sfloat* pBuffer, const uint32 BufferSize, const eSANVBOBUFFERSTATE State)
{
	uint32 VBOID = this->_CreateVBO(pBuffer, BufferSize, State);
	::glBindBuffer(GL_ARRAY_BUFFER, 0);
	::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return VBOID;
}
uint32 cSanGLBufferManager::iUpdateVBOObject(const uint32 VBOID, const sfloat* pBuffer, const uint32 BufferSize, const eSANVBOBUFFERSTATE State)
{
	if (::gloGetResourceType(VBOID) != ::RT_BUFFER_VBO)
	{
		return 0;
	}
	return this->_UpdateVBO(VBOID, pBuffer, BufferSize, State);
}
void cSanGLBufferManager::iReleaseVBOObject(const uint32 VBOID)
{
	if (::gloGetResourceType(VBOID) != ::RT_BUFFER_VBO)
	{
		return;
	}
	list<SANVBO>::iterator pItem = this->m_VBOList.begin();
	while (pItem != this->m_VBOList.end())
	{
		if (pItem->ID == VBOID)
		{
			::glDeleteBuffers(1, (GLuint*) (pItem->Handle));
			::gloReleaseResource(pItem->ID, (SVALUE)this);
			this->m_VBOList.erase(pItem);
			return;
		}
		pItem++;
	}
	return;
}
uint32 cSanGLBufferManager::iCreateVAOObject(const lpSANOBJECT pObject, const eSANVBOBUFFERSTATE State)
{
	if (pObject->VertexListSize == 0)
	{
		return 0;
	}
	SANVAO VAO;
	::gloMemSet(&VAO, 0, sizeof(VAO));
	VAO.ID = ::gloRegisteResource(nullptr, sizeof(SANVAO), RT_BUFFER_VAO, (SVALUE)this);
	if (this->_CreateVAO(VAO, pObject, State))
	{
		this->m_VAOList.insert(this->m_VAOList.end(), VAO);
		return VAO.ID;
	}
	::gloReleaseResource(VAO.ID, (SVALUE)this);
	return 0;
}
uint32 cSanGLBufferManager::iUpdateVAOObject(const uint32 VAOID, const lpSANOBJECT pObject, const eSANVBOBUFFERSTATE State)
{
	if (::gloGetResourceType(VAOID) != ::RT_BUFFER_VAO)
	{
		return 0;
	}
	//SHANDLE VBOHandleArray[eSANVBOTYPE::SVBOT_COUNT];
	list<SANVAO>::iterator pItem = this->m_VAOList.begin();
	while (pItem != this->m_VAOList.end())
	{
		if (pItem->ID == VAOID)
		{
			if (pItem->VAOHandle == nullptr)
			{
				if (this->_CreateVAO(*pItem, pObject, State))
				{
					return pItem->ID;
				}
				this->_DeleteVAO(*pItem);
				return 0;
			}
			if (pObject->VertexListSize != 0)
			{
				uint32 VertexBufferSize = pObject->VertexListSize * 3;
				uint32 TexCoordListSize = pObject->pVertexList[0].vt_size;
				sfloat* pVertexBuffer = new sfloat[VertexBufferSize];
				::gloMemSet(pVertexBuffer, 0, sizeof(sfloat) *VertexBufferSize);
				sfloat* pNormalBuffer = new sfloat[VertexBufferSize];
				::gloMemSet(pNormalBuffer, 0, sizeof(sfloat) *VertexBufferSize);
				sfloat* pColorBuffer = new sfloat[pObject->VertexListSize * 4];//////////////////////////color buffer 4 byte
				::gloMemSet(pColorBuffer, 0, sizeof(sfloat) *pObject->VertexListSize * 4);
				sfloat** pTextureBufferList = nullptr;
				SVALUE* pTextureBufferIDList = nullptr;
				if (TexCoordListSize != 0)
				{
					pTextureBufferList = new sfloat*[TexCoordListSize];
					pTextureBufferIDList = new SVALUE[TexCoordListSize];
					for (uint32 seek = 0; seek<TexCoordListSize; seek = seek + 1)
					{
						pTextureBufferList[seek] = new sfloat[pObject->VertexListSize * 2];///////////////////////texture2d coord buffer 2 byte
						::gloMemSet(pTextureBufferList[seek], 0, sizeof(sfloat) *pObject->VertexListSize * 2);
					}
				}
				uint32 seekbuf = 0;
				uint32 seekcolorbuf = 0;
				uint32 seektexbuf = 0;
				for (uint32 seekv = 0; seekv<pObject->VertexListSize; seekv = seekv + 1)
				{
					pVertexBuffer[seekbuf] = pObject->pVertexList[seekv].v.x;
					pVertexBuffer[seekbuf + 1] = pObject->pVertexList[seekv].v.y;
					pVertexBuffer[seekbuf + 2] = pObject->pVertexList[seekv].v.z;
					pNormalBuffer[seekbuf] = pObject->pVertexList[seekv].vn.x;
					pNormalBuffer[seekbuf + 1] = pObject->pVertexList[seekv].vn.y;
					pNormalBuffer[seekbuf + 2] = pObject->pVertexList[seekv].vn.z;
					pColorBuffer[seekcolorbuf] = pObject->pVertexList[seekv].vc.r;
					pColorBuffer[seekcolorbuf + 1] = pObject->pVertexList[seekv].vc.g;
					pColorBuffer[seekcolorbuf + 2] = pObject->pVertexList[seekv].vc.b;
					pColorBuffer[seekcolorbuf + 3] = pObject->pVertexList[seekv].vc.a;
					if (pTextureBufferList != nullptr)
					{
						for (uint32 seekt = 0; seekt<TexCoordListSize; seekt = seekt + 1)
						{
							pTextureBufferList[seekt][seektexbuf] = pObject->pVertexList[seekv].p_vt[seekt].x;
							pTextureBufferList[seekt][seektexbuf + 1] = pObject->pVertexList[seekv].p_vt[seekt].y;
						}
					}
					seekbuf = seekbuf + 3;
					seekcolorbuf = seekcolorbuf + 4;
					seektexbuf = seektexbuf + 2;
				}
				pItem->TexListSize = TexCoordListSize;
				uint32 TexIndex = 0;
				for (uint32 seek = 0; seek<eSANVBOTYPE::SVBOT_COUNT; seek = seek + 1)
				{
					switch (seek)
					{
					case SVBOT_VERTEX:
						this->_UpdateVBO(pItem->VBOIDArray[seek], pVertexBuffer, VertexBufferSize*sizeof(sfloat), State, &(pItem->VBOHandleArray[seek]));
						delete[] pVertexBuffer;
						pVertexBuffer = nullptr;
						break;
					case SVBOT_NORMAL:
						this->_UpdateVBO(pItem->VBOIDArray[seek], pNormalBuffer, VertexBufferSize*sizeof(sfloat), State, &(pItem->VBOHandleArray[seek]));
						delete[] pVertexBuffer;
						pNormalBuffer = nullptr;
						break;
					case SVBOT_COLOR:
						this->_UpdateVBO(pItem->VBOIDArray[seek], pColorBuffer, pObject->VertexListSize * 4 * sizeof(sfloat), State, &(pItem->VBOHandleArray[seek]));
						delete[] pVertexBuffer;
						pColorBuffer = nullptr;
						break;
					default:
						if ((seek >= SVBOT_TEXTURE_BEGIN) && (seek <= SVBOT_TEXTURE_END))
						{
							TexIndex = seek - SVBOT_TEXTURE_BEGIN;
							if (TexIndex<TexCoordListSize)
							{
								if (pItem->VBOIDArray[seek] == 0)
								{
									pItem->VBOIDArray[seek] = this->_CreateVBO(pTextureBufferList[TexIndex], pObject->VertexListSize * 2 * sizeof(sfloat), State, &(pItem->VBOHandleArray[seek]));
								}
								else
								{
									this->_UpdateVBO(pItem->VBOIDArray[seek], pTextureBufferList[TexIndex], pObject->VertexListSize * 2 * sizeof(sfloat), State, &(pItem->VBOHandleArray[seek]));
								}
								delete[] pTextureBufferList[TexIndex];
								pTextureBufferList[TexIndex] = nullptr;
							}
							else
							{
								if (pItem->VBOIDArray[seek] != 0)
								{
									this->iReleaseVBOObject(pItem->VBOIDArray[seek]);
								}
								pItem->VBOIDArray[seek] = 0;
								pItem->VBOHandleArray[seek] = nullptr;
							}
						}
						break;
					}
				}
				if (TexCoordListSize != 0)
				{
					delete[] pTextureBufferList;
					pTextureBufferList = nullptr;
				}
			}
			else
			{
				for (uint32 seek = 0; seek<eSANVBOTYPE::SVBOT_COUNT; seek = seek + 1)
				{
					if (pItem->VBOIDArray[seek] != 0)
					{
						this->iReleaseVBOObject(pItem->VBOIDArray[seek]);
						pItem->VBOIDArray[seek] = 0;
					}
					pItem->VBOHandleArray[seek] = nullptr;
				}
			}
			::glBindVertexArray((GLuint) pItem->VAOHandle);
			for (uint32 seek_vao = 0; seek_vao<eSANVBOTYPE::SVBOT_COUNT; seek_vao = seek_vao + 1)
			{
				if (pItem->VBOHandleArray[seek_vao] == 0)
				{
					continue;
				}
				::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) (pItem->VBOHandleArray[seek_vao]));
				::glEnableVertexAttribArray(seek_vao);
				switch (seek_vao)
				{
				case SVBOT_VERTEX:
					::glVertexAttribPointer(seek_vao, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
					break;
				case SVBOT_NORMAL:
					::glVertexAttribPointer(seek_vao, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
					break;
				case SVBOT_COLOR:
					::glVertexAttribPointer(seek_vao, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
					break;
				default:
					::glVertexAttribPointer(seek_vao, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
					break;
				}
			}
			::glBindVertexArray(0);
			::glBindBuffer(GL_ARRAY_BUFFER, 0);
			::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			return pItem->ID;
		}
		pItem++;
	}
	return 0;
}
void cSanGLBufferManager::iReleaseVAOObject(const uint32 VAOID)
{
	if (::gloGetResourceType(VAOID) != ::RT_BUFFER_VAO)
	{
		return;
	}
	list<SANVAO>::iterator pItem = this->m_VAOList.begin();
	while (pItem != this->m_VAOList.end())
	{
		if (pItem->ID == VAOID)
		{
			this->_DeleteVAO(*pItem);
			::gloReleaseResource(pItem->ID, (SVALUE)this);
			this->m_VAOList.erase(pItem);
			return;
		}
		pItem++;
	}
}
uint32 cSanGLBufferManager::_CreateVBO(const sfloat* pBuffer, const uint32 BufferSize, const eSANVBOBUFFERSTATE State, SHANDLE* pVBOHandle)
{
	if (pVBOHandle != nullptr)
	{
		*pVBOHandle = nullptr;
	}
	if ((pBuffer == nullptr) || (BufferSize == 0))
	{
		return 0;
	}
	SANVBO VBO;
	VBO.ID = ::gloRegisteResource(nullptr, sizeof(SANVBO), RT_BUFFER_VBO, (SVALUE) this);
	::glGenBuffers(1, (GLuint*) &(VBO.Handle));
	if (VBO.Handle == nullptr)
	{
		::gloReleaseResource(VBO.ID, (SVALUE)this);
		return 0;
	}
	::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) (VBO.Handle));
	switch (State)
	{
	case SVBS_STATIC:
		::glBufferData(GL_ARRAY_BUFFER, BufferSize, pBuffer, GL_STATIC_DRAW);
		break;
	case SVBS_STREAM:
		::glBufferData(GL_ARRAY_BUFFER, BufferSize, pBuffer, GL_STREAM_DRAW);
		break;
	case SVBS_DYNAMIC:
		::glBufferData(GL_ARRAY_BUFFER, BufferSize, pBuffer, GL_DYNAMIC_DRAW);
		break;
	default:
		::glBufferData(GL_ARRAY_BUFFER, BufferSize, pBuffer, GL_STATIC_DRAW);
		break;
	}
	VBO.DataSize = BufferSize;
	VBO.State = State;
	this->m_VBOList.insert(this->m_VBOList.end(), VBO);
	if (pVBOHandle != nullptr)
	{
		*pVBOHandle = VBO.Handle;
	}
	return VBO.ID;
}
uint32 cSanGLBufferManager::_UpdateVBO(const uint32 VBOID, const sfloat* pBuffer, uint32 BufferSize, const eSANVBOBUFFERSTATE State, SHANDLE* pVBOHandle)
{
	if (pVBOHandle != nullptr)
	{
		*pVBOHandle = 0;
	}
	if ((pBuffer == nullptr) || (BufferSize == 0))
	{
		return 0;
	}
	list<SANVBO>::iterator pItem = this->m_VBOList.begin();
	while (pItem != this->m_VBOList.end())
	{
		if (pItem->ID == VBOID)
		{
			if ((pItem->DataSize != BufferSize) || (pItem->State != State))
			{
				::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) (pItem->Handle));
				switch (State)
				{
				case SVBS_STATIC:
					::glBufferData(GL_ARRAY_BUFFER, BufferSize, pBuffer, GL_STATIC_DRAW);
					break;
				case SVBS_STREAM:
					::glBufferData(GL_ARRAY_BUFFER, BufferSize, pBuffer, GL_STREAM_DRAW);
					break;
				case SVBS_DYNAMIC:
					::glBufferData(GL_ARRAY_BUFFER, BufferSize, pBuffer, GL_DYNAMIC_DRAW);
					break;
				default:
					::glBufferData(GL_ARRAY_BUFFER, BufferSize, pBuffer, GL_STATIC_DRAW);
					break;
				}
				::glBindBuffer(GL_ARRAY_BUFFER, 0);
				pItem->DataSize = BufferSize;
				pItem->State = State;
				if (pVBOHandle != nullptr)
				{
					*pVBOHandle = pItem->Handle;
				}
				return pItem->ID;
			}
			else
			{
				::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) (pItem->Handle));
				SHANDLE pBufferPtr = ::glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
				if (pBufferPtr == nullptr)
				{
					return 0;
				}
				::gloMemCopy(pBufferPtr, pBuffer, BufferSize*sizeof(sfloat));
				::glUnmapBuffer(GL_ARRAY_BUFFER);
				::glBindBuffer(GL_ARRAY_BUFFER, 0);
				return pItem->ID;
			}
		}
		pItem++;
	}
	return 0;
}
lpSANVBO cSanGLBufferManager::_FindVBO(uint32 VBOID)
{
	list<SANVBO>::iterator pItem = this->m_VBOList.begin();
	while (pItem != this->m_VBOList.end())
	{
		if (pItem->ID == VBOID)
		{
			return &(*pItem);
		}
		pItem++;
	}
	return nullptr;
}
bool cSanGLBufferManager::_CreateVAO(SANVAO &VAO, const lpSANOBJECT pObject, const eSANVBOBUFFERSTATE State)
{
	if (VAO.VAOHandle == nullptr)
	{
		::glGenVertexArrays(1, (GLuint*) &(VAO.VAOHandle));
		if (VAO.VAOHandle == nullptr)
		{
			//this->m_CurrentMaxVAONum=this->m_CurrentMaxVAONum-1;
			return false;
		}
	}
	::glBindVertexArray((GLuint) VAO.VAOHandle);
	if (pObject->VertexListSize != 0)
	{
		uint32 VertexBufferSize = pObject->VertexListSize * 3;
		uint32 TexCoordListSize = pObject->pVertexList[0].vt_size;
		sfloat* pVertexBuffer = new sfloat[VertexBufferSize];
		::gloMemSet(pVertexBuffer, 0, sizeof(sfloat) *VertexBufferSize);
		sfloat* pNormalBuffer = new sfloat[VertexBufferSize];
		::gloMemSet(pNormalBuffer, 0, sizeof(sfloat) *VertexBufferSize);
		sfloat* pColorBuffer = new sfloat[pObject->VertexListSize * 4];//////////////////////////color buffer 4 byte
		::gloMemSet(pColorBuffer, 0, sizeof(sfloat) *pObject->VertexListSize * 4);
		sfloat** pTextureBufferList = nullptr;
		SVALUE* pTextureBufferIDList = nullptr;
		if (TexCoordListSize != 0)
		{
			pTextureBufferList = new sfloat*[TexCoordListSize];
			pTextureBufferIDList = new SVALUE[TexCoordListSize];
			for (uint32 seek = 0; seek < TexCoordListSize; seek = seek + 1)
			{
				pTextureBufferList[seek] = new sfloat[pObject->VertexListSize * 2];///////////////////////texture2d coord buffer 2 byte
				::gloMemSet(pTextureBufferList[seek], 0, sizeof(sfloat) *pObject->VertexListSize * 2);
			}
		}
		uint32 seekbuf = 0;
		uint32 seekcolorbuf = 0;
		uint32 seektexbuf = 0;
		for (uint32 seekv = 0; seekv < pObject->VertexListSize; seekv = seekv + 1)
		{
			pVertexBuffer[seekbuf] = pObject->pVertexList[seekv].v.x;
			pVertexBuffer[seekbuf + 1] = pObject->pVertexList[seekv].v.y;
			pVertexBuffer[seekbuf + 2] = pObject->pVertexList[seekv].v.z;
			pNormalBuffer[seekbuf] = pObject->pVertexList[seekv].vn.x;
			pNormalBuffer[seekbuf + 1] = pObject->pVertexList[seekv].vn.y;
			pNormalBuffer[seekbuf + 2] = pObject->pVertexList[seekv].vn.z;
			pColorBuffer[seekcolorbuf] = pObject->pVertexList[seekv].vc.r;
			pColorBuffer[seekcolorbuf + 1] = pObject->pVertexList[seekv].vc.g;
			pColorBuffer[seekcolorbuf + 2] = pObject->pVertexList[seekv].vc.b;
			pColorBuffer[seekcolorbuf + 3] = pObject->pVertexList[seekv].vc.a;
			if (pTextureBufferList != nullptr)
			{
				for (uint32 seekt = 0; seekt < TexCoordListSize; seekt = seekt + 1)
				{
					pTextureBufferList[seekt][seektexbuf] = pObject->pVertexList[seekv].p_vt[seekt].x;
					pTextureBufferList[seekt][seektexbuf + 1] = pObject->pVertexList[seekv].p_vt[seekt].y;
				}
			}
			seekbuf = seekbuf + 3;
			seekcolorbuf = seekcolorbuf + 4;
			seektexbuf = seektexbuf + 2;
		}
		VAO.TexListSize = TexCoordListSize;
		//SHANDLE VBOHandle;
		for (uint32 seek_vao = 0; seek_vao < eSANVBOTYPE::SVBOT_COUNT; seek_vao = seek_vao + 1)
		{
			switch (seek_vao)
			{
			case SVBOT_VERTEX:
				VAO.VBOIDArray[seek_vao] = this->_CreateVBO(pVertexBuffer, VertexBufferSize*sizeof(sfloat), State, &(VAO.VBOHandleArray[seek_vao]));
				//::glBindBuffer(GL_ARRAY_BUFFER,(GLuint)(VBOHandle));
				::glVertexAttribPointer(seek_vao, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
				::glEnableVertexAttribArray(SVAOL_POSITION);
				delete[] pVertexBuffer;
				pVertexBuffer = nullptr;
				break;
			case SVBOT_NORMAL:
				VAO.VBOIDArray[seek_vao] = this->_CreateVBO(pNormalBuffer, VertexBufferSize*sizeof(sfloat), State, &(VAO.VBOHandleArray[seek_vao]));
				//::glBindBuffer(GL_ARRAY_BUFFER,(GLuint)(VBOHandle));
				::glVertexAttribPointer(seek_vao, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
				::glEnableVertexAttribArray(SVAOL_NORMAL);
				delete[] pNormalBuffer;
				pNormalBuffer = nullptr;
				break;
			case SVBOT_COLOR:
				VAO.VBOIDArray[seek_vao] = this->_CreateVBO(pColorBuffer, pObject->VertexListSize * 4 * sizeof(sfloat), State, &(VAO.VBOHandleArray[seek_vao]));
				//::glBindBuffer(GL_ARRAY_BUFFER,(GLuint)(VBOHandle));
				::glVertexAttribPointer(seek_vao, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
				::glEnableVertexAttribArray(SVAOL_COLOR);
				delete[] pColorBuffer;
				pNormalBuffer = nullptr;
				break;
			default:
				if ((seek_vao >= SVBOT_TEXTURE_BEGIN) && (seek_vao <= SVBOT_TEXTURE_END))
				{
					uint32 TexID = seek_vao - SVBOT_TEXTURE_BEGIN;
					if (TexID < TexCoordListSize)
					{
						VAO.VBOIDArray[seek_vao] = this->_CreateVBO(pTextureBufferList[TexID], pObject->VertexListSize * 2 * sizeof(sfloat), State, &(VAO.VBOHandleArray[seek_vao]));
						//::glBindBuffer(GL_ARRAY_BUFFER,(GLuint)(VBOHandle));
						::glVertexAttribPointer(seek_vao, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
						::glEnableVertexAttribArray(seek_vao);
						delete[] pTextureBufferList[TexID];
						pTextureBufferList[TexID] = nullptr;
					}
					else
					{
						VAO.VBOIDArray[seek_vao] = 0;
						VAO.VBOHandleArray[seek_vao] = nullptr;
					}
				}
				break;
			}
		}
		/*delete[] pVertexBuffer;
		pVertexBuffer=nullptr;
		delete[] pNormalBuffer;
		pNormalBuffer=nullptr;
		delete[] pColorBuffer;
		pColorBuffer=nullptr;
		for(uint32 seek_tex=0;seek_tex<TexCoordListSize;seek_tex=seek_tex+1)
		{
		delete[] pTextureBufferList[seek_tex];
		pTextureBufferList[seek_tex]=nullptr;
		}*/
		if (TexCoordListSize != 0)
		{
			delete[] pTextureBufferList;
			pTextureBufferList = nullptr;
		}
	}
	::glBindVertexArray(0);
	::glBindBuffer(GL_ARRAY_BUFFER, 0);
	::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return true;
}
void cSanGLBufferManager::_DeleteVAO(SANVAO &VAO)
{
	for (uint32 seek = 0; seek<eSANVBOTYPE::SVBOT_COUNT; seek = seek + 1)
	{
		if (VAO.VBOIDArray[seek] != 0)
		{
			this->iReleaseVBOObject(VAO.VBOIDArray[seek]);
			VAO.VBOIDArray[seek] = 0;
			VAO.VBOHandleArray[seek] = 0;
		}
	}
	VAO.TexListSize = 0;
	::glDeleteVertexArrays(1, (GLuint*) (VAO.VAOHandle));
	::glBindVertexArray(0);
	::glBindBuffer(GL_ARRAY_BUFFER, 0);
	::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
bool cSanGLBufferManager::_CreateFBO(SANFBO &FBO)
{
	if (!this->_CheckFBODesc(FBO.desc))
	{
		return false;
	}
	if (FBO.obj_array[::eSANFBOBUFFERTYPE::SFBT_FBO] == nullptr)
	{
		::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		::glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
		::glGenFramebuffersEXT(1, (GLuint*) &(FBO.obj_array[::eSANFBOBUFFERTYPE::SFBT_FBO]));
		::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, (GLuint) (FBO.obj_array[::eSANFBOBUFFERTYPE::SFBT_FBO]));
		if (FBO.obj_array[::eSANFBOBUFFERTYPE::SFBT_FBO] == nullptr)
		{
			return false;
		}
	}
	uint32 GLBufferFormat = 0;
	uint32 GLPixelFormat = 0;
	for (uint32 seek = 1; seek<::eSANFBOBUFFERTYPE::SFBT_BUFFER_COUNT; seek = seek + 1)
	{
		GLBufferFormat = 0;
		GLPixelFormat = 0;
		switch (seek)
		{
		case ::eSANFBOBUFFERTYPE::SFBT_DEPTH:
			GLBufferFormat = GL_DEPTH_ATTACHMENT_EXT;
			if (FBO.desc.pix_type == eSANGLTEXTURETYPE::SGTT_DEPTH_24)
			{
				GLPixelFormat = GL_DEPTH_COMPONENT24;
			}
			if (FBO.desc.pix_type == eSANGLTEXTURETYPE::SGTT_DEPTH_32)
			{
				GLPixelFormat = GL_DEPTH_COMPONENT32;
			}
			break;
		case ::eSANFBOBUFFERTYPE::SFBT_STENCIL:
			if (FBO.desc.pix_type == eSANGLTEXTURETYPE::SGTT_STENCIL)
			{
				GLBufferFormat = GL_STENCIL_ATTACHMENT_EXT;
				GLPixelFormat = GL_STENCIL_INDEX;
			}
			break;
		default:
			if ((seek >= ::eSANFBOBUFFERTYPE::SFBT_COLOR_BEGIN) && (seek <= ::eSANFBOBUFFERTYPE::SFBT_COLOR_END))
			{
				GLBufferFormat = GL_COLOR_ATTACHMENT0_EXT + seek - ::eSANFBOBUFFERTYPE::SFBT_COLOR_BEGIN;
				if (FBO.desc.attach_flag_array[seek] == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_RBO)
				{
					if (FBO.desc.pix_type == eSANGLTEXTURETYPE::SGTT_RGB_8)
					{
						GLPixelFormat = GL_RGB;
					}
					if (FBO.desc.pix_type == eSANGLTEXTURETYPE::SGTT_RGBA_8)
					{
						GLPixelFormat = GL_RGBA;
					}
					if (FBO.desc.pix_type == eSANGLTEXTURETYPE::SGTT_RGB_F)
					{
						GLPixelFormat = GL_RGB;
					}
					if (FBO.desc.pix_type == eSANGLTEXTURETYPE::SGTT_RGBA_F)
					{
						GLPixelFormat = GL_RGBA;
					}
				}
				/*if(FBO.desc.attach_flag_array[seek]==::eSANFBOBUFFERTYPE::SFBT_ATTACH_TEXTURE)
				{
				if(FBO.desc.pix_size==32)
				{
				GLPixelFormat=GL_RGB8;
				}
				else
				{
				GLPixelFormat=GL_RGBA8;
				}
				}*/
			}
			break;
		}
		if ((GLBufferFormat != 0) && (GLPixelFormat != 0))
		{
			if (FBO.desc.attach_flag_array[seek] == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_RBO)
			{
				::glGenRenderbuffersEXT(1, (GLuint*) &(FBO.obj_array[seek]));
				if (FBO.obj_array[seek] == nullptr)
				{
					::cout << "Can not create runder buffer object\n";
					this->_DeleteFBO(FBO);
					return false;
				}
				::glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, (GLuint) (FBO.obj_array[seek]));
				::glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GLPixelFormat, FBO.desc.width, FBO.desc.height);
				::glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GLBufferFormat, GL_RENDERBUFFER_EXT, (GLuint) (FBO.obj_array[seek]));
				::glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
			}
			if (FBO.desc.attach_flag_array[seek] == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_TEXTURE)
			{
				if (FBO.desc.attach_handle_array[seek] == nullptr)
				{
					SANTBODESC Desc;
					Desc.Type = (eSANGLTEXTURETYPE) GLPixelFormat;
					Desc.Width = FBO.desc.width;
					Desc.Height = FBO.desc.height;
					FBO.desc.attach_handle_array[seek] = (SHANDLE)this->iCreateTBOObject(Desc);
					if (FBO.desc.attach_handle_array[seek] == nullptr)
					{
						::cout << "Can not create texture, format id: " << seek << "\n";
						this->_DeleteFBO(FBO);
						return false;
					}
					FBO.obj_array[seek] = FBO.desc.attach_handle_array[seek];
				}
				GLuint TexHandle = (GLuint) (this->_FindTBOHandle((uint32) FBO.desc.attach_handle_array[seek]));
				::glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GLBufferFormat, GL_TEXTURE_2D, TexHandle, 0);
				::glBindTexture(GL_TEXTURE_2D, 0);///////////////////////////////////////////////////////////
			}
		}
	}
	GLenum result = ::glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result != GL_FRAMEBUFFER_COMPLETE)
	{
		::cout << "Can not create frame buffer object\n";
		this->_DeleteFBO(FBO);
		return false;
	}
	::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	return true;
}
void cSanGLBufferManager::_DeleteFBO(SANFBO &FBO)
{
	::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	::glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	for (uint32 seek = 0; seek<::eSANFBOBUFFERTYPE::SFBT_BUFFER_COUNT; seek = seek + 1)
	{
		if ((seek != ::eSANFBOBUFFERTYPE::SFBT_FBO) && (FBO.obj_array[seek] != nullptr))
		{
			if (FBO.desc.attach_flag_array[seek] == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_RBO)
			{
				::glDeleteRenderbuffersEXT(1, (const GLuint*) (&(FBO.obj_array[seek])));
			}
			if (FBO.desc.attach_flag_array[seek] == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_TEXTURE)
			{
				if (FBO.obj_array[seek] == nullptr)
				{
					this->iReleaseTBOObject((uint32) FBO.obj_array[seek]);
					FBO.obj_array[seek] = nullptr;
					FBO.desc.attach_handle_array[seek] = nullptr;
				}
			}
		}
	}
	if (FBO.obj_array[::eSANFBOBUFFERTYPE::SFBT_FBO] != nullptr)
	{
		::glDeleteFramebuffersEXT(1, (const GLuint*) (&(FBO.obj_array[::eSANFBOBUFFERTYPE::SFBT_FBO])));
	}
	FBO.desc.width = 0;
	FBO.desc.height = 0;
	FBO.desc.pix_type = eSANGLTEXTURETYPE::SGTT_RGB_8;
}
void cSanGLBufferManager::_UpdateTBO(const SHANDLE TBOHandle, const SANTBODESC &Desc)
{
	if (TBOHandle == nullptr)
	{
		return;
	}
	GLint TexComponent = GL_RGB8;
	GLenum TexFormat = GL_RGB;
	GLenum TexDataType = GL_UNSIGNED_BYTE;
	switch (Desc.Type)
	{
	case SGTT_RGB_8:
		TexFormat = GL_RGB;
		TexDataType = GL_UNSIGNED_BYTE;
		break;
	case SGTT_RGBA_8:
		TexComponent = GL_RGBA8;
		TexFormat = GL_RGBA;
		TexDataType = GL_UNSIGNED_BYTE;
		break;
	case SGTT_RGB_F:
		TexComponent = GL_RGB32F;
		TexFormat = GL_RGB;
		TexDataType = GL_FLOAT;
		break;
	case SGTT_RGBA_F:
		TexComponent = GL_RGBA8;
		TexFormat = GL_RGBA;
		TexDataType = GL_FLOAT;
		break;
	case SGTT_DEPTH_24:
	case SGTT_DEPTH_32:
		TexComponent = GL_DEPTH_COMPONENT;
		TexFormat = GL_DEPTH_COMPONENT;
		TexDataType = GL_FLOAT;
		break;
	case SGTT_STENCIL:
		TexComponent = GL_DEPTH_STENCIL;
		TexFormat = GL_STENCIL;
		TexDataType = GL_STENCIL_INDEX;
	default:
		break;
	}
#ifdef _FLOAT_TYPE_DOUBLE
	if (TexDataType == GL_FLOAT)
	{
		TexDataType = GL_DOUBLE;
	}
#endif
	//::glEnable(GL_TEXTURE_2D);
	::glBindTexture(GL_TEXTURE_2D, (GLuint) TBOHandle);
	if ((Desc.pBuffer != nullptr) && (Desc.BufferSize != 0))
	{
		//::glTexImage2D(GL_TEXTURE_2D,0,TexComponent,Desc.Width,Desc.Height,0,TexFormat,TexDataType,Desc.pBuffer);
		//::gluBuild2DMipmaps(GL_TEXTURE_2D,TexComponent,Desc.Width,Desc.Height,TexFormat,TexDataType,Desc.pBuffer);
		::glTexStorage2D(GL_TEXTURE_2D, Desc.MipmapLevel, TexComponent, Desc.Width, Desc.Height);
		::glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Desc.Width, Desc.Height, TexFormat, TexDataType, Desc.pBuffer);
		::glGenerateMipmap(GL_TEXTURE_2D);
	}
	::glBindTexture(GL_TEXTURE_2D, 0);
	//::glDisable(GL_TEXTURE_2D);
}
SHANDLE cSanGLBufferManager::_FindTBOHandle(uint32 TBOID)
{
	if (::gloGetResourceType(TBOID) != ::RT_BUFFER_TBO)
	{
		return 0;
	}
	list<SANTBO>::iterator pItem = this->m_TBOList.begin();
	while (pItem != this->m_TBOList.end())
	{
		if (pItem->ID == TBOID)
		{
			return pItem->Handle;
		}
		pItem++;
	}
	return nullptr;
}
bool cSanGLBufferManager::_CheckFBODesc(const SANFBODESC &Desc)
{
	//uint32 max_size=0;
	uint32 max_width = 0;
	//::glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS_EXT,(GLint*)(&max_size));
	::glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE_EXT, (GLint*) (&max_width));
	if ((Desc.width<1) || (Desc.height<1))
	{
		return false;
	}
	if ((Desc.width>max_width) || (Desc.height>max_width))
	{
		::cout << "FBO buffer size overflew, width: " << Desc.width << "\theight: " << Desc.height << "\tmax: " << max_width << "\n";
		;		return false;
	}
	/*if((Desc.pix_size!=24)&&(Desc.pix_size!=32))
	{
	::cout<<"FBO pixel format mismatch\n";
	return false;
	}*/
	::eSANFBOBUFFERTYPE AttachType;
	for (uint32 seek = 0; seek<::eSANFBOBUFFERTYPE::SFBT_BUFFER_COUNT; seek = seek + 1)
	{
		AttachType = Desc.attach_flag_array[seek];
		switch (seek)
		{
		case ::eSANFBOBUFFERTYPE::SFBT_FBO:
			if (AttachType != ::eSANFBOBUFFERTYPE::SFBT_ATTACH_NULL)
			{
				::cout << "FBO attach flag must be null\n";
				return false;
			}
			break;
		case ::eSANFBOBUFFERTYPE::SFBT_DEPTH:
			//break;
		case ::eSANFBOBUFFERTYPE::SFBT_STENCIL:
			if ((AttachType != ::eSANFBOBUFFERTYPE::SFBT_ATTACH_NULL) && (AttachType != ::eSANFBOBUFFERTYPE::SFBT_ATTACH_RBO))
			{
				::cout << "Depth or Stencil flag only can be null or RBO\n";
				return false;
			}
			break;
		default:
			if ((seek >= ::eSANFBOBUFFERTYPE::SFBT_COLOR_BEGIN) && (seek <= ::eSANFBOBUFFERTYPE::SFBT_COLOR_END))
			{
				if (AttachType == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_NULL)
				{
					break;
				}
				if (AttachType == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_RBO)
				{
					break;
				}
				if (AttachType == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_TEXTURE)
				{
					break;
				}
				::cout << "Color flag type error\n";
				return false;
			}
			break;
		}
	}
	return true;
}
void cSanGLBufferManager::_Release()
{
	list<SANVAO>::iterator pVAO = this->m_VAOList.begin();
	while (pVAO != this->m_VAOList.end())
	{
		this->_DeleteVAO(*pVAO);
		pVAO++;
	}
	this->m_VAOList.clear();
	list<SANVBO>::iterator pVBO = this->m_VBOList.begin();
	while (pVBO != this->m_VBOList.end())
	{
		this->iReleaseVBOObject(pVBO->ID);
		pVBO++;
	}
	this->m_VBOList.clear();
	list<SANFBO>::iterator pFBO = this->m_FBOList.begin();
	while (pFBO != this->m_FBOList.end())
	{
		this->_DeleteFBO(*pFBO);
		pFBO++;
	}
	this->m_FBOList.clear();
	list<SANTBO>::iterator pTBO = this->m_TBOList.begin();
	while (pTBO != this->m_TBOList.end())
	{
		this->iReleaseTBOObject(pTBO->ID);
		pTBO++;
	}
	this->m_TBOList.clear();
}
uint32 cSanGLBufferManager::iCreateFBOObject(const SANFBODESC Desc)
{
	uint32 FBOID = ::gloRegisteResource(nullptr, sizeof(SANFBO), RT_BUFFER_FBO, (SVALUE)this);
	if (FBOID == 0)
	{
		return 0;
	}
	SANFBO Obj;
	Obj.ID = FBOID;
	for (uint32 seek = 0; seek<::eSANFBOBUFFERTYPE::SFBT_BUFFER_COUNT; seek = seek + 1)
	{
		Obj.obj_array[seek] = nullptr;
	}
	Obj.desc = Desc;
	if (this->_CreateFBO(Obj))
	{
		this->m_FBOList.insert(this->m_FBOList.end(), Obj);
		return Obj.ID;
	}
	else
	{
		//this->m_CurrentMaxFBONum=this->m_CurrentMaxFBONum-1;
		return 0;
	}
}
void cSanGLBufferManager::iReleaseFBOObject(const uint32 FBOID)
{
	if (::gloGetResourceType(FBOID) != ::RT_BUFFER_FBO)
	{
		return;
	}
	list<SANFBO>::iterator pItem = this->m_FBOList.begin();
	while (pItem != this->m_FBOList.end())
	{
		if (pItem->ID == FBOID)
		{
			this->_DeleteFBO((*pItem));
			::gloReleaseResource(pItem->ID, (SVALUE)this);
			this->m_FBOList.erase(pItem);
			return;
		}
		pItem++;
	}
}
bool cSanGLBufferManager::iUpdateFBOObject(const uint32 FBOID, const SANFBODESC Desc)
{
	if (::gloGetResourceType(FBOID) != ::RT_BUFFER_FBO)
	{
		return 0;
	}
	if (!this->_CheckFBODesc(Desc))
	{
		return false;
	}
	list<SANFBO>::iterator pItem = this->m_FBOList.begin();
	while (pItem != this->m_FBOList.end())
	{
		if (pItem->ID == FBOID)
		{
			if ((pItem->desc.width != Desc.width) || (pItem->desc.height != Desc.height) || (pItem->desc.pix_type != Desc.pix_type))
			{
				this->_DeleteFBO((*pItem));
				pItem->desc = Desc;
				return this->_CreateFBO((*pItem));
			}
			else
			{
				SANFBO UpdateFbo;
				UpdateFbo.desc.width = pItem->desc.width;
				UpdateFbo.desc.height = pItem->desc.height;
				UpdateFbo.desc.pix_type = pItem->desc.pix_type;
				SANFBO BackupFbo = (*pItem);
				for (uint32 seek = 0; seek<::eSANFBOBUFFERTYPE::SFBT_BUFFER_COUNT; seek = seek + 1)
				{
					switch (Desc.attach_flag_array[seek])
					{
					case ::eSANFBOBUFFERTYPE::SFBT_ATTACH_NULL:
						if (pItem->desc.attach_flag_array[seek] == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_RBO)
						{
							::glDeleteRenderbuffersEXT(1, (const GLuint*) (&(pItem->obj_array[seek])));
						}
						if (pItem->desc.attach_flag_array[seek] == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_TEXTURE)
						{
							if (pItem->obj_array[seek] == nullptr)
							{
								this->iReleaseTBOObject((uint32) pItem->obj_array[seek]);///////////////////
							}
							pItem->obj_array[seek] = nullptr;
							//pItem->desc.attach_handle_array[seek]=nullptr;
						}
						break;
					case ::eSANFBOBUFFERTYPE::SFBT_ATTACH_RBO:
						if (pItem->desc.attach_flag_array[seek] == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_TEXTURE)
						{
							if (pItem->obj_array[seek] == nullptr)
							{
								this->iReleaseTBOObject((uint32) pItem->obj_array[seek]);///////////////////
							}
							pItem->obj_array[seek] = nullptr;
							//pItem->desc.attach_handle_array[seek]=nullptr;
						}
						if (pItem->desc.attach_flag_array[seek] != ::eSANFBOBUFFERTYPE::SFBT_ATTACH_RBO)
						{
							UpdateFbo.desc.attach_flag_array[seek] = ::eSANFBOBUFFERTYPE::SFBT_ATTACH_RBO;
						}
						break;
					case ::eSANFBOBUFFERTYPE::SFBT_ATTACH_TEXTURE:
						if (pItem->desc.attach_flag_array[seek] == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_RBO)
						{
							::glDeleteRenderbuffersEXT(1, (const GLuint*) (&(pItem->obj_array[seek])));
						}
						if (pItem->desc.attach_flag_array[seek] == ::eSANFBOBUFFERTYPE::SFBT_ATTACH_TEXTURE)
						{
							if (pItem->desc.attach_handle_array[seek] != Desc.attach_handle_array[seek])
							{
								if (pItem->obj_array[seek] == Desc.attach_handle_array[seek])
								{
									pItem->desc.attach_handle_array[seek] = Desc.attach_handle_array[seek];
								}
								else
								{
									if (pItem->obj_array[seek] == nullptr)
									{
										this->iReleaseTBOObject((uint32) pItem->obj_array[seek]);///////////////////
									}
									pItem->obj_array[seek] = nullptr;
									pItem->desc.attach_flag_array[seek] = ::eSANFBOBUFFERTYPE::SFBT_ATTACH_NULL;
									break;
								}
							}
						}
						UpdateFbo.desc.attach_flag_array[seek] = Desc.attach_flag_array[seek];
						UpdateFbo.desc.attach_handle_array[seek] = Desc.attach_handle_array[seek];
						break;
					default:
						break;
					}
				}
				if (this->_CreateFBO(UpdateFbo))
				{
					this->_DeleteFBO((*pItem));
					pItem->desc = BackupFbo.desc;
					this->_CreateFBO((*pItem));// create also may return false
					return false;
				}
				for (uint32 seek = 0; seek<::eSANFBOBUFFERTYPE::SFBT_BUFFER_COUNT; seek = seek + 1)
				{
					if (seek != ::eSANFBOBUFFERTYPE::SFBT_FBO)
					{
						if (pItem->desc.attach_flag_array[seek] != Desc.attach_flag_array[seek])
						{
							pItem->obj_array[seek] = UpdateFbo.obj_array[seek];
						}
					}
				}
				pItem->desc = Desc;
				return true;
			}
		}
		pItem++;
	}
	return false;
}
uint32 cSanGLBufferManager::iCreateTBOObject(const SANTBODESC &Desc)
{
	if ((Desc.Width == 0) || (Desc.Height == 0))
	{
		return 0;
	}
	SANTBO TBO;
	TBO.ID = ::gloRegisteResource(nullptr, sizeof(SANTBO), RT_BUFFER_TBO, (SVALUE)this);
	//::glEnable(GL_TEXTURE_2D);
	::glGenTexturesEXT(1, (GLuint*) &(TBO.Handle));
	if (TBO.Handle == nullptr)
	{
		return 0;
	}
	this->_UpdateTBO(TBO.Handle, Desc);
	TBO.Desc = Desc;
	this->m_TBOList.insert(this->m_TBOList.end(), TBO);
	return TBO.ID;
}
uint32 cSanGLBufferManager::iCreateTBOObject(lpSANTEXTURE pTexture)
{
	SANTBODESC Desc;
	::gloMemSet(&Desc, 0, sizeof(Desc));
	switch (pTexture->PixFormat)
	{
	case PT_RGB:
		Desc.Type = SGTT_RGB_8;
		break;
	case PT_RGBA:
		Desc.Type = SGTT_RGBA_8;
		break;
	case PT_RGB_F:
		Desc.Type = SGTT_RGB_F;
		break;
	case PT_RGBA_F:
		Desc.Type = SGTT_RGBA_F;
		break;
	default:
		Desc.Type = SGTT_RGB_8;
		break;
	}
	switch (pTexture->Type)
	{
	case STT_TEX1D:
		return 0;
		break;
	case STT_TEX2D:
		if (pTexture->PixMapPtr != nullptr)
		{
			Desc.Width = ((lpSANPIXMAP2D) (pTexture->PixMapPtr))->width;
			Desc.Height = ((lpSANPIXMAP2D) (pTexture->PixMapPtr))->height;
			Desc.pBuffer = ((lpSANPIXMAP2D) (pTexture->PixMapPtr))->pPixMap;
			Desc.BufferSize = pTexture->DataSize;
			return this->iCreateTBOObject(Desc);
		}
		break;
	case STT_TEX3D:
		/*if(pTexture->DataHandle!=nullptr)
		{
		Desc.Width=((lpSANPIXMAP3D)(pTexture->DataHandle))->width;
		Desc.Height=((lpSANPIXMAP3D)(pTexture->DataHandle))->height;
		Desc.pBuffer=((lpSANPIXMAP3D)(pTexture->DataHandle))->pPixMap;
		Desc.BufferSize=pTexture->DataSize;
		}*/
		break;
	default:
		break;
	}
	return 0;
}
void cSanGLBufferManager::iReleaseTBOObject(uint32 TBOID)
{
	if (gloGetResourceType(TBOID) != ::RT_BUFFER_TBO)
	{
		return;
	}
	list<SANTBO>::iterator pItem = this->m_TBOList.begin();
	while (pItem != this->m_TBOList.end())
	{
		if (pItem->ID == TBOID)
		{
			::glEnable(GL_TEXTURE_2D);
			::glDeleteTexturesEXT(1, (const GLuint*) &(pItem->Handle));
			::glDisable(GL_TEXTURE_2D);
			::gloReleaseResource(pItem->ID, (SVALUE)this);
			this->m_TBOList.erase(pItem);
			return;
		}
		pItem++;
	}
}
uint32 cSanGLBufferManager::iUpdateTBOObject(const uint32 TBOID, const SANTBODESC &Desc)
{
	if (::gloGetResourceType(TBOID) != ::RT_BUFFER_TBO)
	{
		return 0;
	}
	if ((Desc.Width == 0) || (Desc.Height == 0))
	{
		return 0;
	}
	list<SANTBO>::iterator pItem = this->m_TBOList.begin();
	while (pItem != this->m_TBOList.end())
	{
		if (pItem->ID == TBOID)
		{
			this->_UpdateTBO(pItem->Handle, Desc);
			return pItem->ID;
		}
		pItem++;
	}
	return 0;
}
SANFBODESC cSanGLBufferManager::iGetFBODesc(const uint32 FBOID)
{
	SANFBODESC Desc;
	if (::gloGetResourceType(FBOID) != ::RT_BUFFER_FBO)
	{
		return Desc;
	}
	list<SANFBO>::iterator pItem = this->m_FBOList.begin();
	while (pItem != this->m_FBOList.end())
	{
		if (pItem->ID == FBOID)
		{
			return pItem->desc;
		}
		pItem++;
	}
	return Desc;
}
uint32 cSanGLBufferManager::iGetFBOBuffer(const uint32 FBOID, SHANDLE pBuffer, const uint32 Size, const eSANFBOBUFFERTYPE Type)
{
	if (::gloGetResourceType(FBOID) != ::RT_BUFFER_FBO)
	{
		return 0;
	}
	if ((pBuffer == nullptr) || (Size == 0))
	{
		return 0;
	}
	if (Type == ::eSANFBOBUFFERTYPE::SFBT_FBO)
	{
		return 0;
	}
	list<SANFBO>::iterator pItem = this->m_FBOList.begin();
	uint32 TexSize = 0;
	GLuint TexFormat = GL_RGB;
	GLuint DataType = GL_UNSIGNED_BYTE;
	while (pItem != this->m_FBOList.end())
	{
		if (pItem->ID == FBOID)
		{
			TexSize = pItem->desc.width*pItem->desc.height;
			switch (pItem->desc.pix_type)
			{
			case SGTT_RGB_8:
				TexSize = TexSize * 3;
				break;
			case SGTT_RGBA_8:
				TexSize = TexSize * 4;
				TexFormat = GL_RGBA;
				break;
			case SGTT_RGB_F:
				TexSize = TexSize * 12;
				DataType = GL_FLOAT;
				break;
			case SGTT_RGBA_F:
				TexSize = TexSize * 16;
				TexFormat = GL_RGBA;
				DataType = GL_FLOAT;
				break;
			case SGTT_DEPTH_24:
				TexSize = TexSize * 12;
				TexFormat = GL_DEPTH_COMPONENT;
				DataType = GL_FLOAT;
				break;
			case SGTT_DEPTH_32:
				TexSize = TexSize * 16;
				TexFormat = GL_DEPTH_COMPONENT;
				DataType = GL_FLOAT;
				break;
			case SGTT_STENCIL:
				TexSize = TexSize * 12;
				TexFormat = GL_STENCIL;
				DataType = GL_STENCIL_INDEX;
				break;
			default:
				TexSize = TexSize * 3;
				break;
			}
#ifdef _FLOAT_TYPE_DOUBLE
			if (TexFormat == GL_FLOAT)
			{
				TexFormat = GL_DOUBLE;
				TexSize = TexSize * 2;
			}
#endif
			if (TexSize>Size)
			{
				return 0;
			}
			GLuint PBOHandle = 0;
			SHANDLE PBOBuffer = nullptr;
			switch (pItem->desc.attach_flag_array[Type])
			{
			case ::eSANFBOBUFFERTYPE::SFBT_ATTACH_NULL:
				return 0;
			case ::eSANFBOBUFFERTYPE::SFBT_ATTACH_RBO:
				switch (Type)
				{
				case ::eSANFBOBUFFERTYPE::SFBT_DEPTH:
				case ::eSANFBOBUFFERTYPE::SFBT_STENCIL:
					::glReadBuffer(GL_FRONT);
					::glGenBuffers(1, &PBOHandle);
					::glBindBuffer(GL_PIXEL_PACK_BUFFER, PBOHandle);
					::glReadPixels(0, 0, pItem->desc.width, pItem->desc.height, TexFormat, DataType, 0);
					PBOBuffer = ::glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
					if (PBOBuffer != nullptr)
					{
						::gloMemCopy(pBuffer, PBOBuffer, Size);
						::glUnmapBufferARB(GL_PIXEL_PACK_BUFFER);
						::glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
						return Size;
					}
					::glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
					::glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
					return 0;
					break;
				default:
					if ((Type >= ::eSANFBOBUFFERTYPE::SFBT_COLOR_BEGIN) && (Type <= ::eSANFBOBUFFERTYPE::SFBT_COLOR_END))
					{
						::glReadBuffer(GL_FRONT);
						::glGenBuffers(1, &PBOHandle);
						::glBindBuffer(GL_PIXEL_PACK_BUFFER, PBOHandle);
						::glReadPixels(0, 0, pItem->desc.width, pItem->desc.height, TexFormat, DataType, 0);
						PBOBuffer = ::glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
						if (PBOBuffer != nullptr)
						{
							::gloMemCopy(pBuffer, PBOBuffer, Size);
							::glUnmapBufferARB(GL_PIXEL_PACK_BUFFER);
							::glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
							return Size;
						}
						::glUnmapBufferARB(GL_PIXEL_PACK_BUFFER);
						::glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
						return 0;
					}
					break;
				}
				break;
			case ::eSANFBOBUFFERTYPE::SFBT_ATTACH_TEXTURE:
				if ((Type >= ::eSANFBOBUFFERTYPE::SFBT_COLOR_BEGIN) && (Type <= ::eSANFBOBUFFERTYPE::SFBT_COLOR_END))
				{
					GLuint TexHandle = (GLuint)this->_FindTBOHandle((uint32) pItem->obj_array[Type]);
					//::glBindTexture(GL_TEXTURE_2D,TexHandle);//////////////////////////////////
					::glGetTextureImageEXT(TexHandle, GL_TEXTURE_2D, 0, TexFormat, DataType, pBuffer);
					return Size;
				}
				break;
			default:
				return 0;
			}
		}
		pItem++;
	}
	return 0;
}
/*SHANDLE cSanGLBufferManager::iGetFBOBufferHandle(const uint32 FBOID,const eSANFBOBUFFERTYPE Type)
{
if((FBOID<=San::RT_BUFFER_FBO)||(FBOID>=San::RT_BUFFER_FBO_MAX))
{
return nullptr;
}
if(Type==::eSANFBOBUFFERTYPE::SFBT_FBO)
{
return nullptr;
}
list<SANFBO>::iterator pItem=this->m_FBOList.begin();
while(pItem!=this->m_FBOList.end())
{
if(pItem->ID==FBOID)
{
if(pItem->desc.attach_flag_array[Type]!=::eSANFBOBUFFERTYPE::SFBT_ATTACH_NULL)
{
return pItem->obj_array[Type];
}
return nullptr;
}
pItem++;
}
return nullptr;
}*/
SANTBODESC cSanGLBufferManager::iGetTBODesc(uint32 TBOID)
{
	SANTBODESC Desc;
	::gloMemSet(&Desc, 0, sizeof(Desc));
	if (::gloGetResourceType(TBOID) != ::RT_BUFFER_TBO)
	{
		return Desc;
	}
	list<SANTBO>::iterator pItem = this->m_TBOList.begin();
	while (pItem != this->m_TBOList.end())
	{
		if (pItem->ID == TBOID)
		{
			Desc = pItem->Desc;
			Desc.pBuffer = nullptr;
			Desc.BufferSize = 0;
			return Desc;
		}
		pItem++;
	}
	return Desc;
}
void cSanGLBufferManager::iEnableVBOObject(const uint32 VBOID, const eSANVBOTYPE Type, const uint32 ShaderVersion)
{
	if ((VBOID == 0) || (Type == eSANVBOTYPE::SVBOT_COUNT))
	{
		if (ShaderVersion >= 300)
		{
			return;
		}
		::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		::glDisableClientState(GL_VERTEX_ARRAY);
		::glDisableClientState(GL_NORMAL_ARRAY);
		::glDisableClientState(GL_COLOR_ARRAY);
		::glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		::glBindBuffer(GL_ARRAY_BUFFER, 0);
		return;
	}
	if (::gloGetResourceType(VBOID) != ::RT_BUFFER_VBO)
	{
		return;
	}
	list<SANVBO>::iterator pItem = this->m_VBOList.begin();
	while (pItem != this->m_VBOList.end())
	{
		if (pItem->ID == VBOID)
		{
			::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) (pItem->Handle));
			switch (Type)
			{
			case eSANVBOTYPE::SVBOT_VERTEX:
				::glEnableClientState(GL_VERTEX_ARRAY);
				::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) (pItem->Handle));
				::glVertexPointer(3, GL_FLOAT, 0, nullptr);
				break;
			case eSANVBOTYPE::SVBOT_NORMAL:
				::glEnableClientState(GL_NORMAL_ARRAY);
				::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) (pItem->Handle));
				::glNormalPointer(GL_FLOAT, 0, nullptr);
				break;
			case eSANVBOTYPE::SVBOT_COLOR:
				::glEnableClientState(GL_COLOR_ARRAY);
				::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) (pItem->Handle));
				::glColorPointer(4, GL_FLOAT, 0, nullptr);
				break;
			default:
				if ((Type >= eSANVBOTYPE::SVBOT_TEXTURE_BEGIN) || (Type <= eSANVBOTYPE::SVBOT_TEXTURE_END))
				{
					::glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					::glClientActiveTexture(GL_TEXTURE0 + (Type - eSANVBOTYPE::SVBOT_TEXTURE_BEGIN));
					::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) (pItem->Handle));
					::glVertexPointer(2, GL_FLOAT, 0, nullptr);
				}
				break;
			}
			return;
		}
		pItem++;
	}
}
void cSanGLBufferManager::iEnableVAOObject(const uint32 VAOID, const uint32 ShaderVersion)
{
	if (VAOID == 0)
	{
		if (ShaderVersion >= 300)
		{
			::glBindVertexArray(0);
		}
		else
		{
			for (uint32 seek = 0; seek<eSANVBOTYPE::SVBOT_COUNT; seek = seek + 1)
			{
				this->iEnableVBOObject(0, eSANVBOTYPE(seek));
			}
		}
	}
	else
	{
		if (::gloGetResourceType(VAOID) != ::RT_BUFFER_VAO)
		{
			return;
		}
		list<SANVAO>::iterator pItem = this->m_VAOList.begin();
		while (pItem != this->m_VAOList.end())
		{
			if (pItem->ID == VAOID)
			{
				if (ShaderVersion >= 300)
				{
					::glBindVertexArray((GLuint) pItem->VAOHandle);
					return;
				}
				else
				{
					/*for(uint32 seek=0;seek<eSANVBOTYPE::SVBOT_COUNT;seek=seek+1)
					{
					if(pItem->VBOIDArray[seek]!=0)
					{
					this->iEnableVBOObject(pItem->VBOIDArray[seek],eSANVBOTYPE(seek));
					return;
					}
					}//*/
					::glEnableClientState(GL_VERTEX_ARRAY);
					::glEnableClientState(GL_NORMAL_ARRAY);
					::glEnableClientState(GL_COLOR_ARRAY);
					::glEnableClientState(GL_TEXTURE_COORD_ARRAY);
					::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) pItem->VBOHandleArray[::eSANVBOTYPE::SVBOT_VERTEX]);
					::glVertexPointer(3, GL_FLOAT, 0, NULL);
					::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) pItem->VBOHandleArray[::eSANVBOTYPE::SVBOT_NORMAL]);
					::glNormalPointer(GL_FLOAT, 0, NULL);
					::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) pItem->VBOHandleArray[::eSANVBOTYPE::SVBOT_COLOR]);
					::glColorPointer(4, GL_FLOAT, 0, NULL);
					if (pItem->TexListSize != 0)
					{
						UINT TextureID = GL_TEXTURE0;
						for (int seektex = 0; seektex<pItem->TexListSize; seektex = seektex + 1)
						{
							::glClientActiveTexture(TextureID);
							::glBindBuffer(GL_ARRAY_BUFFER, (GLuint) pItem->VBOHandleArray[::eSANVBOTYPE::SVBOT_TEXTURE_BEGIN] + seektex);
							::glTexCoordPointer(2, GL_FLOAT, 0, NULL);
							TextureID = TextureID + 1;///////////////////////////////max hardware texture support
						}
					}//*/
				}
			}
			pItem++;
		}
	}
}
void cSanGLBufferManager::iEnableFBOObject(const uint32 FBOID)
{
	if (FBOID == 0)
	{
		::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		return;
	}
	if (::gloGetResourceType(FBOID) != ::RT_BUFFER_FBO)
	{
		return;
	}
	list<SANFBO>::iterator pItem = this->m_FBOList.begin();
	while (pItem != this->m_FBOList.end())
	{
		if (pItem->ID == FBOID)
		{
			if (pItem->obj_array[::eSANFBOBUFFERTYPE::SFBT_FBO] != nullptr)
			{
				::glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, (GLuint) (pItem->obj_array[::eSANFBOBUFFERTYPE::SFBT_FBO]));
			}
		}
		pItem++;
	}
}
void cSanGLBufferManager::iEnableTBOObject(const uint32 TBOID, const uint32 Position, const SANRENDERDESC &FilterDesc)
{
	if (TBOID == 0)
	{
		//::glEnable(GL_TEXTURE_2D);///////////////////////////////////////
		::glBindTexture(GL_TEXTURE_2D, 0);
		return;
	}
	if (::gloGetResourceType(TBOID) != ::RT_BUFFER_TBO)
	{
		return;
	}
	if (Position >= this->m_MaxDeviceSupportedTextureSize)
	{
		return;
	}
	list<SANTBO>::iterator pItem = this->m_TBOList.begin();
	while (pItem != this->m_TBOList.end())
	{
		if (pItem->ID == TBOID)
		{
			//::glEnable(GL_TEXTURE_2D);///////////////////////////////////
			::glActiveTexture(GL_TEXTURE0 + Position);
			::glBindTexture(GL_TEXTURE_2D, (GLuint) (pItem->Handle));
			if (FilterDesc.tex_mip_level == 0)
			{
				switch (FilterDesc.tex_filter_min)
				{
				case STEXF_NONE:
				case STEXF_NEAREST:
					::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					break;
				case STEXF_LINEAR:
					::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					break;
				case STEXF_ANISOTROPIC:
				case STEXF_BILINEAR:
				case STEXF_TRILINEAR:
				default:
					::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					break;
				}
				switch (FilterDesc.tex_filter_mag)
				{
				case STEXF_NONE:
				case STEXF_NEAREST:
					::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					break;
				case STEXF_LINEAR:
					::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					break;
				case STEXF_ANISOTROPIC:
				case STEXF_BILINEAR:
				case STEXF_TRILINEAR:
				default:
					::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					break;
				}
			}
			else
			{
				switch (FilterDesc.tex_filter_min)
				{
				case STEXF_NONE:
				case STEXF_NEAREST:
					switch (FilterDesc.tex_filter_mip_min)
					{
					case STEXF_NONE:
					case STEXF_NEAREST:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
						break;
					case STEXF_LINEAR:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
						break;
					case STEXF_ANISOTROPIC:
					case STEXF_BILINEAR:
					case STEXF_TRILINEAR:
					default:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
						break;
					}
					break;
				case STEXF_LINEAR:
					switch (FilterDesc.tex_filter_mip_min)
					{
					case STEXF_NONE:
					case STEXF_NEAREST:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
						break;
					case STEXF_LINEAR:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						break;
					case STEXF_ANISOTROPIC:
					case STEXF_BILINEAR:
					case STEXF_TRILINEAR:
					default:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						break;
					}
					break;
				case STEXF_ANISOTROPIC:
				case STEXF_BILINEAR:
				case STEXF_TRILINEAR:
				default:
					switch (FilterDesc.tex_filter_mip_min)
					{
					case STEXF_NONE:
					case STEXF_NEAREST:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
						break;
					case STEXF_LINEAR:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						break;
					case STEXF_ANISOTROPIC:
					case STEXF_BILINEAR:
					case STEXF_TRILINEAR:
					default:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						break;
					}
					break;
				}
				switch (FilterDesc.tex_filter_mag)
				{
				case STEXF_NONE:
				case STEXF_NEAREST:
					switch (FilterDesc.tex_filter_mip_mag)
					{
					case STEXF_NONE:
					case STEXF_NEAREST:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
						break;
					case STEXF_LINEAR:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
						break;
					case STEXF_ANISOTROPIC:
					case STEXF_BILINEAR:
					case STEXF_TRILINEAR:
					default:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
						break;
					}
					break;
				case STEXF_LINEAR:
					switch (FilterDesc.tex_filter_mip_mag)
					{
					case STEXF_NONE:
					case STEXF_NEAREST:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
						break;
					case STEXF_LINEAR:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						break;
					case STEXF_ANISOTROPIC:
					case STEXF_BILINEAR:
					case STEXF_TRILINEAR:
					default:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						break;
					}
					break;
				case STEXF_ANISOTROPIC:
				case STEXF_BILINEAR:
				case STEXF_TRILINEAR:
				default:
					switch (FilterDesc.tex_filter_mip_mag)
					{
					case STEXF_NONE:
					case STEXF_NEAREST:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
						break;
					case STEXF_LINEAR:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						break;
					case STEXF_ANISOTROPIC:
					case STEXF_BILINEAR:
					case STEXF_TRILINEAR:
					default:
						::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						break;
					}
					break;
				}
			}
			if (FilterDesc.b_tex_wrap_s_repeat)
			{
				::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			}
			else
			{
				::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			}
			if (FilterDesc.b_tex_wrap_t_repeat)
			{
				::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			}
			else
			{
				::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			}
			return;
		}
		pItem++;
	}
}
#endif