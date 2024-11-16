#include <time.h>
#include <stdio.h>

int main() {
    time_t timestamp = time(NULL);
    struct tm *heure_locale = localtime(&timestamp);

    printf("Date and time of the last modif : %02d/%02d/%04d %02dh%02d:%02d\n",
           heure_locale->tm_mday, heure_locale->tm_mon + 1, 1900 + heure_locale->tm_year,
           heure_locale->tm_hour, heure_locale->tm_min, heure_locale->tm_sec);

    return 0;
}