import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv

from .. import CONF_POWERMUST_ID, POWERMUST_COMPONENT_SCHEMA

DEPENDENCIES = ["uart"]

CONF_LAST_Q1 = "last_q1"
CONF_LAST_F = "last_f"

TYPES = [
    CONF_LAST_Q1,
    CONF_LAST_F,
]

CONFIG_SCHEMA = POWERMUST_COMPONENT_SCHEMA.extend(
    {cv.Optional(type): text_sensor.text_sensor_schema() for type in TYPES}
)


async def to_code(config):
    paren = await cg.get_variable(config[CONF_POWERMUST_ID])

    for type in TYPES:
        if type in config:
            conf = config[type]
            var = await text_sensor.new_text_sensor(conf)
            cg.add(getattr(paren, f"set_{type}")(var))
