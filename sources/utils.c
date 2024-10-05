#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../includes/colors.h"

void help_message() 
{
    // ASCII sanatını ve yardım mesajını yazdır
    printf("%s", CYAN);
    printf("  _____  __            .__                \n");
    printf("_/ ____\\/  |_   ______ |__| ____    ____  \n");
    printf("\\   __\\   __\\  \\____ \\|  |/    \\  / ___\\ \n");
    printf(" |  |   |  |    |  |_> >  |   |  \\/ /_/  >\n");
    printf(" |__|   |__|____|   __/|__|___|  /\\___  / \n");
    printf("          /_____/__|           \\//_____/  \n");
    printf("%s\n", RESET);

    printf("%sUsage:%s\n", GREEN, RESET);
    printf("  %sft_ping%s [options] <destination>\n\n", BLUE, RESET);

    printf("%sOptions:%s\n", GREEN, RESET);
    printf("  %s-v%s        Enable verbose output\n", YELLOW, RESET);
    printf("  %s-h%s        Show this help message\n", YELLOW, RESET);

    printf("\n%sExample:%s\n", GREEN, RESET);
    printf("  %sft_ping%s -v google.com\n", BLUE, RESET);
}