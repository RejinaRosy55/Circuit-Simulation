#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000
#define MAX_LINE 5
#define MAX_INPUT 5

int main() {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    char *lines[100];  // Array to store lines (up to 100 lines)
    char linespace[100][2 * MAX_LINE_LENGTH];  // Array to store spaced-out lines
    int readvec[100][10];
    int line_count = 0;
    int i;

    // Open the file
    file = fopen("c17.vec", "r");
    if (file == NULL) {
        printf("Error opening file");
        return 1;
    }

    // Read each line from the file
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove the newline character if present
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        // Allocate memory to store the line in the vector
        lines[line_count] = malloc(strlen(line) + 1);  // +1 for null terminator
        if (lines[line_count] == NULL) {
            printf("Error allocating memory");
            return 1;
        }

        // Copy the line to the array
        strcpy(lines[line_count], line);
        line_count++;
    }

    // Close the file
    fclose(file);

    // // Add space after each character and store in linespace
    //  for (i = 0; i < line_count; i++) {
    //     int k = 0;
    //     for (int j = 0; j < strlen(lines[i]); j++) {
    //         linespace[i][k++] = lines[i][j];  // Copy character
    //         linespace[i][k++] = ' ';          // Add space
    //     }
    //     linespace[i][k - 1] = '\0';  // Replace last space with null terminator
    // }

    // // Print the stored lines with spaces
    // printf("\nSpaced-out lines:\n");
    // for (i = 0; i < line_count; i++) {
    //     printf("Line %d: %s\n", i + 1, linespace[i]);
    //     free(lines[i]);  // Free each allocated line
    // }

     for (i=0; i < line_count; i++) {
        for(int j=0; j < strlen(lines[i]); j++) {
            if (lines[i][j]=='0'){
                readvec[i][j]= 0;
            }
            else if(lines[i][j]=='1'){
                readvec[i][j] = 1;
            }
            else if(lines[i][j]=='x' || lines[i][]){
                readvec[i][j] = 2;
            }
            else {
                printf("Error:Invalid Character \n");
                exit(1);
            }
        }
    }
    for (i=0; i < line_count; i++) {
        for(int j=0; j<strlen(line[i]); j++){
            printf("%d",readvec[i][j]);
        }
        printf("\n");
    } 

    return 0;
}
