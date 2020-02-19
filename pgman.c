
#include <libpq-fe.h>
#include <stdlib.h>
#include <string.h>

#include "pgman.h"
#include "shared.h"

#define conn_format "user=postgres dbname=postgres host=psql01"


PGconn *connection;

PGconn *
pg_connection_open () {
    PGconn *temp_connection = PQconnectdb(conn_format);

    if (PQstatus(temp_connection) != CONNECTION_OK) {
        fprintf(stderr, "Connection error: %s", PQerrorMessage(temp_connection));
        PQfinish(temp_connection);
        return NULL;
    }

    return temp_connection;
}

void
pg_initialize () {
    connection = pg_connection_open();
    shared_memory_initialize();
}

void
pg_connection_close() {
    PQfinish(connection);
    shared_memory_close();
}

void
pg_check_result(char *message, PGresult *res) {
    int status = PQresultStatus(res);
    if (status != PGRES_TUPLES_OK && status != PGRES_COMMAND_OK) {
        fprintf(stderr, "%s: %s -- status code: %d\n", message, PQerrorMessage(connection), status);
        pg_connection_close();
        exit(PSQL_CONN_ERR_CODE);
    }
}

void
pg_exec_with_shared_connection (const char function_name[], char *query, const char *params[], int n) {
    PGresult *res;

    if (!get_shared_memory_block()) {
        res = PQexecParams(connection, query, n, NULL, params, NULL, NULL, 0);
        set_shared_memory_block(0);
    }
    else {
        PGconn *new_connection = pg_connection_open();
        res = PQexecParams(new_connection, query, n, NULL, params, NULL, NULL, 0);
        PQfinish(new_connection);
    }

    char err_msg[100];
    sprintf(err_msg, "%s Error", function_name);
    pg_check_result(err_msg, res);
}

void
pg_exec_with_new_connection (const char function_name[], char *query, const char *params[], int n) {
    PGresult *res;

    PGconn *new_connection = pg_connection_open();
    res = PQexecParams(new_connection, query, n, NULL, params, NULL, NULL, 0);
    PQfinish(new_connection);

    char err_msg[100];
    sprintf(err_msg, "%s Error", function_name);
    pg_check_result(err_msg, res);
}

void
add_data (const char *col1, const char *col2) {
    char *query = "INSERT INTO table1 (col1, col2) VALUES ($1, $2)";

    const char *params[2];
    params[0] = col1;
    params[1] = col2;

    pg_exec_with_new_connection(__FUNCTION__, query, params, 2);
    // pg_exec_with_shared_connection(__FUNCTION__, query, params, 2);
}

void
change_data (const char *col1, float col3) {
    char *query = "UPDATE table1 SET col3 = $1 WHERE col1 = $2";

    const char *params[2];
    char str_col3[5];
    gcvt(col3, 4, str_col3);

    params[0] = str_col3;
    params[1] = col1;

    pg_exec_with_new_connection(__FUNCTION__, query, params, 2);
    // pg_exec_with_shared_connection(__FUNCTION__, query, params, 2);
}
