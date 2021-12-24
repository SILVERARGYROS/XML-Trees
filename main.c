// Argyros Konstantinos 
// AM:2022202000014

// Kounadi Vasiliki
// AM:2022202000102

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 4             // set number of childs here
// #define ifInDebugMode 1 // debug mode activation

typedef struct node
{
    char* data;
    struct node *child[M]; 
} tag;

int main (void)
{
    system("tput clear");

    char* fileName = (char*) malloc (sizeof(char) * 1000); 
    printf("Please enter an XML file name: ");
    fgets(fileName, 1000, stdin);
    fileName = strtok(fileName, "\n");
    printf("debug string  == %s\n", fileName);

    FILE* fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("ERROR OPENING FILE \"%s\"\n\n", fileName);
        return 0;
    }
    fclose(fp);

    printf("debug1\n");
    tag* tree = (tag*) malloc(sizeof(tag)*1);
    int i;
    for(i = 0; i < M; i++)
    {
        tree -> child[i] = NULL;
        // printf("debug child[%d] == %s\n", i,tree -> child[i]);
    }

    

    // parse(tree, book);


    printf("debug2\n");

    return 0;
}