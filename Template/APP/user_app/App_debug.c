/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  08/01/2013
  * @brief  用户Debug接口
  */ 
#include <includes.h>



#define DEBUG_OUTPUT/**<使用DEBUG调试*/

#ifdef DEBUG_OUTPUT

//#define DEBUG_PRINT_IN_TERMINAL
#define DEBUG_PRINT_IN_UART/* output debug information in IAR terminal window */



#define usart_memory_blk_number  10 /*!<内存块大小*/
/**
  * @brief 串口发送缓冲区的长度
 */
#define buffer_size 200
/**
  * @brief 串口缓冲区的结构体
 */
typedef struct 
{
    /** @brief 要发送的缓冲区 */
    char buffer[buffer_size];
    /** @brief 要发送的数据长度 */
    int size;
}usart_memory_struct;
/**
  * @brief 申请用于存放串口内存块的内存
 */
static usart_memory_struct USART_MEMORY[usart_memory_blk_number];
/**
  * @brief 串口内存块大小
 */
#define usart_memory_blk_size  ((sizeof(usart_memory_struct)+4)*usart_memory_blk_number)

/**
  * @brief 串口发送队列长度
 */
#define queue_number usart_memory_blk_number




static uint8_t debug_output_flag = 0;

static uint32_t Printf_Queue_Index = 0;//队列号

static void * Task_Debug(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4);
/**
  * @brief 创建串口调试任务
  * @retval 无
 */
uint32_t create_debug_task(void)
{
 
    UINT32 uwRet = 0;
    UINT32 Task_Debug_Index = 0;
    TSK_INIT_PARAM_S stInitParam1;

    /*创建任务:Task_Debug*/
    (VOID)memset((void *)(&stInitParam1), 0, sizeof(TSK_INIT_PARAM_S));
    stInitParam1.pfnTaskEntry = Task_Debug;
    stInitParam1.pcName = "Task_Debug";
    stInitParam1.usTaskPrio = Prio_Task_Debug;
    stInitParam1.uwStackSize =StackSize_Task_Debug;
  
    LOS_TaskLock();//锁住任务，防止新创建的任务比本任务高而发生调度
    uwRet = LOS_TaskCreate(&Task_Debug_Index, &stInitParam1);
    if(uwRet != LOS_OK)
    {
        debug_print_string("create Task_Debug failed!,error:%x\n",uwRet);
        return uwRet;
    }
    
    LOS_TaskUnlock();//解锁任务，只有队列创建后才开始任务调度
    
    return LOS_OK;
}

/**
  * @brief 串口发送服务程序
  * @details 从队列中取出数据，并使用DMA进行发送。然后等待发送完成。
  * @param  pdata 无用
  * @retval 无
 */
static void * Task_Debug(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4)
{
    
    UINT32 uwRet;

    /*创建队列  队列名称|队列大小|队列检索号||0（保留）|每块node 的大小*/
    uwRet = LOS_QueueCreate("p_que", queue_number, &Printf_Queue_Index, 1, 1);
    if(uwRet == LOS_OK)
    {
        debug_print_string("create the Task_Debug queue success!\r\n");
        //return ;
    }
    assert_param(uwRet == LOS_OK);
   /*创建内存管理：内存池地址首地址|总共内存块的大小|总共多少内存块*/ 
    uwRet = LOS_MemboxInit( &USART_MEMORY[0], usart_memory_blk_size, usart_memory_blk_number);
    if(uwRet != LOS_OK)
    {
        debug_print_string("Task_Debug Mem box init failed\r\n");
        //return LOS_NOK;
    }
    else
    {
        debug_print_string("Task_Debug Mem box init ok!\n");
    }  
    debug_output_flag = 1;
    
    debug_printf("[%s] enter.\r\n", __func__);
   
    for(;;)
    {
        usart_memory_struct *p=NULL; 

        if(*get_uart_rx_len() )
        {
          usmart_scan(get_uart_rx_buffer(), *get_uart_rx_len() );	//执行usmart扫描                            
          reset_uart_rx_len();
        }/*读取队列里的数据存入uwReadbuf里*/
        
        uwRet = LOS_QueueRead(Printf_Queue_Index, &p, 1, 10);
        if(uwRet != LOS_OK)
        {
          OSTimeDly (50);
          continue;               
        }
        if(!p)
            continue;
            
#ifdef DEBUG_PRINT_IN_TERMINAL
        printf("%s", p->buffer);
#endif
        
#ifdef DEBUG_PRINT_IN_ZIGBEE
        zigbee_send_data((uint8_t*)p->buffer, p->size);
#endif
        
#ifdef DEBUG_PRINT_IN_UART
          DEBUG_USART_Handle_Transmit((const uint8_t*)p->buffer, p->size);
#endif
     
        LOS_MemboxFree(USART_MEMORY, p);             
      
    }
}



/**
  * @brief 打印数据接口
  * @details 可变参数
  * @param  pstring 字符参数
  * @retval 无
 */
