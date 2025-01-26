import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, text_sensor
from esphome.const import (
    CONF_ID,
    CONF_UART_ID,
)

CODEOWNERS = ["@aquadebug"]
DEPENDENCIES = ["uart"]
AUTO_LOAD = ["text_sensor"]

CONF_MAIN_UART= 'main'
CONF_HMI_UART= 'hmi'

aquadebug_sensor_ns = cg.esphome_ns.namespace("aquadebug_sensor")
AquaDebugSensor = aquadebug_sensor_ns.class_(
    "AquaDebugSensor", text_sensor.TextSensor, cg.Component
)

def output_info(config):
    """_LOGGER.info(config)"""
    return config

CONFIG_SCHEMA = cv.All(
    text_sensor.TEXT_SENSOR_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(AquaDebugSensor),
            cv.Optional(CONF_MAIN_UART, default={}): cv.Schema(
                {
                }
            )
            .extend(uart.UART_DEVICE_SCHEMA),
            cv.Optional(CONF_HMI_UART, default={}): cv.Schema(
                {
                }
            )
            .extend(uart.UART_DEVICE_SCHEMA),
        }
    )
    .extend(cv.COMPONENT_SCHEMA),
    output_info,
)


def final_validate_schema(config):
    if CONF_MAIN_UART in config:
        _main_uart = config.get(CONF_MAIN_UART)
        uart.final_validate_device_schema(
            CONF_MAIN_UART, baud_rate=9550, data_bits=8, parity="NONE", stop_bits=2, require_tx=True, require_rx=True
        )(_main_uart)

    if CONF_HMI_UART in config:
        _hmi_uart = config.get(CONF_HMI_UART)
        uart.final_validate_device_schema(
            CONF_HMI_UART, baud_rate=9550, data_bits=8, parity="NONE", stop_bits=2, require_tx=True, require_rx=True
        )(_hmi_uart)

FINAL_VALIDATE_SCHEMA = final_validate_schema


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await text_sensor.register_text_sensor(var, config)

    if CONF_MAIN_UART in config:
       main_uart = config[CONF_MAIN_UART]
       if CONF_UART_ID in main_uart:
          parent = await cg.get_variable(main_uart[CONF_UART_ID])
          cg.add(var.set_required_main_uart(parent))
       else:
          raise cv.Invalid(
             f"Setting 'uart_id' is required !"
          )
    else:
       raise cv.Invalid(
          f"Setting '" + CONF_MAIN_UART + "' is required !"
       )

    if CONF_HMI_UART in config:
       hmi_uart = config[CONF_HMI_UART]
       if CONF_UART_ID in hmi_uart:
          parent = await cg.get_variable(hmi_uart[CONF_UART_ID])
          cg.add(var.set_required_hmi_uart(parent))
       else:
          raise cv.Invalid(
             f"Setting 'uart_id' is required !"
          )
