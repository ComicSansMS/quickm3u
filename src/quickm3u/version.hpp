#ifndef INCLUDE_GUARD_QUICKM3U_VERSION_HPP
#define INCLUDE_GUARD_QUICKM3U_VERSION_HPP

#include <cstdint>

struct QuickM3UVersion {
    std::uint8_t major;
    std::uint8_t minor;
    std::uint8_t patch;
};

inline constexpr QuickM3UVersion quickm3u_version() {
    return QuickM3UVersion{ 0, 9, 0 };
}

#endif
