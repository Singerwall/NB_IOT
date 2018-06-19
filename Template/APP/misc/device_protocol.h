#ifndef _device_protocol_h_include_
#define _device_protocol_h_include_
#include <stdint.h>


//#define rfid_rx_buffer_max_size 80   
//#define rfid_data_max_size 61

#define MAC_ADDR_LEN 8
#define device_data_max_size 61
#define device_buff_max_size 80

#define EXT_ADDR_LEN          8
typedef enum
{
    DELETE_NODE = 0x00,             /*0 删除节点 */ 
    FIRMWARE_UPDATE,                /*1 固件更新*/
    WRITE_BIN_CHECK,                /*2 写BIN文件校验*/
    NODE_POWER_CONTROL,             /*3 节点电源控制*/
    QUERY_ONLINE_NODE,              /*4 查询在线设备*/
    NODE_ONLINE,                    /*5 设备上线*/
    NODE_LOSE,                      /*6 设备掉线*/
    BEEP_CONTROL,                   /*7 控制蜂鸣器*/
    LED_CONTROL,                    /*8 控制LED*/
    NODE_WHITE_LIST,                /*9 下发白名单*/
    DELETE_WHITE_LIST,              /*10 删除白名单 */
    CHANGE_CHANLE_PANID,            /*11 修改信道和panid*/
    ISSUE_BLE_SCAN = 0x10,          /*16 下发蓝牙扫描*/
    BLE_CONNECTING,                 /*17 蓝牙连接扫描到的从机*/
    BLE_CONNECTED_NETWORK,          /*18 蓝牙节点连接上网络*/
    BLE_NODE_LOSS,                  /*19 蓝牙节点丢失*/
    UPLOAD_SENSOR_DATA,             /*20 上传传感器数据*/
    CONTROL_COMMAND,                /*21 下发控制命令*/
    CAN_DATA_PACKETS,               /*22 CAN数据包传输*/
    RS485_DATA_PACKETS,             /*23 485数据包传输*/
    PRINTER_CONTROL = 0x30,         /*48 打印机控制*/
    SEARCH_CARD,                    /*49 寻卡命令*/
    READ_CARD,                      /*50 读卡命令*/
    WRITE_CARD,                     /*51 写卡命令*/
    NFC_P2P,                        /*52 NFC P2P交互*/
    GET_MAC_ADDR,                   /*53 获取ZigBeeMAC地址*/
    BT_NODE_HEART,                  /*54 蓝牙节点间心跳*/ 
    START_MEASURE = 0x50,           /*80 开始测量      */
    STOP_MEASURE,                   /*81 停止测量     */
} Device_cmd_t;


typedef enum
{
    NODE_ZIGBEE = 0x00,		  /*0 zigbee*/
    NODE_STM32,                   /*1 STM32*/
    NODE_BLUETOOTH,               /*2 蓝牙*/
    NODE_WIFI,                    /*3 wifi*/
    NODE_IPV6,                    /*4 ipv6*/    
    NODE_CAN,                     /*5 CAN*/
    NODE_485,                     /*6 485*/
    NODE_NB,                      /*7 NB_IOT*/
    NODE_NULL = 0xFF              /*255 空节点*/
} Node_type_t;    
    

