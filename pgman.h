
#ifndef PSQL_SCENARIOS_PGMAN_H
#define PSQL_SCENARIOS_PGMAN_H

#define PSQL_CONN_ERR_CODE 21


void
pg_initialize ();

void
pg_connection_close();

void
change_data (const char *col1, float col3);

void
add_data (const char *col1, const char *col2);

#endif //PSQL_SCENARIOS_PGMAN_H
