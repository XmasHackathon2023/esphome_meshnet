import re
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import (
        CONF_NAME,
        CONF_ID
    )

CONF_AUTH_TOKEN = "authorization_token"
CONF_PRIVATE_KEY = "private_key"
CONF_HARDWARE_ID = "hardware_id"

CODEOWNERS = ["@jauler", "@matislovas", "@stalowyjez", "@baltrus1"]

_WG_KEY_REGEX = re.compile(r"^[A-Za-z0-9+/]{42}[AEIMQUYcgkosw480]=$")
_AUTH_TOKEN_REGEX = re.compile(r"[0-9a-f]{64}")
_HARDWARE_ID_REGEX = re.compile(r"[0-9a-z-]{1,32}")

meshnet_ns = cg.esphome_ns.namespace("meshnet")
Meshnet = meshnet_ns.class_("Meshnet", cg.Component, cg.PollingComponent)


def _wireguard_key(value):
    if _WG_KEY_REGEX.match(cv.string(value)) is not None:
        return value
    raise cv.Invalid(f"Invalid WireGuard key: {value}")

def _auth_token(value):
    if _AUTH_TOKEN_REGEX.match(cv.string(value)) is not None:
        return value
    raise cv.Invalid(f"Invalid WireGuard key: {value}")

def _hardware_id(value):
    if _HARDWARE_ID_REGEX.match(cv.string(value)) is not None:
        return value
    raise cv.Invalid(f"Invalid hardware identifier: {value}")


CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(Meshnet),
        cv.Required(CONF_AUTH_TOKEN): _auth_token,
        cv.Required(CONF_PRIVATE_KEY): _wireguard_key,
        cv.Required(CONF_HARDWARE_ID): _hardware_id,
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    cg.add(var.set_private_key(config[CONF_PRIVATE_KEY]))
    cg.add(var.set_auth_token(config[CONF_AUTH_TOKEN]))
    cg.add(var.set_hardware_id(config[CONF_HARDWARE_ID]))

    cg.add_build_flag(f"-DCONFIG_WIREGUARD_MAX_SRC_IPS=2")
    cg.add_build_flag(f"-DCONFIG_WIREGUARD_MAX_PEERS=2")
    cg.add_build_flag(f"-DCONFIG_MAX_INITIATIONS_PER_SECOND=1")
    cg.add_build_flag(f"-DCONFIG_WIREGUARD_ESP_NETIF")
    cg.add_build_flag(f"-DCONFIG_WIREGUARD_x25519_IMPLEMENTATION_DEFAULT")
    cg.add_library("esp-wg", None)

    await cg.register_component(var, config)

