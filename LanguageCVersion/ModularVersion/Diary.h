#ifndef DIARY
#define DIARY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define CREATE_DIR(path) _mkdir(path)
#else

#include <sys/types.h>
#define CREATE_DIR(path) mkdir(path, 0777)
#endif

#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <linux/limits.h>

#define SizeMaxPage 1024

typedef struct{
    char UId[15];
    char UPass[15];
} User;

typedef struct{
    char password[11];      // 10 characters max
    char note[SizeMaxPage]; // max page size
} Page;

// Global function
void BoucleJournal(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, char *CDirectory, char *SourcePath, char *DestinationPath);
void menu(User *TempUser, Page **TempPage, int *NbPage, char *CDirectory, char *SourcePath, char *DestinationPath, int PageToDelete);

// Decoration functions
void WelcomeDiary();
void PersonalAccess();
void ByeJournal(User *TempUser);

// User gestion part
void SignInUser(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete, FILE *file, char *SourcePath, char *DestinationPath);
void LogInUser(FILE *file, char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, char *SourcePath, char *DestinationPath);

// Directory invisible part
void CurrentDirectory(char *CDirector, FILE *file);
void CodeImplementation(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete, FILE *file, char *SourcePath, char *DestinationPath);
void VerificationCreationPasswordtxt(FILE *file, char *CDirectory);
void FolderCreation(char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, FILE *file, char *SourcePath, char *DestinationPath);

// Page gestion part
void CreatePage(User *TempUser, Page **TempPage, int *NbPage, char *CDirectory);
void DeletePageProcess(Page **TempPage, int *NbPage, User *TempUser, char *CDirectory, char *SourcePath, char *DestinationPath);
void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, User *TempUser, char *CDirectory, char *SourcePath, char *DestinationPath);

// invisible part
void SaveToFile(User *TempUser, int *NbPage, Page *TempPage, char *CDirectory); // là c'est regroupé, peut être séparé
int BlockedAccessPage(char *SourcePath, char *DestinationPath, User *TempUser, char *CDirectory, int PageToDelete, int *NbPage, Page **TempPage);
void EnterPasswordPage(Page *TempPage, int page_index, int *NbPage, User *TempUser, char *CDirectory, char *SourcePath, char *DestinationPath, int PageToDelete);
void WriteInPage(Page *TempPage, int page_index);

#endif