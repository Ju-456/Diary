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
    char CDirectory[PATH_MAX];
    char SourcePath[PATH_MAX];
    char DestinationPath[PATH_MAX];
} Path;

typedef struct{
    char password[11];      // 10 characters max
    char note[SizeMaxPage]; // max page size
} Page;

// Global function
void BoucleJournal(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, Path *AllPath);
void menu(User *TempUser, Page **TempPage, int *NbPage, Path *AllPath, int PageToDelete);

// User gestion part
void SignInUser(User *TempUser, Path *AllPath, Page **TempPage, int *NbPage, int PageToDelete, FILE *file);
void LogInUser(FILE *file, Path *AllPath, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete);

// invisible part
void CurrentDirectory(Path *AllPath, FILE *file);
void CodeImplementation(User *TempUser, Path *AllPath, Page **TempPage, int *NbPage, int PageToDelete, FILE *file);
void VerificationCreationPasswordtxt(FILE *file, Path *AllPath);
void FolderCreation(Path *AllPath, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, FILE *file);

// Page gestion part
void CreatePage(User *TempUser, Page **TempPage, int *NbPage, Path *AllPath);
void DeletePageProcess(Page **TempPage, int *NbPage, User *TempUser, Path *AllPath);
void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, User *TempUser, Path *AllPath);

// invisible part
void SaveToFile(User *TempUser, int *NbPage, Page *TempPage, Path *AllPath); // là c'est regroupé, peut être séparé
int BlockedAccessPage(Path *AllPath, User *TempUser, int PageToDelete, int *NbPage, Page **TempPage);
void EnterPasswordPage(Page *TempPage, int page_index, int *NbPage, User *TempUser, Path *AllPath, int PageToDelete);
void WriteInPage(Page *TempPage, int page_index);

// Decoration functions
void WelcomeJournal();
void PersonalAccess();
void ByeJournal(User *TempUser);


void WelcomeJournal(){
    printf(
        "***********************\n"
        "***********_***********\n"
        "*********_____*********\n"
        "*******_________*******\n"
        "*****_____________*****\n"
        "***_____Welcome_____***\n"
        "***_______to________***\n"
        "***_______the_______***\n"
        "***______Diary______***\n"
        "*****_____:D______*****\n"
        "*******_________*******\n"
        "*********_____*********\n"
        "**********___**********\n"
        "***********_***********\n"
        "***********************\n");
    printf("\n");
}

void PersonalAccess(){
    printf(
        "***********************\n"
        "***********_***********\n"
        "*********_____*********\n"
        "*******_________*******\n"
        "*****_____________*****\n"
        "***_____Welcome_____***\n"
        "***_______to________***\n"
        "***______your_______***\n"
        "***____personal_____***\n"
        "****_____space_____****\n"
        "*****_____^^______*****\n"
        "*******_________*******\n"
        "*********_____*********\n"
        "**********___**********\n"
        "***********_***********\n"
        "***********************\n");
    printf("\n");
}

void ByeJournal(User *TempUser){
    printf(
        "***********************\n"
        "***********_***********\n"
        "*********_____*********\n"
        "*******_________*******\n"
        "*****_____________*****\n"
        "***_______See_______***\n"
        "***_______you_______***\n"
        "***______later______***\n"
        "***___ '%s' ___***\n"
        "*****_____Bye______*****\n"
        "******____<3______******\n"
        "*******_________*******\n"
        "*********_____*********\n"
        "**********___**********\n"
        "***********_***********\n"
        "***********************\n",
        TempUser->UId);
    exit(0);
}

void CurrentDirectory(Path *AllPath, FILE *file){
    if (getcwd(AllPath->CDirectory, PATH_MAX) != NULL)
    {
        // printf("Current directory: %s\n", CDirectory);
    }
    else
    {
        perror("Problem of autorization, please fix it in settings");
    }
}

