#include <CLI/CLI11.hpp>
#include <iostream>
#include "application/application.hpp"

// STB implementation has to go somewhere
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

int main(int argc, char** argv) {
#ifndef NDEBUG
    std::cout << "Debug build!\n\n";
#endif

    int seed = time(0);

    CLI::App cli{"Stardank"};
    cli.set_version_flag("-v,--version", "Stardank v0.0.1", "Display Stardank version information");
    cli.add_option("-s,--seed", seed, "Specify random seed value");

    CLI11_PARSE(cli, argc, argv);

    srand(seed);

    std::cout << "Random seed: " << seed << "\n";

    try {
        auto app = Application(argc, argv);
        app.run();
    } catch (char const* msg) {
        std::cerr << msg << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception\n";
        return 1;
    }

    return 0;
}
