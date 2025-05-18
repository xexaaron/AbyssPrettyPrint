#include "PrettyPrint/PrettyPrint.h"
#include <vector>
#include <iostream>
#include <regex>
#ifdef _WIN32
    #include <windows.h>
    #include <io.h>
    #define isatty _isatty
    #define fileno _fileno
#else
    #include <unistd.h>
#endif

namespace aby::util {

    bool supports_ansi_seq() {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return false;
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        return SetConsoleMode(hOut, dwMode) != 0;
#else
        const char* term = std::getenv("TERM");
        return isatty(fileno(stdout)) && term && std::string(term) != "dumb";
#endif
    }

    bool supports_utf8() {
#ifdef _WIN32
        // Check for UTF-8 code page (65001) on Windows
        return GetConsoleOutputCP() == 65001;
#else
        // Check if the terminal's locale supports UTF-8
        const char* lang = std::getenv("LANG");
        if (lang && std::string(lang).find("UTF-8") != std::string::npos) {
            return true;
        }
        return false;
#endif
    }

    bool can_pretty_print() {
        return supports_utf8() && supports_ansi_seq();
    }

    std::size_t count_ansi_chars(const std::string& text) {
        std::regex ansi_regex(R"(\x1B\[[0-9;]*m)");
        std::size_t count = 0;
        auto begin = std::sregex_iterator(text.begin(), text.end(), ansi_regex);
        auto end = std::sregex_iterator();
        for (auto it = begin; it != end; ++it) {
            count += it->length(); // add length of each ANSI sequence
        }
        return count;
    }

    std::size_t calc_max_line_width(const std::vector<std::string>& lines) {
        std::size_t max_width = 0;
        for (const auto& line : lines) {
            std::size_t visible_len = line.size() - count_ansi_chars(line);
            if (visible_len > max_width) max_width = visible_len;
        }
        return max_width;
    }

    std::vector<std::string> split_string(const std::string& str) {
        std::vector<std::string> lines;
        std::string line;
        for (char c : str) {
            if (c == '\n') {
                lines.push_back(line);
                line.clear();
            } else {
                line.push_back(c);
            }
        }
        if (!line.empty()) {
            lines.push_back(line);
        }
        return lines;
    }

    std::pair<std::string, std::string> make_box_top_bottom(std::size_t max_width, const std::string& name, const std::string& box_color, const std::string& ctx_color) {
        std::string reset = "\033[0m";
        std::string top    = box_color + "┌";
        top.append("─");
        top.append(reset + ctx_color + name + "\033[0m" + box_color);
        for (std::size_t i = 0; i < (max_width + 2) - (name.size() + 1); i++) {
            top.append("─");
        }
        top.append("┐" + reset);
        std::string bottom = box_color + "└"; 
            for (std::size_t i = 0; i < max_width + 2; i++) {
            bottom.append("─");
        }
        bottom.append("┘" + reset);
        return { top, bottom };
    }

    std::string color_to_ansi(EColor color, bool bold) {
        std::string out_color = "\033[";
        if (bold) out_color.append("1;");

        switch (color) {
            case EColor::BLACK:    out_color.append("30m"); break;
            case EColor::RED:      out_color.append("31m"); break;
            case EColor::GREEN:    out_color.append("32m"); break;
            case EColor::YELLOW:   out_color.append("33m"); break;
            case EColor::BLUE:     out_color.append("34m"); break;
            case EColor::MAGENTA:  out_color.append("35m"); break;
            case EColor::CYAN:     out_color.append("36m"); break;
            case EColor::WHITE:    out_color.append("37m"); break;
            case EColor::GREY:     out_color.append("90m"); break; 
            default:               return ""; 
        }

        return out_color;
    }

    bool pretty_print(const std::string& str, const std::string& ctx, Colors colors) {
        if (!can_pretty_print()) return false;

        auto box_color     = color_to_ansi(colors.box, true);
        auto ctx_color     = color_to_ansi(colors.ctx, true);
        auto reset         = "\033[0m";
        auto lines         = split_string(str);
        auto max_width     = calc_max_line_width(lines);
        auto [top, bottom] = make_box_top_bottom(max_width, ctx, box_color, ctx_color);

        std::cout << top << std::endl;
        for (auto& line : lines) {
            std::cout << box_color << "│" << reset
                      << line << std::string(max_width - (line.size() - count_ansi_chars(line)) + 2, ' ')
                      << box_color << "│" << reset
                      << std::endl;
        }
        std::cout << bottom << std::endl;

        return true;
    }   

    std::size_t visual_width(const std::string& str) {
        return str.size() - count_ansi_chars(str);
    }



}
