/* struct for cmdEntry*/
struct cmdEntry
{
    char* command; // first string entered is the command

    // initialize array of arguments (up to 512)
    char** args;

    char* inputFile; 
    char* outputFile;
    bool foreBack; // 0 (false) = foreground, 1 (true) = background
};

/* Parse the current line which is space delimited and create a
*  cmdEntry struct with the data in this line
*/
struct cmdEntry* createCmdEntry(char* line)
{
    struct cmdEntry* currCmdEntry = malloc(sizeof(struct cmdEntry));

    currCmdEntry->command = malloc(2048);
    currCmdEntry->command = NULL;

    // initialize array of arguments (up to 512)
    currCmdEntry->args = malloc(512 * sizeof(char*)); 
    if (currCmdEntry->args){
        for (int i = 0; i < 512; i++) {
        currCmdEntry->args[i] = malloc(255 * sizeof *currCmdEntry->args[i]);
        currCmdEntry->args[i] = NULL;
        }
    }

    currCmdEntry->inputFile = malloc(2048); 
    currCmdEntry->inputFile = NULL;
    currCmdEntry->outputFile = malloc(2048);
    currCmdEntry->outputFile = NULL;
    currCmdEntry->foreBack = malloc(1); // 0 (false) = foreground, 1 (true) = background
    currCmdEntry->foreBack = false;

    // The first token is the command
    char* command = strtok(line, " ");
    currCmdEntry->command = calloc(strlen(command) + 1, sizeof(char));
    strcpy(currCmdEntry->command, command);
    // printf("commans");
    // printf("command: %s", currCmdEntry->command);

    // char* currArg = strtok(NULL, " ");
    // currCmdEntry->args[0] = currArg;

    // printf("first argument: %s\n", currArg);

    char* currArg = strtok(NULL, " ");
    int x = 0;
    // printf("currArg");
    while (currArg != NULL) {
        // printf("currArg");
        if (strcmp(currArg, "<") == 0) {
            currArg = strtok(NULL, " ");
            currCmdEntry->inputFile = currArg;
            currArg = strtok(NULL, " ");
            if (currArg != NULL) {
                continue;
            }
        }

        if (strcmp(currArg, ">") == 0) {
            currArg = strtok(NULL, " ");
            currCmdEntry->outputFile = currArg;
            currArg = strtok(NULL, " ");
            if (currArg != NULL) {
                continue;
            }
        }
        
        if (strcmp(currArg, "&") == 0) {
            currCmdEntry->foreBack = true;
            break;
        }

        currCmdEntry->args[x] = currArg;
        currArg = strtok(NULL, " ");
        x++;
    }

    return currCmdEntry;
}

void printCommand(struct cmdEntry* cmdCurr) {
      printf("command: %s\n", cmdCurr->command);
    int i = 0;
    while (cmdCurr->args[i] != NULL){
        printf("arg %d: %s\n", i, cmdCurr->args[i]);
        i++;
    }
    if (cmdCurr->inputFile != NULL) {
        printf("inputFile: %s\n", cmdCurr->inputFile);
    }
    if (cmdCurr->outputFile != NULL) {
        printf("outputFile: %s\n", cmdCurr->outputFile);
    }
    printf("foreBack: %d\n", cmdCurr->foreBack);
}