void CodeImplementation(User *TempUser, Path *AllPath, Page **TempPage, int *NbPage, int PageToDelete, FILE *file){
    printf("Do you already compile this programm on your PC ? (yes/no): ");
    char answer[4];
    scanf("%3s", answer);
    if (strcmp(answer, "no") == 0 || strcmp(answer, "NO") == 0)
    {
        FILE *file = fopen("password.txt", "a"); // create bc doesn't exist
        printf("Go to create an account!\n");
        SignInUser(TempUser, AllPath, TempPage, NbPage, PageToDelete, file);
    }
    else
    {
        printf("So... we can start it !\n");     //
        FILE *file = fopen("password.txt", "r"); // on pourrait mettre "a" par précaution mais ça ecraserait le précédent, s'il a déjà des données..
        if (file != NULL)
        {
            LogInUser(file, AllPath, TempUser, TempPage, NbPage, PageToDelete);
            fclose(file);
        }
        else
        {
            printf("Apparently no ! Please create a new account to join us ;)\n");
            SignInUser(TempUser, AllPath, TempPage, NbPage, PageToDelete, file);
        }
    }
}

void VerificationCreationPasswordtxt(FILE *file, Path *AllPath){
    char *PathPassword[PATH_MAX];
    snprintf(*PathPassword, PATH_MAX, "%s/password.txt", AllPath->CDirectory);
    if (*PathPassword == NULL)
    {
        printf("Failed to create password repository.\n");
    }
    else
    {
        fclose(file);
    }
}

void LogInUser(FILE *file, Path *AllPath, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete){
    char answer[4];
    char VTempUserUId[15], VTempUserUPass[15];
    char PathUser[80];
    printf("Enter your user ID: ");
    scanf("%14s", VTempUserUId);
    snprintf(PathUser, PATH_MAX, "%s/%s", AllPath->CDirectory, VTempUserUId);

    if (access(PathUser, F_OK) != 0)
    {
        printf("This ID doesn't exist.\nDo you want to create an account? (yes/no): ");
        scanf("%3s", answer);
        if (strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0)
        {
            printf("Creating account...\n");
            SignInUser(TempUser, AllPath, TempPage, NbPage, PageToDelete, file);
        }
        else
        {
            ByeJournal(TempUser);
        }
    }
    else
    {
        printf("Enter the password: ");
        scanf("%14s", VTempUserUPass);
        if (fscanf(file, "%14s %14s", TempUser->UId, TempUser->UPass) != EOF)
        {
            int TooLate = 0;
            while (strcmp(VTempUserUPass, TempUser->UPass) != 0 && TooLate < 3)
            {
                TooLate++;
                printf("Wrong password. You only have %d attempt(s) left.\n", 3 - TooLate);
                if (TooLate < 3)
                {
                    printf("Enter the password again: ");
                    scanf("%14s", VTempUserUPass);
                }
                else
                {
                    printf("Access blocked. Contact support.\n");
                    return;
                }
            }
            if (strcmp(VTempUserUId, TempUser->UId) == 0 && strcmp(VTempUserUPass, TempUser->UPass) == 0)
            {
                PersonalAccess();
                menu(TempUser, TempPage, NbPage, AllPath, PageToDelete);
            }
        }
    }
}

void SignInUser(User *TempUser, Path *AllPath, Page **TempPage, int *NbPage, int PageToDelete, FILE *file){
    char UPass2[15];
    PersonalAccess();
    printf("Choose a user id: ");
    scanf("%14s", TempUser->UId);
    // Just in case
    if (TempUser == NULL)
    {
        printf("Error: TempUser is not allocated.\n");
        return;
    }
    printf("Enter a password: ");
    scanf("%14s", TempUser->UPass);
    printf("Confirm the password: ");
    scanf("%14s", UPass2);
    printf("\n");
    if (strcmp(TempUser->UPass, UPass2) == 0)
    {
        FolderCreation(AllPath, TempUser, TempPage, NbPage, PageToDelete, file);
    }
    else
    {
        printf("Passwords don't match.\nPlease try again.\n");
        SignInUser(TempUser, AllPath, TempPage, NbPage, PageToDelete, file);
    }
}

