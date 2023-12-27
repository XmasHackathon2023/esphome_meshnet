#include "meshnet.h"

#ifdef USE_ESP32

#include "esphome/core/application.h"
#include "esphome/core/log.h"
#include <esp_err.h>

namespace esphome {
namespace meshnet {

static const char *const TAG = "meshnet";

void Meshnet::setup() {
    ESP_LOGD(TAG, "Initializing Meshnet...");

    // TODO: add meshnet logic here
}

void Meshnet::update() {
    // TODO: add meshnet status update here
}

void Meshnet::on_shutdown() {
    ESP_LOGD(TAG, "Shutting down meshnet...");

    // TODO: add meshnet shutdown logic here
}

void Meshnet::dump_config() {
  ESP_LOGCONFIG(TAG, "Meshnet:");
  ESP_LOGCONFIG(TAG, "  Private Key: " LOG_SECRET("%s"), mask_key(this->private_key_).c_str());
  ESP_LOGCONFIG(TAG, "  Authorization token: " LOG_SECRET("%s"), mask_key(this->auth_token_).c_str());
  ESP_LOGCONFIG(TAG, "  Hardware ID: " "%s", this->hardware_id_.c_str());
}

void Meshnet::set_private_key(const std::string &private_key) { this->private_key_ = private_key; }
void Meshnet::set_auth_token(const std::string &auth_token) { this->auth_token_ = auth_token; }
void Meshnet::set_hardware_id(const std::string &hardware_id) { this->hardware_id_ = hardware_id; }

std::string mask_key(const std::string &key) { return (key.substr(0, 5) + "[...]="); }

}  // namespace meshnet
}  // namespace esphome


#endif // USE_ESP32
