#include <core/physical_allocator.hpp>
#include <io/vesa.hpp>
namespace eospp::memory{


void init(){
    int i,j;
	double TotalMem = 0 ;
	struct Memory_E820_Formate *p = nullptr;	
	
	io::printf("Display Physics Address MAP,Type(1:RAM,2:ROM or Reserved,3:ACPI Reclaim Memory,4:ACPI NVS Memory,Others:Undefine)\n");
	p = (struct Memory_E820_Formate *)0xffff800000007e00;

	for(i = 0;i < 16;i++)
	{
        if(p->type > 4)
          break;
		io::printf("Address:%ud,%ud\tLength:%ud,%ud\tType:%ud\n",p->address2,p->address1,p->length2,p->length1,p->type);
		unsigned long tmp = 0;
		if(p->type == 1)
		{
			tmp = p->length2;
			TotalMem +=  p->length1;
			TotalMem +=  tmp  << 32;
		}
		p++;

			
	}
    double memory_usage = TotalMem / 1024 / 1024;
    io::printf("TotalMem = %.5f MB\n",memory_usage);
}

unsigned long page_init(struct Page *page, unsigned long flags)
{
	if (!page->attribute)
	{
		*(mm_struct.bits_map + ((page->PHY_address >> PAGE_2M_SHIFT) >> 6)) |= 1UL << (page->PHY_address >> PAGE_2M_SHIFT) % 64;
		page->attribute = flags;
		page->reference_count++;
		page->zone_struct->page_using_count++;
		page->zone_struct->page_free_count--;
		page->zone_struct->total_pages_link++;
	}
	else if ((page->attribute & PG_Referenced) || (page->attribute & PG_K_Share_To_U) || (flags & PG_Referenced) || (flags & PG_K_Share_To_U))
	{
		page->attribute |= flags;
		page->reference_count++;
		page->zone_struct->total_pages_link++;
	}
	else
	{
		*(mm_struct.bits_map + ((page->PHY_address >> PAGE_2M_SHIFT) >> 6)) |= 1UL << (page->PHY_address >> PAGE_2M_SHIFT) % 64;
		page->attribute |= flags;
	}
	return 0;
}

unsigned long page_clean(struct Page *page)
{
	if (!page->attribute)
	{
		page->attribute = 0;
	}
	else if ((page->attribute & PG_Referenced) || (page->attribute & PG_K_Share_To_U))
	{
		page->reference_count--;
		page->zone_struct->total_pages_link--;
		if (!page->reference_count)
		{
			page->attribute = 0;
			page->zone_struct->page_using_count--;
			page->zone_struct->page_free_count++;
		}
	}
	else
	{
		*(mm_struct.bits_map + ((page->PHY_address >> PAGE_2M_SHIFT) >> 6)) &= ~(1UL << (page->PHY_address >> PAGE_2M_SHIFT) % 64);

		page->attribute = 0;
		page->reference_count = 0;
		page->zone_struct->page_using_count--;
		page->zone_struct->page_free_count++;
		page->zone_struct->total_pages_link--;
	}
	return 0;
}

void init_memory()
{
	int i, j;
	unsigned long TotalMem = 0;
	struct E820 *p = nullptr;

	io::printf("isplay Physics Address MAP,Type(1:RAM,2:ROM or Reserved,3:ACPI Reclaim Memory,4:ACPI NVS Memory,Others:Undefine)\n");
	p = (struct E820 *)0xffff800000007e00;

	for (i = 0; i < 32; i++)
	{
		if (p->type > 4 || p->length == 0 || p->type < 1)
			break;

		if (p->type == 1)
			TotalMem += p->length;

		mm_struct.e820[i].address += p->address;

		mm_struct.e820[i].length += p->length;

		mm_struct.e820[i].type = p->type;

		mm_struct.e820_length = i;

		p++;
		
	}

	io::printf("OS Can Used Total RAM:%ul\n", TotalMem >> 20);


	
	io::printf("kernel Can Used Total 2M PAGEs:%ul\n", TotalMem );

	//bits map construction init
	//位图地址以4kb对齐
	io::printf("init bit map\n");
	mm_struct.bits_map = (unsigned long *)((mm_struct.end_brk + PAGE_4K_SIZE - 1) & PAGE_4K_MASK);
	//位图需要映射的物理页的位数
	mm_struct.bits_size = TotalMem >> PAGE_2M_SHIFT;
	//bitmap所占字节数
	mm_struct.bits_length = (mm_struct.bits_size + sizeof(long) * 8 - 1) / 8;

	io::printf("bits_size = %ul bits_length = %ul\n",mm_struct.bits_size,mm_struct.bits_length);
	//全都初始化为1
	memset(mm_struct.bits_map, 0xff, mm_struct.bits_length); //init bits map memory

	//pages construction init
	io::printf("init page\n");

	//bitmap后存放page
	mm_struct.pages_struct = (struct Page *)(((unsigned long)mm_struct.bits_map + mm_struct.bits_length + PAGE_4K_SIZE - 1) & PAGE_4K_MASK);
	//总页数
	mm_struct.pages_size = TotalMem >> PAGE_2M_SHIFT;
	//page所占字节数
	mm_struct.pages_length = ((TotalMem >> PAGE_2M_SHIFT) * sizeof(struct Page) + sizeof(long) - 1) & (~(sizeof(long) - 1));

	//memset(mm_struct.pages_struct, 0x00, mm_struct.pages_length); //init pages memory

	//zones construction init
	//page后存放zone
	io::printf("init zone\n");
	mm_struct.zones_struct = (struct Zone *)(((unsigned long)mm_struct.pages_struct + mm_struct.pages_length + PAGE_4K_SIZE - 1) & PAGE_4K_MASK);
	//
	mm_struct.zones_size = 0;
	//zone所需要的字节数
	mm_struct.zones_length = (5 * sizeof(struct Zone) + sizeof(long) - 1) & (~(sizeof(long) - 1));

	memset(mm_struct.zones_struct, 0x00, mm_struct.zones_length); //init zones memory

	for (i = 0; i <= mm_struct.e820_length; i++)
	{
		unsigned long start, end;
		struct Zone *z;
		struct Page *p;
		unsigned long *b;

		if (mm_struct.e820[i].type != 1)
			continue;
		start = PAGE_2M_ALIGN(mm_struct.e820[i].address);
		end = ((mm_struct.e820[i].address + mm_struct.e820[i].length) >> PAGE_2M_SHIFT) << PAGE_2M_SHIFT;
		if (end <= start)
			continue;

		//zone init

		z = mm_struct.zones_struct + mm_struct.zones_size;
		mm_struct.zones_size++;

		z->zone_start_address = start;
		z->zone_end_address = end;
		z->zone_length = end - start;

		z->page_using_count = 0;
		z->page_free_count = (end - start) >> PAGE_2M_SHIFT;

		z->total_pages_link = 0;

		z->attribute = 0;
		z->GMD_struct = &mm_struct;

		z->pages_length = (end - start) >> PAGE_2M_SHIFT;
		z->pages_group = (struct Page *)(mm_struct.pages_struct + (start >> PAGE_2M_SHIFT));

		//page init
		p = z->pages_group;
		for (j = 0; j < z->pages_length; j++, p++)
		{
			p->zone_struct = z;
			p->PHY_address = start + PAGE_2M_SIZE * j;
			p->attribute = 0;

			p->reference_count = 0;

			p->age = 0;

			*(mm_struct.bits_map + ((p->PHY_address >> PAGE_2M_SHIFT) >> 6)) ^= 1UL << (p->PHY_address >> PAGE_2M_SHIFT) % 64;
		}
	}

	/////////////init address 0 to page struct 0; because the mm_struct.e820[0].type != 1

	mm_struct.pages_struct->zone_struct = mm_struct.zones_struct;

	mm_struct.pages_struct->PHY_address = 0UL;
	mm_struct.pages_struct->attribute = 0;
	mm_struct.pages_struct->reference_count = 0;
	mm_struct.pages_struct->age = 0;

	/////////////

	mm_struct.zones_length = (mm_struct.zones_size * sizeof(struct Zone) + sizeof(long) - 1) & (~(sizeof(long) - 1));

	io::printf("bits_map : %xl,bits_size : %xl,bits_length : %xl\n", mm_struct.bits_map, mm_struct.bits_size, mm_struct.bits_length);

	io::printf("pages_struct : %xl,pages_size : %xl,pages_length : %xl\n", mm_struct.pages_struct, mm_struct.pages_size, mm_struct.pages_length);

	io::printf("zones_struct : %xl,zones_size : %xl,zones_length : %xl\n", mm_struct.zones_struct, mm_struct.zones_size, mm_struct.zones_length);

	ZONE_DMA_INDEX = 0;	   //need rewrite in the future
	ZONE_NORMAL_INDEX = 0; //need rewrite in the future

	for (i = 0; i < mm_struct.zones_size; i++) //need rewrite in the future
	{
		struct Zone *z = mm_struct.zones_struct + i;
		io::printf("zone_start_address : %xl,zone_end_address : %xl,zone_length : %ul,pages_group : %xl,pages_length : %ul\n", z->zone_start_address, z->zone_end_address, z->zone_length, z->pages_group, z->pages_length);

		if (z->zone_start_address == 0x100000000)
			ZONE_UNMAPED_INDEX = i;
	}

	mm_struct.end_of_struct = (unsigned long)((unsigned long)mm_struct.zones_struct + mm_struct.zones_length + sizeof(long) * 32) & (~(sizeof(long) - 1)); ////need a blank to separate mm_struct

	io::printf("start_code : %xl,end_code : %xl,end_data : %xl,end_brk : %xl,end_of_struct : %xl\n",
			     mm_struct.start_code, mm_struct.end_code, mm_struct.end_data, mm_struct.end_brk, mm_struct.end_of_struct);

	i = Virt_To_Phy(mm_struct.end_of_struct) >> PAGE_2M_SHIFT;

	for (j = 0; j <= i; j++)
	{
		page_init(mm_struct.pages_struct + j, PG_PTable_Maped | PG_Kernel_Init | PG_Active | PG_Kernel);
	}

	Global_CR3 = Get_gdt();

	io::printf("Global_CR3:\t%xl\n", Global_CR3);
	io::printf("*Global_CR3:\t%xl\n", *Phy_To_Virt(Global_CR3) & (~0xff));
	io::printf("**Global_CR3:\t%xl\n", *Phy_To_Virt(*Phy_To_Virt(Global_CR3) & (~0xff)) & (~0xff));

	for (i = 0; i < 10; i++)
		*(Phy_To_Virt(Global_CR3) + i) = 0UL;

	flush_tlb();
}

/*

	number: number < 64

	zone_select: zone select from dma , mapped in  pagetable , unmapped in pagetable

	page_flags: struct Page flages

*/

struct Page *alloc_pages(int zone_select, int number, unsigned long page_flags)
{
	int i;
	unsigned long page = 0;

