#include <proc.h>
#include <stdio.h>

struct xthread_event_t e;
bool isEmpty = true;  // Q is empty or not;
int qi = 0;  // track the Q[i]

int k = 1;
void foo() {
	int id = k++;
	printf("foo %d begins\n", id);
	xthread_wait_ev(&e);  // thread will wait/ be inserted to Q
	printf("foo %d Done\n", id);
}

void bar() {
	int id = k++;
	printf("bar %d begins\n", id);
	xthread_set_ev(&e);
	printf("bar %d Done\n", id);
}

void xmain() {
    printf("Starting with xmain_part2\n");
    xthread_init_ev(&e);  // the event is set NOT_OCCURRED

    int usec = ualarm(0,0);  // disable the interrupt
    xthread_create(foo, 0, NULL);
    xthread_create(foo, 0, NULL);
    xthread_create(foo, 0, NULL);
    xthread_create(foo, 0, NULL);
    xthread_create(bar, 0, NULL);
    ualarm(usec, 0);  // enbale the interrupt
    
    xthread_wait_ev(&e);  // main thread will wait

    printf("xmain Done\n");
}
