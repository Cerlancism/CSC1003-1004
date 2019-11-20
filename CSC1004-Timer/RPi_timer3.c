#include "RPi_timer3.h"
#include "stdio.h"

struct bcm2835_peripheral timer0 = {TIMER_BASE};

int map_peripheral(struct bcm2835_peripheral *p)
{
	if((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC))<0)
	{
		printf("Failed to open /dev/mem, try checking permission.\n");
		return -1;
	}

	p->map = mmap(
	NULL,
	BLOCK_SIZE,
	PROT_READ|PROT_WRITE,
	MAP_SHARED,
	p->mem_fd,
	p->addr_p
	);

	if(p->map == MAP_FAILED)
	{
		perror("mmap");
		return -1;
	}

	p->addr = (volatile unsigned int *)p->map;
	return 0;
}

void unmap_peripheral(struct bcm2835_peripheral *p)
{
	munmap(p->map, BLOCK_SIZE);
	close(p->mem_fd);
}

void Enable_Interrupts(void)
{
	asm(
		"mrs	r0, cpsr	\n\t"
		"bic	r0, r0, #0x80	\n\t"
		"msr	cpsr_c, r0"
	);
}
int main()
{
	if(map_peripheral(&timer0)== -1)
	{
		printf("Failed to map the physical timer0 registers into the virtual memory space.\n");
		return -1;
	}
	printf("\n step1\n");
	printf("\n TIMER_LOAD0 = : \n%X", TIMER_LOAD0);
	printf("\n TIMER_CONTROL0 = : \n%X", TIMER_CONTROL0);
	printf("\n TIMER_IRQCLEAR0 = : \n%X", TIMER_IRQCLEAR0);
	printf("\n TIMER_MASK_IRQ0 = : \n%X", TIMER_MASK_IRQ0);
	printf("\n TIMER_PRE_DIVD0 = : \n%X", TIMER_PRE_DIVD0);
	printf("\n TIMER_RELOAD = : \n%X", TIMER_RELOAD);
	int counter = 0;
	TIMER_LOAD0 = 0x0FFF;
	TIMER_RELOAD = 0x0FFF;
	printf("\n Write value into Timer Load register, TIMER_LOAD0 = : \n%X", TIMER_LOAD0);
	TIMER_PRE_DIVD0 = 0x3F0;
	printf("\n Write a pre_divider value into TIMER_PRE_DIVD0 register, TIMER_PRE_DIVD0 = : \n%X", TIMER_PRE_DIVD0);
	TIMER_CONTROL0 |= ((2<<2)& 0x0000000C);
	printf("\n Write Bit 2-3 of Timer Control Register (pre-scale clock/256), TIMER_CONTROL0 = : \n%X", TIMER_CONTROL0);
	TIMER_CONTROL0 |= ((1<<1)&0x00000002);
	printf("\n Write Bit 1 of Timer Control Register (choose 23-bit counter), TIMER_CONTROL0 = :\n%X", TIMER_CONTROL0);
	TIMER_CONTROL0 |= ((1<<7) & 0x00000080);
	printf("\n Write Bit 7 of Timer Control Register (enable timer), TIMER_CONTROL0 = : \n%X", TIMER_CONTROL0);
	TIMER_CONTROL0 |= ((1<<5) & 0x00000020);
	printf("\n Write Bit 5 of Timer Control Register (enable timer interrupt), TIMER_CONTROL0 = : \n%X", TIMER_CONTROL0);
	Enable_Interrupts();
	printf("\n step 3\n");

	while(1)
	{
		if(TIMER_MASK_IRQ0==1)
		{
			if(!counter)
				counter++;
			else
				printf("Interrupt happened, count: %i \n",counter++);
			TIMER_IRQCLEAR0 = 0;
		}
	}
	unmap_peripheral(&timer0);
	return 0;
}

