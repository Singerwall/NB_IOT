#ifndef gate_dirver_h_include_
#define gate_dirver_h_include_


/**
  * @Macro Define Section
  * @宏定义
  */
#define open_door_duty 850
#define close_door_duty 1450

void Gate_init(void);/*初始化门闸*/
void Gate_Open(void);
void Gate_Close(void);

#endif