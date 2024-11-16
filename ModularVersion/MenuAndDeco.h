#ifndef MENUANDDECO_H
#define MENUANDDECO_H

#include "user.h" 

#include "page.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Global function
void menu(User *TempUser, Page **TempPage, int *NbPage, char *filename, FILE *file, int PageToDelete);
void BoucleJournal(User *TempUser,Page **TempPage, int *NbPage, char *filename, FILE *file, int PageToDelete);

// Decoration functions
void WelcomeJournal();
void PersonalAccess();
void ByeJournal();

 #endif