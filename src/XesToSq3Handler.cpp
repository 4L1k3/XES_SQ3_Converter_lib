#include <iostream>
#include <sqlite3.h>
#include <string>
#include <map>

#include "includes/XesToSq3Handler.hpp"
#include "includes/XesSq3Converter.hpp"
#include "includes/PrepareDatabase.hpp"
#include "includes/utils.hpp"
#include "includes/XesElements.hpp"
#include "includes/exceptions.hpp"


using namespace xercesc;


XesToSq3Handler::XesToSq3Handler(XesToSq3Converter* converter) : numSeq(0), converter(converter){ }
XesToSq3Handler::~XesToSq3Handler() {}

XesToSq3Converter::XesToSq3Converter(sqlite3* db): db(db) {}
XesToSq3Converter::~XesToSq3Converter() {}


Log XesToSq3Converter::create_log() {
    std::string attribute_container_id = std::to_string(allocate_atribute_container(db));
    std::string eid = std::to_string(allocate_event_container(db));
    char* err;
    std::string query = "INSERT INTO Logs(LogID, EventContainerID) VALUES(" +
                    attribute_container_id + ", " +
                    eid + ");";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    } else {
        std::cout << "database create log successful\n\n";
    }
    log = Log(attribute_container_id, eid);
    return log;
}


Extension XesToSq3Converter::create_extension(std::string name, std::string prefix, std::string uri) {
    char* err;
    std::string query = "INSERT INTO Extensions(LogID, Name, Prefix, Uri) VALUES(" +
                        log.get_attribute_container_id() + ", '" +
                        name + "', '" +
                        prefix + "', '" +
                        uri + "');";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    } else {
        std::cout << "database create extension successful\n\n";
    }
    return Extension(std::to_string(sqlite3_last_insert_rowid(db)), name, prefix, uri);
}


Global XesToSq3Converter::create_global(std::string scope) {
    std::string attribute_container_id = std::to_string(allocate_atribute_container(db));
    char* err;
    std::string query = "INSERT INTO Globals(LogID, GlobalID, Scope) VALUES(" +
                        log.get_attribute_container_id() + ", " +
                        attribute_container_id + ", '" +
                        scope + "');";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    } else {
        std::cout << "database create global successful\n\n";
    }
    return Global(attribute_container_id, scope);
}


Classifier XesToSq3Converter::create_classifier(std::string name, std::vector<std::string>& keys, std::string scope) {
    char* err;
    std::string query = "INSERT INTO Classifiers(LogID, Name, Scope) VALUES(" +
                        log.get_attribute_container_id() + ", '" +
                        name + "', '" +
                        scope + "');";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    } else {
        std::cout << "database create classifier successful\n\n";
    }
    long long id = sqlite3_last_insert_rowid(db);
    for (auto key: keys) {
        char* err;
        std::string query = "INSERT INTO ClassifierKeys(ClassifierID, Key) VALUES(" +
                            std::to_string(id) + ", '" +
                            key + "');";
        rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
        if (rc != SQLITE_OK) {
            throw DataBaseQueryExecuteError(query, err);
        } else {
            std::cout << "database create classifier keys successful\n\n";
        }
    }
    return Classifier(std::to_string(id), name, keys, scope);
}


Trace XesToSq3Converter::create_trace() {
    commit(db);
    begin(db);
    std::string attribute_container_id = std::to_string(allocate_atribute_container(db));
    std::string eid = std::to_string(allocate_event_container(db));
    char* err;
    std::string query = "INSERT INTO Traces(TraceID, LogID, EventsID) VALUES(" +
                    attribute_container_id + ", " +
                    log.get_attribute_container_id() + ", " +
                    eid + ");";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    } else {
        std::cout << "database create trace successful\n\n";
    }
    return Trace(attribute_container_id, eid);
}


Event XesToSq3Converter::create_event() {
    std::string attribute_container_id = std::to_string(allocate_atribute_container(db));
    char* err;
    std::string contained_event_cont_id = event_containers.empty() ? "null" : event_containers.top().get_event_container_id();
    std::string query = "INSERT INTO Events(EventID, ContainedEventContainerID) VALUES(" +
                    attribute_container_id + ", " +
                    contained_event_cont_id + ");";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    } else {
        std::cout << "database create event successful\n\n";
    }
    return Event(attribute_container_id);
}


