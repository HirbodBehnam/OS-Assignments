#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main() {
        for (int i = 0; i < 100000000; i++);
        fork();
        printf("%ld\n", clock());
        sleep(1);
}

// Output:
// 150025
// 98