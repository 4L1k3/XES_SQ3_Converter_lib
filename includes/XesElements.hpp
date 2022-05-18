#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>


class AttributeContainer {
public:
    AttributeContainer() = default;
    AttributeContainer(std::string attribute_container_id);
    ~AttributeContainer();
    std::string get_attribute_container_id();
protected:
    std::string attribute_container_id;
};


class EventContainer {
public:
    EventContainer() = default;
    EventContainer(std::string event_container_id);
    ~EventContainer();
    std::string get_event_container_id();
protected:
    std::string event_container_id;
};



class Log: public AttributeContainer, public EventContainer {
public:
    Log() = default;
    Log(std::string attribute_container_id, std::string event_container_id);
    ~Log();
};


class Extension {
public:
    Extension() = default;
    Extension(std::string id, std::string name, std::string prefix, std::string uri);
    ~Extension();
    std::string id;
    std::string name;
    std::string prefix;
    std::string uri;
};


class Global: public AttributeContainer {
public:
    Global() = default;
    Global(std::string attribute_container_id, std::string scope);
    ~Global();
    std::string scope;
};


class Classifier {
public:
    Classifier() = default;
    Classifier(std::string id, std::string name, std::vector<std::string> keys, std::string scope);
    ~Classifier();
    std::string id;
    std::string name;
    std::vector<std::string> keys;
    std::string scope;
};


class Trace: public AttributeContainer, public EventContainer {
public:
    Trace() = default;
    Trace(std::string attribute_container_id, std::string event_container_id);
    ~Trace();
};


class Event: public AttributeContainer {
public:
    Event() = default;
    Event(std::string attribute_container_id);
    ~Event();
};


enum AttributeType {
    STRING, DATE, INT, FLOAT, BOOLEAN, ID, LIST
};


class Attribute: public AttributeContainer {
public:
    Attribute() = default;
    Attribute(std::string attribute_container_id, std::string key);
    ~Attribute();
    AttributeType Type();
    std::string key;
};


class ListAttributeValues: public AttributeContainer {
public:
    ListAttributeValues() = default;
    ListAttributeValues(std::string attribute_container_id);
    ~ListAttributeValues();
};


class StringAttribute: public Attribute {
public:
    StringAttribute(std::string attribute_container_id, std::string key, std::string value);
    ~StringAttribute();
    AttributeType Type();
    std::string value;
};


class DateAttribute: public Attribute {
public:
    DateAttribute(std::string attribute_container_id, std::string key, std::string value);
    ~DateAttribute();
    AttributeType Type();
    std::string value;
};


class IntAttribute: public Attribute {
public:
    IntAttribute(std::string attribute_container_id, std::string key, std::string value);
    ~IntAttribute();
    AttributeType Type();
    std::string value;
};


class FloatAttribute: public Attribute {
public:
    FloatAttribute(std::string attribute_container_id, std::string key, std::string value);
    ~FloatAttribute();
    AttributeType Type();
    std::string value;
};


class BooleanAttribute: public Attribute {
public:
    BooleanAttribute(std::string attribute_container_id, std::string key, std::string value);
    ~BooleanAttribute();
    AttributeType Type();
    std::string value;
};


class IdAttribute: public Attribute {
public:
    IdAttribute(std::string attribute_container_id, std::string key, std::string value);
    ~IdAttribute();
    AttributeType Type();
    std::string value;
};


class ListAttribute: public Attribute {
public:
    ListAttribute(std::string attribute_container_id, std::string key, ListAttributeValues values);
    ~ListAttribute();
    AttributeType Type();
    ListAttributeValues values;
};
