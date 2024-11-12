#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <linux/limits.h>

#define SizeMaxPage 256

typedef struct {
    char UId[15];
    char UPass[15];
} User;

typedef struct {
    char password[11];  // 10 characters max
    char note[SizeMaxPage]; // max page size
} Page;

// Global function
void BoucleJournal(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, char *CDirectory);
void menu(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, char *CDirectory);

// User gestion part
void SignInUser(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete, FILE *file);
void LogInUser(FILE *file, char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete);
//invisible part
void CurrentDirectory(char *CDirector, FILE *file);
void CodeImplementation(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete, FILE *file);
void FolderCreation(char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, FILE *file);


// Page gestion part
void CreatePage(User *TempUser, Page **TempPage, int *NbPage,char *CDirectory);
void DeleteProcess(Page **TempPage, int *NbPage, User *TempUser, char *CDirectory); // là que page, rajouter user ? "si on vient de tel fonction"
void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, User *TempUser, char *CDirectory);
//invisible part
void SaveToFile(User *TempUser, int *NbPage, Page *TempPage, char *CDirectory); // là c'est regroupé, peut être séparé
void BlockedAccesPage(Page **TempPage, int *NbPage, int PageToDelete); // à modifier séparer ou regrouper ?
void ConsultPagepB(User *TempUser, int *NbPage, Page **TempPage, int PageToDelete,char *CDirectory);
void EnterPassword(Page *TempPage, int page_index, int *NbPage, User *TempUser, char *CDirectory);
void WriteInPage(Page *TempPage, int page_index);

// Decoration functions
void WelcomeJournal();
void PersonalAccess();
void ByeJournal();

void WelcomeJournal(){
    printf("***********************\n***********_***********\n*********_____*********\n*******_________*******\n*****_____________*****\n***_____Welcome_____***\n***_______to________***\n***_______the_______***\n***_____journal_____***\n*****_____:D______*****\n*******_________*******\n*********_____*********\n**********___**********\n***********_***********\n***********************\n");
    printf("\n");
}

void PersonalAccess(){
    printf("***********************\n***********_***********\n*********_____*********\n*******_________*******\n*****_____________*****\n***_____Welcome_____***\n***_______to________***\n***______your_______***\n***____personal_____***\n****_____space_____****\n*****_____^^_____******\n********_______********\n**********___**********\n***********_***********\n***********************\n");
    printf("\n");
}

void ByeJournal() {
    printf("***********************\n***********_***********\n*********_____*********\n*******_________*******\n*****_____________*****\n***_______See_______***\n***_______you_______***\n***______later______***\n***_________________***\n*****_____Bye______****\n******____<3_____******\n*********_____*********\n**********___**********\n***********_***********\n***********************\n");
    exit(0);
}

void CurrentDirectory(char *CDirectory, FILE *file){
    if (getcwd(CDirectory, PATH_MAX) != NULL) {
       //printf("Current directory: %s\n", CDirectory);
   } else {
       perror("Problem of autorization, please fix it in settings");
   }
    if (file == NULL) {
        printf("Failed to create password repository.\n");
    } else {
        fclose(file); 
    }
}

void CodeImplementation(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete, FILE *file){
    printf("Do you already use compile this programm ? (yes/no): ");
    char answer[4];
    scanf("%3s", answer);

    if (strcmp(answer, "no") == 0) {
        FILE *file = fopen("password.txt", "a"); // create if doesn't exist
        printf("Go to create an account!\n");
        SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete, file);
    } else {
        FILE *file = fopen("password.txt", "r"); // on pourrait mettre "a" par précaution mais ça ecraserait le précédent, s'il a déjà des données..
        if (file != NULL) {
            LogInUser(file, CDirectory, TempUser, TempPage, NbPage, PageToDelete);
            fclose(file);
        } else {
            printf("Account file not found. Please create a new account.\n");
            SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete, file);
        }
    }
}

void LogInUser(FILE *file, char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete) {
    char answer[4];
    char VTempUserUId[15], VTempUserUPass[15];
    char PathUser[80];
    printf("Enter your user ID: ");
    scanf("%14s", VTempUserUId); 

    snprintf(PathUser, PATH_MAX, "%s/%s",CDirectory, VTempUserUId);
    
    if (access(PathUser, F_OK) != 0) { 
        printf("This ID doesn't exist.\nDo you want to create an account? (yes/no): ");
        scanf("%3s", answer); 

        if (strcmp(answer, "yes") == 0) {
            printf("Creating account...\n");
            SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete, file);
        } else {
            ByeJournal();
        }
    } else {
        printf("Enter the password: ");
        scanf("%14s", VTempUserUPass); 

        if (fscanf(file, "%14s %14s", TempUser->UId, TempUser->UPass) != EOF) {
            int TooLate = 0;

            while (strcmp(VTempUserUPass, TempUser->UPass) != 0 && TooLate < 3) {
                TooLate++;
                printf("Wrong password. You only have %d attempt(s) left.\n", 3 - TooLate);

                if (TooLate < 3) {
                    printf("Enter the password again: ");
                    scanf("%14s", VTempUserUPass);
                } else {
                    printf("Access blocked. Contact support.\n");
                    return; 
                }
            }

            if (strcmp(VTempUserUId, TempUser->UId) == 0 && strcmp(VTempUserUPass, TempUser->UPass) == 0) {
                PersonalAccess();
                menu(TempUser, TempPage, NbPage, PageToDelete, CDirectory); 
            }
        }
    }
}

