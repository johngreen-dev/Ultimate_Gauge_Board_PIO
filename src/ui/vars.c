#include "vars.h"
#include <string.h>
#include <stdlib.h>

// Global variables storage
static int32_t temp_arc_var = 0;
static int32_t led_var = 0;
static char temp_str_var[64] = "0°C";

// Temperature arc variable
int32_t get_var_temp_arc_var() {
    return temp_arc_var;
}

void set_var_temp_arc_var(int32_t value) {
    temp_arc_var = value;
}

// LED variable
int32_t get_var_led_var() {
    return led_var;
}

void set_var_led_var(int32_t value) {
    led_var = value;
}

// Temperature string variable
const char *get_var_temp_str_var() {
    return temp_str_var;
}

void set_var_temp_str_var(const char *value) {
    if (value != NULL) {
        strncpy(temp_str_var, value, sizeof(temp_str_var) - 1);
        temp_str_var[sizeof(temp_str_var) - 1] = '\0'; // Ensure null termination
    }
}
