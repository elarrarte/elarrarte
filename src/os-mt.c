#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <wait.h>
#include "../include/posix_exitcodes.h"
#include "../include/boolean.h"
#include "../include/list.h"

<<<<<<< HEAD
#define INSTRUCTION_TYPES_SIZE 2
typedef enum {IO, CPU} instruction_type_t;
char *INSTRUCTION_TYPES[] = {"IO", "CPU"};

#define IO_DEVICES_SIZE 7
typedef enum {KEYBOARD, MONITOR, MOUSE, USB, PRINTER, HDD} io_device_t;
char *IO_DEVICES[] = {"KEYBOARD", "MONITOR", "MOUSE", "USB", "PRINTER", "HDD", "GPU"};

=======
#define EGENERAL                                            -1

#define MAX_PROCESSES                                       10
#define MAX_UID                                             65535
#define MAX_GID                                             65535
#define MAX_INSTRUCTIONS                                    10
#define MAX_INSTRUCTION_SIZE                                5

#define PROCESS_SLEEP                                       1
#define PROCESS_INSTRUCTION_END                             1
#define PROCESS_QUANTUM_END                                 2
#define PROCESS_PCB_END                                     3
#define QUANTUM_BASE                                        2

#define VM_SLEEP                                            1
#define CPU_THREAD_SLEEP                                    1
#define IO_THREAD_SLEEP                                     1
#define MAX_THREAD_NAME                                     255

#define INSTRUCTION_TYPES_SIZE                              2
typedef enum {IO, CPU} instruction_type_t;
char *INSTRUCTION_TYPES[] = {"IO", "CPU"};

#define IO_DEVICES_SIZE                                     7
typedef enum {KEYBOARD, MONITOR, MOUSE, USB, PRINTER, HDD, GPU} io_device_t;
char *IO_DEVICES[] = {"KEYBOARD", "MONITOR", "MOUSE", "USB", "PRINTER", "HDD", "GPU"};

#define PROCESS_PRIORITIES_SIZE                             3
typedef enum {LOW, MEDIUM, HIGH} process_priority_t;
char *PROCESS_PRIORITIES[] = {"LOW", "MEDIUM", "HIGH"};

#define PROCESS_STATES_SIZE                                 6
typedef enum {NEW, ACTIVE, READY, RUNNING, WAITING, FINISHED} process_state_t;
char *PROCESS_STATES[] = {"NEW", "ACTIVE", "READY", "RUNNING", "WAITING", "FINISHED"};

// VM resources
#define CPU_CORES                                           4
list_t *cpu_queue, *io_queues[IO_DEVICES_SIZE], *process_queues[PROCESS_STATES_SIZE];
pthread_t cpu_threads[CPU_CORES], io_threads[IO_DEVICES_SIZE];
//pthread_mutex_t cpu_queue_mutex, io_queues_mutex[IO_DEVICES_SIZE], process_queues_mutex[PROCESS_STATES_SIZE];
pthread_mutex_t mutex;

>>>>>>> dev
typedef struct
{
    instruction_type_t type;
    int size;
    io_device_t device;

} instruction_t;

typedef struct
{
    int execution_time;
    int cpu_time;
    int io_time;

} process_stats_t;

<<<<<<< HEAD
#define PROCESS_PRIORITIES_SIZE 3
typedef enum {LOW, MEDIUM, HIGH} process_priority_t;
char *PROCESS_PRIORITIES[] = {"LOW", "MEDIUM", "HIGH"};

#define PROCESS_STATES_SIZE 6
typedef enum {NEW, ACTIVE, READY, RUNNING, WAITING, FINISHED} process_state_t;
char *PROCESS_STATES[] = {"NEW", "READY", "RUNNING", "WAITING", "FINISHED"};

=======
>>>>>>> dev
typedef struct pcb
{
    pid_t pid;
    uid_t uid;
    gid_t gid;
    int priority;
    instruction_t *ip;
    instruction_t *program;
    int instructions;
    process_state_t state;
    process_stats_t stats;

} pcb_t;

