#include"iostream"
#ifndef __CSTDSANMEMORYMANAGERDEF_H__
#define __CSTDSANMEMORYMANAGERDEF_H__

#ifndef X32
#define X32
#endif

enum eSBOOL
{
	s_false	= 0,
	s_true	= 1,
};

enum eSANMEMORYALGORITHMTYPE
{
	SMAT_STD	= 0x00,
	SMAT_SSM	= 0x01,
};

static const unsigned long SSM_MEM_UNIT_SIZE =	16U;		// Size in byte, 128 byte , 4 -32 sfloat type, min value: 16U [0] block size [1] priv offset [2] next offset [0] 32/64bit [1][2] 32-bit, each page max contain 4G unit

static const unsigned long SSM_MEM_PAGE_SIZE =		8192U*64U*8U;	// 4096 Unit, 64M
static const unsigned long SSM_MEG_PAGE_WEIGHT =	2U;				// meg page weight, meg page size: page size << weight, 256M

static const unsigned long SSM_PAGE_LIST_SIZE =		8U;
static const unsigned long SSM_MEG_PAGE_LIST_SIZE =	2U;
static const unsigned long SSM_MEM_LIST_SIZE =		10U;

static const unsigned long SSM_ABLOCK_HTABLE_SIZE =	512U;	// each table node contain 128K memory address, meg page hash table: hash table << weight
static const unsigned long SSM_HASH_KEY =			9U;		// 0xaddress >>hash key

struct st_ssm_mem_page
{
	void*	page_buffer_ptr;
	void**	hash_table_ptr;
	void*	available_ptr;
	unsigned long	allocated_mem;
	enum eSBOOL	b_is_meg_page;
};

#pragma pack(1)
struct st_ssm_mem_block //don't have real entrty, using for template block access as a pointer
{
	unsigned long	block_size;
	unsigned long	prev_addr;// X32 add equal 32 bit address, X64 add + base address (page upper 32 bit address) equal 64 bit address
	unsigned long	next_addr;
};
#pragma pack(0)

// Simple San Memory Management Algorithm

int _ssm_init();
void _ssm_release();
void _ssm_clear();
void* _ssm_new(size_t mem_size,void* buffer=NULL,size_t buffer_size=0,int page=-1);
void* _ssm_delete(void* mem_ptr,size_t mem_size);
void _ssm_print_mem_state();

#endif