void SignInUser(User *TempUser, char *CDirectory, Page **TempPage, int *NbPage, int PageToDelete, FILE *file){
    char UPass2[15];

    PersonalAccess();
    printf("Choose a user id: ");
    scanf("%14s", TempUser->UId);

    printf("Enter a password: ");
    scanf("%14s", TempUser->UPass);

    printf("Confirm the password: ");
    scanf("%14s", UPass2);
    printf("\n");

    if (strcmp(TempUser->UPass, UPass2) == 0) {
        FolderCreation(CDirectory, TempUser, TempPage, NbPage, PageToDelete, file);
    } else {
        printf("Passwords don't match.\nPlease try again.\n");
        SignInUser(TempUser, CDirectory, TempPage, NbPage, PageToDelete, file);
    }
}

#ifdef _WIN32
    #include <direct.h>
    #define CREATE_DIR(path) _mkdir(path)
#else
    #include <sys/types.h>
    #define CREATE_DIR(path) mkdir(path, 0777)
#endif

void FolderCreation(char *CDirectory, User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, FILE *file ) {
    char TempUserFoldPath[PATH_MAX];
    snprintf(TempUserFoldPath, PATH_MAX, "%s/%s", CDirectory, TempUser->UId);

    if (CREATE_DIR(TempUserFoldPath) == 0) {
        //printf("User folder successfully created: %s\n", TempUserFoldPath);

        char PassFilePath[PATH_MAX];
        snprintf(PassFilePath, PATH_MAX, "%s/password.txt", CDirectory);

        FILE *Afile = fopen(PassFilePath, "a");
        if (Afile == NULL) {
            perror("Error opening password.txt");
            return;
        }

        fprintf(Afile, "%s %s\n", TempUser->UId, TempUser->UPass);
        fclose(Afile);
        printf("User account successfully created.\n");

        menu(TempUser, TempPage, NbPage, PageToDelete, CDirectory);
    } else {
        perror("Error creating account");
        LogInUser(file, CDirectory, TempUser, TempPage, NbPage, PageToDelete);
    }
}

void CreatePage(User *TempUser, Page **TempPage, int *NbPage, char *CDirectory) {
    *TempPage = realloc(*TempPage, (*NbPage + 1) * sizeof(Page));
    if (*TempPage == NULL) {
        printf("Error allocating memory.\n");
        exit(1);
    }

    printf("Enter the password (max 10 characters) : ");
    scanf("%10s", (*TempPage)[*NbPage].password);

    printf("Write your page (max 256 characters):\n ");
    getchar();
    fgets((*TempPage)[*NbPage].note, SizeMaxPage, stdin);

    (*NbPage)++;
    SaveToFile(TempUser, NbPage, *TempPage, CDirectory);
}

