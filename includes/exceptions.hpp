#pragma once

#include <iostream>
#include <exception>
#include <string>

class MissingAttributeException: public std::exception {
public:
    MissingAttributeException(std::string msg);
    virtual const char* what() const noexcept override {return msg.c_str();}
private:
    std::string msg;
};


class NotXesAttributeTagException: public std::exception {
public:
    NotXesAttributeTagException(std::string msg);
    virtual const char* what() const noexcept override {return msg.c_str();}
private:
    std::string msg;
};


class UnknownXesTagNameException: public std::exception {
public:
    UnknownXesTagNameException(std::string msg);
    virtual const char* what() const noexcept override {return msg.c_str();}
private:
    std::string msg;
};


class WriteToXesException: public std::exception {
public:
    WriteToXesException(std::string level, char* err);
    virtual const char* what() const noexcept override {return msg.c_str();}
private:
    std::string msg;
};
