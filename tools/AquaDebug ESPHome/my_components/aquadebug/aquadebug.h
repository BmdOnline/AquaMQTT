#pragma once

#include <RingBuf.h>
#include "esphome.h"
#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include "esphome/core/util.h"

namespace esphome {
namespace aquadebug_sensor {

constexpr uint16_t BUFFER_SIZE = 40;
constexpr uint16_t MQTT_MAX_PAYLOAD_SIZE = 256;

class AquaDebugSensor : public text_sensor::TextSensor, public Component {
  private:
    esphome::uart::UARTComponent *_main_serial;
    esphome::uart::UARTComponent *_hmi_serial;
    uint8_t mTempBuffer[BUFFER_SIZE];
    uint8_t mPayloadBuffer[MQTT_MAX_PAYLOAD_SIZE];
    RingBuf<int, BUFFER_SIZE> mBuffer;

  public:
    void set_required_main_uart(esphome::uart::UARTComponent *main_uart = nullptr);
    void set_required_hmi_uart(esphome::uart::UARTComponent *hmi_uart = nullptr);
    void setup() override;
    void loop() override;
    void dump_config() override;
};

}  // namespace aquadebug_sensor
}  // namespace esphome