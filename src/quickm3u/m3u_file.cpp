#include <quickm3u/m3u_file.hpp>

#include <fstream>
#include <string>

M3UFile m3u_load(std::filesystem::path const& p)
{
    M3UFile ret;
    ret.filename = p;
    std::ifstream fin(p);
    std::string line;
    bool is_first_line = true;
    bool is_extended = false;
    M3UEntry entry;
    while (std::getline(fin, line)) {
        if (line.starts_with("#")) {
            if (is_first_line) {
                if (line == "#EXTM3U") {
                    is_extended = true;
                }
            } else if (is_extended) {
                if (line.starts_with("#EXTINF:")) {
                    auto const length_end = line.find(',', 8);
                    auto const length_str = line.substr(8, length_end - 8);
                    entry.runtime = std::chrono::seconds(std::stoi(length_str));
                    auto const artist_end = line.find('-', length_end);
                    auto const artist_str = line.substr(length_end + 1, artist_end - length_end - 2);
                    entry.artist = artist_str;
                    auto const title_str = line.substr(artist_end + 2);
                    entry.title = title_str;
                }
            }
        } else {
            entry.path = line;
            ret.entries.emplace_back(std::move(entry));
            entry = M3UEntry{};
        }
        is_first_line = false;
    }
    return ret;
}

M3UFile m3u_save(std::filesystem::path const& p);
M3UFile m3u_save_as_extended(std::filesystem::path const& p);
