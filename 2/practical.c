#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

// Methods from https://docs.oracle.com/cd/E19048-01/chorus5/806-6897/auto1/index.html

const char *source_file_name = "file_to_read.txt";
const char *destination_file_name = "file_to_write.txt";

void print_message(const char *msg) {
    puts(msg);
}

int main() {
    int is_child = fork() == 0;
    // Open for read
    int source_file_fd = open(source_file_name, O_RDONLY); // https://linux.die.net/man/3/open
    if (source_file_fd == -1) {
        perror("Cannot open source file"); // https://linux.die.net/man/3/perror
        exit(1); // https://linux.die.net/man/3/exit
    }
    // Open output file
    struct stat st;
    if (fstat(source_file_fd, &st) == -1) {// https://linux.die.net/man/2/fstat
        perror("Cannot get file info");
        exit(1);
    }
    // Allocate memory and read all file into memory because fuck it we have a fuckton of ram these days
    char *buffer = malloc(st.st_size * sizeof(char)); // https://man7.org/linux/man-pages/man3/malloc.3.html
    if (buffer == NULL) {
        print_message("Out of memory!");
        exit(1);
    }
    if (read(source_file_fd, buffer, st.st_size) == -1) {
        perror("Cannot read file");
        exit(1);
    }
    // Done with source so close it
    if (close(source_file_fd) == -1) {
        perror("Cannot close source file (wtf)");
        exit(1);
    }
    // If this is parent wait for child to exit
    if (!is_child)
        wait(NULL); // https://man7.org/linux/man-pages/man2/wait.2.html
    // Open the destination file
    int destination_file_flags = O_WRONLY | O_CREAT;
    if (!is_child) // only append on parent
        destination_file_flags |= O_APPEND;
    else // otherwise delete old file
        destination_file_flags |= O_TRUNC;
    int destination_file_fd = open(destination_file_name, destination_file_flags, 0666);
    if (destination_file_fd == -1) {
        perror("Cannot open destination file");
        exit(1);
    }
    // If this is client, we reverse the array
    if (is_child) {
        size_t start = 0, end = st.st_size - 1;
        while (start < end) {
            char temp = buffer[start];
            buffer[start] = buffer[end];
            buffer[end] = temp;
            start++;
            end--;
        }
    }
    if (write(destination_file_fd, buffer, st.st_size) == -1) {
        perror("Cannot write buffer");
        exit(1);
    }
    // Done
    if (close(destination_file_fd) == -1) {
        perror("Cannot close destination file (wtf)");
        exit(1);
    }
}