	int zone_start = 0;
	int zone_end = 0;

	switch (zone_select)
	{
	case ZONE_DMA:
		zone_start = 0;
		zone_end = ZONE_DMA_INDEX;

		break;

	case ZONE_NORMAL:
		zone_start = ZONE_DMA_INDEX;
		zone_end = ZONE_NORMAL_INDEX;

		break;

	case ZONE_UNMAPED:
		zone_start = ZONE_UNMAPED_INDEX;
		zone_end = mm_struct.zones_size - 1;

		break;

	default:
		io::printf("alloc_pages error zone_select index\n");
		return nullptr;
		break;
	}

	for (i = zone_start; i <= zone_end; i++)
	{
		struct Zone *z;
		unsigned long j;
		unsigned long start, end, length;
		unsigned long tmp;

		if ((mm_struct.zones_struct + i)->page_free_count < number)
			continue;

		z = mm_struct.zones_struct + i;
		start = z->zone_start_address >> PAGE_2M_SHIFT;
		end = z->zone_end_address >> PAGE_2M_SHIFT;
		length = z->zone_length >> PAGE_2M_SHIFT;

		tmp = 64 - start % 64;

		for (j = start; j <= end; j += j % 64 ? tmp : 64)
		{
			unsigned long *p = mm_struct.bits_map + (j >> 6);
			unsigned long shift = j % 64;
			unsigned long k;
			for (k = shift; k < 64 - shift; k++)
			{
				if (!(((*p >> k) | (*(p + 1) << (64 - k))) & (number == 64 ? 0xffffffffffffffffUL : ((1UL << number) - 1))))
				{
					unsigned long l;
					page = j + k - 1;
					for (l = 0; l < number; l++)
					{
						struct Page *x = mm_struct.pages_struct + page + l;
						page_init(x, page_flags);
					}
					goto find_free_pages;
				}
			}
		}
	}

	return nullptr;

find_free_pages:

	return (struct Page *)(mm_struct.pages_struct + page);
}



}