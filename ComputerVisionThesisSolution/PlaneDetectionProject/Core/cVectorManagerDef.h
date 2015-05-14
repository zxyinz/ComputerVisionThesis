#include"../RenderSystem.h"
#include"SanTypes.h"
#include"SanString.h"
using namespace std;
namespace San
{
#ifndef __CSANVECTORMANAGERDEF_H__
#define __CSANVECTORMANAGERDEF_H__

#define VINDEXSIZE 64
	enum eSANVERTEXTYPE
	{
		VT_ROOT			= 0x00000001,// 0000 0001b
		VT_SYS			= 0x00000002,// 0000 0010b
		VT_USER			= 0x00000004,// 0000 0100b
		VT_VAR			= 0x00000008,// 0000 1000b
		VT_INDEX		= 0x00000010,// 0001 0000b
		VT_CONST		= 0x00000020,// 0010 0000b
	};
#define VF_LOCAL           0x00000001// 0000 0001b
#define VF_INDEX           0x00000002// 0000 0010b

	struct stVMEMBER
	{
		SHANDLE		value;
		SString		str_name;
		stVMEMBER*	pprevious;
		stVMEMBER*	pnext;
	};
	typedef stVMEMBER	VMEMBER;
	typedef stVMEMBER*	lpVMEMBER;

	struct stVECTORINFO
	{
#ifndef _DEBUG
		SHANDLE			v_ptr;
		SHANDLE			v_seek;
#else
		union
		{
			SHANDLE		v_ptr;
			lpVMEMBER	v_ptr_mv;
		};
		union
		{
			SHANDLE		v_seek;
			lpVMEMBER	v_seek_mv;
		};
#endif
		uint32			size;
		uint32			maxsize;
		uint32			id;
		uint32			pid;
		SString			str_name;
		SString			str_pname;
		uint32			type;
		(void)			(*prelease)(SHANDLE);
		stVECTORINFO*	ppervious;
		stVECTORINFO*	pnext;
	};

	struct VECTORNODE :public stVECTORINFO
	{
	public:
		VECTORNODE()
		{
			this->id=0;
			this->pid=0;
			this->size=0;
			this->maxsize=0;
			this->str_name.clear();
			this->str_pname.clear();
			this->type=0;
			this->prelease=NULL;
			this->ppervious=NULL;
			this->pnext=NULL;
			this->v_ptr=NULL;
			this->v_seek=NULL;
		};
		~VECTORNODE(){};
	protected:
		VECTORNODE(const VECTORNODE&){};
		VECTORNODE&	operator = (const VECTORNODE&){};
	};
	typedef VECTORNODE*	lpVECTORNODE;

#endif
}