void FolderCreation(Path *AllPath, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, FILE *file){
    char TempUserFoldPath[PATH_MAX];
    char TempUserFoldPathbis[PATH_MAX];
    snprintf(TempUserFoldPath, PATH_MAX, "%s/%s", AllPath->CDirectory, TempUser->UId);
    if (CREATE_DIR(TempUserFoldPath) == 0)
    {
        // printf("User folder successfully created: %s\n", TempUserFoldPath);
        char PassFilePath[PATH_MAX];
        snprintf(PassFilePath, PATH_MAX, "%s/password.txt", AllPath->CDirectory);
        FILE *Afile = fopen(PassFilePath, "a");
        if (Afile == NULL)
        {
            perror("Error opening password.txt");
            return;
        }
        fprintf(Afile, "%s %s\n", TempUser->UId, TempUser->UPass);
        fclose(Afile);
        printf("User account successfully created.\n");
        // Creation répertoire des pages bloqués en même temps que creation du compte
        snprintf(TempUserFoldPathbis, PATH_MAX, "%s/%s/BlockedPages", AllPath->CDirectory, TempUser->UId);
        menu(TempUser, TempPage, NbPage, AllPath, PageToDelete);
    }
    else
    {
        perror("Error creating account");
        LogInUser(file, AllPath, TempUser, TempPage, NbPage, PageToDelete);
    }
}

void CreatePage(User *TempUser, Page **TempPage, int *NbPage, Path *AllPath){
    *TempPage = realloc(*TempPage, (*NbPage + 1) * sizeof(Page));
    if (*TempPage == NULL)
    {
        printf("Error allocating memory.\n");
        exit(1);
    }
    printf("Enter the password (max 10 characters) : ");
    scanf("%10s", (*TempPage)[*NbPage].password);
    printf("Write your page (max 1024 characters):\n ");
    getchar();
    fgets((*TempPage)[*NbPage].note, SizeMaxPage, stdin);
    (*NbPage)++;
    SaveToFile(TempUser, NbPage, *TempPage, AllPath);
}

void SaveToFile(User *TempUser, int *NbPage, Page *TempPage, Path *AllPath){
    char userFolderPath[PATH_MAX];
    snprintf(userFolderPath, PATH_MAX, "%s/%s", AllPath->CDirectory, TempUser->UId);
    if (chdir(userFolderPath) != 0)
    {
        printf("Error changing to user directory.\n");
        return;
    }
    for (int i = 0; i < *NbPage; i++)
    {
        char PageFilePath[PATH_MAX];
        snprintf(PageFilePath, PATH_MAX, "%s/%s/Page%d.txt", AllPath->CDirectory, TempUser->UId, i + 1);
        FILE *Wfile = fopen(PageFilePath, "w");
        if (Wfile == NULL)
        {
            printf("Error opening file for writing page %d.\n", i + 1);
            continue;
        }
        time_t timestamp = time(NULL);
        struct tm *FrenchHour = localtime(&timestamp);
        fprintf(Wfile, "Page%d\nDate: %02d/%02d/%04d\nAt: %02dh%02d:%02d\n\n",
                i + 1, FrenchHour->tm_mday, FrenchHour->tm_mon + 1,
                1900 + FrenchHour->tm_year, FrenchHour->tm_hour, FrenchHour->tm_min, FrenchHour->tm_sec);
        fprintf(Wfile, "Content:\n%s\n\n", TempPage[i].note);
        fclose(Wfile);
    }
    printf("All pages saved successfully in individual files.\n");
}

