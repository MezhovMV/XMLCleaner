#pragma once
#include "pugixml.hpp"
#include <string.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>



// functions checks if one of atributes contains Win32
static bool Contains(std::string str, std::string keyword) {
    size_t pos = str.find(keyword);
    if (pos != std::string::npos) { return true; }
    else
    {
        return false;
    }
}

// saving marked nodes here
std::vector <pugi::xml_node> nodes_to_delete;

// auxiliary element for traversal iteration https://pugixml.org/docs/manual.html#access.walker
const char* node_types[] =
{
    "null", "document", "element", "pcdata", "cdata", "comment", "pi", "declaration"
};

// auxiliary element for traversal iteration https://pugixml.org/docs/manual.html#access.walker
struct simple_walker : pugi::xml_tree_walker
{
    std::string keyword = "Win32";

    virtual bool for_each(pugi::xml_node& node)
    {

        for (int i = 0; i < depth(); ++i);

        // Saving nodes with atributes which contains Win32
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute())

        {
            if (Contains(attr.value(), keyword)) {

                nodes_to_delete.push_back(node);
            };


        }

        return true; // continue traversal
    }
};
std::string GetResultName(std::filesystem::path path) {
    std::string name = path.string();
    auto pos = name.find(".vcxproj");
    name.insert(pos, ".Cleaned");
    return name;
};

void CleanWin32(std::filesystem::path path) {
    //Loading file
    pugi::xml_document doc;
    std::ifstream stream(path);
    pugi::xml_parse_result result = doc.load(stream);

    // Traverse with marking
    simple_walker walker;
    doc.traverse(walker);

    //Deleting marked nodes
    for (int i = 0; i < nodes_to_delete.size(); i++) {
        nodes_to_delete[i].parent().remove_child(nodes_to_delete[i]);
    }



    // save document to file
    doc.save_file(GetResultName(path).c_str());
    nodes_to_delete.clear(); // deleting nodes for next iteration
    return;
}

//Checks if file is xml
bool IsXml(std::filesystem::path path) {
    pugi::xml_document doc;
    std::ifstream stream(path);
    if (!doc.load(stream)) {
        return false;
    }
    return true;
}

//Checks if file is Project
bool IsProject(std::filesystem::path path) {
    pugi::xml_document doc;
    std::ifstream stream(path);
    doc.load(stream);
    if (doc.child("Project").empty()) {
        return false;
    }
    return true;

}