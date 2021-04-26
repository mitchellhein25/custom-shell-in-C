#include "main.h"

bool processInBack  = false; //controls foreground/background mode
bool signalState    = false; //indicates if SIGTSTP has been activated

/* Our signal handler for SIGTSTP */
void handle_SIGTSTP(int signo){
    char* message;
    char* exitMessage;

    //If SIGTSTP entered first time, enter foreground only, otherwise turn that mode off
    if (signalState == true) {
        exitMessage = malloc(30);
	    exitMessage = "Exiting foreground-only mode\n";
        signalState = false;
        //Use write instead of printf since we are in a signal handler
        write(1, exitMessage, 30);
        fflush(stdout);
    } else {
        message = malloc(50);
        message = "Entering foreground-only mode (& is now ignored)\n";
        signalState = true;
        //Use write instead of printf since we are in a signal handler
        write(1, message, 48);
        write(1, "\n", 1);
        fflush(stdout);
    }
}

int main(int argc, char **argv)
{
 
    // Establish the handler for SIGTSTP
	struct sigaction SIGTSTP_foreground = {{0}};
    SIGTSTP_foreground.sa_handler = handle_SIGTSTP;
    sigaction(SIGTSTP, &SIGTSTP_foreground, NULL);

    //Declare variables
    int pid = getpid();         //parent process pid
    int childPid;               //child pid
    int statusVar = 0;          //variable to track status
    int *pair;                  //status and child pid
    struct cmdEntry* cmdCurr;   //Struct for the command entry
    char commandR[2048];        //Holder for command as string before processing
    char* command = malloc(2048);//Holds cleaned command string
    bool run = true;            //holds the while loop open

    while(run == true) {

        // Read in command
        command = readCommand(pid, commandR);

        //If the command is empty or a comment, read in another command
        //This reads the first Char and if it is blank or a #, then continue
        char firstChar[2];
        memset(firstChar, '\0', sizeof(firstChar));
        strncpy(firstChar, command, 1);
        if (strcmp(command, "\0") == 0 || strcmp(firstChar, "#") == 0) {
            continue;
        }

        //Turn the command into a struct, as defined in cmdEntry.h
        cmdCurr = createCmdEntry(command);

        //If SIGTSTP has entered foreground only, set foreBack to false
        if (signalState == true) {
            cmdCurr->foreBack = false;
        }

        //If command is exit, the return and exit the program
        if (strcmp(cmdCurr->command, "exit") == 0){
            return EXIT_SUCCESS;
        }

        //If command has cd in it, pass to the location (the next arg) to the cd function
        if (strstr(command, "cd") != NULL){
            cdFunc(cmdCurr->args[1]);
            continue;
        }

        //If the command is status, then print the status (Stored in statusVar)
        if (strcmp(cmdCurr->command, "status") == 0){
            printf("exit value %d\n", statusVar);
            fflush(stdout);
            continue;
        }

        //If not a predefined function, run the command with the runCommand function (returns status and childPid)
        pair = runCommand(cmdCurr->command, cmdCurr->args, cmdCurr->foreBack, statusVar, cmdCurr->outputFile, cmdCurr->inputFile);

        //Set the new status
        statusVar = pair[0];

        //if the & was entered, set the new childPid
        if (cmdCurr->foreBack == true) {
            childPid = pair[1];
        }
        
        //If the & was entered or the background process is still running
        if (cmdCurr->foreBack == true || processInBack == true) {
            int childStatus;
            int finished;

            //Check the status of the process running in the background
            finished = waitpid(childPid, &childStatus, WNOHANG);

            //If finished is non-zero, the process is done running, and we will print its status and pid
            if (finished != 0) {
                processInBack = false; //no more processes running in Back

                //If it exited normally
                if (WIFEXITED(childStatus)) {
                    statusVar = WEXITSTATUS(childStatus);
                    printf("BACKGROUND pid %d is done. exit value %d\n", childPid, statusVar);
                    fflush(stdout);

                //If exited abnormally
                } else {
                    statusVar = WTERMSIG(childStatus);
                    printf("BACKGROUND pid %d is done. exit value %d\n", childPid, statusVar);
                    fflush(stdout);
                }
            } else {
                processInBack = true;
            }
            
        }
    }
}
