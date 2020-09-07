#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#include <sys/types.h>
#include <sys/stat.h>

#include <X11/XKBlib.h>

int main()
{
    pid_t pid, sid;

    // Fork off parent process
    pid = fork();
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    // If good PID, exit parent process
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    // Change file mode mask
    umask(0);

    // Create new SID for child
    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Change working directory
    if ((chdir("/")) < 0)
    {
        exit(EXIT_FAILURE);
    }

    // Close out sttandard file descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    Display * d = XOpenDisplay((char*)0);

    if (d)
    {
        unsigned n;

        // The big loop
        while (1)
        {
            XkbGetIndicatorState(d, XkbUseCoreKbd, &n);

            if ((n & 1))
                system("dunstify -r 100 'Caps Lock' -t 1100");

            sleep(1);
        }
    }

    exit(EXIT_SUCCESS);
}
