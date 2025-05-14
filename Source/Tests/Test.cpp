#include <PrettyPrint/PrettyPrint.h>

int main() {
    {
    std::string text =
    "     Random text garbage 012322525236\n"
    "  Random text garbage 1123\n"
    "Random text garbage 2232\n"
    "   Random text garbage 3323252\n"
    "          Random text garbage 4434\n";

    aby::util::pretty_print(text, "FOO_BAR_XAN", aby::util::Colors{.box = aby::util::EColor::GREEN});
    }

    std::string red    = "\033[1;31m";
    std::string green  = "\033[1;32m";
    std::string yellow = "\033[1;33m";
    std::string reset  = "\033[0m";

    std::string text =
        "     " + red    + "Random" + reset + " text garbage 012322525236\n"
        "  "    + green  + "Random text" + reset + " garbage 1123\n"
        " "     + yellow + "Random text garbage" + reset + " 2232\n"
        "   Random " + red + "text" + reset + " garbage 3323252\n"
        "      Random text garbage " + green + "4434" + reset + "\n";

    aby::util::pretty_print(text, "FOO_BAR_XAN", aby::util::Colors{.box = aby::util::EColor::GREEN});
}