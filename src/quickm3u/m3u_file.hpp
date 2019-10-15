#ifndef INCLUDE_GUARD_QUICKM3U_M3U_FILE_HPP_
#define INCLUDE_GUARD_QUICKM3U_M3U_FILE_HPP_

#include <chrono>
#include <filesystem>
#include <istream>
#include <ostream>
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
M3UFile m3u_load(std::istream& is);
void m3u_save(M3UFile const& m3u);
void m3u_save(M3UFile const& m3u, std::ostream& os);
void m3u_save_as_extended(M3UFile const& m3u, std::filesystem::path const& p);
void m3u_convert_to_relative_paths(M3UFile& m3u);
void m3u_convert_to_absolute_paths(M3UFile& m3u);

#endif
