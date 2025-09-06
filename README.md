# Ultimate Gauge Board PlatformIO Project

A base setup for PlatformIO for the Ultimate Gauge Board by Garage Tinkering

This project now features an **EEZ Studio generated UI** that displays temperature data from CAN bus with a modern, professional interface.

## Features

- **EEZ Studio UI Integration**: Modern UI created with EEZ Studio for LVGL
- **Temperature Gauge**: Arc-style gauge with real-time temperature display
- **CAN Bus Integration**: Reads coolant temperature from automotive CAN messages
- **Moving Average Smoothing**: Smooths temperature readings for stable display
- **LED Status Indicator**: Visual status feedback
- **Professional Display**: Clean, automotive-style gauge interface

## Hardware Requirements

- Ultimate Gauge Board by Garage Tinkering (ESP32-S3 based)
- CAN transceiver (for automotive data)
- 2.1" LCD display (480x480)

## UI Architecture

### EEZ Studio Integration

This project uses **EEZ Studio** to generate the LVGL UI code. The UI consists of:

- **Temperature Arc Gauge**: Visual arc display showing temperature range
- **Temperature Text Display**: Numeric temperature reading with units
- **Status LED**: Configurable status indicator

## CAN Bus Configuration

The project listens for CAN message ID `0x551` containing coolant temperature data:
- **Byte 0**: Temperature in °C + 40 offset (standard automotive format)
- **Example**: Byte value 115 = 75°C (115 - 40 = 75)

## Customizing the UI

### Using EEZ Studio

1. **Open the Project**: Load `test.eez-project` in EEZ Studio
2. **Modify the UI**: Edit screens, add components, adjust styling
3. **Generate Code**: Export the LVGL code to `src/ui/` directory
4. **Update vars.c**: Add any new variable implementations as needed

### Adding New Variables

To add new variables to the system:

1. **Define in vars.h**:
```c
extern int32_t get_var_new_variable();
extern void set_var_new_variable(int32_t value);
```

2. **Implement in vars.c**:
```c
static int32_t new_variable = 0;

int32_t get_var_new_variable() {
    return new_variable;
}

void set_var_new_variable(int32_t value) {
    new_variable = value;
}
```

3. **Use in main.cpp**:
```c
#include "ui/vars.h"
// Update the variable
set_var_new_variable(some_value);
```
