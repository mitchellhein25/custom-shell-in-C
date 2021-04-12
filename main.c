#include "main.h"

int main(int argc, char **argv)
{

    struct cmdEntry* cmdCurr;
    char* command = malloc(2048);
    bool run = true;

    while(run == true) {

        // Read in command
        command = readCommand(command);
        cmdCurr = createCmdEntry(command);
        printCommand(cmdCurr);
        

        if (strcmp(cmdCurr->command, "exit") == 0){
            return EXIT_SUCCESS;
        }

        if (strstr(cmdCurr->command, "cd") != NULL){
            printf("command that is being passed: %s\n", cmdCurr->args[0]);
            cdFunc(cmdCurr->args[0]);
        }

        if (strcmp(cmdCurr->command, "status") == 0){
        }

    }
}
