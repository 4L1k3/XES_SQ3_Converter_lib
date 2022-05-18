#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <includes/exceptions.hpp>


MissingAttributeException::MissingAttributeException(std::string msg): msg(msg) {}
NotXesAttributeTagException::NotXesAttributeTagException(std::string msg): msg(msg) {}
UnknownXesTagNameException::UnknownXesTagNameException(std::string msg): msg(msg) {}
WriteToXesException::WriteToXesException(std::string level, char* err) {
    std::stringstream m;
    m << "Write " << level << " error:\n" << err;
    msg = m.str();
}