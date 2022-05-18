#pragma once

#include <iostream>
#include <unordered_map>
#include <stack>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


class XesWriter {
public:
    XesWriter();
    void write_start_tag(std::string tag_name);
    void write_end_tag();
    void write_attributes(const std::unordered_map<std::string, std::string>& attrs);
    void write_attribute(std::string key, std::string value);
    void write_to_file(std::string filename);

private:
    boost::property_tree::ptree tree;
    std::stack<boost::property_tree::ptree*> trees;
    char sep = '/';
};
