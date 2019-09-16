#ifndef INCLUDE_GUARD_QUICKM3U_M3U_FILE_HPP_
#define INCLUDE_GUARD_QUICKM3U_M3U_FILE_HPP_

#include <chrono>
#include <filesystem>
#include <vector>

struct M3UEntry {
    std::filesystem::path path;
    std::chrono::seconds runtime = std::chrono::seconds(0);
    std::string artist;
    std::string title;
};

struct M3UFile {
    std::filesystem::path filename;
    std::vector<M3UEntry> entries;
};

M3UFile m3u_load(std::filesystem::path const& p);
M3UFile m3u_save(std::filesystem::path const& p);
M3UFile m3u_save_as_extended(std::filesystem::path const& p);

#endif
