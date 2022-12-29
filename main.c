// Argyros Konstantinos 
// AM:2022202000014

// Kounadi Vasiliki
// AM:2022202000102

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 20               // set number of childs nodes here 
//bigger the number of child nodes, the safer the program is but also the more memory it requires

FILE* fp = NULL;

typedef struct node
{
    char* data;
    struct node *child[M]; 
} tree;

tree* emergencyRoot;        //for emergency program termination for freeing all the tree

tree* parse(tree* root, char* tag);
void traverse(tree* root, int level);
void freeTree(tree* root);

int main (void)
{
    system("tput clear");

    //getting input for file Name
    char* fileName = (char*) malloc (sizeof(char) * 1000); 
    printf("Please enter an XML file name: ");
    fgets(fileName, 1000, stdin);
    fileName = strtok(fileName, "\n");      //removes '\n' from fileName

    //opening file
    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("\nERROR OPENING FILE \"%s\"\nNo such file exists...\n", fileName);
        free(fileName);
        return 0;
    }
    free(fileName);

    //creating root
    tree* root = (tree*) malloc(sizeof(tree));
    emergencyRoot = root;   
    int i;
    for(i = 0; i < M; i++)
    {
        root -> child[i] = NULL;
    }

    // i = 0;
    char c, *tag;
    tag = (char*) malloc(sizeof(char) * 1000);
    while(!feof(fp))
    {
        if(fgetc(fp) == '<')
        {
            // resetting tag
            for(i = 0; i< 1000; i++)
            {
                tag[i] = 0;
            } 

            i = 0;
            do
            {
                c = fgetc(fp);
                if(c != '>')
                {
                    tag[i] = c;
                    i++;
                }
            }while(c != '>');
            
            parse(root, tag);
        }
    }
    printf("\n");
    traverse(root, 0); //prints tag tree

    //safely closes file and frees allocated memory
    fclose(fp);
    freeTree(root);
    free(tag);
    return 0;
}

tree* parse(tree* root, char* tag)
{
    // printf("debug tag11 == %s\n", tag);
    int i, empty=1;
    for(i = 0; i < M; i++)
    {
        if(root -> child[i] != NULL)
        {
            empty=0;
        }
    }
    if(empty)
    {
        root->data = (char*)malloc(sizeof(char)*strlen(tag));   //allocating just enough memory to fit in the tag
        strcpy(root->data, tag);
        // printf("debug root data == %s\n", root->data);
    }

    char c,*s,*f,*tag2;
    tag2 = (char*) malloc(sizeof(char) * 1000);

    while(!feof(fp))
    {
        // resetting tag
        for(i = 0; i< 1000; i++)
        {
            tag2[i] = 0;
        }

        do
        {
            c = fgetc(fp);
        }while(!feof(fp) && c != '<');
        c = fgetc(fp);

        int closingTag;     //boolean
        if(c == '/')        //bypasses first character (which is '/')
        {
            closingTag = 1;
            i = 0;
        }
        else                //stores first character
        {
            closingTag = 0;
            tag2[0] = c;
            i = 1;
        }

        int complex = 0;
        int single = 0;
        //stores tag
        while((c = fgetc(fp)) != '>') 
        {   
            tag2[i] = c;
            i++;
            if(c == ' ')
            {
                complex = 1;
            }
            if(c == '/')
            {
                single = 1;
            }
        }

        if(complex || single) //everytime its single its also complex
        {
            tag2 = strtok(tag2, " ");
        }
        
        if(closingTag) //tag closes </>
        {
            // printf("debug tag1 == %s\n", tag);
            // printf("debug tag2 == %s\n", tag2);
            if(strcmp(root->data,tag2) == 0)
            {
                free(tag2);
                return root;
            }
            else
            {
                printf("ERROR: Tags in file are not properly nested.\n");
                printf("Terminating Programm...\n");
                fclose(fp);
                freeTree(emergencyRoot);
                free(tag);
                free(tag2);
                exit(1);
            }
        }
        else
        {
            int k=M;
            for(i=0; i<M; i++)
            {
                if(root->child[i] == NULL)
                {
                    break;
                }
                else if(strcmp(root->child[i]->data,tag2)==0)
                {
                    k=i;
                    break;
                }
            }
            
            if(k==M) //above didnt work didnt find tag
            {
                for(i = 0; i < M; i++)
                {
                    if(root -> child[i] == NULL)
                    {
                        k=i;
                        // printf("debug before malloc k  == %d\n", k);
                        root->child[i]= (tree*) malloc(sizeof(tree)); //so that its not NULL anymore

                        break;
                    }
                }
            }

            // printf("debug tag22 == %s\n", tag2);
            if(single)
            {
                root->child[k]-> data = (char*) malloc(sizeof(char) * strlen(tag2));
                strcpy(root->child[k]->data,tag2);
            }
            else
            {
                root->child[k] = parse(root->child[k],tag2);
            }
        }
    }  
}

void traverse(tree* root, int level)
{
    int i;

    for(i = 0; i < level; i++)
    {
        printf("    ");         //prints tabs based on current level
    }
    printf("<%s>\n", root -> data);

    for(i = 0; i < M; i++)
    {
        if(root -> child[i] != NULL)
        {
            traverse(root -> child[i], level + 1);
        }
    }
}

void freeTree(tree* root)
{
    int i;
    for(i = 0; i < M; i++)
    {
        if(root->child[i] != NULL)
        {
            freeTree(root->child[i]);
        }
    }
    free(root->data);
    free(root);
}