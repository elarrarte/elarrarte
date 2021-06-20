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

    semid_w = semget(SEM_KEY_W, 0, 0);
    semid_r = semget(SEM_KEY_R, 0, 0);
    shmid_dl = shmget(SHM_KEY_DL, 0, 0);
    shmid_r = shmget(SHM_KEY_R, 0, 0);

    if(semctl(semid_w, 0, IPC_RMID) == EGENERAL)
    {
        printf("SEM_KEY_W | semctl() | errno : %s\n", strerror(errno));
    }

    if(semctl(semid_r, 0, IPC_RMID) == EGENERAL)
    {
        printf("SEM_KEY_R | semctl() | errno : %s\n", strerror(errno));
    }

    if(shmctl(shmid_dl, IPC_RMID, NULL) == EGENERAL)
    {
        printf("SHM_KEY_LD | shmctl() | errno : %s\n", strerror(errno));
    }

    if(shmctl(shmid_r, IPC_RMID, NULL) == EGENERAL)
    {
        printf("SHM_KEY_R | shmctl() | errno : %s\n", strerror(errno));
    }

   exit(EXIT_SUCCESS);
}
