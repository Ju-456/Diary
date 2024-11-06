#ifndef USER_H
#define USER_H

#include "page.h"

typedef struct {
    char UIdentifiant[15];
    char UPassword[15];
} User;

// User gestion part
void DataUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage,char *filename, FILE *file, int PageToDelete);
void LogInUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage, char *filename, FILE *file, int PageToDelete);
void CreateAnUser(User *TempUser, char *FolderName, Page **TempPage,int *NbPage,char *filename, FILE *file, int PageToDelete);

void FolderCreation(char *FolderName, User *TempUser, Page **TempPage, int *NbPage,char *filename, FILE *file, int PageToDelete);
void FolderExist(char *FolderName, User *TempUser, Page **TempPage,int *NbPage, char *filename, FILE *file, int PageToDelete);
void SaveToFile(char *filename, FILE *file, int *NbPage, Page **TempPage,User *TempUser);

#endif
