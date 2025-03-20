#pragma once
#include <regex>
#include "Cleaner.h"
#include <cassert>


namespace fs = std::filesystem;

static bool Found_Test(fs::path path, std::regex rx) {
    return std::regex_match(path.string(), rx);
}



void RunTests() {
    std::string path = fs::current_path().string() + "\\TestDirectory";
    std::regex rx(".*\\.vcxproj$");
    std::ofstream log;
    log.open("log.txt");
    for (const auto& p : fs::recursive_directory_iterator(path)) {
        if (!fs::is_directory(p)) {
            if (Found_Test(p, rx) && (!IsXml(p))) {
                log << p.path().filename() << " is not XML" << std::endl;
            }
            if (Found_Test(p, rx) && IsXml(p) && (!IsProject(p))) {
                log << p.path().filename() << " is not VsProject" << std::endl;
            }
            if (Found_Test(p, rx) && IsXml(p) && IsProject(p)) {
                log << p.path().filename() << " is cleaned" << std::endl;
            }

        }
    }
    log.close();
    std::ifstream file("log.txt");
    std::string str;
    std::string file_contents;
    while (std::getline(file, str))
    {
        file_contents += str;
        file_contents.push_back('\n');
    }

    std::string line_log = "\"ewsConfig.vcxproj\" is cleaned";
    std::string line_log2 = "\"NotProject.vcxproj\" is not VsProject";
    std::string line_log3 = "\"notxml.vcxproj\" is not XML";
    std::string line_log4 = "\"Projects.vcxproj\" is cleaned";
    std::string result_log = line_log + '\n' + line_log2 + '\n' + line_log3 + '\n' + line_log4;
    assert(file_contents.c_str(), result_log.c_str());
    std::cout << "Test finished succesfully" <<std::endl;
    return;
}
