#pragma once

#include <iostream>
#include <unordered_map>
#include <string>

#include <sqlite3.h>

#include "includes/XesElements.hpp"
#include "includes/XesWriter.hpp"
#include "includes/exceptions.hpp"


XesWriter WRITER;
sqlite3* db;
std::vector<std::string> classifier_keys;


static int write_extension_callback(void *data, int argc, char **argv, char **azColName) {
    std::unordered_map<std::string, std::string> value;
    for (int i = 0; i < argc; ++i) {
        value[azColName[i]] = argv[i] ? argv[i] : "null";
    }
    WRITER.write_start_tag("extension");
    WRITER.write_attribute("name", value["Name"]);
    WRITER.write_attribute("prefix", value["Prefix"]);
    WRITER.write_attribute("uri", value["Uri"]);
    WRITER.write_end_tag();
    return 0;
}


void write_extension(Log log) {
    char* err;
    std::string log_id = log.get_attribute_container_id();
    std::string query = "SELECT LogID, Name, Prefix, Uri FROM Extensions WHERE LogID = "
                        + log_id + ";";
    int rc = sqlite3_exec(db, query.c_str(), write_extension_callback, NULL, &err);
    if (rc != SQLITE_OK) {
        throw WriteToXesException("extensions", err);
    }
    return;
}


void write_attributes(AttributeContainer container);


static int write_attribute_callback(void *data, int argc, char **argv, char **azColName) {
    std::unordered_map<std::string, std::string> value;
    for (int i = 0; i < argc; ++i) {
        value[azColName[i]] = argv[i] ? argv[i] : "null";
    }
    Attribute last_attribute = Attribute(value["AttributeID"], value["key"]);
    WRITER.write_start_tag(value["Type"]);
    WRITER.write_attribute("key", value["Key"]);
    if (value["Type"] != "list") {
        WRITER.write_attribute("value", value["Value"]);
    }
    write_attributes(last_attribute);
    if (value["Type"] == "list") {
        if (value["Value"].empty()) {
            throw NoValuesTagInListError("Need values tag in list");
        }
        Attribute list_attribute_values = Attribute(value["Value"], "");
        {
            WRITER.write_start_tag("values");
            write_attributes(list_attribute_values);
            WRITER.write_end_tag();
        }
    }
    WRITER.write_end_tag();
    return 0;
}


void write_attributes(AttributeContainer container) {
    char* err;
    std::string attribute_container_id = container.get_attribute_container_id();
    std::string query = "SELECT AttributeID, Type, Key, Value FROM Attributes WHERE ContainedAttributeContainerID = " +
                        attribute_container_id + ";";
    int rc = sqlite3_exec(db, query.c_str(), write_attribute_callback, NULL, &err);
    if (rc != SQLITE_OK) {
        throw WriteToXesException("attributes", err);
    }
    return;
}


static int write_global_callback(void *data, int argc, char **argv, char **azColName) {
    std::unordered_map<std::string, std::string> value;
    for (int i = 0; i < argc; ++i) {
        value[azColName[i]] = argv[i] ? argv[i] : "null";
    }
    Global global = Global(value["GlobalID"], value["Scope"]);
    WRITER.write_start_tag("global");
    WRITER.write_attribute("scope", global.scope);
    write_attributes(global);
    WRITER.write_end_tag();
    return 0;
}


void write_globals(Log log) {
    char* err;
    std::string log_id = log.get_attribute_container_id();
    std::string query = "SELECT GlobalID, Scope FROM Globals WHERE LogID = "
                        + log_id + ";";
    int rc = sqlite3_exec(db, query.c_str(), write_global_callback, NULL, &err);
    if (rc != SQLITE_OK) {
        throw WriteToXesException("globals", err);
    }
    return;
}


static int get_classifier_keys_callback(void *data, int argc, char **argv, char **azColName) {
    std::unordered_map<std::string, std::string> value;
    for (int i = 0; i < argc; ++i) {
        value[azColName[i]] = argv[i] ? argv[i] : "null";
    }
    classifier_keys.push_back(value["Key"]);
    return 0;
}


void get_classifier_keys(std::string id) {
    char* err;
    std::string query = "SELECT Key FROM ClassifierKeys WHERE ClassifierID = " + id + ";";
    int rc = sqlite3_exec(db, query.c_str(), get_classifier_keys_callback, NULL, &err);
    if (rc != SQLITE_OK) {
        throw WriteToXesException("classifiers when getting keys", err);
    }
    return;
}


