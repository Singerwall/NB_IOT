/** @copyright XunFang Communication Tech Limited. All rights reserved. 2013.
  * @file 
  * @author  hzp
  * @version  V3.0.0
  * @date  08/01/2013
  * @brief  
  */ 
#include <includes.h>

extern void test_rec(void);

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#include <includes.h>
  
  
#define LCD_FIRST_LINE  2

#define LCD_LAST_LINE   7

#define JSON_BUF_LEN    100
  


  
  
typedef enum
{
  NB_NONE=0,
  NB_INIT,          // 初始化操作
  NB_MODULE=2,        // 获取NB模块厂商及固件，频段等信息
  NB_SIGN,
  NB_UDP_CR=4,        // 创建UDP
  NB_UDP_CL,        // 关闭UDP
  NB_UDP_REG=6,       // 此状态用于注册谷雨云平台信息（如果不与谷雨云平台通信，可忽略）
  NB_UDP_ST,        // 利用已经创建的UDP发送数据
  NB_UDP_RE=8,        // UDP接收信息
  NB_CoAP_SEVER,    // CoAP远程地址设置与获取
  NB_CoAP_ST=10,       // 利用CoAP发送消息
  NB_CoAP_RE,       // CoAP返回信息
  NB_RESET=12,         // 复位NB
  NB_END
}NB_STATE_e;

volatile NB_STATE_e  APP_STATE= NB_NONE;
int NB_MsgreportCb(msg_types_t,int ,char*);
static void _new_packet_callback(Device_packet_t *packet);

uint8_t* NB_Module_IMEI = NULL;

//记录NB模块初始化流程顺序
#define  NB_SEQ_INIT        0x01
#define  NB_SEQ_UDP_CR      0X02
#define  NB_SEQ_UDP_REG     0x04
#define  NB_SEQ_COAP_SERVER 0x08



uint8_t  seq_record = 0;     //标志登记变量

//管理json数据
typedef struct
{
	char json_buf[JSON_BUF_LEN];  //用于存放json格式数据
	uint16_t json_len;            //有效数据长度
}json_info_t;

json_info_t json_info;


static void * NB_Task(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4);


/**
  * @brief 创建开关量管理控制任务
  * @retval 无
 */
uint32_t create_NB_Task(void)
{
 
    UINT32 uwRet = 0;
    UINT32 NB_Task_Index = 0;
    TSK_INIT_PARAM_S stTaskInitParam;
    
    (VOID)memset((void *)(&stTaskInitParam), 0, sizeof(TSK_INIT_PARAM_S));
    /*创建任务:NB_Task*/
    stTaskInitParam.pfnTaskEntry = NB_Task;
    stTaskInitParam.pcName = "NB_Task";
    stTaskInitParam.usTaskPrio = Prio_NB_Task;
    stTaskInitParam.uwStackSize =StackSize_NB_Task;

    LOS_TaskLock();//锁住任务，防止新创建的任务比本任务高而发生调度
    uwRet = LOS_TaskCreate(&NB_Task_Index, &stTaskInitParam);
    if(uwRet != LOS_OK)
    {
        debug_print_string("create NB_Task failed!,error:%x\n",uwRet);
        return uwRet;
    }
    
    LOS_TaskUnlock();//解锁任务，
    
    return LOS_OK;
}


/**
  * @brief 串口发送服务程序
  * @details 从队列中取出数据，并使用DMA进行发送。然后等待发送完成。
  * @param  pdata 无用
  * @retval 无
 */
