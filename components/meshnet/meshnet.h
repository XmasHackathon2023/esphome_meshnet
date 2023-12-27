#pragma once

#ifdef USE_ESP32

#include <string>
#include "esphome/core/component.h"


namespace esphome {
namespace meshnet {

class Meshnet : public PollingComponent {
  public:
    void setup() override;
    void update() override;
    void dump_config() override;
    void on_shutdown() override;

    // User configurable parameters
    void set_private_key(const std::string &private_key);
    void set_auth_token(const std::string &auth_token);
    void set_hardware_id(const std::string &hardware_id);

  protected:
    std::string private_key_;
    std::string auth_token_;
    std::string hardware_id_;
};

/// Strip most part of the key only for secure printing
std::string mask_key(const std::string &key);

}
}


#endif // USE_ESP32
