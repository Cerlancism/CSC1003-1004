#ifndef _INC_PJ_GPIO_H
#define _INC_PJ_GPIO_H

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "dirent.h"
#include "fcntl.h"
#include "assert.h"
#include "sched.h"
#include "sys/mman.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "unistd.h"

#define RPI3

#ifdef RPI3
#define BCM2708_PERI_BASE	0x3F000000
#define TIMER_BASE	(BCM2708_PERI_BASE	+ 0xB000)
#endif

#define BLOCK_SIZE	(4*1024)

struct bcm2835_peripheral
{
	unsigned long addr_p;
	int mem_fd;
	void * map;
	volatile unsigned int * addr;
};

extern struct bcm2835_peripheral timer0;

#define TIMER_LOAD0		*(timer0.addr	+	0x100)
#define	TIMER_CONTROL0		*(timer0.addr	+	0x102)
#define	TIMER_IRQCLEAR0		*(timer0.addr	+	0x103)
#define	TIMER_MASK_IRQ0		*(timer0.addr	+	0x105)
#define	TIMER_PRE_DIVD0		*(timer0.addr	+	0x107)
#define TIMER_RELOAD		*(timer0.addr 	+	0x106) /* RELOAD REGISTER */

#endif
