#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h> 

//funktion to delate duplicates in the array
void delete_duplicates(char *array) {
    for (int i = 0; array[i] != '\0'; i++) {
        for (int j = i + 1; array[j] != '\0'; j++) {
            if (array[i] == array[j]) {
                for (int k = j; array[k] != '\0'; k++) {
                    array[k] = array[k + 1];
                }
                j--;
            }
        }
    }
}

//funktion to sort the array in alphabetical order
void sort(char *array) {
    int i, j;
    char temp;
    for (i = 0; array[i] != '\0'; i++) {
        for (j = i + 1; array[j] != '\0'; j++) {
            // if the current element is greater than the next element, swap them
            if (array[i] > array[j]) {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}

int main(int argc, char *argv[]) {
// if user entered more than 1, exit
    if (argc!=1 && argc!=2) {
        printf("Write only one string!\n");
        return 1; 
    }

// 101 is the maximum length of the line with the '\0'
    char adresa[101];

 // arrays to store matching characters, strings and first letters
 // 127 is the number of ASCII characters
    char matching_chars[127]={0};
    char matching_strings[101]={0};
    char first_letters[127]={0};
    
// check if the line is no longer than 100 characters
// if the is only one argument, strote the first letters of the line
    if (argc == 1){
        int i = 0;
        while (fgets(adresa, sizeof(adresa), stdin) != NULL) { 
            if (strlen(adresa) > 100) {
                printf("The line is too long!\n");
                return 1;
            }
            // if the character is not in the array, add it
            if (i < 127 && strchr(first_letters, adresa[0]) == NULL) {
                first_letters[i] = toupper(adresa[0]);
                i++;
            }
        }
        delete_duplicates(first_letters);
        sort(first_letters);
        printf("Enable: %s", first_letters);
        printf("\n");
        return 0;
    }

// if there are two arguments, store the matching characters and strings
    int len = strlen(argv[1]);
    int match = 0;

    if (argc==2){
        int i=0;
        while (fgets(adresa, 101, stdin) != NULL) {
                if (strlen(adresa) > 100) {
                printf("The line is too long!\n");
                return 1;
            }
            // if argument is the prefix of the line
            if (strncasecmp(adresa, argv[1], len) == 0) { 
                match++; 
                if (match == 1) { 
                    for (int i = 0; adresa[i] != '\0'; i++) {
                        matching_chars[0] = toupper(adresa[len]);
                        matching_strings[i] = toupper(adresa[i]); 
                    }
                }
                else if (match > 1) {
                    if (i < 127 && strchr(matching_chars, adresa[len]) == NULL) {
                    matching_chars[match-1] = toupper(adresa[len]);
                    i++;
                    }
                }
            }
        }
    }
    
    if (match == 1) { 
        printf("Found: %s", matching_strings);
    }

    else if (match > 1) { 
        delete_duplicates(matching_chars);
        sort(matching_chars);
        printf("Enable: %s", matching_chars);
    }
    
    else { 
        printf("Not found"); 
    }

    printf("\n");
    return 0;
}
