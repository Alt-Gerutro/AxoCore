#include "tasks/task.h"
#include <tasks/sheduler.h>

void shedule() {
    process_t save;
    for (uint32_t i = 0; i < MAX_PROCESSES; i++) {
        save.esp = cur_process->esp;
        save.pid = cur_process->pid;
        save.state = cur_process->state;
        save.mailbox = cur_process->mailbox;
        
        
    }
}