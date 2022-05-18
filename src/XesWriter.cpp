#include <iostream>
#include <string>
#include <stack>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "includes/XesWriter.hpp"
#include "includes/utils.hpp"


XesWriter::XesWriter() {
    trees.push(&tree);
}


void XesWriter::write_start_tag(std::string tag) {
    boost::property_tree::ptree* last_tree = trees.top();
    trees.push(&last_tree->add(tag, ""));
}


void XesWriter::write_end_tag() {
    trees.pop();
}


void XesWriter::write_attributes(const std::unordered_map<std::string, std::string>& attrs) {
    boost::property_tree::ptree* last_tree = trees.top();
    for (auto attr: attrs) {
        std::string key = attr.first;
        std::string value = attr.second;
        last_tree->add(boost::property_tree::ptree::path_type{"<xmlattr>/" + key, '/'}, value);
    }
}


void XesWriter::write_attribute(std::string key, std::string value) {
    boost::property_tree::ptree* last_tree = trees.top();
    last_tree->add(boost::property_tree::ptree::path_type{"<xmlattr>/" + key, '/'}, value);
}


void XesWriter::write_to_file(std::string filename) {
    boost::property_tree::ptree* tree = trees.top();
    boost::property_tree::xml_writer_settings<std::string> settings(' ', 4);
    boost::property_tree::write_xml(filename, *tree, std::locale(), settings);
    return;
}
