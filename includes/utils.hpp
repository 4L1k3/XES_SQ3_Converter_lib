#pragma once

#include <iostream>
#include <sqlite3.h>
#include <string>
#include <vector>
#include <xercesc/sax/AttributeList.hpp>
#include <map>

#include "includes/XesElements.hpp"

using namespace xercesc;


void assert_contains_attributes(
    std::string tag_name,
    const std::map<std::string, std::string>& attributes,
    const std::vector<std::string>& params
);
const std::map<std::string, std::string> get_attributes(AttributeList& attributes);
std::string to_lower(std::string str);
std::vector<std::string> split(std::string str, char sep=' ');
AttributeType get_attribute_type(std::string tag_name);
std::string attribute_type_to_str(AttributeType type);
std::string delete_last_word(const std::string& str, char sep);
