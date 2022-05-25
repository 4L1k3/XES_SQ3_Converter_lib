#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <includes/exceptions.hpp>


MissingAttributeException::MissingAttributeException(std::string key, std::string tag_name) {
    std::stringstream m;
    m << "Missing " << key << " attribute in " << tag_name;
    msg = m.str();
}


NotXesAttributeTagException::NotXesAttributeTagException(std::string tag_name) {
    std::stringstream m;
    m << "Unknown XES attribute tag name " << tag_name;
    msg  = m.str();
}


UnknownXesTagNameException::UnknownXesTagNameException(std::string tag_name) {
    std::stringstream m;
    m << "Unknown XES tag name " << tag_name;
    msg = m.str();
}


WriteToXesException::WriteToXesException(std::string level, char* err) {
    std::stringstream m;
    m << "Write " << level << " error:\n" << err;
    msg = m.str();
}


DataBaseQueryExecuteError::DataBaseQueryExecuteError(std::string query, char* err) {
    std::stringstream m;
    m << "Database query execute error:\n" << err << "\nQuery:\n" << query;
    msg = m.str();
}


EmptyAttributeContainersError::EmptyAttributeContainersError(std::string msg): msg(msg) {}
NoValuesTagInListError::NoValuesTagInListError(std::string msg): msg(msg) {}