ListAttributeValues XesToSq3Converter::create_list_values() {
    if (attribute_containers.empty()) {
        throw EmptyAttributeContainersError("Attribute containers must be not empty");
    }
    char* err;
    std::string attribute_container_id = std::to_string(allocate_atribute_container(db));
    std::string contained_attribute_cont_id = attribute_containers.top().get_attribute_container_id();
    std::string query = "UPDATE Attributes SET Value = " +
                        attribute_container_id + " WHERE AttributeID = " +
                        contained_attribute_cont_id + ";";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    } else {
        std::cout << "database create attribute successful\n\n";
    }
    return ListAttributeValues(attribute_container_id);
}


std::shared_ptr<Attribute> XesToSq3Converter::create_attribute(AttributeType attribute_type, std::string key, std::string value) {
    std::string attribute_container_id = std::to_string(allocate_atribute_container(db));
    ListAttributeValues list_attribute_values;

    char* err;
    std::string contained_attribute_cont_id = attribute_containers.empty() ? "null" : attribute_containers.top().get_attribute_container_id();
    std::string query = "INSERT INTO Attributes(AttributeID, ContainedAttributeContainerID, Type, Key, Value) VALUES(" +
                    attribute_container_id + ", " +
                    contained_attribute_cont_id + ", '" +
                    attribute_type_to_str(attribute_type) + "', '" +
                    key + "', '" +
                    value + "');";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    } else {
        std::cout << "database create attribute successful\n\n";
    }

    if (attribute_type == AttributeType::DATE) {
        std::shared_ptr<DateAttribute> tmp = std::make_shared<DateAttribute>(attribute_container_id, key, value);
        return std::static_pointer_cast<Attribute>(tmp);
    }
    if (attribute_type == AttributeType::INT) {
        std::shared_ptr<IntAttribute> tmp = std::make_shared<IntAttribute>(attribute_container_id, key, value);
        return std::static_pointer_cast<Attribute>(tmp);
    }
    if (attribute_type == AttributeType::FLOAT) {
        std::shared_ptr<FloatAttribute> tmp = std::make_shared<FloatAttribute>(attribute_container_id, key, value);
        return std::static_pointer_cast<Attribute>(tmp);
    }
    if (attribute_type == AttributeType::BOOLEAN) {
        std::shared_ptr<BooleanAttribute> tmp = std::make_shared<BooleanAttribute>(attribute_container_id, key, value);
        return std::static_pointer_cast<Attribute>(tmp);
    }
    if (attribute_type == AttributeType::ID) {
        std::shared_ptr<IdAttribute> tmp = std::make_shared<IdAttribute>(attribute_container_id, key, value);
        return std::static_pointer_cast<Attribute>(tmp);
    }
    if (attribute_type == AttributeType::LIST) {
        std::shared_ptr<ListAttribute> tmp = std::make_shared<ListAttribute>(attribute_container_id, key, list_attribute_values);
        return std::static_pointer_cast<Attribute>(tmp);
    }
    std::shared_ptr<StringAttribute> tmp = std::make_shared<StringAttribute>(attribute_container_id, key, value);
    return std::static_pointer_cast<Attribute>(tmp);
}


void XesToSq3Converter::push_attribute_container(AttributeContainer attr_cont) {
    attribute_containers.push(attr_cont);
}


void XesToSq3Converter::push_event_container(EventContainer event_cont) {
    event_containers.push(event_cont);
}


void XesToSq3Converter::pop_attribute_container() {
    attribute_containers.pop();
}


void XesToSq3Converter::pop_event_container() {
    event_containers.pop();
}


