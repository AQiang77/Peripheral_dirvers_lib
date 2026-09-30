#pragma once

#include <cstddef>
#include <cstdint>
#include <driver/i2c_master.h>
#include <pdl/interfaces/i2c_device.hpp>

namespace pdl::esp_idf
{
class I2cBus;

class I2cDevice final : public pdl::I2cDevice
{
public:
    I2cDevice(
        I2cBus &bus,
        std::uint16_t address,
        std::uint32_t frequency_hz) noexcept;
    I2cDevice(
        i2c_master_bus_handle_t bus,
        std::uint16_t address,
        std::uint32_t frequency_hz) noexcept;
    ~I2cDevice() override;

    I2cDevice(const I2cDevice &) = delete;
    I2cDevice &operator=(const I2cDevice &) = delete;

    [[nodiscard]] Status initialize() override;
    [[nodiscard]] Status deinitialize() override;

    Status write(const std::uint8_t *data, std::size_t size,
                 std::uint32_t timeout_ms) override;
    Status read(std::uint8_t *data, std::size_t size,
                std::uint32_t timeout_ms) override;
    Status writeRead(
        const std::uint8_t *write_data,
        std::size_t write_size,
        std::uint8_t *read_data,
        std::size_t read_size,
        std::uint32_t timeout_ms) override;

private:
    i2c_master_bus_handle_t bus_;
    i2c_master_dev_handle_t device_{nullptr};
    std::uint16_t address_;
    std::uint32_t frequency_hz_;
};
}
