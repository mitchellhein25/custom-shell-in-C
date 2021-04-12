char* readCommand(char* command) {
    
    printf(": ");
    scanf("%[^\n]%*c", command);
    // fgets(command, 2048, stdin);
    // printf("read command");
    return command;

}

