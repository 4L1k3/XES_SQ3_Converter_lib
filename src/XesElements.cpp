#include <iostream>
#include <string>
#include "includes/XesElements.hpp"
#include "includes/utils.hpp"


AttributeContainer::AttributeContainer(std::string attribute_container_id): attribute_container_id(attribute_container_id) {}
AttributeContainer::~AttributeContainer() {}

EventContainer::EventContainer(std::string event_container_id): event_container_id(event_container_id) {}
EventContainer::~EventContainer() {}


std::string AttributeContainer::get_attribute_container_id() {
    return attribute_container_id;
}


std::string EventContainer::get_event_container_id() {
    return event_container_id;
}


Log::Log(std::string attribute_container_id, std::string event_container_id):
    AttributeContainer(attribute_container_id), EventContainer(event_container_id) {}
Log::~Log() {}


Extension::Extension(std::string id, std::string name, std::string prefix, std::string uri):
    id(id), name(name), prefix(prefix), uri(uri) {}
Extension::~Extension() {}


Global::Global(std::string attribute_container_id, std::string scope):
    AttributeContainer(attribute_container_id), scope(scope) {}
Global::~Global() {}


Classifier::Classifier(std::string id, std::string name, std::vector<std::string> keys, std::string scope):
    id(id), name(name), keys(keys), scope(scope) {}
Classifier::~Classifier() {}


Trace::Trace(std::string attribute_container_id, std::string event_container_id):
    AttributeContainer(attribute_container_id), EventContainer(event_container_id) {}
Trace::~Trace() {}


Event::Event(std::string attribute_container_id):
    AttributeContainer(attribute_container_id) {}
Event::~Event() {}


ListAttributeValues::ListAttributeValues(std::string attribute_container_id):
    AttributeContainer(attribute_container_id) {}
ListAttributeValues::~ListAttributeValues() {}


Attribute::Attribute(std::string attribute_container_id, std::string key):
    AttributeContainer(attribute_container_id), key(key) {}
Attribute::~Attribute() {}


StringAttribute::StringAttribute(std::string attribute_container_id, std::string key, std::string value):
    Attribute(attribute_container_id, key), value(value) {}
StringAttribute::~StringAttribute() {}
AttributeType StringAttribute::Type() {
    return AttributeType::STRING;
}


DateAttribute::DateAttribute(std::string attribute_container_id, std::string key, std::string value):
    Attribute(attribute_container_id, key), value(value) {}
DateAttribute::~DateAttribute() {}
AttributeType DateAttribute::Type() {
    return AttributeType::DATE;
}


IntAttribute::IntAttribute(std::string attribute_container_id, std::string key, std::string value):
    Attribute(attribute_container_id, key), value(value) {}
IntAttribute::~IntAttribute() {}
AttributeType IntAttribute::Type() {
    return AttributeType::INT;
}


FloatAttribute::FloatAttribute(std::string attribute_container_id, std::string key, std::string value):
    Attribute(attribute_container_id, key), value(value) {}
FloatAttribute::~FloatAttribute() {}
AttributeType FloatAttribute::Type() {
    return AttributeType::FLOAT;
}


BooleanAttribute::BooleanAttribute(std::string attribute_container_id, std::string key, std::string value):
    Attribute(attribute_container_id, key), value(value) {}
BooleanAttribute::~BooleanAttribute() {}
AttributeType BooleanAttribute::Type() {
    return AttributeType::BOOLEAN;
}


IdAttribute::IdAttribute(std::string attribute_container_id, std::string key, std::string value):
    Attribute(attribute_container_id, key), value(value) {}
IdAttribute::~IdAttribute() {}
AttributeType IdAttribute::Type() {
    return AttributeType::ID;
}


ListAttribute::ListAttribute(std::string attribute_container_id, std::string key, ListAttributeValues values):
    Attribute(attribute_container_id, key), values(values) {}
ListAttribute::~ListAttribute() {}
AttributeType ListAttribute::Type() {
    return AttributeType::LIST;
}