void DeletePageProcess(Page **TempPage, int *NbPage, User *TempUser, Path *AllPath){
    int PageToDelete;
    char answer[4];
    char TempUserFoldPath[PATH_MAX];

    printf("Which page do you want to delete? (number): ");
    scanf("%d", &PageToDelete);
    if (PageToDelete > *NbPage || PageToDelete <= 0)
    {
        printf("This page doesn't exist.\n");
        return;
    }
    PageToDelete--;
    EnterPasswordPage(*TempPage, PageToDelete, NbPage, TempUser, AllPath, PageToDelete); // si le mdp est bon, on fait la suite ?
    printf("Are you sure you want to delete page number %d (yes/no): ", PageToDelete + 1);
    scanf("%3s", answer);
    if (strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0)
    {
        for (int i = PageToDelete; i < *NbPage - 1; i++)
        {
            (*TempPage)[i] = (*TempPage)[i + 1];
        }
        *TempPage = realloc(*TempPage, (*NbPage - 1) * sizeof(Page));
        if (*TempPage == NULL && *NbPage > 1)
        {
            printf("Error allocating memory.\n");
            exit(1);
            snprintf(TempUserFoldPath, PATH_MAX, "%s/%s/Page%d.txt", AllPath->CDirectory, TempUser->UId, PageToDelete + 1);
            if (remove(TempUserFoldPath) == 0)
            {
                printf("Page%d deleted successfully.\n", PageToDelete + 1);
            }
            else
            {
                perror("Error during suppression of page");
            }
            (*NbPage)--;
            printf("Page deleted successfully.\n");
        }
    }
    else
    {
        printf("Do you prefer to change the note? (yes/no): ");
        scanf("%3s", answer);
        if (strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0)
        {
            WriteInPage(*TempPage, PageToDelete);
        }
        else
        {
            printf("Right, returning to the menu.\n");
            menu(TempUser, TempPage, NbPage, AllPath, PageToDelete);
        }
    }
}

int BlockedAccessPage(Path *AllPath, User *TempUser, int PageToDelete, int *NbPage, Page **TempPage){
    char TempUserFoldPath[PATH_MAX];
    snprintf(AllPath->SourcePath, PATH_MAX, "%s/%s/Page1.txt", AllPath->CDirectory, TempUser->UId);
    FILE *Source = fopen(AllPath->SourcePath, "rb");
    if (Source == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier source");
        return 1;
    }
    snprintf(AllPath->DestinationPath, PATH_MAX, "%s/%s/BlockedPages/Page1.txt", AllPath->CDirectory, TempUser->UId);
    FILE *destination = fopen(AllPath->DestinationPath, "wb");
    if (destination == NULL)
    {
        perror("Erreur lors de la création du fichier destination");
        fclose(Source);
        return 1;
    }
    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), Source)) > 0)
    {
        fwrite(buffer, 1, bytesRead, destination);
    }
    fclose(Source);
    fclose(destination);
    printf("Fichier copié avec succès de %s vers %s\n", AllPath->SourcePath, AllPath->DestinationPath);
    if (BlockedAccessPage(AllPath, TempUser, PageToDelete, NbPage, TempPage) == 0)
    {
        for (int i = PageToDelete; i < *NbPage - 1; i++)
        {
            (*TempPage)[i] = (*TempPage)[i + 1];
        }
        *TempPage = realloc(*TempPage, (*NbPage - 1) * sizeof(Page));
        if (*TempPage == NULL && *NbPage > 1)
        {
            printf("Error allocating memory.\n");
            exit(1);
        }
        snprintf(TempUserFoldPath, PATH_MAX, "%s/%s/Page%d.txt", AllPath->CDirectory, TempUser->UId, PageToDelete + 1);
        if (remove(TempUserFoldPath) == 0)
        {
            printf("Page%d deleted successfully from disk.\n", PageToDelete + 1);
        }
        else
        {
            perror("Error during file deletion");
        }
        (*NbPage)--;
        printf("Page deleted successfully.\n");
        printf("Copy fonctionnal\n");
    }
    else
    {
        printf("Error Copy\n");
    }
}

void EnterPasswordPage(Page *TempPage, int page_index, int *NbPage, User *TempUser, Path *AllPath, int PageToDelete){
    char password[11];
    int TooLate = 0;

    printf("Enter the password for this page (you have 3 attempts):\n ");

    while (TooLate < 3)
    {
        printf("Attempt %d: ", TooLate + 1);
        scanf("%10s", password);
        if (strcmp(TempPage[page_index].password, password) == 0)
        {
            printf("*Authorized access.*\n");
            printf("Note: %s\n", TempPage[page_index].note);
            return;
        }
        else
        {
            TooLate++;
            if (TooLate < 3)
            {
                printf("Wrong password. You have %d attempt(s) left.\n", 3 - TooLate);
            }
            else
            {
                printf("Access to this page is blocked.\n");
                BlockedAccessPage(AllPath, TempUser, PageToDelete, NbPage, &TempPage);
                printf("Returning to the menu.\n");
                menu(TempUser, &TempPage, NbPage, AllPath, page_index);
            }
        }
    }
}

