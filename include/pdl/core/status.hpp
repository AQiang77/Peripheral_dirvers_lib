#pragma once

#include <cstdint>

namespace pdl
{
    enum class Status : std::uint8_t
    {
        ok = 0,

        invalid_argument,
        not_initialized,
        already_initialized,

        device_not_found,
        busy,
        timeout,
        no_memory,

        io_error,
        invalid_data,
        unsupported,
        internal_error,
    };

    [[nodiscard]] constexpr bool isOk(Status status) noexcept
    {
        return status == Status::ok;
    }

    [[nodiscard]] constexpr bool isError(Status status) noexcept
    {
        return !isOk(status);
    }

    [[nodiscard]] constexpr const char *statusName(Status status) noexcept
    {
        switch (status)
        {
        case Status::ok:
            return "ok";
        case Status::invalid_argument:
            return "invalid_argument";
        case Status::not_initialized:
            return "not_initialized";
        case Status::already_initialized:
            return "already_initialized";
        case Status::device_not_found:
            return "device_not_found";
        case Status::busy:
            return "busy";
        case Status::timeout:
            return "timeout";
        case Status::no_memory:
            return "no_memory";
        case Status::io_error:
            return "io_error";
        case Status::invalid_data:
            return "invalid_data";
        case Status::unsupported:
            return "unsupported";
        case Status::internal_error:
            return "internal_error";
        }

        return "unknown";
    }
}