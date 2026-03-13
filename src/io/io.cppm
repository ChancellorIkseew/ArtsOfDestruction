module;
export module IO;

import std;
import Logger;

namespace fs = std::filesystem;
static debug::Logger logger("io");

template <class... Args>
static std::vector<std::byte> stopAndLog(const std::format_string<Args...> fmt, Args&&... args) {
    logger.error() << std::format(fmt, std::forward<Args>(args)...);
    return std::vector<std::byte>();
}

export namespace IO {
    std::vector<std::byte> readBinFile(const fs::path& path) {
        std::error_code errCode;
        if (!fs::exists(path, errCode) || !fs::is_regular_file(path, errCode))
            return stopAndLog("File does not exist: {} {}", fs::absolute(path).string(), errCode.message());
        //
        std::ifstream fin(path, std::ios::binary | std::ios::ate);
        if (!fin.is_open())
            return stopAndLog("Failed to open file: {}", fs::absolute(path).string());
        //
        std::streamsize size = fin.tellg();
        fin.seekg(0, std::ios::beg);
        std::vector<std::byte> buffer(size);
        if (!fin.read(reinterpret_cast<char*>(buffer.data()), size))
            return stopAndLog("Failed to read file: {}", fs::absolute(path).string());
        //
        return buffer;
    }

    std::string readTextFile(const fs::path& path) {
        auto blob = readBinFile(path);
        if (blob.empty())
            return "";
        return std::string(reinterpret_cast<const char*>(blob.data()), blob.size());
    }
};
