#include <stdio.h>
#include <stdlib.h>

#include "user.h"
#include "page.h"
#include "MenuAndDeco.h"

int main() {
    User TempUser;
    Page *TempPage = NULL;
    int NbPage = 0;
    int PageToDelete = 0;
    char FolderName[1024];
    char filename[80];
    FILE *file = fopen(filename, "w");

    WelcomeJournal();
    while (1) {
        DataUser(&TempUser, FolderName, &TempPage, &NbPage, filename, file, PageToDelete);
    }

    free(TempPage);
    return 0;
}
