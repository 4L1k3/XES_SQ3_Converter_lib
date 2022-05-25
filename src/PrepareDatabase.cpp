#include <iostream>
#include <sqlite3.h>
#include <string>

#include "includes/exceptions.hpp"
#include "includes/PrepareDatabase.hpp"


void begin(sqlite3* db) {
    char* err;
    std::string query = "BEGIN TRANSACTION;";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }
}


void commit(sqlite3* db) {
    char* err;
    std::string query = "Commit;";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }
}


void prepare_database(sqlite3* db) {
    char* err;
    std::string query = "CREATE TABLE IF NOT EXISTS AttributeContainers(ID INTEGER PRIMARY KEY AUTOINCREMENT);";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }

    query = "CREATE TABLE IF NOT EXISTS EventContainers(ID INTEGER PRIMARY KEY AUTOINCREMENT);";
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }

    query = "CREATE TABLE IF NOT EXISTS Logs("
            "LogID INTEGER PRIMARY KEY,"
            "EventContainerID INTEGER NOT NULL,"
            "FOREIGN KEY (LogID) REFERENCES AttributeContainers(ID),"
            "FOREIGN KEY (EventContainerID) REFERENCES EventContainers(ID));";
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }

    query = "CREATE TABLE IF NOT EXISTS Extensions("
            "ExtensionID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "LogID INTEGER NOT NULL,"
            "Name TEXT,"
            "Prefix TEXT,"
            "Uri TEXT,"
            "FOREIGN KEY (LogID) REFERENCES Logs(LogID));";
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }

    query = "CREATE TABLE IF NOT EXISTS Classifiers("
            "ClassifierID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "LogID INTEGER NOT NULL,"
            "Name TEXT,"
            "Scope TEXT,"
            "FOREIGN KEY (LogID) REFERENCES Logs(LogID)"
            "CONSTRAINT ScopeConstraint CHECK (Scope IN ('trace', 'event')));";
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }

    query = "CREATE TABLE IF NOT EXISTS ClassifierKeys("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
            "ClassifierID INTEGER,"
            "Key TEXT,"
            "FOREIGN KEY (ClassifierID) REFERENCES Classifiers(ClassifierID));";
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }

    query = "CREATE TABLE IF NOT EXISTS Globals("
            "GlobalID INTEGER PRIMARY KEY,"
            "LogID INTEGER NOT NULL,"
            "Scope TEXT,"
            "FOREIGN KEY (GlobalID) REFERENCES AttributeContainers(ID),"
            "FOREIGN KEY (LogID) REFERENCES Logs(LogID)"
            "CONSTRAINT ScopeConstraint CHECK (Scope IN ('trace', 'event')));";
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }

    query = "CREATE TABLE IF NOT EXISTS Traces("
            "TraceID INTEGER PRIMARY KEY,"
            "EventsID INTEGER NOT NULL,"
            "LogID INTEGER NOT NULL,"
            "FOREIGN KEY (TraceID) REFERENCES AttributeContainers(ID),"
            "FOREIGN KEY (EventsID) REFERENCES EventContainers(ID),"
            "FOREIGN KEY (LogID) REFERENCES Logs(LogID));";
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }

    query = "CREATE TABLE IF NOT EXISTS Events("
            "EventID INTEGER PRIMARY KEY,"
            "ContainedEventContainerID INTEGER,"
            "FOREIGN KEY (EventID) REFERENCES AttributeContainers(ID),"
            "FOREIGN KEY (ContainedEventContainerID) REFERENCES EventContainers(ID));";
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }

    query = "CREATE TABLE IF NOT EXISTS Attributes("
            "AttributeID INTEGER PRIMARY KEY,"
            "ContainedAttributeContainerID INTEGER,"
            "Type TEXT,"
            "Key TEXT,"
            "Value TEXT,"
            "FOREIGN KEY (AttributeID) REFERENCES AttributeContainers(ID),"
            "FOREIGN KEY (ContainedAttributeContainerID) REFERENCES AttributeContainers(ID)"
            "CONSTRAINT AttrTypeConstraint CHECK "
            "(Type IN ('string', 'date', 'int', 'float', 'boolean', 'id', 'list')));";
    rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }

    begin(db);
}


void post_process_database(sqlite3* db) {
    commit(db);
}


sqlite3_int64 allocate_atribute_container(sqlite3* db) {
    char* err;
    std::string query = "INSERT INTO AttributeContainers(ID) VALUES(NULL);";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }
    sqlite3_int64 last_row_id = sqlite3_last_insert_rowid(db);
    return last_row_id;
}

sqlite3_int64 allocate_event_container(sqlite3* db) {
    char* err;
    std::string query = "INSERT INTO EventContainers(ID) VALUES(NULL);";
    int rc = sqlite3_exec(db, query.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        throw DataBaseQueryExecuteError(query, err);
    }
    sqlite3_int64 last_row_id = sqlite3_last_insert_rowid(db);
    return last_row_id;
}