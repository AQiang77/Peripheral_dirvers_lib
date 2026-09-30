#pragma once

#include <cstdint>
#include <driver/i2c_master.h>
#include <pdl/core/status.hpp>

namespace pdl::esp_idf
{
class I2cBus
{
public:
    struct Config
    {
        gpio_num_t sda;
        gpio_num_t scl;
        i2c_port_num_t port{I2C_NUM_0};
        std::uint8_t glitch_ignore_count{7};
        bool enable_internal_pullup{false};
    };

    I2cBus() = default;
    ~I2cBus();

    I2cBus(const I2cBus &) = delete;
    I2cBus &operator=(const I2cBus &) = delete;

    Status initialize(gpio_num_t sda, gpio_num_t scl);
    Status initialize(const Config &config);
    Status deinitialize();

    [[nodiscard]] bool isInitialized() const noexcept;
    [[nodiscard]] i2c_master_bus_handle_t nativeHandle() const noexcept;

private:
    i2c_master_bus_handle_t handle_{nullptr};
};
}
