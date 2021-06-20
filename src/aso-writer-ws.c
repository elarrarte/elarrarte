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
#define SEM_KEY_R                                           0x52    // READERS
#define SEM_KEY_W                                           0x57    // WRITERS

void semwait(int semid, int semnum)
{
    struct sembuf semop_def;
    semop_def.sem_num = semnum;
    semop_def.sem_op = -1;
    semop_def.sem_flg = 0;
    semop(semid, &semop_def, 1);
}

void semsignal(int semid, int semnum)
{
    struct sembuf semop_def;
    semop_def.sem_num = semnum;
    semop_def.sem_op = 1;
    semop_def.sem_flg = 0;
    semop(semid, &semop_def, 1);
}

int main()
{
    int pid;
    int shmid_dl, *shm_ld;
    int shm_pos, shm_opcode;
    int semid_w;

    srand(time(0));
    pid = getpid();

    semid_w = semget(SEM_KEY_W, 0, 0);
    shmid_dl = shmget(SHM_KEY_DL, 0, 0);
    shm_ld = (int *) shmat(shmid_dl, NULL, 0);

    while(TRUE)
    {
        shm_pos = random() % SHM_SIZE;
        shm_opcode = random() % SHM_OPCODES;
        semwait(semid_w, 0);
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
        semsignal(semid_w, 0);
    }
    
    exit(EXIT_SUCCESS);
}