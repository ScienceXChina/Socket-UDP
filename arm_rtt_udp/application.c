/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup. 
 */

#include <board.h>
#include <rtthread.h>
#include "led.h"

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#include <netif/ethernetif.h>
#include "stm32f4xx_eth.h"
#endif

#ifdef RT_USING_GDB
#include <gdb_stub.h>
#endif

void rt_init_thread_entry(void* parameter)
{
    /* GDB STUB */
#ifdef RT_USING_GDB
    gdb_set_device("uart6");
    gdb_start();
#endif

    /* LwIP Initialization */
#ifdef RT_USING_LWIP
    {
        extern void lwip_sys_init(void);
			  extern void udpclient(const char* url, int port, int count);
        //extern void netio_init(void);
			
        /* register ethernetif device */
        eth_system_device_init();

        rt_hw_stm32_eth_init();

        /* init lwip system */
        lwip_sys_init();			
        rt_kprintf("TCP/IP initialized!\n");
			
			  rt_thread_delay(300);
			  rt_kprintf("Running RT-thread now.\n");
        //netio_init();
			  udpclient("192.168.1.101", 6666, 100000);
					
    }
#endif

}

ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t led_stack[512];
static struct rt_thread led_thread;
static void rt_thread_entry_led(void* parameter)
{
	rt_hw_led_init();
	
	while(1)
	{
	    rt_hw_led_on(12);
	    rt_thread_delay(RT_TICK_PER_SECOND);
			rt_hw_led_off(12);
		  rt_thread_delay(RT_TICK_PER_SECOND);
	    //rt_kprintf("LED->12 LED thread\r\n");
		
	    rt_hw_led_on(14);
	    rt_thread_delay(RT_TICK_PER_SECOND);
			rt_hw_led_off(14);
		  rt_thread_delay(RT_TICK_PER_SECOND);
	    //rt_kprintf("LED->14 LED thread\r\n");
	}
}

//ALIGN(RT_ALIGN_SIZE)
//static rt_uint8_t udp_client_stack[4096];
//static struct rt_thread udp_client_thread;
//static void rt_thread_entry_udp_client(void* parameter)
//{
//	extern void udpclient(const char* url, int port, int count);
//	const char *server_ip = "192.168.1.106";
//	int server_port = 6666;
//	
//  //rt_kprintf("server_ip = %s \n", server_ip);

//	while(1)
//	{
//		//udpclient(server_ip, server_port, 100);
//		udpclient("192.168.1.106", 6666, 10);
//	}
//}

int rt_application_init()
{
    rt_thread_t tid;

    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        8192, RT_THREAD_PRIORITY_MAX/3, 20);
	
    if (tid != RT_NULL)
        rt_thread_startup(tid);

	  //------- init led1 thread
    rt_thread_init(&led_thread,
                   "led",
                   rt_thread_entry_led,
                   RT_NULL,
                   &led_stack[0],
                   sizeof(led_stack),20,10);
    rt_thread_startup(&led_thread);	
		
//		//------- init UDP Client
//		rt_thread_init(&udp_client_thread,
//									 "UDP_Client",
//		               rt_thread_entry_udp_client,
//		               RT_NULL,
//		               &udp_client_stack[0],
//		               sizeof(udp_client_stack),20,100);
//		rt_thread_startup(&udp_client_thread);
		
    return 0;
}

/*@}*/
