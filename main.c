//Saúl Enrique Labra Cruz A01020725
//Homework 2 - Matrix multiplication

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

struct mat{
    int numRows, numCols;
    float **elements;
};

void readMatrix(char * fileName, struct mat * matrix)
{
    FILE *matFile;
    char tmpString[6];

    //Opening matrix file for reading
    matFile=fopen(fileName, "r");

    //Scanning dimensions
    fscanf(matFile, "%s", tmpString);
    matrix->numRows = atoi(tmpString);
    fscanf(matFile, "%s", tmpString);
    matrix->numCols = atoi(tmpString);

    //Allocating memory for the matrix
    matrix->elements = (float**) malloc(matrix->numRows * sizeof(float*));
    for(int i=0; i<matrix->numRows; i++)
    {
        matrix->elements[i] = (float*)malloc(matrix->numCols * sizeof(float));
    }

    //Reading matrix contents
    for(int i=0; i<matrix->numRows; i++)
    {
        for(int j=0; j<matrix->numCols; j++)
        {
            fscanf(matFile, "%s", tmpString);
            matrix->elements[i][j] = atof(tmpString);
        }
        printf("\n");
    }

    //closing file
    fclose(matFile);

    return;
}

void writeMatrix(char * fileName, struct mat * matrix)
{
    //File instance
    FILE *matFile;

    //open file for writing
    matFile=fopen(fileName, "w");

    //convert integers into strings and write
    char tmpString[30];
    
    //converting and writing dimensions
    sprintf(tmpString, "%d ", matrix->numRows);
    fputs(tmpString, matFile);
    sprintf(tmpString, "%d\n", matrix->numCols);
    fputs(tmpString, matFile);

    //converting and writing elements
    for(int i=0; i<matrix->numRows; i++)
    {
        for(int j=0; j<matrix->numCols; j++)
        {
            sprintf(tmpString, "%f ", matrix->elements[i][j]);
            fputs(tmpString, matFile);
        }
        fputs("\n", matFile);
    }

    //closing file to release memory
    fclose(matFile);
}

void printMatrix(struct mat * matrix)
{
    for(int i=0; i<matrix->numRows; i++)
    {
        for(int j=0; j<matrix->numCols; j++)
        {
            printf("%f\t", matrix->elements[i][j]);
        }
        printf("\n");
    }
}

void freeMatrix (struct mat * matrix)
{
    //freeing both dimensions of matrix array
    for(int i=0; i<matrix->numRows; i++)
    {
        free(matrix->elements[i]);
    }

    free (matrix->elements);
}

void multMatrix (struct mat * matA, struct mat * matB, struct mat * resMat)
{
    //Checking if the multiplication can be performed
    if(matA->numCols != matB->numRows)
    {
        printf("The multiplication is not posible\n");
        resMat->numRows=0;
        resMat->numCols=0;
        resMat->elements=NULL;
        return;
    }else{
        //Getting dimensions for resulting matrix
        resMat->numRows = matA->numRows;
        resMat->numCols = matB->numCols;
        int commonDimsnsion = matA->numCols;

        //allocating memory for resulting matrix
        resMat->elements = (float**) malloc(resMat->numRows * sizeof(float*));
        for(int i=0; i<resMat->numRows; i++)
        {
            resMat->elements[i] = (float*)malloc(resMat->numCols * sizeof(float));
        }
        
        //matrix multiplication
        float sum=0;
        for(int i=0; i<resMat->numRows; i++)
        {
            for(int j=0; j<resMat->numCols; j++)
            {
                sum = 0;
                for(int k=0; k<commonDimsnsion; k++)
                {
                    sum += matA->elements[i][k] * matB->elements[k][j];
                }
                resMat->elements[i][j] = sum;
            }
        }        
    }
}

int main(int argc, char *argv[])
{
    int c=0;
    char * matApath = NULL;
    char * matBpath = NULL;
    char * resMatPath = NULL;

    while((c=getopt(argc, argv, "1:2:r:")) != -1)
    {
        switch (c)
        {
            case '1':
                matApath = optarg;
                break;
        
            case '2':
                matBpath = optarg;
                break;

            case 'r':
                resMatPath = optarg;
                break;

            case '?':
                printf("The input option is not valid!\n");
                return -1;
        }
    }

    struct mat matrixA, matrixB, resMatrix;
    readMatrix(matApath, &matrixA);
    readMatrix(matBpath, &matrixB);

    multMatrix(&matrixA, &matrixB, &resMatrix);

    printMatrix(&matrixA);
    printf("\n");
    printMatrix(&matrixB);
    printf("\n");
    printMatrix(&resMatrix);

    writeMatrix(resMatPath, &resMatrix);

    freeMatrix(&matrixA);
    freeMatrix(&matrixB);
    freeMatrix(&resMatrix);

    return 0;
}