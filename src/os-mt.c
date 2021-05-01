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
#include <curses.h>
#include "../include/posix_exitcodes.h"
#include "../include/boolean.h"
#include "../include/list.h"
#include "../include/window.h"

#define EGENERAL                                            -1

#define MAX_PROCESSES                                       10
#define MAX_UID                                             65535
#define MAX_GID                                             65535
#define MAX_INSTRUCTIONS                                    3
#define MAX_INSTRUCTION_SIZE                                5

#define PROCESS_SLEEP                                       1
#define PROCESS_INSTRUCTION_END                             1
#define PROCESS_QUANTUM_END                                 2
#define PROCESS_PCB_END                                     3
#define QUANTUM_BASE                                        2

#define INITIALIZATION_SLEEP                                500000
#define VM_SLEEP                                            1
#define CPU_THREAD_SLEEP                                    1
#define IO_THREAD_SLEEP                                     1
#define MAX_THREAD_NAME                                     255

#define WINDOW_WIDTH                                        10
#define WINDOW_HEIGHT                                       12

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

typedef struct
{
    window_t *window;
    list_t *list;

} window_list_t;

// VM resources
#define CPU_CORES                                           4
list_t *cpu_queue, *io_queues[IO_DEVICES_SIZE], *process_queues[PROCESS_STATES_SIZE];
window_list_t windows[IO_DEVICES_SIZE + PROCESS_STATES_SIZE + 1];
window_t *process_window, *log_window;
pthread_t cpu_threads[CPU_CORES], io_threads[IO_DEVICES_SIZE];
pthread_mutex_t mutex;

pcb_t *create_pcb(pid_t pid)
{
    pcb_t *pcb;
    instruction_t *instruction;
    int index;

    pcb = (pcb_t *) malloc (sizeof(pcb_t));
    //pcb->priority = HIGH;
    pcb->priority = rand() % PROCESS_PRIORITIES_SIZE;
    pcb->pid = pid;
    pcb->uid = rand() % MAX_UID;
    pcb->gid = rand() % MAX_GID;
    memset(&pcb->stats, 0, sizeof(process_stats_t));
    //pcb->instructions = 1;
    pcb->instructions = (rand() % MAX_INSTRUCTIONS) + 1;
    pcb->program = (instruction_t *) malloc(sizeof(instruction_t) * pcb->instructions);
    instruction = pcb->program;
    for(index = 0; index < pcb->instructions; index++)
    {
        //instruction->type = IO;
        instruction->type = rand() % INSTRUCTION_TYPES_SIZE;
        //instruction->size = 10;
        instruction->size = (rand() % MAX_INSTRUCTION_SIZE) + 1;
        if(instruction->type == IO)
        {
            //instruction->device = HDD;
            instruction->device = rand() % IO_DEVICES_SIZE;   
        }
        instruction++;
    }
    pcb->ip = pcb->program;
    pcb->state = READY;

    return(pcb);
}

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

    wclear(process_window->user_window);
    wprintw(process_window->user_window, "PCB\n");
    wprintw(process_window->user_window, "[GENERAL]\n");
    wprintw(process_window->user_window, "PRIORITY             : %s\n", PROCESS_PRIORITIES[pcb->priority]);
    wprintw(process_window->user_window, "PID                  : %d\n", pcb->pid);
    wprintw(process_window->user_window, "UID                  : %d\n", pcb->uid);
    wprintw(process_window->user_window, "GID                  : %d\n", pcb->gid);
    wprintw(process_window->user_window, "STATE                : %s\n", PROCESS_STATES[pcb->state]);
    wprintw(process_window->user_window, "\n[STATS]\n");
    wprintw(process_window->user_window, "EXECUTION TIME       : %d\n", pcb->stats.execution_time);
    wprintw(process_window->user_window, "CPU TIME             : %d\n", pcb->stats.cpu_time);
    wprintw(process_window->user_window, "IO TIME              : %d\n", pcb->stats.io_time);
    wprintw(process_window->user_window, "\n[PROGRAM]\n");
    wprintw(process_window->user_window, "INSTRUCTIONS         : %d\n", pcb->instructions);
    instruction = pcb->program;
    for(index = 0; index < pcb->instructions; index++)
    {
        wprintw(process_window->user_window, "INSTRUCION[%d] TYPE   : %s\n", index, INSTRUCTION_TYPES[instruction->type]);
        wprintw(process_window->user_window, "INSTRUCION[%d] SIZE   : %d\n", index, instruction->size);
        if(instruction->type == IO)
        {
            wprintw(process_window->user_window, "INSTRUCION[%d] DEVICE : %s\n", index, IO_DEVICES[instruction->device]);
        }
        instruction++;
    }
    wrefresh(process_window->user_window);
}


void print_queues()
{
    node_t *node;
    pcb_t *pcb;
    int index;

    pthread_mutex_lock(&mutex);
    for(index = 0; index < PROCESS_STATES_SIZE + IO_DEVICES_SIZE + 1; index++)
    {
        wclear(windows[index].window->user_window);
        node = windows[index].list->head;
        while(node != NULL)
        {
            pcb = (pcb_t *) node->data;
            wprintw(windows[index].window->user_window, "%d\n", pcb->pid);
            node = node->next;
        }
        wnoutrefresh(windows[index].window->user_window);
    }
    doupdate();
    pthread_mutex_unlock(&mutex);
}