static void * NB_Task(UINT32 uwParam1,
                UINT32 uwParam2,
                UINT32 uwParam3,
                UINT32 uwParam4)
{
  
  
  u8 test[12]={01,0x5b,0x11,0x27,0x01,0x02,0x11,00,01,0x1A,0x44,0xCA};
    debug_printf("[%s] enter.\r\n", __func__);
    
    NBModule_open(&nb_config);
    APP_STATE = NB_NONE;
   
    for(;;)
    { 
      
    //  test_rec();
      MX_TimerPoll();
      NBModule_Main(&nb_config);
 
      switch(APP_STATE)
      {
      case NB_NONE:
        {
          //wait for key
            APP_STATE = NB_INIT;
        }
        break;
      case NB_INIT:
        {
          printf("\r\n<----BC95 Init---->\r\n");
          LCD_Print("Init start...",NULL);
          seq_record = 0;
          NBModule_Init(&nb_config);
          APP_STATE = NB_END;
        }
        break;
      case NB_SIGN:
        {
           printf("\r\n<----BC95 get signal---->\r\n");
           NBModule_Sign(&nb_config);
           APP_STATE = NB_END;
        }
        break;
      case NB_MODULE:
        {
          printf("\r\n<----Module info ---->\r\n");
          LCD_Print("Module info...",NULL);
          NBModule_Info(&nb_config);
          APP_STATE = NB_END;
        }
        break;
      case NB_UDP_CR:
        {
          printf("\r\n<----Create udp ---->\r\n");
          LCD_Print("UDP Create...",NULL);
          NBModule_CreateUDP(&nb_config);
          APP_STATE = NB_END;
        }
        break;
      case NB_UDP_CL:
        {
          printf("\r\n<----Close udp ---->\r\n");
          NBModule_CloseUDP(&nb_config);
          APP_STATE = NB_END;
        }
        break;
      case NB_UDP_REG:
        {
          //发送注册包
          //如果不与谷雨云平台通信，则不需要发送注册包
          printf("\r\n<----NB registes to Cloud ---->\r\n");
          LCD_Print("Udp send...",NULL);
          
          char regPacket[30];
          uint8_t msgLen = 0;
          
          //利用库函数进行合格化数据
          msgLen = sprintf(regPacket,"ep=%s&pw=123456",NB_Module_IMEI);
          regPacket[msgLen] = 0;
          
          //char* regPacket = "ep=863703036005069&pw=123456";
          NBModule_SendData(&nb_config,msgLen,regPacket);
          seq_record |= NB_SEQ_UDP_REG;
          LCD_Print("S3 send UDP",NULL);
          APP_STATE = NB_END;
        }
        break;
      case NB_UDP_ST:
        {
          printf("\r\n<---- Send udp ---->\r\n");
          LCD_Print("Udp send...",NULL);
          char* userPacket = "NB_EK_L476";
          NBModule_SendData(&nb_config,sizeof("NB_EK_L476"),userPacket);

          APP_STATE = NB_END;
        }
        break;
        
      case NB_UDP_RE:
        {
          
        }
        break;
      case NB_CoAP_SEVER:
        {
          printf("\r\n<---- CoAP Server set ---->\r\n");
          LCD_Print("Coap remote...",NULL);
          
          NBModule_CoAPServer(&nb_config,1,NULL);
          APP_STATE = NB_END;
        }
        break;
      case NB_CoAP_ST:
        {
           //  bc95_coapSendMsg(&nb_config,12,test);
    
             app_transmit_packet(10000,01,02,03,05,3,test);

            APP_STATE = NB_END;
        }
        break;
      case NB_CoAP_RE:
        {
          
        }
        break;
      default:
        {
        }
        break;
      }
      OSTimeDly (10); 
    }

}

/**
  * @brief NB_MsgreportCb
  * @details NB模块消息上报回调  显示层
  * @param  pdata 无用
  * @retval 无
 */
