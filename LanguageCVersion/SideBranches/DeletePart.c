#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define SizeMaxPage 256

typedef struct {
    char UId[15];
    char UPass[15];
} User;

typedef struct {
    char password[11];  // 10 characters max
    char note[SizeMaxPage]; // max page size
} Page;


void CurrentDirectory(char *CDirectory) {
    if (getcwd(CDirectory, PATH_MAX) != NULL) {
        printf("Current directory: %s\n", CDirectory);
    } else {
        perror("Problem with authorization, please fix it in settings");
    }
}

int CREATE_DIR(const char *path) {
    return mkdir(path, 0777);  
}

void FolderCreation(char *CDirectory, User *TempUser) {

    char TempUserFoldPath[PATH_MAX];
    char TempUserFoldPathbis[PATH_MAX];

    snprintf(TempUserFoldPath, PATH_MAX, "%s/%s", CDirectory, TempUser->UId);

    if (CREATE_DIR(TempUserFoldPath) == 0) {
        printf("User folder successfully created: %s\n", TempUserFoldPath);

        char PassFilePath[PATH_MAX];
        snprintf(PassFilePath, PATH_MAX, "%s/password.txt", CDirectory);

        FILE *Afile = fopen(PassFilePath, "a");
        if (Afile == NULL) {
            perror("Error opening password.txt");
            return;
        }

        fprintf(Afile, "%s %s\n", TempUser->UId, TempUser->UPass);
        fclose(Afile);
        printf("User account successfully created.\n");

        snprintf(TempUserFoldPathbis, PATH_MAX, "%s/%s/BlockedPages", CDirectory, TempUser->UId);
        if (CREATE_DIR(TempUserFoldPathbis) == 0) {
            printf("BlockedPages folder successfully created: %s\n", TempUserFoldPathbis);
        } else {
            perror("Error creating BlockedPages folder");
        }
    } else {
        perror("Error creating account folder");
    }
}

void BlockedAccessPage(char *SourcePath, char *DestinationPath, User *TempUser, char *CDirectory) {
    
    snprintf(SourcePath, PATH_MAX, "%s/%s/Page1.txt", CDirectory, TempUser->UId);

    FILE *Source = fopen(SourcePath, "rb");  
    if (Source == NULL) {
        perror("Error during open SourcePath");
        return 1;
    }

    snprintf(DestinationPath, PATH_MAX, "%s/%s/BlockedPages/Page1.txt", CDirectory, TempUser->UId);
    FILE *destination = fopen(DestinationPath, "wb");  
    if (destination == NULL) {
        perror("Error during create DestinationPath");
        fclose(Source);
        return 1;
    }

    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), Source)) > 0) {
        fwrite(buffer, 1, bytesRead, destination);
    }

    fclose(Source);
    fclose(destination);

    printf("File successfully copy at %s to %s\n", SourcePath, DestinationPath);
}

int main() {
    char CDirectory[PATH_MAX];
    User TempUser;

    printf("Choose a user ID: ");
    scanf("%14s", TempUser.UId); 

    printf("Choose a user password: ");
    scanf("%14s", TempUser.UPass); 

    CurrentDirectory(CDirectory);
    FolderCreation(CDirectory, &TempUser);

    char SourcePath[PATH_MAX];
    char DestinationPath[PATH_MAX];

    if (BlockedAccessPage(SourcePath, DestinationPath, &TempUser, CDirectory) == 0) {
        //supprimer la page du répertoire source 
        printf("Copy fonctionnal\n");
    } else {
        printf("Error Copy\n");
    }

    return 0;
}