void SaveToFile(User *TempUser, int *NbPage, Page *TempPage, char *CDirectory) {
    char userFolderPath[PATH_MAX];
    snprintf(userFolderPath, PATH_MAX, "%s/%s", CDirectory, TempUser->UId);

    if (chdir(userFolderPath) != 0) {
        printf("Error changing to user directory.\n");
        return;
    }

    for (int i = 0; i < *NbPage; i++) {
        char PageFilePath[PATH_MAX];
        snprintf(PageFilePath, PATH_MAX, "%s/%s/Page%d.txt", CDirectory, TempUser->UId, i + 1);

        FILE *Wfile = fopen(PageFilePath, "w");
        if (Wfile == NULL) {
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

void DeleteProcess(Page **TempPage, int *NbPage, User *TempUser, char *CDirectory) {
    int PageToDelete;

    printf("Which page do you want to delete? (number): ");
    scanf("%d", &PageToDelete);

    if (PageToDelete > *NbPage || PageToDelete <= 0) {
        printf("This page doesn't exist.\n");
        return;
    }
    PageToDelete--;

    ConsultPagepB(TempUser, NbPage, TempPage, PageToDelete, CDirectory);
}

void ConsultPagepB(User *TempUser, int *NbPage, Page **TempPage, int PageToDelete,char *CDirectory) {
    char answer[4];

    if (strcmp((*TempPage)[PageToDelete].password, "MdpAdminOnly2104") == 0) {
        printf("You can't access this page, this page was blocked\n");
        printf("Returning to the menu.\n");
        menu(TempUser, TempPage, NbPage, PageToDelete, CDirectory);
        return;
    }
    EnterPassword(*TempPage, PageToDelete, NbPage, TempUser, CDirectory);

    printf("Are you sure you want to delete page number %d (yes/no): ", PageToDelete + 1);
    scanf("%3s", answer);
    if (strcmp(answer, "yes") == 0) {
        for (int i = PageToDelete; i < *NbPage - 1; i++) {
            (*TempPage)[i] = (*TempPage)[i + 1];
        }
        *TempPage = realloc(*TempPage, (*NbPage - 1) * sizeof(Page));
        if (*TempPage == NULL && *NbPage > 1) {
            printf("Error allocating memory.\n");
            exit(1);
        }
        (*NbPage)--;
        printf("Page deleted successfully.\n");
    } else {
        printf("Do you prefer to change the note? (yes/no): ");
        scanf("%3s", answer);
        if (strcmp(answer, "yes") == 0) {
            WriteInPage(*TempPage, PageToDelete);
        } else {
            printf("Right, returning to the menu.\n");
            menu(TempUser, TempPage, NbPage, PageToDelete, CDirectory);
        }
    }
}

void BlockedAccesPage(Page **TempPage, int *NbPage, int PageToDelete) {
    char CopyBlockedPass[11];
    char CopyBlockedNote[SizeMaxPage];

    strncpy(CopyBlockedPass, (*TempPage)[PageToDelete].password, sizeof((*TempPage)[PageToDelete].password));
    strncpy(CopyBlockedNote, (*TempPage)[PageToDelete].note, sizeof((*TempPage)[PageToDelete].note));

    strncpy((*TempPage)[PageToDelete].password, "MdpAdminOnly2104", sizeof((*TempPage)[PageToDelete].password));
    memset((*TempPage)[PageToDelete].note, 0, sizeof((*TempPage)[PageToDelete].note));

    printf("Blocked password was: %s\n", CopyBlockedPass);
}

void EnterPassword(Page *TempPage, int page_index, int *NbPage, User *TempUser, char *CDirectory) {
    char password[11];
    int TooLate = 0;
    
    printf("Enter the password for this page (you have 3 attempts):\n ");
    
    while (TooLate < 3) {
        printf("Attempt %d: ", TooLate + 1);
        scanf("%10s", password);  

        if (strcmp(TempPage[page_index].password, password) == 0) { 
            printf("*Authorized access.*\n");
            printf("Note: %s\n", TempPage[page_index].note);
            return;
        } else {
            TooLate++;
            if (TooLate < 3) {
                printf("Wrong password. You have %d attempt(s) left.\n", 3 - TooLate);  
            } else {
                printf("Access to this page is blocked.\n");
                BlockedAccesPage(&TempPage, NbPage, page_index); 
                printf("Returning to the menu.\n");
                menu(TempUser, &TempPage, NbPage, page_index, CDirectory); 
            }
        }
    }
}

void ConsultPage(Page **TempPage, int PageToDelete, int NbPage, User *TempUser, char *CDirectory) {
    int page_requested;

    printf("Which page do you want to consult? (number): ");
    scanf("%d", &page_requested);

    if (page_requested > NbPage || page_requested <= 0) {
        printf("This page doesn't exist.\n");
        return;
    }
    EnterPassword(*TempPage, page_requested - 1, &NbPage, TempUser, CDirectory);

    printf("Do you want to write in this page? (yes/no): ");
    char answer[4];
    scanf("%3s", answer);
    if (strcmp(answer, "yes") == 0) {
        WriteInPage(*TempPage, page_requested - 1);
    }
}

void WriteInPage(Page *TempPage, int page_index) {
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

void BoucleJournal(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, char *CDirectory){
    while (1) {
        menu(TempUser, TempPage, NbPage, PageToDelete, CDirectory);

        printf("Do you want to do something else? (yes/no) : ");
        char answer[4];
        scanf("%s", answer);
        printf("\n");
        if (strcmp(answer, "yes") == 0) {
            menu(TempUser, TempPage, NbPage, PageToDelete, CDirectory);
        } else {
            ByeJournal();
        }
    }
}

void menu(User *TempUser, Page **TempPage, int *NbPage, int PageToDelete, char *CDirectory) {
    int choice = 0;
        printf("\n1. Create Page\n2. Delete Page\n3. Consult Page\n4. Quit\n\nChoose: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                CreatePage(TempUser, TempPage, NbPage, CDirectory);
                break;
            case 2:
                DeleteProcess(TempPage, NbPage, TempUser, CDirectory);
                break;
            case 3:
                ConsultPage(TempPage, PageToDelete, *NbPage, TempUser, CDirectory);
                break;
            case 4:
                ByeJournal();
                break;
            default:
                printf("Invalid choice.\n");
    }
}

int main() {
    User TempUser;
    Page *TempPage = NULL;
    int NbPage = 0;
    int PageToDelete = 0;
    char CDirectory[PATH_MAX];
    FILE *file = fopen(CDirectory, "w"); 

    CurrentDirectory(CDirectory, file); 
    CodeImplementation(&TempUser, CDirectory, &TempPage, &NbPage, PageToDelete, file);

    WelcomeJournal();
    while(1){
        BoucleJournal(&TempUser, &TempPage, &NbPage, PageToDelete, CDirectory);
    }
    
    free(TempPage);
    return 0;
}
