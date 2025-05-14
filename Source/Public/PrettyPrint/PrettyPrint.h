#pragma once
#include <string>

namespace aby::util {

    enum class EColor {
        WHITE = 0,
        YELLOW,
        MAGENTA,
        CYAN,
        GREY,
        GREEN,
        RED,
        BLACK,
        BLUE
    };

    struct Colors {
        EColor box = EColor::GREEN;
        EColor ctx = EColor::YELLOW;
    };

    bool pretty_print(const std::string& str, const std::string& ctx = "", Colors colors = {});

}