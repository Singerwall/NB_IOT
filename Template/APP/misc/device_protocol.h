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
    DELETE_NODE = 0x00,             /*0 ɾ���ڵ� */ 
    FIRMWARE_UPDATE,                /*1 �̼�����*/
    WRITE_BIN_CHECK,                /*2 дBIN�ļ�У��*/
    NODE_POWER_CONTROL,             /*3 �ڵ��Դ����*/
    QUERY_ONLINE_NODE,              /*4 ��ѯ�����豸*/
    NODE_ONLINE,                    /*5 �豸����*/
    NODE_LOSE,                      /*6 �豸����*/
    BEEP_CONTROL,                   /*7 ���Ʒ�����*/
    LED_CONTROL,                    /*8 ����LED*/
    NODE_WHITE_LIST,                /*9 �·�������*/
    DELETE_WHITE_LIST,              /*10 ɾ�������� */
    CHANGE_CHANLE_PANID,            /*11 �޸��ŵ���panid*/
    ISSUE_BLE_SCAN = 0x10,          /*16 �·�����ɨ��*/
    BLE_CONNECTING,                 /*17 ��������ɨ�赽�Ĵӻ�*/
    BLE_CONNECTED_NETWORK,          /*18 �����ڵ�����������*/
    BLE_NODE_LOSS,                  /*19 �����ڵ㶪ʧ*/
    UPLOAD_SENSOR_DATA,             /*20 �ϴ�����������*/
    CONTROL_COMMAND,                /*21 �·���������*/
    CAN_DATA_PACKETS,               /*22 CAN���ݰ�����*/
    RS485_DATA_PACKETS,             /*23 485���ݰ�����*/
    PRINTER_CONTROL = 0x30,         /*48 ��ӡ������*/
    SEARCH_CARD,                    /*49 Ѱ������*/
    READ_CARD,                      /*50 ��������*/
    WRITE_CARD,                     /*51 д������*/
    NFC_P2P,                        /*52 NFC P2P����*/
    GET_MAC_ADDR,                   /*53 ��ȡZigBeeMAC��ַ*/
    BT_NODE_HEART,                  /*54 �����ڵ������*/ 
    START_MEASURE = 0x50,           /*80 ��ʼ����      */
    STOP_MEASURE,                   /*81 ֹͣ����     */
} Device_cmd_t;


typedef enum
{
    NODE_ZIGBEE = 0x00,		  /*0 zigbee*/
    NODE_STM32,                   /*1 STM32*/
    NODE_BLUETOOTH,               /*2 ����*/
    NODE_WIFI,                    /*3 wifi*/
    NODE_IPV6,                    /*4 ipv6*/    
    NODE_CAN,                     /*5 CAN*/
    NODE_485,                     /*6 485*/
    NODE_NB,                      /*7 NB_IOT*/
    NODE_NULL = 0xFF              /*255 �սڵ�*/
} Node_type_t;    
    

