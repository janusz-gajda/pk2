#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "src/log.c"
#include <ctype.h>

#define MAX_SIZE 512

#ifndef ERROR_FILE
#define ERROR_FILE "log.log"
#endif

// # - digit
// U - upper case
// l - lower case
// ... - and so on
// * - any symbol

//  https://github.com/rxi/log.c

int numbers(char *str)
{
    //Check if phone number (9) or NIP(10) or PESEL (11)
    char ch;
    int i = 0;
    int err = 0;
    int length = (int)strlen(str);
    log_info("Cheking, if %s has only numbers", str);
    while (i < length)
    {
        if (!(length == 9 || length == 10 || length == 11))
        {
            log_warn("%s has different length range <9,11>", str);
            return 0;
        }
        ch = str[i];
        if (!isdigit(ch))
        {
            err = 1;
            log_warn("%c isn't a digit", ch);
            break;
        }
        else
        {
            i++;
            log_debug("%c is a digit", ch);
        }
    }
    if (err == 0 && (i == 9 || i == 10 || 11))
    {
        if (i == 9)
        {
            log_info("%s is a phone number", str);
        }
        if (i == 10)
        {
            log_info("%s is a NIP number", str);
        }
        if (i == 11)
        {
            log_info("%s is a PESEL number", str);
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

int nip_patter(char *str)
{
    char ch;
    int i = 0;
    int err = 0;
    int length = (int)strlen(str);
    //Looking for ###-##-##-### pattern
    log_info("Cheking, if %s has ###-##-##-### (NIP) pattern", str);
    while (i < length)
    {
        if (length != 13)
        {
            err = 1;
            log_warn("%s has different length than 13");
            break;
        }
        ch = str[i];
        if (i < 3 && isdigit(ch))
        {
            i++;
            log_debug("%c is a digit", ch);
        }
        else if (i == 3 && ch == '-')
        {
            i++;
            log_debug("%c is dash", ch);
        }
        else if (i < 6 && isdigit(ch))
        {
            i++;
            log_debug("%c is a digit", ch);
        }
        else if (i == 6 && ch == '-')
        {
            i++;
            log_debug("%c is dash", ch);
        }
        else if (i < 9 && isdigit(ch))
        {
            i++;
            log_debug("%c is a digit", ch);
        }
        else if (i == 9 && ch == '-')
        {
            i++;
            log_debug("%c is dash", ch);
        }
        else if (i > 9 && isdigit(ch))
        {
            i++;
            log_debug("%c is a digit", ch);
        }
        else
        {
            err = 1;
            break;
        }
    }
    if (err == 0)
    {
        log_info("%s is a NIP number", str);
        return 1;
    }
    else
    {
        return 0;
    }
}

int phone_pattern(char *str)
{
    //Looking for ###-###-### or ### ### ### pattern
    char ch;
    int i = 0;
    int err = 0;
    int length = (int)strlen(str);
    log_info("Cheking, if %s has ###-###-### or ### ### ### (phone) pattern", str);
    while (i < length)
    {
        if (length != 11)
        {
            err = 1;
            log_warn("%s has different length than 11");
            break;
        }
        ch = str[i];
        if (i < 3 && isdigit(ch))
        {
            i++;
            log_debug("%c is a digit", ch);
        }
        else if (i == 3 && (ch == '-' || ch == ' '))
        {
            i++;
            log_debug("%c is dash/space", ch);
        }
        else if (i < 7 && isdigit(ch))
        {
            i++;
            log_debug("%c is a digit", ch);
        }
        else if (i == 7 && (ch == '-' || ch == ' '))
        {
            i++;
            log_debug("%c is dash/space", ch);
        }
        else if (i > 7 && isdigit(ch))
        {
            i++;
            log_debug("%c is a digit", ch);
        }
        else
        {
            log_warn("%c fell out of pattern", ch);
            err = 1;
            break;
        }
    }
    if (err == 0)
    {
        log_info("%s is a phone number", str);
        return 1;
    }
    else
    {
        return 0;
    }
}

int phone_pattern2(char *str)
{
    //Looking for ##-###-##-## or ## ### ## ## pattern
    char ch;
    int i = 0;
    int err = 0;
    int length = (int)strlen(str);
    log_info("Cheking, if %s has ##-###-##-## or ## ### ## ## (phone) pattern", str);
    while (i < length)
    {
        if (length != 12)
        {
            err = 1;
            log_warn("%s has different length than 12");
            break;
        }
        ch = str[i];
        if (i < 2 && isdigit(ch))
        {
            i++;
            log_debug("%c is a digit", ch);
        }
        else if (i == 2 && (ch == '-' || ch == ' '))
        {
            i++;
            log_debug("%c is dash/space", ch);
        }
        else if (i < 6 && isdigit(ch))
        {
            i++;
            log_debug("%c is a digit", ch);
        }
        else if (i == 6 && (ch == '-' || ch == ' '))
        {
            i++;
            log_debug("%c is dash/space", ch);
        }
        else if (i < 9 && isdigit(ch))
        {
            i++;
            log_debug("%c is a digit", ch);
        }
        else if (i == 9 && (ch == '-' || ch == ' '))
        {
            i++;
            log_debug("%c is dash/space", ch);
        }
        else if (i > 9 && isdigit(ch))
        {
            i++;
            log_debug("%c is a digit", ch);
        }
        else
        {
            log_warn("%c fell out of pattern", ch);
            err = 1;
            break;
        }
    }
    if (err == 0)
    {
        log_info("%s is a phone number", str);
        return 1;
    }
    else
    {
        return 0;
    }
}

int name_sirname_pattern(char *str)
{
    //Looking for Ulll... or Ulll...-Ulll... pattern
    char ch;
    int i = 0;
    int err = 0;
    int length = (int)strlen(str);
    log_info("Cheking, if %s has Ulll... or  Ulll...-Ulll... pattern", str);
    while (i < length)
    {
        ch = str[i];
        if (i == 0 && !isupper(ch))
        {
            err = 1;
            i++;
            log_debug("%c isn't upper case", ch);
            break;
        }
        else if (i == 0 && isupper(ch))
        {
            err = 0;
            i++;
            log_debug("%c is upper case", ch);
        }
        else if (err == 0 && islower(ch))
        {
            i++;
            log_debug("%c is lower case", ch);
        }
        else if (err == 0 && ch == '-')
        {
            err = 2;
            i++;
            log_debug("%c is dash", ch);
        }
        else if (err == 2 && isupper(ch))
        {
            err = 3;
            i++;
            log_debug("%c is upper case", ch);
        }
        else if (err == 3 && islower(ch))
        {
            i++;
            log_debug("%c is lower case", ch);
        }
        else
        {
            err = 1;
            log_warn("%c fell out of pattern", ch);
            break;
        }
    }
    if (err != 1)
    {
        log_info("%s is name/surname", str);
        return 1;
    }
    else
    {
        return 0;
    }
}

int name_and_surname_pattern(char *str)
{
    //Looking for Ulll... (Ulll... || Ulll...-Ulll...) pattern
    char ch;
    int i = 0;
    int err = 0;
    int length = (int)strlen(str);
    log_info("Cheking, if %s has Ulll... Ulll... or Ulll... Ulll...-Ulll... pattern", str);
    while (i < length)
    {
        ch = str[i];
        if (i == 0 && !isupper(ch))
        {
            err = 1;
            i++;
            log_debug("%c isn't upper case", ch);
            break;
        }
        else if (i == 0 && isupper(ch))
        {
            err = 0;
            i++;
            log_debug("%c is upper case", ch);
        }
        else if (err == 0 && islower(ch))
        {
            i++;
            log_debug("%c is lower case", ch);
        }
        else if (err == 0 && ch == ' ')
        {
            err = 2;
            i++;
            log_debug("%c is space", ch);
        }
        else if (err == 2 && !isupper(ch))
        {
            err = 1;
            i++;
            log_debug("%c isn't upper case", ch);
            break;
        }
        else if (err == 2 && isupper(ch))
        {
            err = 3;
            i++;
            log_debug("%c is upper case", ch);
        }
        else if (err == 3 && islower(ch))
        {
            i++;
            log_debug("%c is lower case", ch);
        }
        else if (err == 3 && ch == '-')
        {
            err = 4;
            i++;
            log_debug("%c is dash", ch);
        }
        else if (err == 4 && isupper(ch))
        {
            err = 5;
            i++;
            log_debug("%c is upper case", ch);
        }
        else if (err == 5 && islower(ch))
        {
            i++;
            log_debug("%c is lower case", ch);
        }
        else
        {
            err = 1;
            log_warn("%c fell out of pattern", ch);
            break;
        }
    }
    if (err != 1)
    {
        log_info("%s is name and sirname", str);
        return 1;
    }
    else
    {
        return 0;
    }
}
int mail_pattern(char *str)
{
    //Looking for a...@a...(.)a...) pattern
    char ch;
    int i = 0;
    int err = 0;
    int length = (int)strlen(str);
    log_info("Cheking, if %s has *@*.* pattern", str);
    int j = 0;
    if (length < 6)
    {
        log_warn("%s is too short");
    }
    while (i < length)
    {
        ch = str[i];
        if (i == 0 && ch == '@')
        {
            log_warn("%c came too soon", ch);
        }
        else if (err == 0 && i > 0 && ch == '@')
        {
            err = 2;
            i++;
            log_debug("Found @ at position %d", i);
        }
        else if (err == 0)
        {
            i++;
        }
        else if (err == 2 && j < 63 && (isdigit(ch) || islower(ch) || ch == '-' || ch == '.'))
        {
            i++;
            j++;
            log_debug("%c is valid domain character", ch);
        }
        else if (err == 2 && j > 63)
        {
            log_warn("Domain name is too long");
            err = 1;
            break;
        }
        else
        {
            err = 1;
            log_warn("%c fell out of pattern", ch);
            break;
        }
    }
    if (err == 2)
    {
        log_info("%s is mail address", str);
        return 1;
    }
    else
    {
        return 0;
    }
}

int address_pattern(char *str)
{
    //Holly shit, here we go again
    //Looking for (ul.) Ulll... (###,###l,###/###) ##-### Ulll...
    char ch;
    int i = 0;
    int err = 0;
    int length = (int)strlen(str);
    log_info("Cheking, if %s has (ul.) Ulll... (###,###l,###/###) ##-### Ulll... pattern", str);
    while (i < length)
    {
        ch = str[i];
        if (err == 0 && isupper(ch))
        {
            err = 2;
            i++;
        }
        else if (err == 0 && ch == 'u')
        {
            err = 20;
            i++;
        }
        else if (err == 20 && ch == 'l')
        {
            err = 21;
            i++;
        }
        else if (err == 21 && ch == '.')
        {
            err = 22;
            i++;
        }
        else if ((err == 22 || err == 21) && ch == ' ')
        {
            log_debug("Found ul. prefix");
            err = 0;
            i++;
        }
        else if (err == 2 && islower(ch))
        {
            i++;
        }
        else if (err == 2 && ch == ' ')
        {
            log_debug("Found street name");
            err = 3;
            i++;
        }
        else if (err == 3 && isdigit(ch))
        {
            err = 4;
            i++;
        }
        else if (err == 4 && isdigit(ch))
        {
            i++;
        }
        else if (err == 4 && islower(ch))
        {
            err = 5;
            i++;
        }
        else if ((err == 4 || err == 5) && ch == '/')
        {
            err = 6;
            i++;
        }
        else if (err == 6 && isdigit(ch))
        {
            err = 7;
            i++;
        }
        else if (err == 7 && isdigit(ch))
        {
            i++;
        }
        else if (err == 7 && islower(ch))
        {
            err = 8;
            i++;
        }
        else if ((err == 4 || err == 5 || err == 7 || err == 8) && ch == ' ')
        {
            log_debug("Found property number");
            i++;
            err = 9;
        }
        else if (err == 9 && isdigit(ch))
        {
            i++;
            err = 10;
        }
        else if (err == 10 && isdigit(ch))
        {
            i++;
            err = 11;
        }
        else if (err == 11 && ch == '-')
        {
            i++;
            err = 12;
        }
        else if (err == 12 && isdigit(ch))
        {
            i++;
            err = 13;
        }
        else if (err == 13 && isdigit(ch))
        {
            i++;
            err = 14;
        }
        else if (err == 14 && isdigit(ch))
        {
            i++;
            err = 15;
            log_debug("Found postal code");
        }
        else if (err == 15 && ch == ' ')
        {
            err = 19;
            i++;
        }
        else if (err == 19 && isupper(ch))
        {
            err = 16;
            i++;
        }
        else if (err == 16 && islower(ch))
        {
            i++;
        }
        else if (err == 16 && (ch == ' ' || ch == '-'))
        {
            err = 17;
            i++;
        }
        else if (err == 17 && isupper(ch))
        {
            err = 18;
            i++;
        }
        else if (err == 18 && islower(ch))
        {
            i++;
        }
        else
        {
            err = 1;
            log_warn("%c fell out of pattern", ch);
            break;
        }
    }
    if (err != 1)
    {
        log_debug("Found city");
        log_info("%s is an address", str);
        return 1;
    }
    else
    {
        return 0;
    }
}

int check_string(char *str)
{
    log_info("%s is now being checked", str);
    return numbers(str) || nip_patter(str) || phone_pattern(str) || phone_pattern2(str) || name_sirname_pattern(str) || name_and_surname_pattern(str) || address_pattern(str) || mail_pattern(str);
}

int main(int argc, char *argv[])
{
    freopen(ERROR_FILE, "w", stderr);
    if (argc < 3)
    {
        log_error("Didn't specify enought arguments!");
        printf("ERROR! Look into log file");
        return 0;
    }

    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w+");
    char temp[MAX_SIZE];
    log_info("# - digit");
    log_info("U - upper case");
    log_info("l - lower case");
    log_info("* - any symbol(s)");
    log_info("... - and so on");

    while (fgets(temp, MAX_SIZE, in))
    {
        strtok(temp, "\n");
        if (temp[0] == '\n')
            continue;
        if (!check_string(temp))
        {
            log_info("%s is safe to be passed", temp);
            fprintf(out, "%s\n", temp);
        }
    };

    return 0;
}
