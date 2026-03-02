module;
#ifdef _WIN32
#include <windows.h>
#endif
module Logger;

using namespace debug;
constexpr std::string_view CYAN    = "\033[36m"; // [cite: 75]
constexpr std::string_view RED     = "\033[31m"; // [cite: 75]
constexpr std::string_view YELLOW  = "\033[33m"; // [cite: 75]
constexpr std::string_view DEFAULT = "\033[0m";
static std::ofstream fout;
static std::mutex mutex;
static std::string utcOffset = "";

LogMessage::~LogMessage() {
    logger->log(level, ss.str());
}

static void write(LogLevel level, const std::string& name, const std::string& message) {
#ifdef NDEBUG
    if (level == LogLevel::debug) return;
#endif
    char levelTag;
    std::string_view color;
    switch (level) {
    case LogLevel::attention: levelTag = 'A'; color = CYAN;    break;
    case LogLevel::debug:     levelTag = 'D'; color = DEFAULT; break;
    case LogLevel::info:      levelTag = 'I'; color = DEFAULT; break;
    case LogLevel::warning:   levelTag = 'W'; color = YELLOW;  break;
    case LogLevel::error:     levelTag = 'E'; color = RED;     break;
    }

    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::string finalString = std::format("[{}] {:%Y/%m/%d %H:%M:%S}.{:03d}{} [{:>20}] {}",
        levelTag, now, ms.count(), utcOffset, name, message);
    {
        std::lock_guard lock(mutex);
        if (fout.good()) {
            fout << finalString << '\n';
            fout.flush();
        }
        std::println("{}{}", color, finalString);
    }
}

void Logger::init(const std::string& filename) {
#ifdef _WIN32
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD consoleMode = 0;
    if (GetConsoleMode(consoleHandle, &consoleMode))
        SetConsoleMode(consoleHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
    fout.open(filename);
    auto now = std::chrono::system_clock::now();
    try {
        auto local_tz = std::chrono::current_zone();
        auto offset = local_tz->get_info(now).offset;
        auto hours = std::chrono::duration_cast<std::chrono::hours>(offset);
        utcOffset = std::format("{:+03d}00", hours.count());
    }
    catch (...) {
        utcOffset = "+0000";
    }
}

void Logger::log(LogLevel level, const std::string& message) const {
    write(level, name, message);
}
