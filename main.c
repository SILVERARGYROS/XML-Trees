// Argyros Konstantinos 
// AM:2022202000014

// Kounadi Vasiliki
// AM:2022202000102

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 100             // set number of childs here
// #define ifInDebugMode 1 // debug mode activation
FILE* fp = NULL;

typedef struct node
{
    char* data;
    struct node *child[M]; 
} tree;

tree* parse(tree* root, char* tag);
void traverse(tree* root, int level);

int main (void)
{
    // system("tput clear");

    //getting input for file Name
    char* fileName = (char*) malloc (sizeof(char) * 1000); 
    printf("Please enter an XML file name: ");
    fgets(fileName, 1000, stdin);
    fileName = strtok(fileName, "\n");      //removes '\n' from fileName
    // printf("debug string == %s\n", fileName);

    //opening file
    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("ERROR OPENING FILE \"%s\"\n\n", fileName);
        free(fileName);
        return 0;
    }

    //creating root
    // printf("debug1\n");
    tree* root = (tree*) malloc(sizeof(tree));
    int i;
    for(i = 0; i < M; i++)
    {
        root -> child[i] = NULL;
        // printf("debug child[%d] == %s\n", i,tree -> child[i]);
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
                    // printf("debug i == %d\n", i);
                    tag[i] = c;
                    i++;
                }
            }while(c != '>');
            
            parse(root, tag);
        }
    }
    // printf("debug after while i == %d\n", i);

    //         tag = strtok(tag, " ");
    //         printf("debug tag == ");
    //         for(i = 0; i < 1000; i++)
    //         {
    //             // printf("debug 333\n"); 
    //                 // printf("debug i == %d\n", i);

    //             printf("%c", tag[i]);
    //         }
    //         printf("\n");
    // printf("debug2\n");


    traverse(root, 0);
    fclose(fp);
    return 0;
}

tree* parse(tree* root, char* tag)
{
    printf("debug tag11 == %s\n", tag);
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

        //start of tag
        // s=strchr(tag,'<'); //needs change here
        
        int closingTag;     //boolean
        if(c == '/')    //bypasses '/' 
        {
            closingTag = 1;
            i = 0;
        }
        else            //stores first character
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
            // c = fgetc(fp); //bypasses '/' 
            printf("debug tag1 == %s\n", tag);
            printf("debug tag2 == %s\n", tag2);

            if(strcmp(root->data,tag2) == 0)
            {
                return root;
            }
            else
            {
                printf("Tags in files are not properly nested.\n");
                printf("Terminating Programm...\n");
                exit(1);// check later
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
            printf("debug before if k  == %d\n", k);
            if(k==M) //above didnt work didnt find tag
            {
                for(i = 0; i < M; i++)
                {
                    if(root -> child[i] == NULL)
                    {
                        k=i;
                        printf("debug before malloc k  == %d\n", k);
                        root->child[i]= (tree*) malloc(sizeof(tree)); //so that its not NULL anymore

                        break;
                    }
                }
            }

            printf("debug tag22 == %s\n", tag2);
            printf("debug after k  == %d\n", k);
            if(single)
            {
                printf("debug before segm\n");
                root->child[k]-> data =(char*) malloc(sizeof(char) * strlen(tag2));
                strcpy(root->child[k]->data,tag2);
                printf("debug after segm\n");
            }
            else
            {
                root->child[k]= parse(root->child[k],tag2);
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