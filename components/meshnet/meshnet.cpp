#include "meshnet.h"

#ifdef USE_ESP32

#include "esphome/core/application.h"
#include "esphome/core/log.h"
#include "esphome/components/network/util.h"
#include <esp_err.h>

#include <wireguard.h>

namespace esphome {
namespace meshnet {

static const char *const TAG = "meshnet";

void Meshnet::setup() {
    ESP_LOGD(TAG, "Meshnet module starting up...");
    esp_err_t err = ESP_FAIL;

    esp_log_level_set("esp_wireguard", ESP_LOG_DEBUG);
    esp_log_level_set("wireguardif", ESP_LOG_DEBUG);
    esp_log_level_set("wireguard", ESP_LOG_DEBUG);

    // Configure wireguard
    this->wg_config = ESP_WIREGUARD_CONFIG_DEFAULT();
    this->wg_config.private_key = "<REDACTED>";
    this->wg_config.listen_port = 51820;
    this->wg_config.public_key = "pKPnO/qgxlJQ276cVobJdN1o+qwKPZPoyLjEMowaQxU=";
    this->wg_config.preshared_key = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=";
    this->wg_config.allowed_ip = "100.92.236.82";
    this->wg_config.allowed_ip_mask = "255.192.0.0";
    this->wg_config.endpoint = "192.168.88.246";
    this->wg_config.port = 49031;
    this->wg_config.persistent_keepalive = 5;

    err = esp_wireguard_init(&this->wg_config, &this->ctx);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Failed to initialize wireguard %d", err);
    } else {
        ESP_LOGI(TAG, "esp_wireguard initialized succesfully");
    }
}

void Meshnet::update() {
    esp_err_t err = ESP_FAIL;

    if (network::is_connected() && !this->meshnet_enabled) {
        ESP_LOGD(TAG, "Network was brought up, starting meshnet");
        this->start_meshnet_();
    } else if(!network::is_connected && this->meshnet_enabled) {
        ESP_LOGD(TAG, "Network was brought down, stopping meshnet");
        this->stop_meshnet_();
    }
}

void Meshnet::on_shutdown() {
    ESP_LOGD(TAG, "Shutting down meshnet...");
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

void Meshnet::start_meshnet_() {
    esp_err_t err = ESP_FAIL;

    err = esp_wireguard_connect(&this->ctx);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Failed to connect to peer %d", err);
    } else {
        ESP_LOGI(TAG, "Attempting to connect to peer succeded");
    }

    this->meshnet_enabled = true;
}

void Meshnet::stop_meshnet_() {
    esp_err_t err = ESP_FAIL;

    err = esp_wireguard_disconnect(&ctx);
    if (err != ESP_OK) {
        ESP_LOGW(TAG, "Failed to disconnect from peer %d", err);
    } else {
        ESP_LOGI(TAG, "Attempting to disconnect from peer succeded");
    }


    this->meshnet_enabled = false;
}

std::string mask_key(const std::string &key) { return (key.substr(0, 5) + "[...]="); }

}  // namespace meshnet
}  // namespace esphome


#endif // USE_ESP32
