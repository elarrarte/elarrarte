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

#define SHM_PERM                                            0600
#define SHM_KEY_DL                                          0x444C  // DATALIST
#define SHM_KEY_R                                           0x52    // READERS
#define SHM_SIZE                                            10
#define SHM_DO_NOT_READ                                     0
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
    int shmid_r, *shm_r;
    int shm_pos, shm_pos_val;
    int semid_w, semid_r;

    srand(time(0));
    pid = getpid();
    
    semid_w = semget(SEM_KEY_W, 0, 0);
    semid_r = semget(SEM_KEY_R, 0, 0);
    shmid_dl = shmget(SHM_KEY_DL, 0, 0);
    shm_ld = (int *) shmat(shmid_dl, NULL, SHM_RDONLY);
    shmid_r = shmget(SHM_KEY_R, 0, 0);
    shm_r = (int *) shmat(shmid_r, NULL, 0);

    while(TRUE)
    {
        semwait(semid_r, 0);
        (*shm_r) += 1;
        printf("READER [%d] SHM_R : %d\n", pid, *shm_r);
        if(*shm_r == 1) semwait(semid_w, 0);
        semsignal(semid_r, 0);

        shm_pos = random() % SHM_SIZE;
        shm_pos_val = shm_ld[shm_pos];

        if(shm_pos_val != SHM_DO_NOT_READ)
        {
            printf("READER [%d] SHM READ [%d] : %d\n", pid, shm_pos, shm_ld[shm_pos]);
        }

        semwait(semid_r, 0);
        (*shm_r) -= 1;
        printf("READER [%d] SHM_R : %d\n", pid, *shm_r);
        if(*shm_r == 0) semsignal(semid_w, 0);
        semsignal(semid_r, 0);
    }
    
    exit(EXIT_SUCCESS);
}