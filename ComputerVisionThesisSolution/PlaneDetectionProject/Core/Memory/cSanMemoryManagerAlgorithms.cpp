#include<stdlib.h>
#include"cSanMemoryManagerAlgorithms.h"
struct st_ssm_mem_page ssm_page_list[SSM_MEM_LIST_SIZE];
//struct st_ssm_mem_page ssm_meg_page_list[SSM_MEG_PAGE_LIST_SIZE];
enum eSBOOL b_ssm_pages_inited=s_false;
enum eSBOOL b_ssm_mem_locked=s_true;
inline void* _ssm_new_i(size_t mem_size,unsigned int page);

int _ssm_init()
{
	size_t seek=0;
	size_t seek_i=0;
	size_t page_size=0;
	size_t hash_size=0;
	struct st_ssm_mem_block* block_ptr=NULL;
	if(!b_ssm_pages_inited)
	{
		if(SSM_MEM_UNIT_SIZE<16U)
		{
			return s_false;
		}
		_ssm_clear();
		page_size=SSM_MEM_UNIT_SIZE*SSM_MEM_PAGE_SIZE;
		hash_size=SSM_ABLOCK_HTABLE_SIZE;
		for(seek=0;seek<SSM_MEM_LIST_SIZE;seek=seek+1)
		{
			if(seek>=SSM_PAGE_LIST_SIZE)
			{
				page_size=SSM_MEM_UNIT_SIZE*(SSM_MEM_PAGE_SIZE<<SSM_MEG_PAGE_WEIGHT);
				hash_size=(SSM_ABLOCK_HTABLE_SIZE<<SSM_MEG_PAGE_WEIGHT);
				ssm_page_list[seek].b_is_meg_page=s_true;
			}
			else
			{
				ssm_page_list[seek].b_is_meg_page=s_false;
			}
			ssm_page_list[seek].page_buffer_ptr=new unsigned char[page_size];//malloc(page_size);
			if(ssm_page_list[seek].page_buffer_ptr==NULL)
			{
				_ssm_clear();
				return s_false;
			}
			ssm_page_list[seek].hash_table_ptr=new void*[hash_size];//(void**)malloc(hash_size*sizeof(void*));
			if(ssm_page_list[seek].hash_table_ptr==NULL)
			{
				_ssm_clear();
				return s_false;
			}
			ssm_page_list[seek].available_ptr=ssm_page_list[seek].page_buffer_ptr;
			block_ptr=(struct st_ssm_mem_block*)(ssm_page_list[seek].available_ptr);
			block_ptr->block_size=page_size/SSM_MEM_UNIT_SIZE;
			block_ptr->prev_addr=NULL;
			block_ptr->next_addr=NULL;
			for(seek_i=0;seek_i<hash_size;seek_i=seek_i+1)
			{
				ssm_page_list[seek].hash_table_ptr[0]=ssm_page_list[seek].available_ptr;
			}
			ssm_page_list[seek].allocated_mem=0;
		}
		b_ssm_pages_inited=s_true;
		b_ssm_mem_locked=s_false;
		return s_true;
	}
	return s_true;
}
void _ssm_release()
{
	while(b_ssm_mem_locked)
	{
	}
	b_ssm_mem_locked=s_true;
	_ssm_clear();
	b_ssm_pages_inited=s_false;
	b_ssm_mem_locked=s_true;
}
void _ssm_clear()
{
	size_t seek=0;
	for(seek=0;seek<SSM_MEM_LIST_SIZE;seek=seek+1)
	{
		if(ssm_page_list[seek].page_buffer_ptr!=NULL)
		{
			delete[] ssm_page_list[seek].page_buffer_ptr;
			//free(ssm_page_list[seek].page_buffer_ptr);
			ssm_page_list[seek].page_buffer_ptr=NULL;
			//free(ssm_page_list[seek].hash_table_ptr);
			delete[] ssm_page_list[seek].hash_table_ptr;
			ssm_page_list[seek].hash_table_ptr=NULL;
			ssm_page_list[seek].available_ptr=NULL;
			ssm_page_list[seek].allocated_mem=0;
			ssm_page_list[seek].b_is_meg_page=s_false;
		}
	}
}
void* _ssm_new(size_t mem_size,void* buffer_ptr,size_t buffer_size,int page)
{
	void* mem_ptr=nullptr;
	size_t size=0;
	if(mem_size>buffer_size)
	{
		size=buffer_size;
	}
	else
	{
		size=mem_size;
	}
	if(mem_size==0)
	{
		return (void*)mem_ptr;
	}
	if(b_ssm_pages_inited==s_false)
	{
		return (void*)mem_ptr;
	}
	if(page>=(int)SSM_MEM_LIST_SIZE)
	{
		return (void*)mem_ptr;
	}
	if(page>=0)
	{
		mem_ptr=_ssm_new_i(mem_size,page);
		if(mem_ptr!=nullptr)
		{
			if((buffer_ptr!=nullptr)&(buffer_size>0))
			{
				for(int seek=0;seek<size;seek=seek+1)
				{
					((unsigned char*)mem_ptr)[seek]=((unsigned char*)buffer_ptr)[seek];
				}
			}
		}
		return mem_ptr;
	}
	else
	{
		for(int seek=0;seek<SSM_MEM_LIST_SIZE;seek=seek+1)
		{
			mem_ptr=_ssm_new_i(mem_size,seek);
			if(mem_ptr!=nullptr)
			{
				if((buffer_ptr!=nullptr)&(buffer_size>0))
				{
					for(int seek=0;seek<size;seek=seek+1)
					{
						((unsigned char*)mem_ptr)[seek]=((unsigned char*)buffer_ptr)[seek];
					}
				}
				return mem_ptr;
			}
		}
	}
	return mem_ptr;
}//*/
void* _ssm_new_i(size_t mem_size,unsigned int page)
{
	st_ssm_mem_block* mem_ptr=nullptr;
	st_ssm_mem_page* page_ptr=nullptr;
	st_ssm_mem_block* prev_mem_ptr=nullptr;
	st_ssm_mem_block* new_block=nullptr;
	size_t size=0;
	while(b_ssm_mem_locked)
	{
	}
	b_ssm_mem_locked=s_true;
	size=mem_size/SSM_MEM_UNIT_SIZE+(mem_size%SSM_MEM_UNIT_SIZE+SSM_MEM_UNIT_SIZE-1)/SSM_MEM_UNIT_SIZE;
	if(size==0)
	{
		b_ssm_mem_locked=s_false;
		return mem_ptr;
	}
	page_ptr=ssm_page_list+page;
	mem_ptr=(st_ssm_mem_block*)(page_ptr->available_ptr);
	prev_mem_ptr=nullptr;
	while((mem_ptr!=NULL)&&(mem_ptr->block_size<size))
	{
		prev_mem_ptr=mem_ptr;
		mem_ptr=(st_ssm_mem_block*)mem_ptr->next_addr;// X32 X64
	}
	if(mem_ptr==NULL)
	{
		b_ssm_mem_locked=s_false;
		return mem_ptr;
	}
	if(mem_ptr->block_size!=size)
	{
		new_block=(st_ssm_mem_block*)((size_t)mem_ptr+size*SSM_MEM_UNIT_SIZE);
		new_block->block_size=mem_ptr->block_size-size;
		new_block->next_addr=mem_ptr->next_addr;
	}
	else
	{
		new_block=(st_ssm_mem_block*)mem_ptr->next_addr;
	}
	if(new_block!=NULL)
	{
		new_block->prev_addr=mem_ptr->prev_addr;
		if(prev_mem_ptr!=nullptr)
		{
			prev_mem_ptr->next_addr=(unsigned long)(&new_block);// X32 X64
		}
		else
		{
			page_ptr->available_ptr=(void*)new_block;
			prev_mem_ptr=(st_ssm_mem_block*)page_ptr->available_ptr;
		}
	}
	else
	{
		if(prev_mem_ptr!=nullptr)
		{
			prev_mem_ptr->next_addr=NULL;
		}
		else
		{
			page_ptr->available_ptr=NULL;
		}
	}
	size_t begin_index=(size_t)mem_ptr-(size_t)(page_ptr->page_buffer_ptr);
	size_t end_index=(size_t)new_block-(size_t)(page_ptr->page_buffer_ptr)-1;
	begin_index=begin_index%SSM_HASH_KEY;
	end_index=end_index%SSM_HASH_KEY;
	for(size_t seek=begin_index;seek<=end_index;seek=seek+1)
	{
		page_ptr->hash_table_ptr[seek]=(void*)prev_mem_ptr;
	}
	page_ptr->allocated_mem=page_ptr->allocated_mem+size;
	b_ssm_mem_locked=s_false;
	return mem_ptr;
}
void* _ssm_delete(void* mem_ptr,size_t mem_size)
{
	st_ssm_mem_page* page_ptr=nullptr;
	st_ssm_mem_block* block_ptr=nullptr;
	st_ssm_mem_block* prev_block_ptr=nullptr;
	size_t size;
	size_t page_size;
	size_t hash_index=0;
	if(mem_ptr==nullptr)
	{
		return mem_ptr;
	}
	if(mem_size==0)
	{
		return nullptr;
	}
	while(b_ssm_mem_locked==s_true)
	{
	}
	b_ssm_mem_locked=s_true;
	size=mem_size/SSM_MEM_UNIT_SIZE+(mem_size%SSM_MEM_UNIT_SIZE+SSM_MEM_UNIT_SIZE-1)/SSM_MEM_UNIT_SIZE;
	page_size=SSM_MEM_PAGE_SIZE*SSM_MEM_UNIT_SIZE;
	for(size_t seek=0;seek<SSM_MEM_LIST_SIZE;seek=seek+1)
	{
		if((size_t)mem_ptr<(size_t)(ssm_page_list[seek].page_buffer_ptr))
		{
			continue;
		}
		if(seek>=SSM_PAGE_LIST_SIZE)
		{
			page_size=(SSM_MEM_PAGE_SIZE<<SSM_MEG_PAGE_WEIGHT)*SSM_MEM_UNIT_SIZE;
		}
		if((size_t)mem_ptr>((size_t)(ssm_page_list[seek].page_buffer_ptr)+page_size))
		{
			continue;
		}
		page_ptr=ssm_page_list+seek;
		hash_index=((size_t)mem_ptr-(size_t)page_ptr->page_buffer_ptr)%SSM_HASH_KEY;
		block_ptr=(st_ssm_mem_block*)(page_ptr->hash_table_ptr[hash_index]);
		prev_block_ptr=(st_ssm_mem_block*)(page_ptr->page_buffer_ptr);
		while(block_ptr!=NULL)
		{
			if((prev_block_ptr<=mem_ptr)&&(block_ptr>mem_ptr))
			{
				break;
			}
			prev_block_ptr=block_ptr;
			block_ptr=(st_ssm_mem_block*)block_ptr->next_addr;// X32 X64
		}
		if(prev_block_ptr==(st_ssm_mem_block*)(page_ptr->page_buffer_ptr))
		{
			prev_block_ptr=(st_ssm_mem_block*)mem_ptr;
			prev_block_ptr->prev_addr=NULL;
			prev_block_ptr->next_addr=(unsigned long)block_ptr;// X32 X64
			prev_block_ptr->block_size=size;
			page_ptr->available_ptr=prev_block_ptr;
		}
		else
		{
			page_size=prev_block_ptr->block_size*SSM_MEM_UNIT_SIZE;
			if(((size_t)prev_block_ptr+page_size+1)==(size_t)mem_ptr)
			{
				prev_block_ptr->block_size=prev_block_ptr->block_size+size;
			}
			else
			{
				prev_block_ptr->next_addr=(unsigned long)mem_ptr;// X32 X64
				unsigned long addr=prev_block_ptr->prev_addr;
				prev_block_ptr=(st_ssm_mem_block*)mem_ptr;
				prev_block_ptr->prev_addr=addr;
				prev_block_ptr->next_addr=(unsigned long)block_ptr;// X32 X64
				prev_block_ptr->block_size=size;
			}
		}
		if(block_ptr!=NULL)
		{
			if(((size_t)prev_block_ptr+page_size+1)==(size_t)block_ptr)
			{
				prev_block_ptr->block_size=prev_block_ptr->block_size+block_ptr->block_size;
				prev_block_ptr->next_addr=block_ptr->next_addr;
			}
			else
			{
				prev_block_ptr->next_addr=(unsigned long)block_ptr; // X32 X64
				block_ptr->prev_addr=(unsigned long)prev_block_ptr; // X32 X64
			}
		}
		block_ptr=prev_block_ptr;
		size_t begin_index=(size_t)block_ptr-(size_t)(page_ptr->page_buffer_ptr);
		size_t end_index=(size_t)block_ptr-(size_t)(page_ptr->page_buffer_ptr)-1;
		begin_index=begin_index%SSM_HASH_KEY;
		end_index=end_index%SSM_HASH_KEY;
		if(block_ptr->prev_addr!=NULL)
		{
			page_size=((st_ssm_mem_block*)(block_ptr->prev_addr))->block_size*SSM_MEM_UNIT_SIZE;
			if(((block_ptr->prev_addr+page_size)%SSM_HASH_KEY)==begin_index)
			{
				begin_index=begin_index+1;
			}
		}
		for(size_t seek=begin_index;seek<=end_index;seek=seek+1)
		{
			page_ptr->hash_table_ptr[seek]=(void*)block_ptr;
		}
		page_ptr->allocated_mem=page_ptr->allocated_mem-size;
		b_ssm_mem_locked=s_false;
		return nullptr;
	}
	b_ssm_mem_locked=s_false;
	return mem_ptr;
}//*/
void _ssm_print_mem_state()
{
	int seek=0;
	int step=20;
	sfloat memory_usage=0.0f;
	printf("\nSSM Memory Aloorithm Const Variable State:\n");
	printf("\tSSM_MEM_UNIT_SIZE:\t\t%dU\t\t%dByte\n",SSM_MEM_UNIT_SIZE,SSM_MEM_UNIT_SIZE);
	printf("\tSSM_MEM_PAGE_SIZE:\t\t%dU\t%dMByte\n",SSM_MEM_PAGE_SIZE,SSM_MEM_PAGE_SIZE*SSM_MEM_UNIT_SIZE>>20);
	printf("\tSSM_MEG_PAGE_WEIGHT:\t\t%dU\t\t%dMByte\n",SSM_MEG_PAGE_WEIGHT,(SSM_MEM_PAGE_SIZE<<SSM_MEG_PAGE_WEIGHT)*SSM_MEM_UNIT_SIZE>>20);
	printf("\tSSM_PAGE_LIST_SIZE:\t\t%dU\n",SSM_PAGE_LIST_SIZE);
	printf("\tSSM_MEG_PAGE_LIST_SIZE:\t\t%dU\n",SSM_MEG_PAGE_LIST_SIZE);
	printf("\tSSM_MEM_LIST_SIZE:\t\t%dU\n",SSM_MEM_LIST_SIZE);
	printf("\tSSM_ABLOCK_HTABLE_SIZE:\t\t&dU\n",SSM_ABLOCK_HTABLE_SIZE);
	printf("\tSSM_MEG_ABLOCK_HTABLE_SIZE:\t%dU\n",SSM_ABLOCK_HTABLE_SIZE<<SSM_MEG_PAGE_WEIGHT);
	printf("\tSSM_HASH_KEY:\t\t\t%dU\n",SSM_HASH_KEY);
	printf("\nSSM Memory Algorithm State:\n");
	if(b_ssm_pages_inited==s_true)
	{
		printf("\t memory is inited:\ttrue\n");
	}
	else
	{
		printf("\t memory is inited:\tfalse\n");
	}
	if(b_ssm_mem_locked!=s_true)
	{
		printf("\t memory is unlocked:\ttrue\n");
	}
	else
	{
		printf("\t memory is unlocked:\tfalse\n");
	}
	printf("------------------------------\n");
	for(seek=0;seek<SSM_MEM_LIST_SIZE;seek=seek+1)
	{
		printf("\tpage index:\t\t\t%d\n",seek);
		printf("\tpage buffer ptr:\t\t0x%p\n",ssm_page_list[seek].page_buffer_ptr);
		printf("\tpage hash table ptr:\t\t0x%p\n",ssm_page_list[seek].hash_table_ptr);
		printf("\tpage available buffer ptr:\t0x%p\n",ssm_page_list[seek].available_ptr);
		printf("\tpage allocated mem size:\t%d\n",ssm_page_list[seek].allocated_mem);
		printf("\tpage buffer ptr:\t\t0x%p\n",ssm_page_list[seek].page_buffer_ptr);
		if(ssm_page_list[seek].b_is_meg_page==s_true)
		{
			printf("\tis meg page:\t\t\ttrue\n");
			memory_usage=(sfloat)ssm_page_list[seek].allocated_mem/(const sfloat)(SSM_MEM_PAGE_SIZE<<SSM_MEG_PAGE_WEIGHT);
		}
		else
		{
			printf("\tis meg page:\t\t\tfalse\n");
			memory_usage=(sfloat)ssm_page_list[seek].allocated_mem/(const sfloat)SSM_MEM_PAGE_SIZE;
		}
		printf("\tpage usage map:\t\t\t%2.2f%%\n",memory_usage*100);
		printf("\t------------------------------\n");
	}
}