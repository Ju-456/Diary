#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // Objet sys : (fichiers, répertoires, processus).
#include <sys/stat.h> // Fonctions : stat(), mkdir(), obt info file/folder + create repertory 
#include <unistd.h>  // manipulation files and folder
#include <dirent.h> // read and browse the folders


typedef struct {
    char UIdentifiant[50];
    char UPassword[50];
} User;

void IdentificationUser(User *user1);

void FolderCreation(char *FolderName, User *user1) {
    // Construction of Folder based on Uid and Upass
    snprintf(FolderName, 1024, "/home/ju456/%s_%s", user1->UIdentifiant, user1->UPassword);

    // Creation Folder
    #ifdef _WIN32
        if (_mkdir(FolderName) == 0) {
            printf("Folder créé avec succès : %s\n", FolderName);
        } else {
            perror("Erreur lors de la création du dossier");
        }
    #else
        if (mkdir(FolderName, 0777) == 0) {
            printf("Folder créé avec succès : %s\n", FolderName);
        } else {
            printf("You already have an account, Identifiez vous :");
            IdentificationUser(user1);
        }
    #endif
}

void FolderExist(char *FolderName, User *user1) {

    int FolderAcces = access(FolderName, F_OK); 
    if (FolderAcces == 0) {
        printf("***Welcome to your personal space***\n");
        // ConsultPage(); 
    } else {
        printf("You don't have an account");
        printf("Do you want to create an account? (yes/no) ");
        char reponse[4];
        scanf("%3s", reponse);
        if (strcmp(reponse, "yes") == 0) {
            printf("Go to create an account...\n");
            // Create account avec mdp verif
        } else {
            printf("Goodbye!\n");
            exit(0); 
        }
    }
}

void IdentificationUser(User *user1) {
    printf("User id: ");
    scanf("%49s", user1->UIdentifiant); 

    printf("Password: ");
    scanf("%49s", user1->UPassword); 

    char FolderName[1024]; 
    snprintf(FolderName, sizeof(FolderName), "/home/ju456/%s_%s", user1->UIdentifiant, user1->UPassword);

    FolderExist(FolderName, user1); 
}

int main() {
    User user1;
    char reponse1[4];

    printf("\nDo you have an account? (yes/no): ");
    scanf("%3s", reponse1);

    if (strcmp(reponse1, "yes") == 0) {
        IdentificationUser(&user1); 
    } else {
        printf("Creating a new account...\n");
        char FolderName[1024]; // Buffer for FolderName
        IdentificationUser(&user1); 
        FolderCreation(FolderName, &user1); 
    }

    return 0;
}
