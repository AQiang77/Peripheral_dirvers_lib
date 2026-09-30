#pragma once

#include <cstdint>

#include <pdl/core/status.hpp>
#include <pdl/interfaces/i2c_device.hpp>

namespace pdl::charge
{
class Axp517
{
public:
    static constexpr std::uint16_t defaultI2cAddress = 0x34;

    explicit Axp517(I2cDevice &device) noexcept;

    [[nodiscard]] Status initialize();

    [[nodiscard]] Status deinitialize();

    // Reads the 16-bit TCPC Vendor ID. The output is unchanged on failure.
    [[nodiscard]] Status readId(
        std::uint16_t &id,
        std::uint32_t timeout_ms = 100) const;

private:
    I2cDevice &device_;
    bool initialized_{false};
};
}