void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, User *TempUser, Path *AllPath){
    int page_requested;
    printf("Which page do you want to consult? (number): ");
    scanf("%d", &page_requested);
    if (page_requested > NbPage || page_requested <= 0)
    {
        printf("This page doesn't exist.\n"); // ou peut être bloqué
        return;
    }
    EnterPasswordPage(*TempPage, page_requested - 1, &NbPage, TempUser, AllPath, PageToDelete);
    printf("Do you want to write in this page? (yes/no): ");
    char answer[4];
    scanf("%3s", answer);
    if (strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0)
    {
        WriteInPage(*TempPage, page_requested - 1);
    }
    else
    {
        BoucleJournal(TempUser, TempPage, &NbPage, PageToDelete, AllPath);
    }
}

void WriteInPage(Page *TempPage, int page_index){
    time_t timestamp = time(NULL);
    struct tm *FrenchHour = localtime(&timestamp);

    printf("Enter a new note for this page: ");
    getchar();
    fgets(TempPage[page_index].note, SizeMaxPage, stdin);
    char date_time[64];
    snprintf(date_time, sizeof(date_time), "Page%d\nDate: %02d/%02d/%04d\nAt: %02dh%02d:%02d\n\n",
             page_index + 1, FrenchHour->tm_mday, FrenchHour->tm_mon + 1,
             1900 + FrenchHour->tm_year, FrenchHour->tm_hour, FrenchHour->tm_min, FrenchHour->tm_sec);

    char UpdatedNote[SizeMaxPage + 64];
    snprintf(UpdatedNote, sizeof(UpdatedNote), "%s%s", date_time, TempPage[page_index].note);
    strncpy(TempPage[page_index].note, UpdatedNote, SizeMaxPage);

    printf("The note was changed successfully.\n");
}

void BoucleJournal(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, Path *AllPath){
    while (1)
    {
        menu(TempUser, TempPage, NbPage, AllPath, PageToDelete);
        printf("Do you want to do something else? (yes/no) : ");
        char answer[4];
        scanf("%s", answer);
        printf("\n");
        if (strcmp(answer, "yes") == 0 || strcmp(answer, "YES") == 0)
        {
            menu(TempUser, TempPage, NbPage, AllPath, PageToDelete);
        }
        else
        {
            ByeJournal(TempUser);
        }
    }
}

void menu(User *TempUser, Page **TempPage, int *NbPage, Path *AllPath, int PageToDelete){
    int choice = 0;
    printf(
        "\n1. Create Page\n"
        "2. Delete Page\n"
        "3. Consult Page\n"
        "4. Quit\n"
        "\nChoose: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        CreatePage(TempUser, TempPage, NbPage, AllPath);
        break;
    case 2:
        DeletePageProcess(TempPage, NbPage, TempUser, AllPath);
        break;
    case 3:
        ConsultPage(TempPage, PageToDelete, *NbPage, TempUser, AllPath);
        break;
    case 4:
        ByeJournal(TempUser);
        break;
    default:
        printf("Invalid choice.\n");
    }
}

int main(){
    User *TempUser = malloc(sizeof(User));
    Path *AllPath = malloc(sizeof(PATH_MAX));
    Page *TempPage = NULL;
    int NbPage = 0;
    int PageToDelete = 0;
    FILE *file = fopen(AllPath->CDirectory, "w");
    CurrentDirectory(AllPath, file);
    CodeImplementation(TempUser, AllPath, &TempPage, &NbPage, PageToDelete, file);
    WelcomeJournal();
    while (1)
    {
        BoucleJournal(TempUser, &TempPage, &NbPage, PageToDelete, AllPath);
    }

    free(TempPage);
    return 0;
}