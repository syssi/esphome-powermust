import esphome.codegen as cg
from esphome.components import uart
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ["uart"]
CODEOWNERS = ["@syssi"]
AUTO_LOAD = ["binary_sensor", "sensor", "switch", "text_sensor"]
MULTI_CONF = True

CONF_POWERMUST_ID = "powermust_id"

powermust_ns = cg.esphome_ns.namespace("powermust")
PowermustComponent = powermust_ns.class_("Powermust", cg.Component)

POWERMUST_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.Required(CONF_POWERMUST_ID): cv.use_id(PowermustComponent),
    }
)

CONFIG_SCHEMA = cv.All(
    cv.Schema({cv.GenerateID(): cv.declare_id(PowermustComponent)})
    .extend(cv.polling_component_schema("1s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)
