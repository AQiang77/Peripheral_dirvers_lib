#include <pdl/drivers/charge/axp517/axp517.hpp>
#include "axp517_registers.hpp"

namespace pdl::charge
{
    Axp517::Axp517(I2cDevice &device) noexcept
        : device_(device)
    {
    }

    Status Axp517::initialize()
    {
        if (initialized_)
            return Status::already_initialized;

        Status status = device_.initialize();
        if (isError(status))
            return status;

        std::uint16_t id{};
        status = readId(id);
        if (isError(status))
        {
            device_.deinitialize();
            return status;
        }

        if (id != 0x1F3A && id != 0xE0C5)
        {
            device_.deinitialize();
            return Status::device_not_found;
        }

        initialized_ = true;
        return Status::ok;
    }

    Status Axp517::deinitialize()
    {
        if (!initialized_)
            return Status::not_initialized;

        const Status status = device_.deinitialize();
        if (isOk(status))
            initialized_ = false;

        return status;
    }

    Status Axp517::readId(std::uint16_t &id, std::uint32_t timeout_ms) const
    {
        if (timeout_ms == 0)
            return Status::invalid_argument;

        const std::uint8_t register_address = AXP517_REG_TCPC_VENDOR_ID_L;
        std::uint8_t data[2]{};

        const Status status = device_.writeRead(
            &register_address,
            sizeof(register_address),
            data,
            sizeof(data),
            timeout_ms);
        if (isError(status))
            return status;

        id = static_cast<std::uint16_t>(data[0]) |
             static_cast<std::uint16_t>(static_cast<std::uint16_t>(data[1]) << 8U);
        return Status::ok;
    }
}
