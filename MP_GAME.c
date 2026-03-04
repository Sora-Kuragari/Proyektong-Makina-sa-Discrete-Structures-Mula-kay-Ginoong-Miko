#include <stdio.h>
#include <string.h>
#define M_SIZE 3
#define true 1
#define false 0

/*
Applicable Sets
Set C = {x ∈ Z+ | x < 4}
Set N = {x ∈ Z+ ∪ {0} | x <= 16}
Set M = C × C
Set V = {true, false}
*/

struct Sys_VarTag // System Variables
{
    int good, go, start, over, found; // Elements of Set V
    int val; // Element of Set N
    int R[M_SIZE][M_SIZE], B[M_SIZE][M_SIZE], S[M_SIZE][M_SIZE], T[M_SIZE][M_SIZE], F[M_SIZE][M_SIZE]; // All vars are subsets of set M

    char result[10]; // {"R wins", "B wins", "draw"}
};

typedef struct Sys_VarTag Sys_Var;

void SysInit(Sys_Var *sys) // Initializes all system variables according to specs
{
    int i, j;
    sys->good = false;
    sys->go = false;
    sys->start = false;
    sys->over = false;
    sys->found = false;
    sys->val = 0;
    for (i = 0; i < M_SIZE; i++)
    {
        for (j = 0; j < M_SIZE; j++)
        {
            sys->R[i][j] = false;
            sys->B[i][j] = false;
            sys->S[i][j] = false;
            sys->T[i][j] = false;
            sys->F[i][j] = false;
        }
    }
}

int isF(Sys_Var *sys, int row, int col) // Checks if specific cell is part of set F
{
    if (sys->R[row][col] == false && sys->B[row][col] == false)
    {
        return true;
    }
    return false;
}

int CountF(Sys_Var *sys) // Calculates how many cells are part of set F
{
    int count = 0;
    int i, j;
    for (i = 0; i < M_SIZE; i++)
    {
        for (j = 0; j < M_SIZE; j++)
        {
            if (isF(sys, i, j) == true)
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
            if (set[i][j] == true)
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
    if (countF == 3 || sys->val >= 20 || sys->start == false && ((countR > 0 && countB == 0) || (countR == 0 && countB > 0)))
    {
        sys->over = true;
    }
    else
    {
        sys->over = false;
    }
}


void Remove(Sys_Var *pos, int row, int col){
    if (pos->go == true){
        pos->R[row][col] = false;
    }
    if (pos->go == false){
        pos->B[row][col] = false;
    }

    pos->S[row][col] = false;
    pos->T[row][col] = false;
}

void GameOver(Sys_Var *var)
{
    int row, col;
    int Rcount = 0;
    int Bcount = 0;

    // Counts
    for (row = 0; row < M_SIZE; row++)
    {
        for (col = 0; col < M_SIZE; col++)
        {
            if (var->R[row][col] == true)
            {
                Rcount++;
            }
            if (var->B[row][col] == true)
            {
                Bcount++;
            }
        }
    }

    if (var->over == true)
    {
        if (Rcount > Bcount)
        {
            strcpy(var->result, "R wins");
        } else if (Rcount < Bcount)
        {
            strcpy(var->result, "B wins");
        } else if (Rcount == Bcount)
        {
            strcpy(var->result, "draw");
        }
    }
}

int main()
{
    Sys_Var Variables;
    
    SysInit(&Variables);

    return 0;
}