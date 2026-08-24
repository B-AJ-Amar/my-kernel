#ifndef SHED_H
#define SHED_H

#include <stdint.h>

typedef enum {
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED
} process_state_t;

typedef struct process_t{
    uint32_t pid;
    process_state_t state;
    uint32_t *stack;
    uint32_t *page_directory;
    struct process_t *next;
} process_t;


void shed_init(void);
#endif