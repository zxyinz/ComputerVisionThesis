#include"cSanLightManager.h"
using namespace std;
using namespace San;
using namespace San::Graphics;
bool cSanLightManager::m_bGlobalLightState = false;
uint32 cSanLightManager::m_MaxLightManagerID = 1;
uint32 cSanLightManager::m_ActiveLightManagerID = 0;
bool cSanLightManager::iCreateLightManager()
{
	if (this->m_strLightManagerName.empty())
	{
		return false;
	}
	if (::gloFoundVector(this->m_strLightManagerName))
	{
		return false;
	}
	::gloRegisterVector(this->m_strLightManagerName, VT_SYS | VT_VAR, (SHANDLE)this);
	this->m_GlobalLightList.clear();
	this->m_PointLightList.clear();
	this->m_DirectionalLightList.clear();
	this->m_SpotLightList.clear();
	this->m_UnknownLightList.clear();
	return true;
}
void cSanLightManager::iReleaseLightManager()
{
	if (this->m_LightManagerID == this->m_ActiveLightManagerID)
	{
		this->m_ActiveLightManagerID = 0;
	}
	this->m_GlobalLightList.clear();
	this->m_DirectionalLightList.clear();
	this->m_SpotLightList.clear();
	this->m_UnknownLightList.clear();
	::gloDestoryVector(this->m_strLightManagerName);
}
eSANLIGHTTYPE cSanLightManager::_GetLightPtr(SString strLightName, list<cSanLight>::iterator* pLightElement)
{
	list<cSanLight>* pLightList;
	list<cSanLight>::iterator pLight;
	for (int seek = ::SLT_GLOBAL; seek<::SLT_LIGHTTYPECOUNT; seek = seek + 1)
	{
		switch (seek)
		{
		case SLT_GLOBAL:
			pLightList = &(this->m_GlobalLightList);
			break;
		case SLT_POINT:
			pLightList = &(this->m_PointLightList);
			break;
		case SLT_DIRECTIONAL:
			pLightList = &(this->m_DirectionalLightList);
			break;
		case SLT_SPOT:
			pLightList = &(this->m_SpotLightList);
			break;
		default:
			pLightList = nullptr;
			break;
		}
		if (pLightList == nullptr)
		{
			return ::SLT_MAXSOFTWARELIGHT;
		}
		pLight = pLightList->begin();
		while (pLight != pLightList->end())
		{
			if (pLight->iGetLightName() == strLightName)
			{
				if (pLightElement != nullptr)
				{
					*pLightElement = pLight;
				}
				return eSANLIGHTTYPE(seek);
			}
			pLight++;
		}
	}
	pLight = this->m_UnknownLightList.begin();
	while (pLight != this->m_UnknownLightList.end())
	{
		if (pLight->iGetLightName() == strLightName)
		{
			if (pLightElement != nullptr)
			{
				*pLightElement = pLight;
			}
			return ::SLT_MAXHARDWAERLIGHT;
		}
		pLight++;
	}
	return ::SLT_MAXSOFTWARELIGHT;
}
cSanLight* cSanLightManager::iRegisteLight(SString strLightName)
{
	if (this->_GetLightPtr(strLightName, nullptr) == San::Graphics::SLT_MAXSOFTWARELIGHT)
	{
		return nullptr;
	}
	cSanLight Light(strLightName);
	this->m_UnknownLightList.insert(this->m_UnknownLightList.end(), Light);
	return &(*(this->m_UnknownLightList.end()--));
}
void cSanLightManager::iUnRegisteLight(SString strLightName)
{
	list<cSanLight>::iterator pLightElement;
	eSANLIGHTTYPE Type = this->_GetLightPtr(strLightName, &pLightElement);
	switch (Type)
	{
	case SLT_GLOBAL:
		this->m_GlobalLightList.erase(pLightElement);
		break;
	case SLT_POINT:
		this->m_PointLightList.erase(pLightElement);
		break;
	case SLT_DIRECTIONAL:
		this->m_DirectionalLightList.erase(pLightElement);
		break;
	case SLT_SPOT:
		this->m_SpotLightList.erase(pLightElement);
		break;
	default:
		if (Type != ::SLT_MAXSOFTWARELIGHT)
		{
			this->m_UnknownLightList.erase(pLightElement);
		}
		break;
	}
}
cSanLight* cSanLightManager::iGetLightPtr(SString strLightName)
{
	list<cSanLight>::iterator pLightElement;
	if (this->_GetLightPtr(strLightName, &pLightElement) != ::SLT_MAXSOFTWARELIGHT)
	{
		return &(*pLightElement);
	}
	return nullptr;
}
void cSanLightManager::iEnableGraphicsLight()
{
	if (this->m_LightManagerID == this->m_ActiveLightManagerID)
	{
		this->m_bGlobalLightState = true;
	}
}
void cSanLightManager::iDisableGraphicsLight()
{
	if (this->m_LightManagerID == this->m_ActiveLightManagerID)
	{
		this->m_bGlobalLightState = false;
	}
}
void cSanLightManager::iEnableLightManager()
{
	this->m_ActiveLightManagerID = this->m_LightManagerID;
}
void cSanLightManager::iDisableLightManager()
{
	this->m_ActiveLightManagerID = 0;
}
bool cSanLightManager::iGetGraphicsLightState()
{
	return this->m_bGlobalLightState;
}
bool cSanLightManager::iGetLightManagerState()
{
	if (this->m_LightManagerID == this->m_ActiveLightManagerID)
	{
		return true;
	}
	else
	{
		return false;
	}
}
uint32 cSanLightManager::iGetCurrentActiveLightManagerID()
{
	return this->m_ActiveLightManagerID;
}
void cSanLightManager::iUpdateLightManager()
{
	list<cSanLight>* pLightList;
	for (int typeseek = ::SLT_GLOBAL; typeseek <= ::SLT_LIGHTTYPECOUNT; typeseek = typeseek + 1)
	{
		switch (typeseek)
		{
		case SLT_GLOBAL:
			pLightList = &(this->m_GlobalLightList);
			break;
		case SLT_POINT:
			pLightList = &(this->m_PointLightList);
			break;
		case SLT_DIRECTIONAL:
			pLightList = &(this->m_DirectionalLightList);
			break;
		case SLT_SPOT:
			pLightList = &(this->m_SpotLightList);
			break;
		default:
			pLightList = &(this->m_UnknownLightList);
			break;
		}
		list<cSanLight>::iterator pLightElement = pLightList->begin();
		while (pLightElement != pLightList->end())
		{
			uint32 LightType = pLightElement->iGetLightDescription().type;
			if (LightType != typeseek)
			{
				cSanLight Light = *pLightElement;
				pLightList->erase(pLightElement);
				switch (LightType)
				{
				case SLT_GLOBAL:
					this->m_GlobalLightList.insert(this->m_GlobalLightList.end(), Light);
					break;
				case SLT_POINT:
					this->m_PointLightList.insert(this->m_PointLightList.end(), Light);
					break;
				case SLT_DIRECTIONAL:
					this->m_DirectionalLightList.insert(this->m_DirectionalLightList.end(), Light);
					break;
				case SLT_SPOT:
					this->m_SpotLightList.insert(this->m_SpotLightList.end(), Light);
					break;
				default:
					this->m_UnknownLightList.insert(this->m_UnknownLightList.end(), Light);
					break;
				}
				if (pLightList->empty())
				{
					break;
				}
			}
			pLightElement++;
		}
	}
}
void cSanLightManager::iUpdateLight(SString strLightName)
{
	cSanLight* pLight = nullptr;
	eSANLIGHTTYPE LightType = eSANLIGHTTYPE::SLT_GLOBAL;
	/*list<cSanLight>::iterator pLightElement=this->m_UnknownLightList.begin();
	while(pLightElement!=this->m_UnknownLightList.end())
	{
	if(pLightElement->iGetLightName()==strLightName)
	{
	if(pLightElement->iGetLightDescription().type>=::SLT_MAXHARDWAERLIGHT)
	{
	return;
	}
	pLight=&(*pLightElement);
	LightType=::SLT_MAXSOFTWARELIGHT;
	break;
	}
	pLightElement++;
	}
	if(pLight==nullptr)
	{
	pLightElement=this->m_GlobalLightList.begin();
	while(pLightElement!=this->m_GlobalLightList.end())
	{
	if(pLightElement->iGetLightName()==strLightName)
	{
	if(pLightElement->iGetLightDescription().type==::SLT_GLOBAL)
	{
	return;
	}
	pLight=&(*pLightElement);
	LightType=::SLT_GLOBAL;
	break;
	}
	pLightElement++;
	}
	}
	if(pLight==nullptr)
	{
	pLightElement=this->m_PointLightList.begin();
	while(pLightElement!=this->m_PointLightList.end())
	{
	if(pLightElement->iGetLightName()==strLightName)
	{
	if(pLightElement->iGetLightDescription().type==::SLT_POINT)
	{
	return;
	}
	pLight=&(*pLightElement);
	LightType=::SLT_POINT;
	break;
	}
	pLightElement++;
	}
	}
	if(pLight==nullptr)
	{
	pLightElement=this->m_DirectionalLightList.begin();
	while(pLightElement!=this->m_DirectionalLightList.end())
	{
	if(pLightElement->iGetLightName()==strLightName)
	{
	if(pLightElement->iGetLightDescription().type==::SLT_DIRECTIONAL)
	{
	return;
	}
	pLight=&(*pLightElement);
	LightType=::SLT_DIRECTIONAL;
	break;
	}
	pLightElement++;
	}
	}
	if(pLight==nullptr)
	{
	pLightElement=this->m_SpotLightList.begin();
	while(pLightElement!=this->m_SpotLightList.end())
	{
	if(pLightElement->iGetLightName()==strLightName)
	{
	if(pLightElement->iGetLightDescription().type==::SLT_SPOT)
	{
	return;
	}
	pLight=&(*pLightElement);
	LightType=::SLT_SPOT;
	break;
	}
	pLightElement++;
	}
	}*/
	list<cSanLight>* pLightList;
	list<cSanLight>::iterator pLightElement;
	for (uint32 seek = ::SLT_GLOBAL; seek <= ::SLT_LIGHTTYPECOUNT; seek = seek + 1)
	{
		switch (seek)
		{
		case SLT_GLOBAL:
			pLightList = &(this->m_GlobalLightList);
			break;
		case SLT_POINT:
			pLightList = &(this->m_PointLightList);
			break;
		case SLT_DIRECTIONAL:
			pLightList = &(this->m_DirectionalLightList);
			break;
		case SLT_SPOT:
			pLightList = &(this->m_SpotLightList);
			break;
		default:
			pLightList = &(this->m_UnknownLightList);
			break;
		}
		pLightElement = pLightList->begin();
		while (pLightElement != pLightList->end())
		{
			if (pLightElement->iGetLightName() == strLightName)
			{
				if (pLightElement->iGetLightDescription().type == seek)
				{
					return;
				}
				if (seek == ::SLT_LIGHTTYPECOUNT)
				{
					if (pLightElement->iGetLightDescription().type >= ::SLT_MAXHARDWAERLIGHT)
					{
						return;
					}
				}
				pLight = &(*pLightElement);
				break;
			}
			pLightElement++;
		}
		if (pLight != nullptr)
		{
			break;
		}
	}
	if (pLight == nullptr)
	{
		return;
	}
	pLightList = &(this->m_UnknownLightList);
	switch (LightType)
	{
	case SLT_GLOBAL:
		pLightList = &(this->m_GlobalLightList);
		break;
	case SLT_POINT:
		pLightList = &(this->m_PointLightList);
		break;
	case SLT_DIRECTIONAL:
		pLightList = &(this->m_DirectionalLightList);
		break;
	case SLT_SPOT:
		pLightList = &(this->m_SpotLightList);
		break;
	default:
		break;
	}
	pLightElement = pLightList->begin();
	cSanLight Light = *pLight;
	strLightName = pLight->iGetLightName();//
	pLight = nullptr;
	while (pLightElement != pLightList->end())
	{
		if (pLightElement->iGetLightName() == strLightName)
		{
			pLightList->erase(pLightElement);
			break;
		}
		pLightElement++;
	}
	switch (Light.iGetLightDescription().type)
	{
	case SLT_GLOBAL:
		this->m_GlobalLightList.insert(this->m_GlobalLightList.end(), Light);
		break;
	case SLT_POINT:
		this->m_PointLightList.insert(this->m_PointLightList.end(), Light);
		break;
	case SLT_DIRECTIONAL:
		this->m_DirectionalLightList.insert(this->m_DirectionalLightList.end(), Light);
		break;
	case SLT_SPOT:
		this->m_SpotLightList.insert(this->m_SpotLightList.end(), Light);
		break;
	default:
		this->m_PointLightList.insert(this->m_PointLightList.end(), Light);
		break;
	}
}
SRESULT cSanLightManager::iDoSomething(SVALUE wParam, SVALUE lParam)
{
	if (this->m_LightManagerID != this->m_ActiveLightManagerID)
	{
		return 0;
	}
	if (this->m_bGlobalLightState)
	{
#ifdef _OPENGL
		::glEnable(GL_LIGHTING);
#endif
	}
	else
	{
#ifdef _OPENGL
		::glDisable(GL_LIGHTING);
#endif
		return 0;
	}
	if (this->m_GlobalLightList.size() != 0)
	{
		cSanLight GlobalLight(_SSTR("GlobalLight"));
		SANCOLORF color_a = SANCOLORF(0.0, 0.0, 0.0, 0.0);
		SANCOLORF color_d = SANCOLORF(0.0, 0.0, 0.0, 0.0);
		SANCOLORF color_s = SANCOLORF(0.0, 0.0, 0.0, 0.0);
		SANCOLORF color_e = SANCOLORF(0.0, 0.0, 0.0, 0.0);
		list<cSanLight>::iterator pLight = this->m_GlobalLightList.begin();
		while (pLight != this->m_GlobalLightList.end())
		{
			color_a = color_a + pLight->iGetLightDescription().ambient;/////////////////////////////need operator SANCOLOR plus max value 255
			color_d = color_d + pLight->iGetLightDescription().diffuse;
			color_s = color_s + pLight->iGetLightDescription().specular;
			color_e = color_e + pLight->iGetLightDescription().emission;
			pLight->iSetLightDisable();
			pLight++;
		}
		GlobalLight.iSetLightColor(color_d, color_s, color_a, color_e);
		GlobalLight.iSetLightEnable();
		GlobalLight.iDoSomething(NULL, NULL);
		GlobalLight.iSetLightDisable();
	}
	uint32 lightsize = ::SLT_MAXHARDWAERLIGHT;
	if (this->m_GlobalLightList.size()<lightsize)
	{
		lightsize = this->m_GlobalLightList.size();
	}
	list<cSanLight>::iterator pLightPtr = this->m_GlobalLightList.begin();
	for (int seek = 0; seek<lightsize; seek = seek + 1)
	{
		pLightPtr->iDoSomething(NULL, NULL);
		pLightPtr++;
	}
	return 0;
}