#include <pdl/ports/esp_idf/i2c_device.hpp>

#include <pdl/ports/esp_idf/i2c_bus.hpp>

#include <limits>

namespace
{
pdl::Status toStatus(esp_err_t error) noexcept
{
    switch (error)
    {
    case ESP_OK:
        return pdl::Status::ok;
    case ESP_ERR_INVALID_ARG:
        return pdl::Status::invalid_argument;
    case ESP_ERR_INVALID_STATE:
        return pdl::Status::not_initialized;
    case ESP_ERR_TIMEOUT:
        return pdl::Status::timeout;
    case ESP_ERR_NO_MEM:
        return pdl::Status::no_memory;
    case ESP_ERR_NOT_FOUND:
        return pdl::Status::device_not_found;
    default:
        return pdl::Status::io_error;
    }
}

bool isValidTimeout(std::uint32_t timeout_ms) noexcept
{
    return timeout_ms > 0 &&
           timeout_ms <= static_cast<std::uint32_t>(std::numeric_limits<int>::max());
}
}

namespace pdl::esp_idf
{
I2cDevice::I2cDevice(
    I2cBus &bus,
    std::uint16_t address,
    std::uint32_t frequency_hz) noexcept
    : I2cDevice(bus.nativeHandle(), address, frequency_hz)
{
}

I2cDevice::I2cDevice(
    i2c_master_bus_handle_t bus,
    std::uint16_t address,
    std::uint32_t frequency_hz) noexcept
    : bus_(bus), address_(address), frequency_hz_(frequency_hz)
{
}

I2cDevice::~I2cDevice()
{
    if (device_ != nullptr)
        i2c_master_bus_rm_device(device_);
}

Status I2cDevice::initialize()
{
    if (device_ != nullptr)
        return Status::already_initialized;
    if (bus_ == nullptr || address_ > 0x7FU || frequency_hz_ == 0)
        return Status::invalid_argument;

    i2c_device_config_t config{};
    config.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    config.device_address = address_;
    config.scl_speed_hz = frequency_hz_;

    return toStatus(i2c_master_bus_add_device(bus_, &config, &device_));
}

Status I2cDevice::deinitialize()
{
    if (device_ == nullptr)
        return Status::not_initialized;

    const esp_err_t error = i2c_master_bus_rm_device(device_);
    if (error == ESP_OK)
        device_ = nullptr;
    return toStatus(error);
}

Status I2cDevice::write(
    const std::uint8_t *data,
    std::size_t size,
    std::uint32_t timeout_ms)
{
    if (device_ == nullptr)
        return Status::not_initialized;
    if (data == nullptr || size == 0 || !isValidTimeout(timeout_ms))
        return Status::invalid_argument;

    return toStatus(i2c_master_transmit(
        device_, data, size, static_cast<int>(timeout_ms)));
}

Status I2cDevice::read(
    std::uint8_t *data,
    std::size_t size,
    std::uint32_t timeout_ms)
{
    if (device_ == nullptr)
        return Status::not_initialized;
    if (data == nullptr || size == 0 || !isValidTimeout(timeout_ms))
        return Status::invalid_argument;

    return toStatus(i2c_master_receive(
        device_, data, size, static_cast<int>(timeout_ms)));
}

Status I2cDevice::writeRead(
    const std::uint8_t *write_data,
    std::size_t write_size,
    std::uint8_t *read_data,
    std::size_t read_size,
    std::uint32_t timeout_ms)
{
    if (device_ == nullptr)
        return Status::not_initialized;
    if (write_data == nullptr || write_size == 0 || read_data == nullptr ||
        read_size == 0 || !isValidTimeout(timeout_ms))
        return Status::invalid_argument;

    return toStatus(i2c_master_transmit_receive(
        device_,
        write_data,
        write_size,
        read_data,
        read_size,
        static_cast<int>(timeout_ms)));
}
}
