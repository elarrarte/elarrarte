#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "../include/posix_exitcodes.h"
#include "../include/list.h"

#define SHM_KEY_DL                                          0x444C  // DATALIST
#define SHM_KEY_R                                           0x52    // READERS
#define SHM_SIZE                                            10
#define SHM_DO_NOT_READ                                     0
#define SHM_OPCODES                                         2
#define SHM_OPCODE_REMOVE                                   0
#define SHM_OPCODE_WRITE                                    1

int main()
{
    int pid;
    int shmid_dl, *shm_ld;
    int shm_pos, shm_opcode;

    srand(time(0));
    pid = getpid();

    shmid_dl = shmget(SHM_KEY_DL, 0, 0);
    shm_ld = (int *) shmat(shmid_dl, NULL, 0);

    while(TRUE)
    {
        shm_pos = random() % SHM_SIZE;
        shm_opcode = random() % SHM_OPCODES;
        switch(shm_opcode)
        {
            case SHM_OPCODE_REMOVE:
                shm_ld[shm_pos] = SHM_DO_NOT_READ;
                printf("WRITER [%d] SHM REMOVE [%d]\n", pid, shm_pos);
            break;

            case SHM_OPCODE_WRITE:
                shm_ld[shm_pos] = (rand() % 100) + 1;
                printf("WRITER [%d] SHM WRITE [%d] : %d\n", pid, shm_pos, shm_ld[shm_pos]);
            break;
        }
    }
    
    exit(EXIT_SUCCESS);
}