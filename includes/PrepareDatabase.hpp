#include <iostream>
#include <sqlite3.h>

void prepare_database(sqlite3* db);
void post_process_database(sqlite3* db);
void begin(sqlite3* db);
void commit(sqlite3* db);
sqlite3_int64 allocate_atribute_container(sqlite3* db);
sqlite3_int64 allocate_event_container(sqlite3* db);