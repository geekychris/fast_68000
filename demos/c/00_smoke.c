/* 00_smoke.c — smallest C demo.  Every core prints "hi core N" and
 * halts with code 0.  Pass = all cores halt 0. */
#include "sync.h"
#include "io.h"

int main(int core)
{
    print("hi core ");
    putu((uint32_t)core);
    putc('\n');
    return 0;
}
