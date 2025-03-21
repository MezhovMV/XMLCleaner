#pragma once
#include <regex>
#include "Cleaner.h"
#include "Test.h"
#include <stdexcept>

namespace fs = std::filesystem;

static bool Found(fs::path path, std::regex rx) {
    return std::regex_match(path.string(), rx);
}

int main(int argc, char* argv[])
{
    RunTests();
    std::string path;
    std::cout << "Please write directory path" << std::endl;
    std::cout << "For example : current path is " << fs::current_path() << std::endl;
    
    while (true) {
        try { std::cin >> path; 
        std::regex rx(".*\\.vcxproj$");
        std::ofstream log;
        log.open("log.txt");

        for (const auto& p : fs::recursive_directory_iterator(path.c_str())) {
            if (!fs::is_directory(p)) {
                if (Found(p, rx) && (!IsXml(p))) {
                    log << p.path().filename() << " is not XML" << std::endl;
                }
                if (Found(p, rx) && IsXml(p) && (!IsProject(p))) {
                    log << p.path().filename() << " is not VsProject" << std::endl;
                }
                if (Found(p, rx) && IsXml(p) && IsProject(p)) {
                    CleanWin32(p);
                    log << p.path().filename() << " is cleaned" << std::endl;
                }

            }
        }

        std::cout << "Program has finished succesfully, log is saved in program direcory";
        break;
        }
        catch (fs::filesystem_error &ex) {
            std::cout << "Invalid path" << std::endl;
        }
    };


}