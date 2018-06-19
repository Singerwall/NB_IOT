#ifndef _app_debug_h_include_
#define _app_debug_h_include_


uint32_t create_debug_task(void);
void debug_printf(const char *pstring, ...);
void debug_print_buffer(const uint8_t *buffer, uint8_t size);
void debug_output_control(uint8_t en);
void debug_print_string(const char *pstring, ...);
#endif /* _app_debug_h_include_ */