void XesToSq3Handler::startElement(const XMLCh* const name, AttributeList& attributes)
{
    numSeq++;
    std::cout << numSeq << "\t-> Start : " << toCh(name) << std::endl;
    if (toCh(name) == (std::string)"log") {
        Log log = converter->create_log();
        converter->push_attribute_container(log);
        converter->push_event_container(log);
        return;
    }
    if (toCh(name) == (std::string)"extension") {
        const std::map<std::string, std::string> attrs = get_attributes(attributes);
        assert_contains_attributes("extension", attrs, {"name", "prefix", "uri"});
        converter->create_extension(attrs.at("name"), attrs.at("prefix"), attrs.at("uri"));
        return;
    }
    if (toCh(name) == (std::string)"global") {
        const std::map<std::string, std::string> attrs = get_attributes(attributes);
        std::string scope = "event";
        if (attrs.find("scope") != attrs.end() && attrs.at("scope") == "trace") {
            scope = "trace";
        }
        Global global_element = converter->create_global(scope);
        converter->push_attribute_container(global_element);
        return;
    }
    if (toCh(name) == (std::string)"classifier") {
        const std::map<std::string, std::string> attrs = get_attributes(attributes);
        assert_contains_attributes("classifier", attrs, {"name", "keys"});
        std::string scope = "event";
        if (attrs.find("scope") != attrs.end() && attrs.at("scope") == "trace") {
            scope = "trace";
        }
        std::vector<std::string> keys = split(attrs.at("keys"));
        converter->create_classifier(attrs.at("name"), keys, scope);
        return;
    }
    if (toCh(name) == (std::string)"trace") {
        Trace trace = converter->create_trace();
        converter->push_attribute_container(trace);
        converter->push_event_container(trace);
        return;
    }
    if (toCh(name) == (std::string)"event") {
        Event event = converter->create_event();
        converter->push_attribute_container(event);
        return;
    }
    if (
        (toCh(name) == (std::string)"string") or
        (toCh(name) == (std::string)"date") or
        (toCh(name) == (std::string)"int") or
        (toCh(name) == (std::string)"float") or
        (toCh(name) == (std::string)"boolean") or
        (toCh(name) == (std::string)"id") or
        (toCh(name) == (std::string)"list")
    ) {
        std::string tag_name = to_lower(toCh(name));
        AttributeType attribute_type = get_attribute_type(tag_name);

        const std::map<std::string, std::string> attrs = get_attributes(attributes);
        assert_contains_attributes(tag_name, attrs, {"key"});
        std::string key = attrs.at("key");
        std::string value;
        if (attribute_type != AttributeType::LIST) {
            assert_contains_attributes(tag_name, attrs, {"value"});
            value = attrs.at("value");
        }

        std::shared_ptr<Attribute> attribute = converter->create_attribute(attribute_type, key, value);
        converter->push_attribute_container(*attribute);
        return;
    }
    if (toCh(name) == (std::string)"values") {
        ListAttributeValues list_attr_values = converter->create_list_values();
        converter->push_attribute_container(list_attr_values);
        return;
    }
    XMLSize_t len = attributes.getLength();
    for (XMLSize_t index = 0; index < len; index++) {
        std::cout << "\t\t-> " << toCh(attributes.getName(index))
             << "="
             << toCh(attributes.getValue(index)) << std::endl;
    }
    throw UnknownXesTagNameException(toCh(name));
}


void XesToSq3Handler::endElement(const XMLCh* const name)
{
    numSeq++;
    std::cout << numSeq <<  "\t-> End   : " << toCh(name) << std::endl;
    if (
        (toCh(name) == (std::string)"log") or
        (toCh(name) == (std::string)"global") or
        (toCh(name) == (std::string)"trace") or
        (toCh(name) == (std::string)"event") or
        (toCh(name) == (std::string)"string") or
        (toCh(name) == (std::string)"date") or
        (toCh(name) == (std::string)"int") or
        (toCh(name) == (std::string)"float") or
        (toCh(name) == (std::string)"boolean") or
        (toCh(name) == (std::string)"id") or
        (toCh(name) == (std::string)"list") or
        (toCh(name) == (std::string)"values")
    ) {
        converter->pop_attribute_container();
    }
    if ((toCh(name) == (std::string)"log") or (toCh(name) == (std::string)"trace")) {
        converter->pop_event_container();
    }
}


void XesToSq3Handler::fatalError(const SAXParseException& exception)
{
    std::cout << "Fatal Error: " << toCh(exception.getMessage())
         << " at line: " << exception.getLineNumber()
         << std::endl;
}