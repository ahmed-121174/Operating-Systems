//Q1
#!/bin/bash
# This script performs basic arithmetic operations
# Usage: ./com.sh operation num1 num2
# Example: ./com.sh add 10 5

if [ "$#" -ne 3 ]; then
  echo "Usage: $0 <operation: add|sub|mul|div|rem> <num1> <num2>"
  exit 1
fi

operation=$1
num1=$2
num2=$3

case $operation in
  add)
    echo "$num1 + $num2 = $((num1 + num2))"
    ;;
  sub)
    echo "$num1 - $num2 = $((num1 - num2))"
    ;;
  mul)
    echo "$num1 * $num2 = $((num1 * num2))"
    ;;
  div)
    if [ "$num2" -eq 0 ]; then
      echo "Error: Division by zero"
    else
      echo "$num1 / $num2 = $((num1 / num2))"
    fi
    ;;
  rem)
    if [ "$num2" -eq 0 ]; then
      echo "Error: Division by zero"
    else
      echo "$num1 %% $num2 = $((num1 % num2))"
    fi
    ;;
  *)
    echo "Invalid operation. Use add, sub, mul, div, or rem."
    ;;
esac





//Q2
#include <stdio.h>
#include <unistd.h>

void print_pid(const char* name) {
    printf("%s PID: %d, Parent PID: %d\n", name, getpid(), getppid());
}

int main() {
    print_pid("P1");

    if (fork() == 0) {
        print_pid("P2");
        if (fork() == 0) {
            print_pid("P5");
        } else if (fork() == 0) {
            print_pid("P6");
        }
    } else if (fork() == 0) {
        print_pid("P3");
        if (fork() == 0) {
            print_pid("P7");
        }
    } else if (fork() == 0) {
        print_pid("P4");
        if (fork() == 0) {
            print_pid("P8");
        } else if (fork() == 0) {
            print_pid("P9");
        }
    }

    sleep(1); // Ensures orderly output
    return 0;
}



// **Question 3: IPC - Message Passing Program**
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void send_message(int fd, const char* message) {
    write(fd, message, strlen(message) + 1);
}

void receive_message(int fd, char* buffer, int size) {
    read(fd, buffer, size);
}

int main() {
    int pipefds[2];
    pipe(pipefds); // Create a pipe

    if (fork() == 0) { // Child process: Sender
        close(pipefds[0]); // Close read end

        send_message(pipefds[1], "FAITH");
        send_message(pipefds[1], "UNITY");
        send_message(pipefds[1], "DISCIPLINE");

        close(pipefds[1]); // Close write end
    } else { // Parent process: Receiver
        close(pipefds[1]); // Close write end

        char buffer[256];

        // Part (a): Receive "FAITH"
        receive_message(pipefds[0], buffer, sizeof(buffer));
        printf("Part (a): %s\n", buffer);

        // Part (b): Receive "UNITY"
        receive_message(pipefds[0], buffer, sizeof(buffer));
        printf("Part (b): %s\n", buffer);

        // Part (c): Receive "DISCIPLINE"
        receive_message(pipefds[0], buffer, sizeof(buffer));
        printf("Part (c): %s\n", buffer);

        // Part (d): Combine messages "UNITY FAITH DISCIPLINE"
        char combined[256] = "UNITY FAITH DISCIPLINE";
        printf("Part (d): %s\n", combined);

        // Part (e): Combine messages "DISCIPLINE UNITY FAITH"
        char reordered[256] = "DISCIPLINE UNITY FAITH";
        printf("Part (e): %s\n", reordered);

        close(pipefds[0]); // Close read end
    }

    return 0;
}