std::string get_str_classifier_keys(std::vector<std::string>& keys) {
    std::string str = "";
    if (keys.empty()) {
        return str;
    }

    if (keys.size() == 1) {
        return keys[0];
    }
    str = keys[0];
    for (size_t i = 1; i < keys.size(); ++i) {
        str += " " + keys[i];
    }
    return str;
}


static int write_classifier_callback(void *data, int argc, char **argv, char **azColName) {
    std::unordered_map<std::string, std::string> value;
    for (int i = 0; i < argc; ++i) {
        value[azColName[i]] = argv[i] ? argv[i] : "null";
    }
    classifier_keys.clear();
    get_classifier_keys(value["ClassifierID"]);
    Classifier classifier = Classifier(value["LogID"], value["Name"], classifier_keys, value["Scope"]);
    WRITER.write_start_tag("classifier");
    WRITER.write_attribute("name", classifier.name);
    WRITER.write_attribute("scope", classifier.scope);
    WRITER.write_attribute("keys", get_str_classifier_keys(classifier.keys));
    WRITER.write_end_tag();
    return 0;
}


void write_classifiers(Log log) {
    char* err;
    std::string log_id = log.get_attribute_container_id();
    std::string query = "SELECT ClassifierID, LogID, Name, Scope FROM Classifiers WHERE LogID = "
                        + log_id + ";";
    int rc = sqlite3_exec(db, query.c_str(), write_classifier_callback, NULL, &err);
    if (rc != SQLITE_OK) {
        throw WriteToXesException("classifiers", err);
    }
    return;
}


static int write_event_callback(void *data, int argc, char **argv, char **azColName) {
    std::unordered_map<std::string, std::string> value;
    for (int i = 0; i < argc; ++i) {
        value[azColName[i]] = argv[i] ? argv[i] : "null";
    }
    Event event = Event(value["EventID"]);
    WRITER.write_start_tag("event");
    write_attributes(event);
    WRITER.write_end_tag();
    return 0;
}


void write_events(EventContainer container) {
    char* err;
    std::string event_container_id = container.get_event_container_id();
    std::string query = "SELECT EventID FROM Events WHERE ContainedEventContainerID = "
                        + event_container_id + ";";
    int rc = sqlite3_exec(db, query.c_str(), write_event_callback, NULL, &err);
    if (rc != SQLITE_OK) {
        throw WriteToXesException("events", err);
    }
    return;
}


static int write_trace_callback(void *data, int argc, char **argv, char **azColName) {
    std::unordered_map<std::string, std::string> value;
    for (int i = 0; i < argc; ++i) {
        value[azColName[i]] = argv[i] ? argv[i] : "null";
    }
    Trace trace = Trace(value["TraceID"], value["EventsID"]);
    WRITER.write_start_tag("trace");
    write_attributes(trace);
    write_events(trace);
    WRITER.write_end_tag();
    return 0;
}


void write_traces(Log log) {
    char* err;
    std::string log_id = log.get_attribute_container_id();
    std::string query = "SELECT TraceID, EventsID FROM Traces WHERE LogID = "
                        + log_id + ";";
    int rc = sqlite3_exec(db, query.c_str(), write_trace_callback, NULL, &err);
    if (rc != SQLITE_OK) {
        throw WriteToXesException("traces", err);
    }
    return;
}


static int write_log_callback(void *data, int argc, char **argv, char **azColName) {
    std::unordered_map<std::string, std::string> value;
    for (int i = 0; i < argc; ++i) {
        value[azColName[i]] = argv[i] ? argv[i] : "null";
    }
    WRITER.write_start_tag("log");
    WRITER.write_attribute("xes.version", "1.0");
    Log log = Log(value["LogID"], value["EventContainerID"]);
    write_extension(log);
    write_globals(log);
    write_classifiers(log);
    write_attributes(log);
    write_traces(log);
    write_events(log);
    WRITER.write_end_tag();
    return 0;
}


void write_logs() {
    char* err;
    std::string query = "SELECT LogID, EventContainerID FROM Logs;";
    int rc = sqlite3_exec(db, query.c_str(), write_log_callback, NULL, &err);
    if (rc != SQLITE_OK) {
        throw WriteToXesException("logs", err);
    }
    return;
}


void convert_from_sq3_to_xes(std::string xes_filename) {
    write_logs();
    WRITER.write_to_file(xes_filename);
    std::cout << "Сonversion to XES successful\n";
}


int sq3_to_xes(char* db_name, std::string xes_filename) {
    sqlite3_open(db_name, &db);
    try {
        convert_from_sq3_to_xes(xes_filename);
    } catch (const std::exception& e) {
        std::cout << "Exception message is: \n"
        << e.what() << std::endl; ;
        return -1;
    }
    return 0;
}
