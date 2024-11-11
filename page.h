#ifndef PAGE_H
#define PAGE_H

#define SizeMaxPage 256
#include <stdio.h>

typedef struct {
    // point de vie 3
    char password[11]; // 10 characters max
    char note[SizeMaxPage]; // max page size
} Page;

void CreatePage(User *TempUser, Page **TempPage, int *NbPage,char *filename, FILE *file);
void DeleteProcess(Page **TempPage, int *NbPage, User *TempUser, char *filename, FILE *file);
void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, char *filename, FILE *file, User *TempUser);

// Auxiliary functions for gestion of page 
void ConsultPagepB(User *TempUser, int *NbPage, Page **TempPage,char *filename, FILE *file, int PageToDelete);
void WriteInPage(Page *TempPage, int page_index);

void EnterPassword(Page *TempPage, int page_index, int *NbPage, char *filename, FILE *file, int PageToDelete, User *TempUser);
//void BlockedAccessPage(Page **TempPage, int PageToDelete); à corriger

void DateAndHour(FILE *file, int *NbPage);

#endif
