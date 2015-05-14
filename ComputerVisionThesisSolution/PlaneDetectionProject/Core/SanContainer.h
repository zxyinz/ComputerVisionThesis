#include"../RenderSystem.h"
#include"Container\SanContainerDef.h"
#include"Container\SanContainerStream.h"
#include"Container\SanContainerTree.h"
#pragma once
namespace San
{
#ifndef __STDSANCONTAINER_H__
#define __STDSANCONTAINER_H__

	//----------------------------C Container Definition---------------------------//

	struct st_san_array
	{
		uint32*	p_data_buffer;
		uint32*	p_weight_buffer;
		uint32	array_size;
	};

	struct st_san_list
	{
		st_san_list*	p_prev;
		st_san_list*	p_next;
		uint32	weight;
		SHANDLE	data_ptr;
	};

	typedef st_san_list	st_san_queue;
	typedef st_san_list	st_san_stack;

	struct st_san_tree
	{
		st_san_tree* p_trunk;
		st_san_tree* p_offshoot_list;
		uint32	list_size;
		uint32	index;
		uint32	weight;
		SHANDLE	data_ptr;
	};
	//-----------------------------------------------------------------------------//
#endif
}