void finish_pcb(pcb_t *pcb)
{
    int child_status;

    pcb->state = FINISHED;
    kill(pcb->pid, SIGUSR1);
    waitpid(pcb->pid, &child_status, 0);
    pthread_mutex_lock(&mutex);
    list_remove_data(cpu_queue, pcb);
    list_remove_data(process_queues[RUNNING], pcb);
    list_remove_data(process_queues[ACTIVE], pcb);
    list_add_tail(process_queues[FINISHED], pcb);
    print_pcb(pcb);
    pthread_mutex_unlock(&mutex);
    wprintw(log_window->user_window, "PROCESS [%d] finished with status [%d]\n", pcb->pid, child_status);
    wrefresh(log_window->user_window);
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

void initilize_gui()
{
    int index, window_index = 0;

    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);

    process_window = window_create(WINDOW_HEIGHT * 3, WINDOW_WIDTH * 4, WINDOW_HEIGHT, 0);
    window_title(process_window, "PROCESS");
    wnoutrefresh(process_window->border_window);

    log_window = window_create(WINDOW_HEIGHT * 3, WINDOW_WIDTH * 10, WINDOW_HEIGHT, WINDOW_WIDTH * 4);
    scrollok(log_window->user_window, TRUE);
    window_title(log_window, "LOG");
    wnoutrefresh(log_window->border_window);

    for(index = 0; index < PROCESS_STATES_SIZE; index++)
    {
        windows[window_index].window = window_create(WINDOW_HEIGHT, WINDOW_WIDTH, 0, WINDOW_WIDTH * window_index);
        window_title(windows[window_index].window, PROCESS_STATES[index]);
        wnoutrefresh(windows[window_index].window->border_window);
        window_index++;
    }

    for(index = 0; index < IO_DEVICES_SIZE; index++)
    {
        windows[window_index].window = window_create(WINDOW_HEIGHT, WINDOW_WIDTH, 0, WINDOW_WIDTH * window_index);
        window_title(windows[window_index].window, IO_DEVICES[index]);
        wnoutrefresh(windows[window_index].window->border_window);
        window_index++;
    }

    windows[window_index].window = window_create(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 10 * window_index);
    window_title(windows[window_index].window, "CPU");
    wnoutrefresh(windows[window_index].window->border_window);

    doupdate();
}

void initialize_resources()
{
    int index, window_index = 0, exitcode, *device, *cpuid;
    char thread_name[MAX_THREAD_NAME];
    node_t *node;

    initilize_gui();

    wprintw(log_window->user_window, "Initializing resources ...\n");
    wrefresh(log_window->user_window);
    
    for(index = 0; index < PROCESS_STATES_SIZE; index++)
    {
        process_queues[index] = list_new();
        windows[window_index].list = process_queues[index];
        window_index++;
    }
    usleep(INITIALIZATION_SLEEP);
    wprintw(log_window->user_window, "* PROCESS queues\n");
    wrefresh(log_window->user_window);

    for(index = 0; index < IO_DEVICES_SIZE; index++)
    {
        io_queues[index] = list_new();
        windows[window_index].list = io_queues[index];
        window_index++;
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
            wprintw(log_window->user_window, "could not create io_thread()");
            wrefresh(log_window->user_window);
        }
    }
    usleep(INITIALIZATION_SLEEP);
    wprintw(log_window->user_window, "* DEVICE queues and threads\n");
    wrefresh(log_window->user_window);

    cpu_queue = list_new();
    windows[window_index].list = cpu_queue;
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
            wprintw(log_window->user_window, "could not create cpu_thread()");
            wrefresh(log_window->user_window);
        }
    }
    usleep(INITIALIZATION_SLEEP);
    wprintw(log_window->user_window, "* CPU queue and threads\n");
    wrefresh(log_window->user_window);
}

void create_processes()
{
    int processes;
    pcb_t *pcb;

    // start the random seed
    srand(time(0));

    //for(processes = 6; processes; processes--)
    wprintw(log_window->user_window, "Creating random processes ...\n");
    wrefresh(log_window->user_window);
    for(processes = (rand() % MAX_PROCESSES) + 1; processes; processes--)
    {
        pcb = create_process();
        pcb->state = NEW;
        pthread_mutex_lock(&mutex);
        list_add_tail(process_queues[NEW], pcb);
        pthread_mutex_unlock(&mutex);
        usleep(500000);
        wprintw(log_window->user_window, "* PROCESS [%d]\n", pcb->pid);
        wrefresh(log_window->user_window);
        print_pcb(pcb);
    }
}

void update_execution_time()
{
    node_t *node_index;
    pcb_t *pcb;

    pthread_mutex_lock(&mutex);
    node_index = process_queues[ACTIVE]->head;
    while(node_index != NULL)
    {
        pcb = node_index->data;
        pcb->stats.execution_time++;
        node_index = node_index->next;
    }
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

        // update execution time on active processes
        update_execution_time();
        
        // take some tome between cycles
        sleep(VM_SLEEP);
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