typedef enum
{
    /*��������*/
    DEVICE_SENSOR_TEMP = 0x01,    /*1 �¶ȴ�����*/
    DEVICE_SENSOR_HUMI,           /*2 ����ʪ�ȴ�����*/
    DEVICE_SENSOR_LIGHT,          /*3 ����������*/
    DEVICE_SENSOR_HALL,           /*4 ����������*/
    DEVICE_SENSOR_CGAS,           /*5 ú����⴫����*/
    DEVICE_SENSOR_VIBRA,          /*6 �𶯴�����*/
    DEVICE_SENSOR_GYROS,          /*7 ������ٶȴ�����*/
    DEVICE_SENSOR_ULTRA,          /*8 ������������*/
    DEVICE_SENSOR_PYRO,           /*9 ���͵�����ߴ�����*/
    DEVICE_SENSOR_PRES,           /*10 ѹ��������*/
    DEVICE_SENSOR_SOUND,          /*11 ����������*/
    DEVICE_SENSOR_CURR,           /*12 ����������*/
    DEVICE_SENSOR_PELE,           /*13 ��紫����*/
    DEVICE_SENSOR_PULSE,          /*14 ����������*/
    DEVICE_SENSOR_VIOCE,          /*15 ����ʶ�𴫸���*/
    DEVICE_SENSOR_BOTYT,          /*16 ������´�����*/
    DEVICE_SENSOR_ECG,            /*17 �ĵ紫����*/
    DEVICE_SENSOR_HR,             /*18 ���ʴ�����*/
    DEVICE_SENSOR_BLDOX,          /*19 Ѫ��������*/
    DEVICE_SENSOR_BLDPR,          /*20 Ѫѹ������*/
    DEVICE_SENSOR_GPS,            /*21 GPS������*/
    /*������*/
    DEVICE_IR_CONTROL = 0x30,     /*48 ���������*/
    DEVICE_DOOR,                  /*49 ��բ*/
    DEVICE_LOCK,                  /*50 ������*/   
    DEVICE_RELAY,                 /*51 �̵��� */
    DEVICE_STEPPER_MOTOR,         /*52 �������*/    
    DEVICE_PRINTER,               /*53 ������ӡ��*/    
    /*RFID��*/
    DEVICE_RFID_125K = 0x50,      /*80 125K��ͷ*/
    DEVICE_RFID_13P56M,           /*81 13.56M��ͷ*/
    DEVICE_RFID_900M,             /*82 900M��ͷ*/
    DEVICE_RFID_2P4G,             /*83 2.4G��ͷ*/
    NDEVICE_RFID_MAG_READER,      /*84 ��������ͷ*/
    DEVICE_RFID_IC_READER,        /*85 IC����д��*/    
    DEVICE_RFID_NFC,              /*86 NFC�豸*/
    /*���ݴ�����*/
    DEVICE_433M = 0x70,           /*112 433M�ڵ�*/
    DEVICE_WIFI_CAMERA,           /*113 ����ͷ*/
     /*������ʾ��*/
    DEVICE_LCD_DW = 0x82,          /* 150 ������*/
    /*���豸����*/
    DEVICE_NULL = 0xFF,           /*255 ���豸����*/
} Device_type_t;

    
        
    
typedef enum
{
    ACK_OK = 0x00,                /*��������ɹ�*/
    ACK_NOT_ONLINE,               /*�ڵ㲻����*/
    ACK_NOT_REPLY,                /*�ڵ��޻ظ�*/
    ACK_DELETE_NODE_FAIL,         /*ɾ���ڵ�ʧ��*/
    ACK_NODE_TYPE_ERR,            /*�豸���ʹ���*/
    ACK_INVALID_CMD,              /*��Ч����*/
    ACK_UPDATE_FAIL,              /*�̼�����ʧ��Ӧ��*/
    ACK_CHECK_FAIL,               /*дBIN�ļ�У��ʧ��Ӧ��*/
    ACK_CARD_HANDLE_ERR,          /*Ѱ����ʱ/���ͳ�ʱ/��ֵ��ʱ/���ó�ʱ*/
    ACK_OTHER_ERR,                /*��������*/
    ACK_GET_MAC_ERR,              /*��ȡ�ڵ�MAC��ַʧ��*/
} Device_ack_t;


typedef struct
{
    uint16_t msg_num;                     /* ��Ϣ��� */
   // uint8_t  igw_mac_addr[MAC_ADDR_LEN];  /* ����mac��ַ*/
    uint16_t node_type;                   /* �ڵ����� */
    uint16_t device_type;                 /* �豸���� */
    //uint8_t  mac_addr[MAC_ADDR_LEN];      /* MAC��ַ  */  
    uint16_t cmd;                         /* ����     */
    uint16_t ack;                         /* Ӧ��     */  
    uint16_t data_size;                   /* ���ݳ��� */  
    uint8_t  data[device_data_max_size];  /* ����     */  
} Device_packet_t;

uint8_t device_packet_to_buffer(const Device_packet_t *packet, uint8_t *buffer);
uint8_t device_buffer_to_packet(const uint8_t *buffer, Device_packet_t *packet);

#endif /* _device_protocol_h_include_ */
