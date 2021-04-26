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
    
    //Initialize variables/properties
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

    //The first argument in the args array will also be the command
    currCmdEntry->args[0] = currCmdEntry->command;
    char* currArg = strtok(NULL, " ");
    int x = 1;
    //Iterate through all of the arguments
    while (currArg != NULL) {

        //If < is encountered, next arg is the input file
        if (strcmp(currArg, "<") == 0) {
            currArg = strtok(NULL, " ");
            currCmdEntry->inputFile = currArg;
            currArg = strtok(NULL, " ");
            if (currArg == NULL) {
                continue;
            }
        }

        //If > is encountered, next arg is the output file
        if (strcmp(currArg, ">") == 0) {
            currArg = strtok(NULL, " ");
            currCmdEntry->outputFile = currArg;
            currArg = strtok(NULL, " ");
            if (currArg == NULL) {
                continue;
            }
        }
        
        //If & is encountered, process will run in background, so we set foreBack to true
        if (strcmp(currArg, "&") == 0) {
            currCmdEntry->foreBack = true;
            break;
        }

        //If no special case met, add arg to the args array and go to next argument
        currCmdEntry->args[x] = currArg;
        currArg = strtok(NULL, " ");
        x++;
    }

    return currCmdEntry;
}

//This was a helper function to print out the command entry, not actually used in finished product
void printCommand(struct cmdEntry* cmdCurr) {
    fprintf(stdout, "command: %s\n", cmdCurr->command);
    int i = 0;
    while (cmdCurr->args[i] != NULL){
         fprintf(stdout, "arg %d: %s\n", i, cmdCurr->args[i]);
        i++;
    }
    fprintf(stdout, "inputFile: %s\n", cmdCurr->inputFile);
    fprintf(stdout, "outputFile: %s\n", cmdCurr->outputFile);
    fprintf(stdout, "foreBack: %d\n", cmdCurr->foreBack);
}