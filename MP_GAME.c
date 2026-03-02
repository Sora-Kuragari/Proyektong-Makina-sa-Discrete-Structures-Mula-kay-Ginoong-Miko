#include <stdio.h>
#define M_SIZE 3
#define TRUE 1
#define FALSE 0
/*
Applicable Sets
Set C = 1 2 3
Set N = 0 - 16
Set M = 3 x 3 Grid comprised of numbers 1-3
Set V = True, False
*/

struct Sys_VarTag // System Variables
{
    int good, go, start, over, found, val; // var before val are elements of set V, var val is element of set N
    int R[M_SIZE][M_SIZE], B[M_SIZE][M_SIZE], S[M_SIZE][M_SIZE], T[M_SIZE][M_SIZE], F[M_SIZE][M_SIZE]; // All vars are subsets of set M
};
typedef struct Sys_VarTag Sys_Var;
int isF(Sys_Var *sys, int row, int col) // Checks if specific cell is part of set F
{
    if (sys->R[row][col] == FALSE && sys->B[row][col] == FALSE)
    {
        return TRUE;
    }
    return FALSE;
}

int CountF(Sys_Var *sys) // Calculates how many cells are part of set F
{
    int count = 0;
    int i, j;
    for (i = 0; i < M_SIZE; i++)
    {
        for (j = 0; j < M_SIZE; j++)
        {
            if (isF(sys, i, j) == TRUE)
            {
                count++;
            }
        }
    }
    return count;
}

int CountSet(int set[M_SIZE][M_SIZE]) // Counts how many cells are part of a given set
{
    int count = 0;
    int i, j;   
    for (i = 0; i < M_SIZE; i++)
    {
        for (j = 0; j < M_SIZE; j++)
        {
            if (set[i][j] == TRUE)
            {
                count++;
            }
        }
    }
    return count;
}

void CheckOver(Sys_Var *sys) // Checks if the game is over and updates over variable 
{
    int countF = CountF(sys);
    int countR = CountSet(sys->R);
    int countB = CountSet(sys->B);
    if (countF == 3 || sys->val >= 20 || sys->start == FALSE && ((countR > 0 && countB == 0) || (countR == 0 && countB > 0)))
    {
        sys->over = TRUE;
    }
    else
    {
        sys->over = FALSE;
    }
}

void SysInit(Sys_Var *sys) // Initializes all system variables accdg to specs
{
    int i, j;
    sys->good = FALSE;
    sys->go = FALSE;
    sys->start = FALSE;
    sys->over = FALSE;
    sys->found = FALSE;
    sys->val = 0;
    for (i = 0; i < M_SIZE; i++)
    {
        for (j = 0; j < M_SIZE; j++)
        {
            sys->R[i][j] = FALSE;
            sys->B[i][j] = FALSE;
            sys->S[i][j] = FALSE;
            sys->T[i][j] = FALSE;
            sys->F[i][j] = FALSE;
        }
    }
}

void replace(Sys_Var *pos, int row, int col){
    if (pos->go == TRUE){
        pos->R[row][col] = FALSE;
    }
    if (pos->go == FALSE){
        pos->R[row][col] = FALSE;
    }

    pos->S[row][col] = FALSE;
    pos->T[row][col] = FALSE;
}