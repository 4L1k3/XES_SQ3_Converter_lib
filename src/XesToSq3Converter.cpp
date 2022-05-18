#include <iostream>
#include <sqlite3.h>
#include <string>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>

#include "includes/exceptions.hpp"
#include "includes/utils.hpp"
#include "includes/PrepareDatabase.hpp"
#include "includes/XesElements.hpp"
#include "includes/XesSq3Converter.hpp"
#include "includes/XesToSq3Handler.hpp"
#include "includes/XesWriter.hpp"


int xes_to_sq3(char* filename) {
    sqlite3* db;
    sqlite3_open("myDb.db", &db);
    prepare_database(db);
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Error during initialization! :\n"
             << message << "\n";
        XMLString::release(&message);
        return 1;
    }

    SAXParser* parser = new SAXParser();
    parser->setValidationScheme(SAXParser::Val_Always);
    parser->setDoNamespaces(true);    // optional

    // Customize Docment Handler during parseing XES
    XesToSq3Converter* converter = new XesToSq3Converter(db);
    XesToSq3Handler* docHandler = new XesToSq3Handler(converter);
    ErrorHandler* errHandler = (ErrorHandler*) docHandler;

    parser->setDocumentHandler(docHandler);
    parser->setErrorHandler(errHandler);

    try {

        // SAX 1.0 Parser interface's
        // Return is void
        // void parse( const InputSource &source )
        parser->parse(filename);
        post_process_database(db);
        std::cout << "\n\n\n all ok\n";
    } catch (const XMLException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
        return -1;
    } catch (const SAXParseException& toCatch) {
        char* message = XMLString::transcode(toCatch.getMessage());
        cout << "Exception message is: \n"
             << message << "\n";
        XMLString::release(&message);
        return -1;
    } catch (const exception& e) {
        cout << "Exception message is: \n"
        << e.what() << endl; ;
        return -1;
    }

    delete parser;
    delete converter;
    delete docHandler;

    // And call the termination method
    XMLPlatformUtils::Terminate();
    return 0;
}
