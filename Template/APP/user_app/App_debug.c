/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  08/01/2013
  * @brief  �û�Debug�ӿ�
  */ 
#include <includes.h>



#define DEBUG_OUTPUT/**<ʹ��DEBUG����*/

#ifdef DEBUG_OUTPUT

//#define DEBUG_PRINT_IN_TERMINAL
#define DEBUG_PRINT_IN_UART/* output debug information in IAR terminal window */



#define usart_memory_blk_number  10 /*!<�ڴ���С*/
/**
  * @brief ���ڷ��ͻ������ĳ���
 */
#define buffer_size 200
/**
  * @brief ���ڻ������Ľṹ��
 */
typedef struct 
{
    /** @brief Ҫ���͵Ļ����� */
    char buffer[buffer_size];
    /** @brief Ҫ���͵����ݳ��� */
    int size;
}usart_memory_struct;
/**
  * @brief �������ڴ�Ŵ����ڴ����ڴ�
 */
static usart_memory_struct USART_MEMORY[usart_memory_blk_number];
/**
  * @brief �����ڴ���С
 */
#define usart_memory_blk_size  ((sizeof(usart_memory_struct)+4)*usart_memory_blk_number)

/**
  * @brief ���ڷ��Ͷ��г���
 */
#define queue_number usart_memory_blk_number




static uint8_t debug_output_flag = 0;

static uint32_t Printf_Queue_Index = 0;//���к�

static void * Task_Debug(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4);
/**
  * @brief �������ڵ�������
  * @retval ��
 */
uint32_t create_debug_task(void)
{
 
    UINT32 uwRet = 0;
    UINT32 Task_Debug_Index = 0;
    TSK_INIT_PARAM_S stInitParam1;

    /*��������:Task_Debug*/
    (VOID)memset((void *)(&stInitParam1), 0, sizeof(TSK_INIT_PARAM_S));
    stInitParam1.pfnTaskEntry = Task_Debug;
    stInitParam1.pcName = "Task_Debug";
    stInitParam1.usTaskPrio = Prio_Task_Debug;
    stInitParam1.uwStackSize =StackSize_Task_Debug;
  
    LOS_TaskLock();//��ס���񣬷�ֹ�´���������ȱ�����߶���������
    uwRet = LOS_TaskCreate(&Task_Debug_Index, &stInitParam1);
    if(uwRet != LOS_OK)
    {
        debug_print_string("create Task_Debug failed!,error:%x\n",uwRet);
        return uwRet;
    }
    
    LOS_TaskUnlock();//��������ֻ�ж��д�����ſ�ʼ�������
    
    return LOS_OK;
}

/**
  * @brief ���ڷ��ͷ������
  * @details �Ӷ�����ȡ�����ݣ���ʹ��DMA���з��͡�Ȼ��ȴ�������ɡ�
  * @param  pdata ����
  * @retval ��
 */
static void * Task_Debug(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4)
{
    
    UINT32 uwRet;

    /*��������  ��������|���д�С|���м�����||0��������|ÿ��node �Ĵ�С*/
    uwRet = LOS_QueueCreate("p_que", queue_number, &Printf_Queue_Index, 1, 1);
    if(uwRet == LOS_OK)
    {
        debug_print_string("create the Task_Debug queue success!\r\n");
        //return ;
    }
    assert_param(uwRet == LOS_OK);
   /*�����ڴ�����ڴ�ص�ַ�׵�ַ|�ܹ��ڴ��Ĵ�С|�ܹ������ڴ��*/ 
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
          usmart_scan(get_uart_rx_buffer(), *get_uart_rx_len() );	//ִ��usmartɨ��                            
          reset_uart_rx_len();
        }/*��ȡ����������ݴ���uwReadbuf��*/
        
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
  * @brief ��ӡ���ݽӿ�
  * @details �ɱ����
  * @param  pstring �ַ�����
  * @retval ��
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
//    if(!p->size)  /*size Ϊ 0 �ͷ��ڴ� */
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

    if(!p->size)  /*size Ϊ 0 �ͷ��ڴ� */
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
  * @retval ��
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
  * @brief ��ӡbuffer,��HEXתΪchar��ӡ����
  * @details 
  * @param  buffer ��ӡ���ݵ��ڴ���ʼ��ַ
  * @param  size  ��ӡ���ݴ�С��in byte��
  * @retval ��
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
  * @brief �򴮿ڷ��Ͷ���ѹ�����ݡ�
  * @details ���ݴ���ڷ��Ͷ����С����ǵ����Ͷ������ˣ��������ݷ�����������0
  * @param  pbuffer ָ��Ҫ���͵Ļ�����
  * @param  size ���ݳ���
  * @retval ���ݳɹ�ѹ�뷢�Ͷ����򷵻�1��ѹ��ʧ���򷵻�0
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
  * @brief �򴮿ڴ�ӡ����,Ӳ��ӡ
  * @details �ɱ������
  * @param  pstring �ַ�����
  * @retval ��
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
    /*��Ӳ�����*/
    DEBUG_USART_Handle_Transmit((uint8_t *)tx_buffer, size);

}


#else /* DEBUG_OUTPUT */


void create_debug_task(void){}
void debug_print_buffer(const uint8_t *buffer, uint8_t size){}
void debug_printf(const char *pstring, ...){}
#endif /* DEBUG_OUTPUT */


