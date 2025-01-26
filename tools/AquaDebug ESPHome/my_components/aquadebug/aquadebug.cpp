#include "esphome/core/log.h"
#include "aquadebug.h"

namespace esphome {
namespace aquadebug_sensor {

static const char *TAG = "aquadebug_sensor";
// Levels:
//  - ERROR: ESP_LOGE
//  - WARNING: ESP_LOGW
//  - INFO: ESP_LOGI
//  - DEBUG: ESP_LOGD
//  - VERBOSE: ESP_LOGV
//  - VERY_VERBOSE: ESP_LOGVV

using uart::UARTDevice;  
using uart::UARTComponent;

void toHexStr(uint8_t* data, uint8_t data_len, char* buffer) {
    const size_t num_bytes = data_len / sizeof(uint8_t);
    // char         hex_str[num_bytes * 2 + 1];
    for (size_t i = 0; i < num_bytes; i++)
    {
        sprintf(&buffer[i * 2], "%02X", data[i]);
    }
    buffer[num_bytes * 2] = '\0';
}

void AquaDebugSensor::set_required_main_uart(esphome::uart::UARTComponent *main_uart) {
    _main_serial = main_uart;
}

void AquaDebugSensor::set_required_hmi_uart(esphome::uart::UARTComponent *hmi_uart) {
    _hmi_serial = hmi_uart;
}

void AquaDebugSensor::setup() {
}

void AquaDebugSensor::loop() {
    // read serial into buffer
    while (_main_serial->available())
    {
        // push new element to the back
        //mBuffer.push(_main_serial.read());
        uint8_t data;
        _main_serial->read_byte(&data);
        mBuffer.push(data);

        // if buffer is full, we emit everything and clear the buffer
        if (mBuffer.isFull())
        {
            for (int i = 0; i < BUFFER_SIZE; i++)
            {
                int retVal;
                mBuffer.pop(retVal);
                mTempBuffer[i] = retVal;
            }
            toHexStr(mTempBuffer, BUFFER_SIZE, reinterpret_cast<char*>(mPayloadBuffer));
            //ESP_LOGI(TAG,"Received data : %s", reinterpret_cast<char*>(mPayloadBuffer));
            if(this->state!=(reinterpret_cast<char*>(mPayloadBuffer))){
                this->publish_state(reinterpret_cast<char*>(mPayloadBuffer));
            }
        }
    }
}

void AquaDebugSensor::dump_config(){
    ESP_LOGCONFIG(TAG, "AquaDebug Sensor");
}

}  // namespace aquadebug_sensor
}  // namespace esphome