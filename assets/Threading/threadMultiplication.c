/*
  * threadMultiplication.c -- A simple segfaulting program
  *
  * Compile with
  *    > gcc -g -Wall -Werror -D_POSIX_THREAD_SEMANTICS threadMultiplication.c -c -o threadMultiplication.o
  *    > gcc -g -Wall -Werror -D_POSIX_THREAD_SEMANTICS thread.c -c -o thread.o
  *    > gcc -pthread threadHello.o thread.o -o threadNultiplication
  * Run with
  *    > ./threadMultiplication "matrix1.txt" "matrix2.txt"
  */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "thread.h"
#define MAXSIZE 128
/*declaring public variables */
struct Matrix {
    int array[MAXSIZE][MAXSIZE];
    int columns;
    int rows;
};
struct Matrix result;
struct Matrix matrix1;
struct Matrix matrix2;
/*creating thread components */
static thread_t threads[MAXSIZE];
static void threadMultiply(int n);

/* itoa: was stolen from a website because my compiler did not appreciate me using the declaration without it's definition. It uses reverse, also defined from the website */
/* This is the K&R representation given on https://en.wikibooks.org/wiki/C_Programming/stdlib.h/itoa */
/* itoa:  convert n to characters in s */
/* reverse:  reverse string s in place */
void reverse(char s[]){
    int i, j;
    char c;
 
    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char s[]){
     int i, sign;
 
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }
 
struct Matrix getMatrices(FILE *file){
    //gives the matrix of multiplication
    struct Matrix matrix;
    matrix.rows = 0;
    char line[MAXSIZE*2]; // double the allowed size of a row of the matrix to account for whitespace
    while(fgets(line, MAXSIZE*2, file) != NULL) {
        matrix.rows++;
        matrix.columns = 0;
        char *token;
        token = strtok(line, " \r\n");
        while (token != NULL) {
            matrix.columns++;
            matrix.array[matrix.rows-1][matrix.columns-1] = atoi(token);
            token = strtok(NULL, " \r\n");
        }
    }
    printf("thanks for giving me this lovely array\n");
    for (int i=0; i < matrix.rows; i++) {
        for (int j=0; j < matrix.columns; j++)
            printf("%d ", matrix.array[i][j]);
        printf("\n");
    }
    return matrix;
}
int checkCompliance(struct Matrix matrix1, struct Matrix matrix2){
    //checks for columns of matrix1 and rows of matrix2 for compatable multiplication
    if(matrix1.columns == matrix2.rows){
        return 1;
    }
    else{
        return 0;
    }
}

void writeToResults(struct Matrix matrix){
    //writes the global result array to results.
    FILE *results = fopen("results.txt", "w");
    if(results == NULL){
        printf("problems opening results.txt\n");
    }
    else{
        for (int i=0; i < matrix.rows; i++) {
            for (int j=0; j < matrix.columns; j++)
                fprintf(results, "%d ", matrix.array[i][j]);
            fprintf(results, "\n");
        }
        printf("results.txt uploaded\n");
    }
}
void multiplyMatrices(){
    result.rows = matrix1.rows;
    result.columns = matrix2.columns;
    for(int i = 0; i < result.rows; i++){
        thread_create(&(threads[i]), &threadMultiply, i); 
    }
    for(int i = 0; i < result.rows; i++){
        thread_join(threads[i]); 
    }
    printf("this the final array:\n");
    for (int i=0; i < result.rows; i++) {
        for (int j=0; j < result.columns; j++)
            printf("%d ", result.array[i][j]);
        printf("\n");
    }

}
int main(int argc, char **argv){
    FILE *file1 = fopen(argv[1], "r");
    FILE *file2 = fopen(argv[2], "r");

    /* Create Function that reads the lines placing the lines in proper array locations*/
    matrix1 = getMatrices(file1);
    matrix2 = getMatrices(file2);
    /* A function that checks if the number of columns in array1 match the number of rows in array2. */
    int canMultiply = checkCompliance(matrix1, matrix2);
    if(canMultiply){
        /* thread multiplication */
        multiplyMatrices();
        /* Write our array to results.txt in the proper format and prints the final result to the terminal */
        writeToResults(result);
    }
    else{
        printf("these matrices don't work for multiplication please try again.\n");
    }

    printf("program complete\n");

}
void threadMultiply(int n){
    /* Does single row of multiplication for our result array */
    for(int j = 0; j < result.columns; j++){
            for(int k = 0; k < matrix1.columns; k++){
                result.array[n][j] = result.array[n][j] + matrix1.array[n][k] * matrix2.array[k][j];
            }
        }
    thread_exit(n);
}