<<<<<<< HEAD
#define MAX_UID                 65535
#define MAX_GID                 65535
#define MAX_INSTRUCTIONS        10
#define MAX_INSTRUCTION_SIZE    5
=======
>>>>>>> dev
pcb_t *create_pcb(pid_t pid)
{
    pcb_t *pcb;
    instruction_t *instruction;
    int index;

    pcb = (pcb_t *) malloc (sizeof(pcb_t));
<<<<<<< HEAD
=======
    //pcb->priority = HIGH;
>>>>>>> dev
    pcb->priority = rand() % PROCESS_PRIORITIES_SIZE;
    pcb->pid = pid;
    pcb->uid = rand() % MAX_UID;
    pcb->gid = rand() % MAX_GID;
    memset(&pcb->stats, 0, sizeof(process_stats_t));
<<<<<<< HEAD
    //pcb->instructions = (rand() % MAX_INSTRUCTIONS) + 1;
    pcb->instructions = 1;
=======
    pcb->instructions = 1;
    //pcb->instructions = (rand() % MAX_INSTRUCTIONS) + 1;
>>>>>>> dev
    pcb->program = (instruction_t *) malloc(sizeof(instruction_t) * pcb->instructions);
    instruction = pcb->program;
    for(index = 0; index < pcb->instructions; index++)
    {
<<<<<<< HEAD
        //instruction->type = rand() % INSTRUCTION_TYPES_SIZE;
        instruction->type = CPU;
        //instruction->size = (rand() % MAX_INSTRUCTION_SIZE) + 1;
        instruction->size = 5;
        if(instruction->type == IO)
        {
            instruction->device = rand() % IO_DEVICES_SIZE;
=======
        //instruction->type = CPU;
        instruction->type = rand() % INSTRUCTION_TYPES_SIZE;
        //instruction->size = 10;
        instruction->size = (rand() % MAX_INSTRUCTION_SIZE) + 1;
        if(instruction->type == IO)
        {
            instruction->device = PRINTER;
            //instruction->device = rand() % IO_DEVICES_SIZE;   
>>>>>>> dev
        }
        instruction++;
    }
    pcb->ip = pcb->program;
    pcb->state = READY;

    return(pcb);
}

<<<<<<< HEAD
#define EGENERAL                -1
=======
>>>>>>> dev
pcb_t *create_process(void)
{
    pid_t pid;
    int exitcode;

    pid = fork();
    switch(pid)
    {
        case EGENERAL:
            return(NULL);

        // child process
        case 0:
            exitcode = execl("../exec/os-process", "os-process", NULL);
            if(exitcode == EGENERAL)
            {
                exit(EXIT_FAILURE);
            }

        // parent process
        default:
            return (create_pcb(pid));
    }
}

void print_pcb(pcb_t *pcb)
{
    int index;
    instruction_t *instruction;

    printf("PCB\n");
    printf("-----------------------------------------------\n");
    printf("[GENERAL]\n");
    printf("PRIORITY             : %s\n", PROCESS_PRIORITIES[pcb->priority]);
    printf("PID                  : %d\n", pcb->pid);
    printf("UID                  : %d\n", pcb->uid);
    printf("GID                  : %d\n", pcb->gid);
    printf("STATE                : %s\n", PROCESS_STATES[pcb->state]);
    printf("\n[STATS]\n");
    printf("EXECUTION TIME       : %d\n", pcb->stats.execution_time);
    printf("CPU TIME             : %d\n", pcb->stats.cpu_time);
    printf("IO TIME              : %d\n", pcb->stats.io_time);
    printf("\n[PROGRAM]\n");
    printf("INSTRUCTIONS         : %d\n", pcb->instructions);
    instruction = pcb->program;
    for(index = 0; index < pcb->instructions; index++)
    {
        printf("INSTRUCION[%d] TYPE   : %s\n", index, INSTRUCTION_TYPES[instruction->type]);
        printf("INSTRUCION[%d] SIZE   : %d\n", index, instruction->size);
        if(instruction->type == IO)
        {
            printf("INSTRUCION[%d] DEVICE : %s\n", index, IO_DEVICES[instruction->device]);
        }
        instruction++;
    }
    printf("-----------------------------------------------\n");
}

<<<<<<< HEAD
void print_queues(list_t *cpu_queue, list_t *io_queues[], list_t *process_queues[])
=======
void print_queues()
>>>>>>> dev
{
    int index;
    int queue_data;
    node_t *cpu, *io[IO_DEVICES_SIZE], *process[PROCESS_STATES_SIZE];

<<<<<<< HEAD
=======
    pthread_mutex_lock(&mutex);

>>>>>>> dev
    cpu = cpu_queue->head;
    for(index = 0; index < IO_DEVICES_SIZE; index++)
    {
        io[index] = io_queues[index]->head;
    }
    for(index = 0; index < PROCESS_STATES_SIZE; index++)
    {
        process[index] = process_queues[index]->head;
    }

    printf("     CPU KEYBOARD  MONITOR    MOUSE      USB  PRINTER      HDD      GPU      NEW   ACTIVE    READY  RUNNING  WAITING FINISHED\n");
    do
    {
        queue_data = FALSE;

        if(cpu != NULL)
        {
            printf("%8d ", ((pcb_t *) cpu->data)->pid);
            cpu = cpu->next;
            if(cpu != NULL) queue_data = TRUE;
        }
        else
        {
            printf("%8d ", 0);
        }

        for(index = 0; index < IO_DEVICES_SIZE; index++)
        {
            if(io[index] != NULL)
            {
                printf("%8d ", ((pcb_t *) io[index]->data)->pid);
                io[index] = io[index]->next;
                if(io[index] != NULL) queue_data = TRUE;
            }
            else
            {
                printf("%8d ", 0);
            }
        }

        for(index = 0; index < PROCESS_STATES_SIZE; index++)
        {
            if(process[index] != NULL)
            {
                printf("%8d ", ((pcb_t *) process[index]->data)->pid);
                process[index] = process[index]->next;
                if(process[index] != NULL) queue_data = TRUE;
            }
            else
            {
                printf("%8d ", 0);
            }
        }
        printf("\n");

    } while (queue_data);
    printf("\n");
<<<<<<< HEAD
}

// VM resources
#define CPU_CORES        1
list_t *cpu_queue, *io_queues[IO_DEVICES_SIZE], *process_queues[PROCESS_STATES_SIZE];
#define CPU_CYCLE        1
#define QUANTUM          2
#define QUANTUM_UNSET   -1
int quantum = QUANTUM_UNSET;
pthread_t cpu_threads[CPU_CORES], io_threads[IO_DEVICES_SIZE];
pthread_mutex_t cpu_queue_mutex, io_queues_mutex[IO_DEVICES_SIZE], process_queues_mutex[PROCESS_STATES_SIZE];

#define CPU_THREAD_SLEEP 1
void *cpu_thread()
{
    pcb_t *pcb = NULL;
    int child_status;
    node_t *node_index;

    while (TRUE)
    {
        if(pcb != NULL && cpu_queue->size)
        {
            pcb = list_remove_head(cpu_queue);
            while(pcb->instructions)
            {
                // handle instruction
                switch(pcb->ip->type)
                {
                    case CPU:
                        handle_cpu_instruction(pcb);
                    break;

                    case IO:
                        handle_io_instruction(pcb);
                    break;
                }

                pcb->instructions--;
                pcb->ip++;
            }

            // end PCB (no more instructions to process)
            list_remove_data(cpu_queue, pcb);
            list_remove_data(process_queues[RUNNING], pcb);
            list_remove_data(process_queues[ACTIVE], pcb);
            list_add_tail(process_queues[FINISHED], pcb);
            pcb->state = FINISHED;
            print_pcb(pcb);
            kill(pcb->pid, SIGUSR1);
            waitpid(pcb->pid, &child_status, 0);
            printf("child exitcode    : %d\n\n", WEXITSTATUS(child_status));
        }

        sleep(CPU_THREAD_SLEEP);
    }


    if(cpu_queue->size)
    {
        node_index = cpu_queue->head;
        while(node_index != NULL)
        {
            pcb = node_index->data;
            node_index = node_index->next;

            if(!pcb->ip->size)
            {
                pcb->instructions--;
                if(pcb->instructions)
                {
                    // read the next instruction
                    pcb->ip++;
                }
                else
                {
                    // end PCB (no more instructions to process)
                    list_remove_data(cpu_queue, pcb);
                    list_remove_data(process_queues[RUNNING], pcb);
                    list_remove_data(process_queues[ACTIVE], pcb);
                    list_add_tail(process_queues[FINISHED], pcb);
                    pcb->state = FINISHED;
                    print_pcb(pcb);
                    kill(pcb->pid, SIGUSR1);
                    waitpid(pcb->pid, &child_status, 0);
                    printf("child exitcode    : %d\n\n", WEXITSTATUS(child_status));
                }
            }
            else
            {
                // handle instructions
                switch(pcb->ip->type)
                {
                    case CPU:
                        handle_cpu_instruction(pcb);
                    break;

                    case IO:
                        handle_io_instruction(pcb);
                    break;
                }
            }
        }
    }
}

void *io_thread()
{
    while(TRUE)
    {
        sleep(1);
    }
}

#define MAX_THREAD_NAME     255
void initialize_resources()
{
    int index, exitcode;
    char thread_name[MAX_THREAD_NAME];

    cpu_queue = list_new();
    for(index = 0; index < IO_DEVICES_SIZE; index++)
    {
        io_queues[index] = list_new();
    }
=======

   pthread_mutex_unlock(&mutex);
}

void finish_pcb(pcb_t *pcb)
{
    int child_status;

    pthread_mutex_lock(&mutex);
    list_remove_data(cpu_queue, pcb);
    list_remove_data(process_queues[RUNNING], pcb);
    list_remove_data(process_queues[ACTIVE], pcb);
    list_add_tail(process_queues[FINISHED], pcb);
    pcb->state = FINISHED;
    print_pcb(pcb);
    kill(pcb->pid, SIGUSR1);
    waitpid(pcb->pid, &child_status, 0);
    printf("child exitcode    : %d\n\n", WEXITSTATUS(child_status));
    pthread_mutex_unlock(&mutex);
}

int process_io_instruction(pcb_t *pcb)
{
    int quantum = QUANTUM_BASE * (pcb->priority + 1);

    while(pcb->ip->size)
    {
        if(quantum)
        {
            pcb->ip->size--;
            pcb->stats.io_time++;
            sleep(PROCESS_SLEEP);
            quantum--;            
        }
        else
        {
            return(PROCESS_QUANTUM_END);
        }
    }

    return(PROCESS_INSTRUCTION_END);
}

void handle_io_instrucion(pcb_t *pcb)
{
    if(pcb->ip->size)
    {
        pthread_mutex_lock(&mutex);
        list_remove_data(cpu_queue, pcb);
        list_remove_data(process_queues[RUNNING], pcb);
        list_add_tail(io_queues[pcb->ip->device], pcb);
        list_add_tail(process_queues[WAITING], pcb);
        pcb->state = WAITING;
        pthread_mutex_unlock(&mutex);
    }
    else
    {
        pcb->instructions--;
        pcb->ip++;

        if(pcb->instructions)
        {
            pthread_mutex_lock(&mutex);
            list_remove_data(process_queues[RUNNING], pcb);
            list_remove_data(cpu_queue, pcb);
            list_add_tail(process_queues[READY], pcb);
            pcb->state = READY;
            pthread_mutex_unlock(&mutex);
        }
        else
        {
            finish_pcb(pcb);
        }                        
    }
}

int process_cpu_instruction(pcb_t *pcb)
{
    int quantum = QUANTUM_BASE * (pcb->priority + 1);

    while(pcb->instructions)
    {
        while(pcb->ip->size)
        {
            if(quantum)
            {
                pcb->ip->size--;
                pcb->stats.cpu_time++;
                quantum--;
                sleep(PROCESS_SLEEP);
            }
            else
            {
                return(PROCESS_QUANTUM_END);
            }
        }

        pcb->instructions--;
        pcb->ip++;
    }

    return(PROCESS_PCB_END);
}

void handle_cpu_instruction(pcb_t *pcb)
{
    switch(process_cpu_instruction(pcb))
    {
        case PROCESS_QUANTUM_END:
            pthread_mutex_lock(&mutex);
            list_remove_data(process_queues[RUNNING], pcb);
            list_remove_data(cpu_queue, pcb);
            list_add_tail(process_queues[READY], pcb);
            pcb->state = READY;
            pthread_mutex_unlock(&mutex);
        break;

        case PROCESS_PCB_END:
            finish_pcb(pcb);
        break;
    }
}

void *cpu_thread(void *vp_cpuid)
{
    pcb_t* pcb;
    int cpuid = *((int *) vp_cpuid);

    while(TRUE)
    {
        pthread_mutex_lock(&mutex);
        if(process_queues[READY]->size)
        {
            pcb = list_remove_head(process_queues[READY]);
            list_add_tail(process_queues[RUNNING], pcb);
            list_add_tail(cpu_queue, pcb);
            pcb->state = RUNNING;
            pthread_mutex_unlock(&mutex);

            switch(pcb->ip->type)
            {
                case CPU:
                    handle_cpu_instruction(pcb);
                break;

                case IO:
                    handle_io_instrucion(pcb);
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
        sleep(CPU_THREAD_SLEEP);
    }
}

void *io_thread(void *vp_device)
{
    int device = *((int *) vp_device);
    pcb_t *pcb;

    while(TRUE)
    {
        pthread_mutex_lock(&mutex);
        if(io_queues[device]->size)
        {
            pcb = io_queues[device]->head->data;
            pthread_mutex_unlock(&mutex);

            switch(process_io_instruction(pcb))
            {
                case PROCESS_INSTRUCTION_END:
                    pthread_mutex_lock(&mutex);
                    list_remove_data(io_queues[device], pcb);
                    list_remove_data(process_queues[WAITING], pcb);
                    list_add_tail(process_queues[READY], pcb);
                    pcb->state = READY;
                    pthread_mutex_unlock(&mutex);
                break;

                case PROCESS_QUANTUM_END:
                    pthread_mutex_lock(&mutex);
                    list_remove_data(io_queues[device], pcb);
                    list_add_tail(io_queues[device], pcb);
                    pthread_mutex_unlock(&mutex);
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
        sleep(IO_THREAD_SLEEP);
    }
}

void initialize_resources()
{
    int index, exitcode, *device, *cpuid;
    char thread_name[MAX_THREAD_NAME];
>>>>>>> dev

    for(index = 0; index < PROCESS_STATES_SIZE; index++)
    {
        process_queues[index] = list_new();
    }

<<<<<<< HEAD
    // start the random seed
    srand(time(0));

    // create CPU threads
    for(index = 0; index < CPU_CORES; index++)
    {
        exitcode = pthread_create(&cpu_threads[index], NULL, cpu_thread, NULL);
        if(exitcode == EXIT_SUCCESS)
        {
            sprintf(thread_name, "cpu-t-%d", index);
            pthread_setname_np(cpu_threads[index], thread_name);
        }
        else
        {
            printf("could not create cpu_thread()");
        }
    }

    // create IO threads
    for(index = 0; index < IO_DEVICES_SIZE; index++)
    {
        exitcode = pthread_create(&io_threads[index], NULL, io_thread, NULL);
        if(exitcode == EXIT_SUCCESS)
        {
            sprintf(thread_name, "io-t-%s", IO_DEVICES[index]);            
            pthread_setname_np(io_threads[index], thread_name);
        }
        else
        {
            printf("could not create io_thread()");
=======
    for(index = 0; index < IO_DEVICES_SIZE; index++)
    {
        io_queues[index] = list_new();
        device = (int *) malloc(sizeof(int));
        *device = index;
        exitcode = pthread_create(&io_threads[index], NULL, io_thread, (void *) device);
        if(exitcode == EXIT_SUCCESS)
        {
            sprintf(thread_name, "io-t-%s", IO_DEVICES[index]);            
            pthread_setname_np(io_threads[index], thread_name);
        }
        else
        {
            printf("could not create io_thread()");
        }
    }

    cpu_queue = list_new();
    for(index = 0; index < CPU_CORES; index++)
    {
        cpuid = (int *) malloc(sizeof(int));
        *cpuid = index;
        exitcode = pthread_create(&cpu_threads[index], NULL, cpu_thread, (void *) cpuid);
        if(exitcode == EXIT_SUCCESS)
        {
            sprintf(thread_name, "cpu-t-%d", index);
            pthread_setname_np(cpu_threads[index], thread_name);
        }
        else
        {
            printf("could not create cpu_thread()");
>>>>>>> dev
        }
    }
}

<<<<<<< HEAD
#define MAX_PROCESSES 10
=======
>>>>>>> dev
void create_processes()
{
    int processes;
    pcb_t *pcb;

<<<<<<< HEAD
    for(processes = (rand() % MAX_PROCESSES) + 1; processes; processes--)
    {
        pcb = create_process();
        list_add_tail(process_queues[NEW], pcb);
        pcb->state = NEW;
        print_pcb(pcb);
    }
}

void process_io_instructions()
{
    int index;
    pcb_t *pcb;

    for(index = 0; index < IO_DEVICES_SIZE; index++)
    {
        if(!list_empty(io_queues[index]))
        {
            pcb = io_queues[index]->head->data;
            if(pcb->ip->size)
            {
                // process one IO operation and update process stats
                pcb->ip->size--;
                pcb->stats.io_time++;
            }
            else
            {
                // move the PCB to the READY queue when IO instruction ends
                list_remove_head(io_queues[index]);
                list_remove_data(process_queues[WAITING], pcb);
                list_add_tail(process_queues[READY], pcb);
                pcb->state = READY;
            }
        }
    }
}

void handle_io_instruction(pcb_t *pcb)
{
    list_remove_data(cpu_queue, pcb);
    list_remove_data(process_queues[RUNNING], pcb);
    list_add_tail(io_queues[pcb->ip->device], pcb);
    list_add_tail(process_queues[WAITING], pcb);
    pcb->state = WAITING;
}

void handle_cpu_instruction(pcb_t *pcb)
{
    // set the QUANTUM after context switching    
    if(quantum == QUANTUM_UNSET)
    {
        quantum = QUANTUM;
    }

    // process only if QUANTUM is available
    if(quantum)
    {
        quantum--;
        if(pcb->ip->size)
        {
            // process one CPU operation and update process stats
            pcb->ip->size--;
            pcb->stats.cpu_time++;
        }
    }
    else
    {
        // reset the QUANTUM
        quantum = QUANTUM_UNSET;

        // start context switching if there is more than one process in the READY queue
        if(process_queues[READY]->size > 1)
        {
            list_remove_data(cpu_queue, pcb);
            list_remove_data(process_queues[RUNNING], pcb);
            list_add_tail(process_queues[READY], pcb);
            pcb->state = READY;
        }
    }
}

void process_cpu_instructions()
{
    pcb_t *pcb;
    int child_status;
    node_t *node_index;

    if(cpu_queue->size)
    {
        node_index = cpu_queue->head;
        while(node_index != NULL)
        {
            pcb = node_index->data;
            node_index = node_index->next;

            if(!pcb->ip->size)
            {
                pcb->instructions--;
                if(pcb->instructions)
                {
                    // read the next instruction
                    pcb->ip++;
                }
                else
                {
                    // end PCB (no more instructions to process)
                    list_remove_data(cpu_queue, pcb);
                    list_remove_data(process_queues[RUNNING], pcb);
                    list_remove_data(process_queues[ACTIVE], pcb);
                    list_add_tail(process_queues[FINISHED], pcb);
                    pcb->state = FINISHED;
                    print_pcb(pcb);
                    kill(pcb->pid, SIGUSR1);
                    waitpid(pcb->pid, &child_status, 0);
                    printf("child exitcode    : %d\n\n", WEXITSTATUS(child_status));
                }
            }
            else
            {
                // handle instructions
                switch(pcb->ip->type)
                {
                    case CPU:
                        handle_cpu_instruction(pcb);
                    break;

                    case IO:
                        handle_io_instruction(pcb);
                    break;
                }
            }
        }
=======
    // start the random seed
    srand(time(0));

    //for(processes = 6; processes; processes--)
    for(processes = (rand() % MAX_PROCESSES) + 1; processes; processes--)
    {
        pcb = create_process();
        pthread_mutex_lock(&mutex);
        list_add_tail(process_queues[NEW], pcb);
        pcb->state = NEW;
        print_pcb(pcb);
        pthread_mutex_unlock(&mutex);
>>>>>>> dev
    }
}

void update_execution_time()
{
    node_t *node_index;
    pcb_t *pcb;

<<<<<<< HEAD
=======
    pthread_mutex_lock(&mutex);
>>>>>>> dev
    node_index = process_queues[ACTIVE]->head;
    while(node_index != NULL)
    {
        pcb = node_index->data;
        pcb->stats.execution_time++;
        node_index = node_index->next;
    }
<<<<<<< HEAD
}

void vm()
{
    pcb_t *pcb;

    while(TRUE)
    {
        // print system queues
        print_queues(cpu_queue, io_queues, process_queues);

        // process IO instructions
        // process_io_instructions();

        // process CPU instructions
        // process_cpu_instructions();

        // if CPU is in idle state, try to take a process from the READY queue
        if(cpu_queue->size < CPU_CORES)
        {
            if(!list_empty(process_queues[READY]))
            {
                pcb = list_remove_head(process_queues[READY]);
                list_add_tail(cpu_queue, pcb);
                list_add_tail(process_queues[RUNNING], pcb);
                pcb->state = RUNNING;
            }
        }

        // if there is NEW processes, move them to READY queue
        if(!list_empty(process_queues[NEW]))
        {
            pcb = list_remove_head(process_queues[NEW]);
            list_add_tail(process_queues[ACTIVE], pcb);
            list_add_tail(process_queues[READY], pcb);
            pcb->state = READY;
        }
=======
    pthread_mutex_unlock(&mutex);
}

void cpu_planning()
{
    pcb_t *pcb;

    // if there are NEW processes, move them to READY queue
    pthread_mutex_lock(&mutex);
    if(!list_empty(process_queues[NEW]))
    {
        pcb = list_remove_head(process_queues[NEW]);
        list_add_tail(process_queues[ACTIVE], pcb);
        list_add_tail(process_queues[READY], pcb);
        pcb->state = READY;
    }
    pthread_mutex_unlock(&mutex);
}

void vm()
{
    while(TRUE)
    {
        // print system queues
        print_queues();

        // CPU planning
        cpu_planning();
>>>>>>> dev

        // update execution time on active processes
        update_execution_time();
        
        // take some tome between cycles
<<<<<<< HEAD
        sleep(CPU_CYCLE);    
=======
        sleep(VM_SLEEP);    
>>>>>>> dev
    }
}

int main(void)
{
    // initialize resources
    initialize_resources();

    // create a bunch of processes
    create_processes();

    // VM
    vm();

    return(EXIT_SUCCESS);
}
