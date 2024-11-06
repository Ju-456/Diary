#ifndef MENUANDDECO_H
#define MENUANDDECO_H

#include <stdio.h>
#include <string.h>
#include "user.h"  
#include "page.h"

// Global function
void menu(User *TempUser, Page **TempPage, int *NbPage, char *filename, FILE *file, int PageToDelete);
void BoucleJournal(User *TempUser,Page **TempPage, int *NbPage, char *filename, FILE *file, int PageToDelete);

// Decoration functions
void WelcomeJournal();
void PersonalAcces();
void ByeJournal();

 #endif