
#ifndef PSQL_SCENARIOS_SHARED_H
#define PSQL_SCENARIOS_SHARED_H

typedef struct {
    int pg_block;
} app_shared;

void
shared_memory_initialize ();

void
shared_memory_close ();

void
set_shared_memory_block (int block);

int
get_shared_memory_block ();

#endif //PSQL_SCENARIOS_SHARED_H