//void debug_printf(const char *pstring, ...)
//{
//   usart_memory_struct *p;
//   uint32_t uwRet=0;
//    va_list args;
//    
//    if(!debug_output_flag)
//        return;    
//    
//    p = (usart_memory_struct *)LOS_MemboxAlloc(USART_MEMORY);
//    if(!p) 
//        return;
//    va_start(args, pstring);
//    p->size = vsnprintf(p->buffer, usart_memory_blk_size, pstring, args);
//    va_end(args);
//    
//    assert_param(p->size < usart_memory_blk_size);
//
//    if(!p->size)  /*size 为 0 释放内存 */
//    {
//      LOS_MemboxFree(USART_MEMORY, p);  
//  
//        return;
//    }
//    if(p->size < 20)
//    {
//       DEBUG_USART_Handle_Transmit((uint8_t *)p->buffer, p->size);
//       LOS_MemboxFree(USART_MEMORY, p);  
//        return;
//    }
//     uwRet = LOS_QueueWrite(Printf_Queue_Index, p, 1, 0);
//     assert_param(uwRet==LOS_OK);
//     uwRet++;
//    return;
//    
//
//}
void debug_printf(const char *pstring, ...)
{
  usart_memory_struct temp;
  
   usart_memory_struct *p=&temp;
   
   va_list args;
    
    if(!debug_output_flag)
        return;    
    va_start(args, pstring);
    p->size = vsnprintf(p->buffer, usart_memory_blk_size, pstring, args);
    va_end(args);
    
    assert_param(p->size < usart_memory_blk_size);

    if(!p->size)  /*size 为 0 释放内存 */
    {  
        return;
    }
    DEBUG_USART_Handle_Transmit((uint8_t *)p->buffer, p->size);  
    return;
 
    

}
/**
  * @brief hex_to_string
  * @details 
  * @param   
  * @retval 无
 */
static uint8_t hex_to_string(uint8_t hex, uint8_t *pbuffer)
{
    uint8_t temp;
    
    temp = (hex & 0xf0) >> 4;
    if(temp <= 9)
        *pbuffer++ = temp + '0';
    else
        *pbuffer++ = temp - 10 + 'A';
   
    temp = (hex & 0x0f) >> 0;
    if(temp <= 9)
        *pbuffer++ = temp + '0';
    else
        *pbuffer++ = temp  - 10 + 'A';
   
    *pbuffer++ = ' ';
    
     return 3;
}
/**
  * @brief 打印buffer,将HEX转为char打印出来
  * @details 
  * @param  buffer 打印数据的内存起始地址
  * @param  size  打印数据大小（in byte）
  * @retval 无
 */
void debug_print_buffer(const uint8_t *buffer, uint8_t size)
{
    usart_memory_struct *p;
  
    int i;

    if(!debug_output_flag || (!size))
        return;
    
    
    p = LOS_MemboxAlloc(USART_MEMORY);
    if(!p) 
        return;
    
    p->size = 0;
    for(i = 0; i < size; ++i)
    {
        if(buffer_size - p->size < 6)
        {
            /* not enough memory to fill the buffer, fix the EOF */
            p->buffer[p->size] = '\0';
            p->buffer[p->size - 1] = '\n';
            p->buffer[p->size - 2] = '\r';
            p->buffer[p->size - 3] = '.';
            p->buffer[p->size - 4] = '.';
            p->buffer[p->size - 5] = '.';
            break;  
        }
        p->size += hex_to_string(buffer[i], (uint8_t *)&p->buffer[p->size]);
    }
    /* all buffer filled to memory */
    if(i == size)
    {
        p->buffer[p->size++] = '\r';
        p->buffer[p->size++] = '\n';
        p->buffer[p->size++] = '\0';
    }
    if(!p->size)
    {
       LOS_MemboxFree(USART_MEMORY, p);
        return;
    }  
    LOS_QueueWrite(Printf_Queue_Index, p, 1, 0);
}


/**
  * @brief 向串口发送队列压入数据。
  * @details 数据存放在发送队列中。但是当发送队列满了，则会把数据放弃。并返回0
  * @param  pbuffer 指向要发送的缓冲区
  * @param  size 数据长度
  * @retval 数据成功压入发送队列则返回1，压入失败则返回0
 */
uint8_t debug_transmit_buffer(const char *pbuffer, int size)
{
    usart_memory_struct *p;
  
    if((size > buffer_size) || (!size))
        return 1;
    p = LOS_MemboxAlloc(USART_MEMORY);
    if(!p) 
        return 1;
    memcpy(p->buffer, pbuffer, size);
    p->size = size;
    
     LOS_QueueWrite(Printf_Queue_Index, p, 1, 0);
    
    return 0;
}

void debug_output_control(uint8_t en)
{
    debug_output_flag = en;
}


/**
  * @brief 向串口打印数据,硬打印
  * @details 可变参数。
  * @param  pstring 字符参数
  * @retval 无
 */
void debug_print_string(const char *pstring, ...)
{
#define tx_buffer_size 200
    int size;
  
    char tx_buffer[tx_buffer_size];
    
    va_list args;
    va_start(args, pstring);
    size = vsnprintf(tx_buffer, tx_buffer_size, pstring, args);
    va_end(args);
    
    assert_param(size < tx_buffer_size);
    /*和硬件相关*/
    DEBUG_USART_Handle_Transmit((uint8_t *)tx_buffer, size);

}


#else /* DEBUG_OUTPUT */


void create_debug_task(void){}
void debug_print_buffer(const uint8_t *buffer, uint8_t size){}
void debug_printf(const char *pstring, ...){}
#endif /* DEBUG_OUTPUT */