typedef enum
{
    /*传感器类*/
    DEVICE_SENSOR_TEMP = 0x01,    /*1 温度传感器*/
    DEVICE_SENSOR_HUMI,           /*2 环境湿度传感器*/
    DEVICE_SENSOR_LIGHT,          /*3 光敏传感器*/
    DEVICE_SENSOR_HALL,           /*4 霍尔传感器*/
    DEVICE_SENSOR_CGAS,           /*5 煤气检测传感器*/
    DEVICE_SENSOR_VIBRA,          /*6 震动传感器*/
    DEVICE_SENSOR_GYROS,          /*7 三轴加速度传感器*/
    DEVICE_SENSOR_ULTRA,          /*8 超声波传感器*/
    DEVICE_SENSOR_PYRO,           /*9 热释电红外线传感器*/
    DEVICE_SENSOR_PRES,           /*10 压力传感器*/
    DEVICE_SENSOR_SOUND,          /*11 声音传感器*/
    DEVICE_SENSOR_CURR,           /*12 电流传感器*/
    DEVICE_SENSOR_PELE,           /*13 光电传感器*/
    DEVICE_SENSOR_PULSE,          /*14 脉搏传感器*/
    DEVICE_SENSOR_VIOCE,          /*15 语音识别传感器*/
    DEVICE_SENSOR_BOTYT,          /*16 红外测温传感器*/
    DEVICE_SENSOR_ECG,            /*17 心电传感器*/
    DEVICE_SENSOR_HR,             /*18 心率传感器*/
    DEVICE_SENSOR_BLDOX,          /*19 血氧传感器*/
    DEVICE_SENSOR_BLDPR,          /*20 血压传感器*/
    DEVICE_SENSOR_GPS,            /*21 GPS传感器*/
    /*控制类*/
    DEVICE_IR_CONTROL = 0x30,     /*48 红外控制器*/
    DEVICE_DOOR,                  /*49 门闸*/
    DEVICE_LOCK,                  /*50 电子锁*/   
    DEVICE_RELAY,                 /*51 继电器 */
    DEVICE_STEPPER_MOTOR,         /*52 步进电机*/    
    DEVICE_PRINTER,               /*53 热敏打印机*/    
    /*RFID类*/
    DEVICE_RFID_125K = 0x50,      /*80 125K读头*/
    DEVICE_RFID_13P56M,           /*81 13.56M读头*/
    DEVICE_RFID_900M,             /*82 900M读头*/
    DEVICE_RFID_2P4G,             /*83 2.4G读头*/
    NDEVICE_RFID_MAG_READER,      /*84 磁条卡读头*/
    DEVICE_RFID_IC_READER,        /*85 IC卡读写器*/    
    DEVICE_RFID_NFC,              /*86 NFC设备*/
    /*数据传输类*/
    DEVICE_433M = 0x70,           /*112 433M节点*/
    DEVICE_WIFI_CAMERA,           /*113 摄像头*/
     /*数据显示类*/
    DEVICE_LCD_DW = 0x82,          /* 150 串口屏*/
    /*空设备类型*/
    DEVICE_NULL = 0xFF,           /*255 空设备类型*/
} Device_type_t;

    
        
    
typedef enum
{
    ACK_OK = 0x00,                /*命令操作成功*/
    ACK_NOT_ONLINE,               /*节点不在线*/
    ACK_NOT_REPLY,                /*节点无回复*/
    ACK_DELETE_NODE_FAIL,         /*删除节点失败*/
    ACK_NODE_TYPE_ERR,            /*设备类型错误*/
    ACK_INVALID_CMD,              /*无效命令*/
    ACK_UPDATE_FAIL,              /*固件更新失败应答*/
    ACK_CHECK_FAIL,               /*写BIN文件校验失败应答*/
    ACK_CARD_HANDLE_ERR,          /*寻卡超时/发送超时/充值超时/设置超时*/
    ACK_OTHER_ERR,                /*其他错误*/
    ACK_GET_MAC_ERR,              /*获取节点MAC地址失败*/
} Device_ack_t;


typedef struct
{
    uint16_t msg_num;                     /* 消息序号 */
   // uint8_t  igw_mac_addr[MAC_ADDR_LEN];  /* 网关mac地址*/
    uint16_t node_type;                   /* 节点类型 */
    uint16_t device_type;                 /* 设备类型 */
    //uint8_t  mac_addr[MAC_ADDR_LEN];      /* MAC地址  */  
    uint16_t cmd;                         /* 命令     */
    uint16_t ack;                         /* 应答     */  
    uint16_t data_size;                   /* 数据长度 */  
    uint8_t  data[device_data_max_size];  /* 数据     */  
} Device_packet_t;

uint8_t device_packet_to_buffer(const Device_packet_t *packet, uint8_t *buffer);
uint8_t device_buffer_to_packet(const uint8_t *buffer, Device_packet_t *packet);

#endif /* _device_protocol_h_include_ */
