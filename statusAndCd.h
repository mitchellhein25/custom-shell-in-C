void cdFunc(char* dir) {
    char s[100];
    
    printf("printing dir: %s\n", dir);

    // printing current working directory
    printf("CWD: %s\n", getcwd(s, 100));

    if (dir == NULL){
        printf("No directory given, use home\n");
        chdir(getenv("HOME"));
    }
    else {
        char* newDir = malloc(2048);
        strcpy(newDir, dir);
        printf("new_dir: %s\n", newDir);
        char* backslash = "/";
        char* relPath= getcwd(s, 100);
        printf("%s\n", relPath);
        char* relWSlash = strcat(relPath, backslash); 
        char* fullRelPath = strcat(relWSlash, newDir);
        printf("%s\n", fullRelPath);
        chdir(fullRelPath);
        if (chdir(fullRelPath) != 0) {
            perror("ERROR");
        }
    }

    // printing current working directory
    printf("CWD: %s\n", getcwd(s, 100));

    // // printf("%s", "entered cd\n");
    // char* env = getenv("HOME");
    // printf("%s\n", env);
        
}