#include <quickm3u/m3u_file.hpp>

#include <fstream>
#include <string>

void to_relative_path(std::filesystem::path const& root, M3UEntry& entry)
{
    auto const& p = entry.path;
    auto [it_root, it_p] = std::mismatch(root.begin(), root.end(), p.begin(), p.end());

    std::filesystem::path rel_path;
    for (; it_root != root.end(); ++it_root) { rel_path /= ".."; }
    for (; it_p != p.end(); ++it_p) { rel_path /= *it_p; }
    entry.path.assign(std::move(rel_path));
}

void to_absolute_path(std::filesystem::path const& root, M3UEntry& entry)
{
    auto const& p = entry.path;
    std::filesystem::path abs_path = root;
    auto it = p.begin();
    for (; (it != p.end()) && (*it == ".."); ++it) { abs_path = abs_path.parent_path(); }
    for (; it != p.end(); ++it) { abs_path /= *it; }

    entry.path.assign(std::move(abs_path));
}

M3UFile m3u_load(std::filesystem::path const& p)
{
    std::ifstream fin(p);
    M3UFile ret = m3u_load(fin);
    ret.filename = p;
    return ret;
}

M3UFile m3u_load(std::istream& is)
{
    M3UFile ret;
    std::string line;
    bool is_first_line = true;
    bool is_extended = false;
    M3UEntry entry;
    while (std::getline(is, line)) {
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

void m3u_save(M3UFile const& m3u)
{
    std::ofstream fout(m3u.filename);
    m3u_save(m3u, fout);
}

void m3u_save(M3UFile const& m3u, std::ostream& os)
{
    for (auto const& p : m3u.entries) {
        os << p.path << '\n';
    }
}

void m3u_save_as_extended(M3UFile const& m3u, std::filesystem::path const& p);

void m3u_convert_to_relative_paths(M3UFile& m3u)
{
    std::filesystem::path root = m3u.filename;
    root = root.remove_filename().parent_path();
    for (auto& e : m3u.entries) {
        if (e.path.is_absolute()) {
            to_relative_path(root, e);
        }
    }
}

void m3u_convert_to_absolute_paths(M3UFile& m3u)
{
    std::filesystem::path root = m3u.filename;
    root = root.remove_filename().parent_path();
    for (auto& e : m3u.entries) {
        if (e.path.is_relative()) {
            to_absolute_path(root, e);
        }
    }
}
