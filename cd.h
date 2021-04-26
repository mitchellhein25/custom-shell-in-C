void cdFunc(char* dir) {
    char s[100];

    // If command has no argument, go to HOME environment variable
    if (dir == NULL){
        chdir(getenv("HOME"));

    // For absolute paths, which will contain / characters, process directly
    }else if (strstr(dir, "/") != NULL){
        chdir(dir);
        if (chdir(dir) != 0) {
            perror("ERROR");
            fflush(stdout);
        }

    // For relative paths, which will be run relative to the current directory
    // We have to build out the current directory string and add on the new directory to the end
    } else {
        char* newDir = malloc(2048);
        strcpy(newDir, dir);
        char* backslash = "/";
        char* relPath= getcwd(s, 100);
        char* relWSlash = strcat(relPath, backslash); 
        char* fullRelPath = strcat(relWSlash, newDir);
        chdir(fullRelPath);
        if (chdir(fullRelPath) != 0) {
            perror("ERROR");
            fflush(stdout);
        }
    }    
}