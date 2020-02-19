
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>

#include "shared.h"


app_shared *shared;
int shmid;

void
set_shared_memory_block (int block) {
    shared->pg_block = block;
}

void
shared_memory_initialize () {
    shmid = shmget(IPC_PRIVATE, sizeof(app_shared *), IPC_CREAT | 0666);
    shared = (app_shared *) shmat(shmid, (void *)0, 0);

    set_shared_memory_block(0);
}

void
shared_memory_close () {
    shmdt(shared);
    shmctl(shmid, IPC_RMID, NULL);
}

int
get_shared_memory_block () {
    int block = shared->pg_block;
    if (block == 0)
        set_shared_memory_block(1);
    return block;
}
