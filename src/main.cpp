#include "core/program.h"

int main(void) {
    Program program;

    program.Init("Boids");
    program.PrintStats();

    program.RunLoop();

    program.Shutdown();

    return 0;
}