#include "Diary.h"
#include "Folder.c"
#include "DecorsMenu.c"
#include "MenuOption.c"

int main()
{
    User *TempUser = malloc(sizeof(User));
    Page *TempPage = NULL;
    int NbPage = 0;
    int PageToDelete = 0;
    char CDirectory[PATH_MAX];
    FILE *file = fopen(CDirectory, "w");
    char SourcePath[PATH_MAX];
    char DestinationPath[PATH_MAX];

    CurrentDirectory(CDirectory, file);
    CodeImplementation(TempUser, CDirectory, &TempPage, &NbPage, PageToDelete, file, SourcePath, DestinationPath);
    while (1)
    {
        BoucleJournal(TempUser, &TempPage, &NbPage, PageToDelete, CDirectory, SourcePath, DestinationPath);
    }

    free(TempPage);
    return 0;
}