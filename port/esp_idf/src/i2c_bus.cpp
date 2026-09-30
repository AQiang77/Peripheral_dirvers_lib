#include <pdl/ports/esp_idf/i2c_bus.hpp>

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
        return pdl::Status::busy;
    case ESP_ERR_NO_MEM:
        return pdl::Status::no_memory;
    case ESP_ERR_NOT_FOUND:
        return pdl::Status::busy;
    default:
        return pdl::Status::io_error;
    }
}
}

namespace pdl::esp_idf
{
I2cBus::~I2cBus()
{
    if (handle_ != nullptr)
        i2c_del_master_bus(handle_);
}

Status I2cBus::initialize(gpio_num_t sda, gpio_num_t scl)
{
    Config config{};
    config.sda = sda;
    config.scl = scl;
    return initialize(config);
}

Status I2cBus::initialize(const Config &config)
{
    if (handle_ != nullptr)
        return Status::already_initialized;
    if (config.sda == GPIO_NUM_NC || config.scl == GPIO_NUM_NC ||
        config.sda == config.scl)
        return Status::invalid_argument;

    i2c_master_bus_config_t bus_config{};
    bus_config.i2c_port = config.port;
    bus_config.sda_io_num = config.sda;
    bus_config.scl_io_num = config.scl;
    bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
    bus_config.glitch_ignore_cnt = config.glitch_ignore_count;
    bus_config.flags.enable_internal_pullup = config.enable_internal_pullup;

    return toStatus(i2c_new_master_bus(&bus_config, &handle_));
}

Status I2cBus::deinitialize()
{
    if (handle_ == nullptr)
        return Status::not_initialized;

    const esp_err_t error = i2c_del_master_bus(handle_);
    if (error == ESP_OK)
        handle_ = nullptr;
    return toStatus(error);
}

bool I2cBus::isInitialized() const noexcept
{
    return handle_ != nullptr;
}

i2c_master_bus_handle_t I2cBus::nativeHandle() const noexcept
{
    return handle_;
}
}
