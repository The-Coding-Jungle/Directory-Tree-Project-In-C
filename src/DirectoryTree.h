#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void directoryTree(char* path, FILE* fp, int depth, bool hiddenFiles) {
    DIR* dir;
    struct dirent* ent;
    
    dir = opendir(path);

    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            // Skip current dir and parent dir to avoid infinite recursion
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
                continue;
            }

            // If hidden files are not to be displayed, skip them
            if (!hiddenFiles && ent->d_name[0] == '.') {
                continue;
            }
            
            for (int i = 0; i < depth; i++) {
                fprintf(fp, "--");
            }
            fprintf(fp, "%s\n", ent->d_name);

            if (ent->d_type == DT_DIR) {
                char* newPath = malloc(strlen(path) + strlen(ent->d_name) + 2);
                strcpy(newPath, path);
                #if defined(_WIN32) || defined(__CYGWIN__) 
                    // Concat path and "\" and ent->d_name
                    strcat(newPath, "\\");
                #else 
                    // Concat path and "/" and ent->d_name
                    strcat(newPath, "/");
                #endif
                strcat(newPath, ent->d_name);
                directoryTree(newPath, fp, depth + 1, hiddenFiles);
            }
        }
        closedir(dir);
    }
}