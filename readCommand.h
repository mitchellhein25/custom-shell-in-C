char* varExpand(int pid, char* command) {

    int alreadyExpanded = 0; //If already expanded, tack on to expanded instead of original
    char pidS[100];         // Holds the pid as a string

    //Convert pid to String
    sprintf(pidS, "%d\n", pid); 
    char expanded[2048];    //Holds the expanded variable command

    //If the $$ is in the command, parse it
    if (strstr(command, "$$") != NULL){

        //Iterate through until the end of the command
        for (int x = 0; command[x] != '\0'; x++) {

            //Iterate until hitting $$
            if (command[x] == '$') {
                x++;
                if (command[x] == '$') {
                    int y = 0;
                    char copy[x];

                    //Iterate through the string until $$, copying the command into copy variable
                    while (y < x - 1) {
                        if (command[y] == '$') {
                            break;
                        }
                        copy[y] = command[y];
                        y++;
                    }
                    copy[x-1] = '\0';

                    //Count the size of the pid that will be appended to copy
                    int count = 0;
                    while (pid != 0) {
                        count++;
                        pid = pid / 10;
                    }

                    //If not yet expanded, append the pidS to copy 
                    if (alreadyExpanded == 0) {
                        snprintf(expanded, sizeof(copy) + count, "%s%s", copy, pidS);
                        alreadyExpanded = 1;

                    //If already expanded, append the pidD to expanded
                    } else {
 
                        snprintf(expanded, sizeof(copy) + count, "%s%s", expanded, pidS);
                    }

                    //Return the expanded command
                    command = expanded;
                    return command;
   
                }
            }
        }
    }
    //If no $$, return the original command
    return command;
}

// Reads in the command from the user using fgets then returns it, after expanding
char* readCommand(int pid, char command[2048]) {
    
    fprintf(stdout, ": ");
    fflush(stdout);
    scanf("%[^\n]%*c", command); // Reads in any non newline characters

    //Expand any $$ that were entered
    strcpy(command, varExpand(pid, command));

    return command;

}

//Runs the command that was passed by the user
int * runCommand(char* command, char** args, bool foreBack, int statusVar, char* outputFile, char* inputFile) {

        int   childStatus;

        //Fork a child process
        pid_t childPid = fork();
    
        // Code in this branch will be exected by the parent when fork() fails and the creation of child process fails as well
        if(childPid == -1){
                perror("fork() failed!");
                fflush(stdout);
                exit(1);

        // childPid is 0. This means the child will execute the code in this branch  
        } else if(childPid == 0){     

                //If there is an input file
                if (inputFile != NULL) {
                    
                    //Open the input file
                    int file = open(inputFile, O_RDWR, 0600);
                    if (file == -1){
                        printf("open() failed on \"%s\"\n", inputFile);
                        fflush(stdout);
                        perror("Error");
                        fflush(stdout);
                        exit(1);
                    }

                    //dup2 redirects input of stdin from the file
                    dup2(file, 0); 
                    
                    //If there is an output file
                    if (outputFile != NULL) {
                        
                        //open output file
                        int file2 = open(outputFile, O_RDWR | O_CREAT, 0600);
                        if (file == -1){
                            printf("open() failed on \"%s\"\n", outputFile);
                            fflush(stdout);
                            perror("Error");
                            fflush(stdout);
                            exit(1);
                        }
  
                        //dup2 redirects output of stdout to the file
                        dup2(file2, 1);

                        //This will execute the command from the input file into the output file
                        execvp(command, args);
                        perror("That command could not be executed: ");
                        fflush(stdout);
                        // exit(errno);
                        close(file2);
    
                    } else {

                        //This will execute the command from the input file to standard output
                        execvp(command, args);
                        perror("That command could not be executed: ");
                        fflush(stdout);
                        close(file);
                    }
                    
                //If no input file, but still an output file
                } else if (outputFile != NULL) {
                    
                    // Open output file
                    int file = open(outputFile, O_RDWR | O_CREAT, 0600);
                    if (file == -1){
                            printf("open() failed on \"%s\"\n", outputFile);
                            fflush(stdout);
                            perror("Error");
                            fflush(stdout);
                            exit(1);
                        }
    
                    //dup2 redirects output of stdout to the file
                    dup2(file, 1); 

                    //Execute the command to the output file
                    execvp(command, args);
                    perror("That command could not be executed: ");
                    fflush(stdout);
                    close(file);
                
                //No input or output file
                } else {
                    
                    //Execute command to standard input/output
                    execvp(command, args);
                    perror("That command could not be executed: ");
                    fflush(stdout);
                    exit(EXIT_FAILURE);
                }
        
        //Parent process handling
        } else {

            // If no & sign, run in the foreground and call waitpid on child 
            if (foreBack == false) {

                // Since process is running in the foreground, the parent will wait for the child
                waitpid(childPid, &childStatus, 0); 
                
                //If exited normally, set the status accordingly
                if (WIFEXITED(childStatus)) {
                    statusVar = WEXITSTATUS(childStatus);
                    
                //If exited abnormally, set the status accordingly
                } else {
                    statusVar = WTERMSIG(childStatus);
                }
 
            } else {
                // waitpid with NOHANG to run in the background
                // Since process is running in the background, the parent will not wait for the child
                fprintf(stdout, "BACKGROUND pid is %d\n", childPid);
                fflush(stdout);
                waitpid(childPid, &childStatus, WNOHANG);
            }
        }

        //Return the status and childs pid
        static int ret[2];
        ret[0] = statusVar;
        ret[1] = childPid;
        return ret;
     
}

