#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

// Methods from https://man7.org/linux/man-pages/man2/syscalls.2.html

const char *source_file_name = "file_to_read.txt";
const char *destination_file_name = "file_to_write.txt";

size_t strlen(const char *msg) { // self defined strlen to don't use libc
    for (size_t i = 0;; i++)
        if (msg[i] == '\0')
            return i;
}

void print_message(const char *msg) {
    size_t len = strlen(msg); // NOT LIBC strlen! See above
    write(STDOUT_FILENO, msg, len);
    const char *new_line = "\n";
    write(STDOUT_FILENO, new_line, 1);
}

int main() {
    int is_child = fork() == 0;
    // Open for read
    int source_file_fd = open(source_file_name, O_RDONLY); // https://man7.org/linux/man-pages/man2/open.2.html
    if (source_file_fd == -1) {
        print_message("Cannot open source file");
        _exit(1); // https://man7.org/linux/man-pages/man2/exit.2.html
    }
    // Open output file
    struct stat st;
    if (fstat(source_file_fd, &st) == -1) { // https://man7.org/linux/man-pages/man2/fstat.2.html
        print_message("Cannot get file info");
        _exit(1);
    }
    // Allocate memory and read all file into memory because fuck the large files
    // We use sbrk to change data segment and don't reach stack limits while not using malloc
    char *buffer = sbrk(st.st_size * sizeof(char));
    if (buffer == (void *) (-1)) {
        print_message("Out of memory!");
        _exit(1);
    }
    if (read(source_file_fd, buffer, st.st_size) == -1) { // https://man7.org/linux/man-pages/man2/read.2.html
        print_message("Cannot read file");
        _exit(1);
    }
    // Done with source so close it
    if (close(source_file_fd) == -1) { // https://man7.org/linux/man-pages/man2/close.2.html
        print_message("Cannot close source file (wtf)");
        _exit(1);
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
        print_message("Cannot open destination file");
        _exit(1);
    }
    // If this is child, we reverse the array
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
    if (write(destination_file_fd, buffer, st.st_size) == -1) { // https://man7.org/linux/man-pages/man2/write.2.html
        print_message("Cannot write buffer");
        _exit(1);
    }
    // Done
    if (close(destination_file_fd) == -1) {
        print_message("Cannot close destination file (wtf)");
        _exit(1);
    }
}
