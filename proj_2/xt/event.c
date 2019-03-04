#include <proc.h>

struct xthread_event_t {
    bool occured;
    int Q[NPROC];
};

//extern struct xthread_event_t e;
