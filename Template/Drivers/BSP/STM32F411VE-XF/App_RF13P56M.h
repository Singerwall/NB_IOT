#ifndef _app_13p56m_h_include_
#define _app_13p56m_h_include_

#include "device_protocol.h"

/**
  * @brief  13.56MѰ��
 */
int _cmd_13p56m_list_card(Device_packet_t *packet);
/**
  * @brief  13.56M����
 */
int _cmd_13p56m_read_card(Device_packet_t *packet);
/**
  * @brief  13.56Mд��
 */
int _cmd_13p56m_write_card(Device_packet_t *packet);


#endif /* _app_13p56m_h_include_ */
