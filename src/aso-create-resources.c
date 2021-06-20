#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include "../include/posix_exitcodes.h"
#include "../include/boolean.h"

#define IPC_PERM                                            0660
#define SHM_PERM                                            0660
#define SHM_KEY_DL                                          0x444C  // DATALIST
#define SHM_KEY_R                                           0x52    // READERS
#define SHM_SIZE                                            10
#define SEM_KEY_R                                           0x52    // READERS
#define SEM_KEY_W                                           0x57    // WRITERS

int main()
{
    int semid_w, semid_r;
    int shmid_dl, shmid_r;

    semid_w = semget(SEM_KEY_W, 1, IPC_CREAT | IPC_EXCL | IPC_PERM);
    if(semid_w == EGENERAL)
    {
        printf("SEM_KEY_W | semget() | errno : %s\n", strerror(errno));
    }
    else
    {
        printf("semid_w (id)    : %d\n", semid_w);
        if(semctl(semid_w, 0, SETVAL, 1) == EGENERAL)
        {
            printf("SEM_KEY_W | semctl() | errno : %s\n", strerror(errno));
        }
        else
        {
            printf("semid_w (value) : %d\n", semctl(semid_w, 0, GETVAL));
        }
    }

    semid_r = semget(SEM_KEY_R, 1, IPC_CREAT | IPC_EXCL | IPC_PERM);
    if(semid_r == EGENERAL)
    {
        printf("SEM_KEY_R | semget() | errno : %s\n", strerror(errno));
    }
    else
    {
        printf("semid_r (id)    : %d\n", semid_r);
        if(semctl(semid_r, 0, SETVAL, 1) == EGENERAL)
        {
            printf("SEM_KEY_R | semctl() | errno : %s\n", strerror(errno));
        }
        else
        {
            printf("semid_r (value) : %d\n", semctl(semid_r, 0, GETVAL));            
        }
    }

    shmid_dl = shmget(SHM_KEY_DL, sizeof(int) * SHM_SIZE, IPC_CREAT | IPC_EXCL | IPC_PERM);
    if(shmid_dl == EGENERAL)
    {
        printf("SHM_KEY_DL | shmget() | errno : %s\n", strerror(errno));
    }
    else
    {
        printf("shmid_dl (id)   : %d\n", shmid_dl);
    }

    shmid_r = shmget(SHM_KEY_R, sizeof(int), IPC_CREAT | IPC_EXCL | IPC_PERM);
    if(shmid_dl == EGENERAL)
    {
        printf("SHM_KEY_R | shmget() | errno : %s\n", strerror(errno));
    }
    else
    {
        printf("shmid_r (id)    : %d\n", shmid_r);
    }

   exit(EXIT_SUCCESS);
}
