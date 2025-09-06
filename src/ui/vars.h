#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_TEMP_ARC_VAR = 0,
    FLOW_GLOBAL_VARIABLE_LED_VAR = 1,
    FLOW_GLOBAL_VARIABLE_TEMP_STR_VAR = 2
};

// Native global variables

extern int32_t get_var_temp_arc_var();
extern void set_var_temp_arc_var(int32_t value);
extern int32_t get_var_led_var();
extern void set_var_led_var(int32_t value);
extern const char *get_var_temp_str_var();
extern void set_var_temp_str_var(const char *value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/