int  NB_MsgreportCb(msg_types_t types,int len,char* msg)
{
  switch(types)
  {
  case MSG_INIT:
    {
      printf("\r\nINIT=%s\r\n",msg);
      LCD_Print("Init=%s",msg);
      if(*msg == 'S')
      {
        //HAL_LED_SET(LED_INDEX_1,0);
        Display_print(1,0,"NET=ON");
        APP_STATE = NB_SIGN;
        seq_record |= NB_SEQ_INIT;  //记录初始化成功
        LCD_Print("S2 to get info",NULL);
        LCD_Print("S3 to set remote",NULL);  
      }
      else
      {
        printf("Reinit NB with S1\r\n");
        LCD_Print("Reinit NB...",NULL);
      }
    }
    break;
  case MSG_IMSI:
    {
      printf("\r\nIMSI=%s\r\n",msg);
      LCD_Print(msg,NULL);
    }
    break;
  case MSG_REG:
    {
      Display_print(1,0,"NET=%s",(*msg) == 1 ?"ON":"0FF");
    }
    break;
  case MSG_SIGN:
    {
      Display_print(1,8,"%sdbm  ",msg);
      
			APP_STATE = NB_CoAP_SEVER;   //设定远程地址
    }
    break;
  case MSG_MODULE_INFO:
    {
      printf("\r\nMinfo=%s\r\n",msg);
    }
    break;
  case MSG_MID:
    {
      printf("\r\nMID=%s\r\n",msg);
    }
    break;
  case MSG_MMODEL:
    {
      printf("\r\nModel=%s\r\n",msg);
    }
    break;
  case MSG_MREV:
    {
      printf("\r\nREV=%s\r\n",msg);
    }
    break;
  case MSG_BAND:
    {
      printf("\r\nFreq=%s\r\n",msg);
    }
    break;
  case MSG_IMEI:
    {
      printf("\r\nIMEI=%s\r\n",msg);
      //保存IMEI信息
      NB_Module_IMEI = (uint8_t*)msg;
    }
    break;
  case MSG_UDP_CREATE:
    {
      printf("\r\nUDP_CR=%s\r\n",msg);
      LCD_Print("UDP=%s",msg);
      if(*msg == 'S')  //"S" 表示创建成功，'F'表示失败
      {
        seq_record |= NB_SEQ_UDP_CR;  //记录初始化成功
      }
      else
      {
        printf("Please, recreate udp\r\n");
      }
    }
    break;
  case MSG_UDP_CLOSE:
    {
      printf("\r\nUDP_CL=%s\r\n",msg);

      if(*msg == 'S')
      {
        //清除UDP 创建成功标志
        seq_record &= ~NB_SEQ_UDP_CR;
      }
    }
    break;
  case MSG_UDP_SEND:
    {
      printf("\r\nUDP_SEND=%s\r\n",msg);
      LCD_Print("Send=%s",msg);
    }
    break;
  case MSG_UDP_RECE:
    {
      printf("\r\nUDP_RECE=%s\r\n",msg);
      LCD_Print(msg,NULL);
      
    }
    break;
  case MSG_COAP:
    {
      printf("\r\nCOAP=%s\r\n",msg);
      if(*msg == 'S')
      {
        //针对设置
        seq_record |= NB_SEQ_COAP_SERVER;
        LCD_Print("S4 Send",NULL);
      }
    }
    break;
  case MSG_COAP_SEND:
    {
      printf("\r\nCOAP_SENT=%s\r\n",msg);
    }
    break;
    
  case MSG_COAP_RECE:
    {
      Device_packet_t packet;
      
      printf("\r\nCOAP_RECE =%s\r\n",msg);
      LCD_Print(msg,NULL);
      //add 协议解析代码
      
      if(device_buffer_to_packet((uint8_t *)msg, &packet))
      {
        _new_packet_callback( &packet);
      }
      
      
    }
    break;
  default :
    {
      break;
    }
  }
  return 0;
}

/**
  * @brief 打印一包数据的接口
  * @param packet 要打印的数据
 */
void app_platform_print_packet(const Device_packet_t *packet)
{

        debug_printf("S/N %d,Type %d,Cmd %d,Size %d,Data \r\n", 
            packet->msg_num,
            packet->node_type, 
            packet->cmd, 
            packet->data_size);
        debug_print_buffer(packet->data, packet->data_size);
  
}

