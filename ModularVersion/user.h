#ifndef USER_H
#define USER_H

typedef struct {
    char UIdentifiant[20];
    char UPassword[20];
} User;

#include "page.h"

// User gestion part
void DataUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage,char *filename, FILE *file, int PageToDelete);
void LogInUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage, char *filename, FILE *file, int PageToDelete);
void CreateAnUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage,char *filename, FILE *file, int PageToDelete);

void FolderCreation(char *FolderName, User *TempUser, Page **TempPage, int *NbPage,char *filename, FILE *file, int PageToDelete);
void FolderExist(char *FolderName, User *TempUser, Page **TempPage,int *NbPage, char *filename, FILE *file, int PageToDelete);
void SaveToFile(char *filename, FILE *file, int *NbPage, Page **TempPage,User *TempUser);

#endif
