/* Basic Driver setup for the ESP32-S3 2.1 inch LCD Driver board  */
/* Author: Andy Valentine - Garage Tinkering                      */
/*                                                                */

#include <Arduino.h>
#include "CANBus_Driver.h"
#include "LVGL_Driver.h"
#include "I2C_Driver.h"
#include <vector>

// EEZ Studio UI
#include "ui/ui.h"
#include "ui/vars.h"

// DATA STORE
typedef struct struct_gauge_data {
  int coolant_temp;
}struct_gauge_data;

struct_gauge_data GaugeData;
#define TAG "TWAI"

// CONTROL CONSTANTS
const int AVERAGE_VALUES      = 20;

// CONTROL VARIABLE INIT
bool initial_load             = false; // has the first data been received
volatile bool data_ready      = false; // new incoming data

uint8_t average_coolant_temp  = 0;

void Drivers_Init(void) {
  I2C_Init();

  Serial.println("Scanning for TCA9554...");
  bool found = false;
  for (int attempt = 0; attempt < 10; attempt++) {
    if (I2C_ScanAddress(0x20)) { // 0x20 is default for TCA9554
      found = true;
      break;
    }
    delay(50); // wait a bit before retrying
  }

  if (!found) {
    Serial.println("TCA9554 not detected! Skipping expander init.");
  } else {
    TCA9554PWR_Init(0x00);
  }
  LCD_Init();
  CANBus_Init();
  Lvgl_Init();
}

// HELPER FUNCTIONS

// create moving average for smoothing
uint8_t Get_Moving_Average(int new_value) {
    static int values[AVERAGE_VALUES] = {0};
    static int index = 0;
    static int count = 0;
    static double sum = 0;

    // Subtract the value being replaced
    sum -= values[index];

    // Insert the new value
    values[index] = new_value;
    sum += new_value;

    // Update index and count
    index = (index + 1) % AVERAGE_VALUES;
    if (count < AVERAGE_VALUES) count++;

    return std::round(sum / count);
}

void Update_Coolant_Temp(void) {
  // use a moving average of the last x values for smoothing
  average_coolant_temp = Get_Moving_Average(GaugeData.coolant_temp);
  
  // Update EEZ Studio UI variables
  set_var_temp_arc_var(average_coolant_temp);
  
  // Format temperature string
  char temp_str[32];
  snprintf(temp_str, sizeof(temp_str), "%d°C", average_coolant_temp);
  set_var_temp_str_var(temp_str);
  if (average_coolant_temp < 60) {
    set_var_led_var(0); // off
  } else if (average_coolant_temp < 80) {
    set_var_led_var(128); // half brightness
  } else {
    set_var_led_var(255); // full brightness
  }
}

// update parts with incoming values
void Update_Values(void) {
  Update_Coolant_Temp();
}

// build the screens
void Screens_Init(void) {
  // Initialize EEZ Studio UI
  ui_init();
}

void Process_Coolant_Temp(uint8_t *byte_data) {
    // byte 0
    // mod -40

    int byte_pos = 0;
    int final_temp = byte_data[byte_pos] - 40;

    Serial.println(final_temp);

    GaugeData.coolant_temp = final_temp;
}
bool status_led = false;

void Receive_CAN_Task(void *arg) {
    while (1) {
      twai_message_t message;
      esp_err_t err = twai_receive(&message, pdMS_TO_TICKS(1000));
      if (err == ESP_OK) {
          Serial.print("Received CAN message: ID=0x");
          Serial.print(message.identifier, HEX);
          Serial.print(" DLC=");
          Serial.print(message.data_length_code);
          Serial.print(" Data=");
          for (int i = 0; i < message.data_length_code; i++) {
            Serial.printf("%02X ", message.data[i]);
          }
          Serial.println();

          switch (message.identifier) {
            case 0x551:
              Process_Coolant_Temp(message.data);
              data_ready = true;
              break;
            default:
              break;
          }
      } else if (err == ESP_ERR_TIMEOUT) {
          ESP_LOGW(TAG, "Reception timed out");
      } else {
          ESP_LOGE(TAG, "Message reception failed: %s", esp_err_to_name(err));
      }
          if (!status_led) {
        Set_EXIO(EXIO_PIN4, High);
        status_led = true;
      } else {
        Set_EXIO(EXIO_PIN4, Low);
        status_led = false;
      }
      vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void setup(void) {
  Serial.begin(115200);
  Serial.println("begin");
  Drivers_Init();
  Set_Backlight(50);

  Screens_Init();
  Set_EXIO(EXIO_PIN4, Low);

  xTaskCreate(Receive_CAN_Task, "Receive_CAN_Task", 8192, NULL, 1, NULL);
  Serial.print("setup complete");
}

void loop(void) {
  lv_timer_handler();
  ui_tick(); // EEZ Studio UI tick

  if (data_ready) {
    data_ready = false;
    Update_Values();
  }

  vTaskDelay(pdMS_TO_TICKS(5));
}