#include <timer/pit.h>

void sleep(unsigned int ms) { pit_wait(ms); }
