#ifndef _TASKS_TASK_AXOCORE_H
#define _TASKS_TASK_AXOCORE_H 1

#include <stdint.h>

#define MAX_PROCESSES 0xFFFF

typedef struct {
    uint32_t esp;   /* ESP register state */
    uint32_t pid;   /* Process ID */
    uint8_t state;  /* State of process.
                     * 0 - READY
                     * 1 - BUSY
                     * 2 - IDLE
                     */
    void* mailbox;
} process_t;

extern process_t* cur_process;
extern process_t process_table[MAX_PROCESSES];

#endif // _TASKS_TASK_AXOCORE_H