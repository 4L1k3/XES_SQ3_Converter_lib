#ifndef MY_SAX_PARSER_H
#define MY_SAX_PARSER_H

#include <xercesc/util/XMLUniDefs.hpp>
#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <vector>
#include <sqlite3.h>
#include <string>
#include <stack>
#include <memory>

#include <includes/XesElements.hpp>


using namespace xercesc;


class XesToSq3Converter {
public:
    XesToSq3Converter(sqlite3* db);
    ~XesToSq3Converter();

    Log create_log();
    Extension create_extension(std::string name, std::string prefix, std::string uri);
    Global create_global(std::string scope);
    Classifier create_classifier(std::string name, std::vector<std::string>& keys, std::string scope);
    Trace create_trace();
    Event create_event();
    ListAttributeValues create_list_values();
    std::shared_ptr<Attribute> create_attribute(AttributeType attribute_type, std::string key, std::string value);

    void push_attribute_container(AttributeContainer attr_cont);
    void push_event_container(EventContainer event_cont);

    void pop_attribute_container();
    void pop_event_container();

private:
    sqlite3* db;
    std::stack<AttributeContainer> attribute_containers;
    std::stack<EventContainer> event_containers;
    Log log;
};


class XesToSq3Handler: public HandlerBase {
public:
    XesToSq3Handler(XesToSq3Converter* converter);
    ~XesToSq3Handler();

    void startElement(const XMLCh* const name, AttributeList& attributes); // start of an element.
    void endElement(const XMLCh* const name); //  end of an element.


    void fatalError(const SAXParseException&); // Report a fatal XML parsing error.

private:
    int numSeq;
    XesToSq3Converter* converter;
};

#endif