/**
  * @brief 收到来自平台的消息处理回调函数
  * @param packet 来自平台的消息包
 */
static void _new_packet_callback(Device_packet_t *packet)
{

    app_platform_print_packet(packet);/*打印出数据包*/
 
    
    switch (packet->cmd)
    {
        case NODE_ONLINE:	/*网关下发节点连接上网络，上传数据包含本节点信息应答*/
            /* 删除节点上线标志，不在向网关发送该节点上线信息 */
            //DeleteUpOnLineFlag( long_addr );
            break;
        case FIRMWARE_UPDATE:			
     
            break;
        case WRITE_BIN_CHECK:                  
     
            break;
        case NODE_POWER_CONTROL:                  
     
            break;      
        case QUERY_ONLINE_NODE:                  
     
            break;      
        case NODE_LOSE:                  
     
            break;
        case BEEP_CONTROL:  
            app_sw_blink_interface(BEEP, 1, 100, packet->data[0]);
            break;      
        case LED_CONTROL:                  
            app_sw_blink_interface(LED_1, 1, 100, packet->data[0]);/*默认是控制led1*/
            break;  
        case NODE_WHITE_LIST:			
     
            break;
        case DELETE_WHITE_LIST:                  
     
            break;
        case CHANGE_CHANLE_PANID:                  
     
            break;      
        case ISSUE_BLE_SCAN:                  
     
            break;      
        case BLE_CONNECTING:                  
     
            break;      
        case BLE_CONNECTED_NETWORK:                  
     
            break;  
        case BLE_NODE_LOSS:			
     
            break;
        case UPLOAD_SENSOR_DATA:                  
     
            break;
        case CONTROL_COMMAND:                  
     
            break;      
        case PRINTER_CONTROL: 
          
     
            break; 
        case SEARCH_CARD:                  
     
            break;      
        case READ_CARD:                  
     
            break;  
        case WRITE_CARD:			
     
            break;
        case NFC_P2P:                  
     
            break;
        case GET_MAC_ADDR:                  
     
            break;                 
        default:
            debug_printf("platform unknowed cmd %d\n", packet->cmd);
            break;
    }
}
void _cmd_power_control(Device_packet_t *packet)
{
  switch(packet->device_type)
  {
    case DEVICE_SENSOR_GPS :
      control_device_pow( GPS, packet->data[0]);
      break;
    case NODE_BLUETOOTH :
      control_device_pow( BL, packet->data[0]);
      break ; 
    case DEVICE_LCD_DW :
      control_device_pow( LCD, packet->data[0]);
      break  ;     
//    case DEVICE_LCD_DW :
//      control_device_pow( LCD, packet->data[0]);
      break  ;  
        
  }   
}
 void APP_STATEset(u8 i)
 {

  APP_STATE =(NB_STATE_e) i;
 }
/**
  * @brief 向平台发送消息包
  * @param packet packet 要发送的消息包
  * @retval 发送成功返回值大于0。
 */
int app_transmit_packet( uint16_t msg_num,
                         uint16_t node_type,
                         uint16_t device_type,
                         uint16_t cmd,
                         uint16_t ack,
                         uint16_t data_size,
                         uint8_t *p)
{
    int ret = 0;
    uint16_t tx_size;
    uint8_t tx_buffer[200] = {0};
    
    Device_packet_t packet;
            packet.msg_num = msg_num;
            packet.node_type = node_type;
            packet.device_type = device_type;
            packet.cmd = cmd;
            packet.ack = ack;
            packet.data_size = data_size; 
     memcpy(packet.data,p,data_size); 
 
    tx_size = device_packet_to_buffer(&packet, (tx_buffer));
    if(!tx_size)
        return ret;

     ret=bc95_coapSendMsg(&nb_config,tx_size,(char *)tx_buffer);
    
    return ret;
}





#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


