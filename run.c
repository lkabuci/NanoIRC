#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    for (int i = 0; i < 10; ++i) {
        if (fork() == 0) {
            system("python3 main.py");
        }
    }
}
