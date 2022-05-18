#include <iostream>
#include <algorithm>
#include <exception>
#include <map>
#include <sqlite3.h>
#include <string>
#include <sstream>
#include <vector>
#include <xercesc/sax/AttributeList.hpp>
#include "includes/PrepareDatabase.hpp"
#include "includes/XesSq3Converter.hpp"
#include "includes/utils.hpp"
#include "includes/exceptions.hpp"
#include "includes/XesElements.hpp"

using namespace xercesc;


std::string to_lower(std::string str) {
    std::for_each(str.begin(), str.end(), [](char & c){
        c = std::tolower(c);
    });
    return str;
}


std::vector<std::string> split(const std::string str, char sep) {
    std::vector<std::string> tmp;
    std::string s = "";
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] != sep) {
            s += str[i];
        } else {
            if (s != "") {
                tmp.push_back(s);
                s = "";
            }
        }
    }
    if (s != "") {
        tmp.push_back(s);
    }
    return tmp;
}


static int callback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}


long long get_lrid(sqlite3* db) {
    char* err;
    string query = "SELECT last_insert_rowid();";
    std::cout << "database last rowid query = " << query << "\n\n";
    int rc = sqlite3_exec(db, query.c_str(), callback, NULL, &err);
    if (rc != SQLITE_OK) {
        cout << "database last rowid error: " << err << "\n\n";
    } else {
        std::cout << "database last rowid successful\n\n";
    }
    return 1;
}

const std::map<std::string, std::string> get_attributes(AttributeList& attributes) {
    XMLSize_t len = attributes.getLength();
    std::map<std::string, std::string> attrs;
    for (XMLSize_t index = 0; index < len; index++) {
        std::string key = toCh(attributes.getName(index));
        std::string value = toCh(attributes.getValue(index));
        attrs[to_lower(key)] = value;
    }
    return attrs;
}


void assert_contains_attributes(std::string tag_name, const std::map<std::string, std::string>& attrs, const std::vector<std::string>& keys) {
    std::cout << "assert_contains inned" << std::endl;
    for (auto key: keys) {
        if (attrs.find(key) == attrs.end()) {
            std::stringstream msg;
            msg << "Missing " << key << " attribute in " << tag_name;
            throw MissingAttributeException(msg.str());
        }
    }
    return;
}


AttributeType get_attribute_type(std::string tag_name) {
    tag_name = to_lower(tag_name);
    if (tag_name == "string") {
        return AttributeType::STRING;
    }
    if (tag_name == "date") {
        return AttributeType::DATE;
    }
    if (tag_name == "int") {
        return AttributeType::INT;
    }
    if (tag_name == "float") {
        return AttributeType::FLOAT;
    }
    if (tag_name == "boolean") {
        return AttributeType::BOOLEAN;
    }
    if (tag_name == "id") {
        return AttributeType::ID;
    }
    if (tag_name == "list") {
        return AttributeType::LIST;
    }
    std::stringstream msg;
    msg << "Unknown XES attribute tag name " << tag_name;
    throw NotXesAttributeTagException(msg.str());
}


std::string attribute_type_to_str(AttributeType type) {
    if (type == AttributeType::DATE) {
        return "date";
    }
    if (type == AttributeType::INT) {
        return "int";
    }
    if (type == AttributeType::FLOAT) {
        return "float";
    }
    if (type == AttributeType::BOOLEAN) {
        return "boolean";
    }
    if (type == AttributeType::ID) {
        return "id";
    }
    if (type == AttributeType::LIST) {
        return "list";
    }
    return "string";
}


std::string delete_last_word(const std::string& str, char sep) {
    int last_sep_ind = str.find_last_of(sep);
    if (last_sep_ind == -1) {
        return "";
    }
    return str.substr(0, last_sep_ind);
}
