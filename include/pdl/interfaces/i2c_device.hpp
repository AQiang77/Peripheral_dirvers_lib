#pragma once

#include <cstddef>
#include <cstdint>

#include <pdl/core/status.hpp>

namespace pdl
{
class I2cDevice
{
public:
    virtual ~I2cDevice() = default;
    virtual Status initialize() = 0;
    virtual Status deinitialize() = 0;
    
    virtual Status write(
        const std::uint8_t *data,
        std::size_t size,
        std::uint32_t timeout_ms) = 0;

    virtual Status read(
        std::uint8_t *data,
        std::size_t size,
        std::uint32_t timeout_ms) = 0;

    // Keeps repeated-start between write and read operations.
    virtual Status writeRead(
        const std::uint8_t *write_data,
        std::size_t write_size,
        std::uint8_t *read_data,
        std::size_t read_size,
        std::uint32_t timeout_ms) = 0;
};
}
