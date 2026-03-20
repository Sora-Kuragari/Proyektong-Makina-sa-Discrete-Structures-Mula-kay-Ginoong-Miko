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
    int xPos, yPos;

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

int CountF(Sys_Var *sys) // Calculates how many cells are part of set F
{
    int count = 0;
    int i, j;

    for (i = 0; i < M_SIZE; i++)
    {
        for (j = 0; j < M_SIZE; j++)
        {
            if (sys->R[sys->xPos][sys->yPos] == false && sys->B[sys->xPos][sys->yPos] == false)
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


void Remove(Sys_Var *pos){
    if (pos->go == true){
        pos->R[pos->xPos][pos->yPos] = false;
    }
    if (pos->go == false){
        pos->B[pos->xPos][pos->yPos] = false;
    }

    pos->S[pos->xPos][pos->yPos] = false;
    pos->T[pos->xPos][pos->yPos] = false;
}

void GameOver(Sys_Var *sys)
{
    int Rcount = 0;
    int Bcount = 0;

    // Counts
    for (sys->xPos = 0; sys->xPos < M_SIZE; sys->xPos++)
    {
        for (sys->yPos = 0; sys->yPos < M_SIZE; sys->yPos++)
        {
            if (sys->R[sys->xPos][sys->yPos] == true)
            {
                Rcount++;
            }
            if (sys->B[sys->xPos][sys->yPos] == true)
            {
                Bcount++;   
            }
        }
    }

    if (sys->over == true)
    {
        if (Rcount > Bcount)
        {
            strcpy(sys->result, "R wins");
        } else if (Rcount < Bcount)
        {
            strcpy(sys->result, "B wins");
        } else if (Rcount == Bcount)
        {
            strcpy(sys->result, "draw");
        }
    }
}

void Expand(Sys_Var *pos)
{
    //(a, b) = pos
    int a = pos->xPos;
    int b = pos->yPos;
    //u, d, k, r ∈ M
    int U[M_SIZE][M_SIZE];
    int D[M_SIZE][M_SIZE];
    int K[M_SIZE][M_SIZE];
    int R[M_SIZE][M_SIZE];

    //u = (a − 1, b)
    U[a - 1][b] = true;
    //d = (a + 1, b)
    D[a + 1][b] = true;
    //k = (a, b − 1)
    K[a][b - 1] = true;
    //r = (a, b + 1)
    R[a][b + 1] = true;
}

void Replace(Sys_Var *pos)
{
    pos->found = false;

    //(go ∧ pos ∈ B) → (B = B − {pos} ∧ found = true)
    if (pos->go == true && pos->B[pos->xPos][pos->yPos] == true)
    {
        pos->B[pos->xPos][pos->yPos] = false;
        pos->found = true;
    }
    //(go ∧ pos ∈ R) → found = true
    if (pos->go == true && pos->R[pos->xPos][pos->yPos] == true)
    {
        pos->found = true;
    }
    //(go ∧ pos̸∈ R) → (R = R ∪ {pos})
    if (pos->go == true && pos->R[pos->xPos][pos->yPos] == false)
    {
        pos->R[pos->xPos][pos->yPos] == true;
    }
    //(¬go ∧ pos ∈ R) → (R = R − {pos} ∧ found = true)
    if (pos->go == false && pos->R[pos->xPos][pos->yPos] == true)
    {
        pos->R[pos->xPos][pos->yPos] == false;
        pos->found = true;
    }
    //(¬go ∧ pos ∈ B) → found = true
    if (pos->go == false && pos->B[pos->xPos][pos->yPos] == true)
    {
        pos->found = true;
    }
    //(¬go ∧ pos̸∈ B) → (B = B ∪ {pos})
    if (pos->go == false && pos->B[pos->xPos][pos->yPos] == false)
    {
        pos->B[pos->xPos][pos->yPos] = true;
    }
    //(found ∧ pos̸∈ S) → (S = S ∪ {pos} ∧ f ound = false)
    if (pos->found == true && pos->S[pos->xPos][pos->yPos] == false)
    {
        pos->S[pos->xPos][pos->yPos] == true;
        pos->found = false;
    }
   //(found ∧ pos ∈ S ∧ pos̸∈ T ) → (T = T ∪ {pos} ∧ Expand(pos))
   if (pos->found == true && pos->S[pos->xPos][pos->yPos] == true && pos->T[pos->xPos][pos->yPos] == false)
   {
        pos->T[pos->xPos][pos->yPos] == true;
        Expand(pos);
   }
}


void Update(Sys_Var *pos)
{
    if (pos->xPos < 0 || pos->xPos >= M_SIZE || pos->yPos < 0 || pos->yPos >= M_SIZE) 
        {
            return;
        }

    pos->good = false;

    if (pos->S[pos->xPos][pos->yPos] == false)
    {
        pos->S[pos->xPos][pos->yPos] = true;

        if (pos->good == false)
        {
            pos->good = true;
                if (pos->good == false)
                {
                    pos->good = true;
                }
                else
                {
                    pos->good = false;
                }
        }
    }
    if (pos->good == false && pos->S[pos->xPos][pos->yPos] == true && pos->T[pos->xPos][pos->yPos] == false)
    {
        pos->T[pos->xPos][pos->yPos] = true;
    }
}

void NextPlayerMove(Sys_Var *pos)
{
    int countR = CountSet(pos->R);
    int countB = CountSet(pos->B);

    //(¬over ∧ start ∧ go) → (R = R ∪ {pos} ∧ S = S ∪ {pos} ∧ good = true)
    if (pos->over == false && pos->start == true && pos->go == true)
    {
        pos->R[pos->xPos][pos->yPos] == true;
        pos->S[pos->xPos][pos->yPos] == true;
        pos->good = true;
    }
    //(¬over ∧ start ∧ ¬go) → (B = B ∪ {pos} ∧ S = S ∪ {pos} ∧ good = true)
    if (pos->over == false && pos->start == true && pos->go == false)
    {
        pos->B[pos->xPos][pos->yPos] == true;
        pos->S[pos->xPos][pos->yPos] == true;
        pos->good = true;
    }
    //(¬over ∧ ¬start ∧ (go ∧ pos ∈ R ∨ ¬go ∧ pos ∈ B)) → (Update(pos) ∧ good = true)
    if (pos->over == false && pos->start == false && (pos->go == true && pos->R[pos->xPos][pos->yPos] == true || pos->go == false && pos->B[pos->xPos][pos->yPos]))
    {
        Update(pos);
        pos->good = true;
    }
    //(start ∧ |R| = 1 ∧ |B| = 1) → start = false
    if (pos->start == true && countR == 1 && countB == 1)
    {
        pos->start = false;
    }
    //(¬over ∧ good) → (good = ¬good ∧ go = ¬go ∧ val = val + 1)
    if (pos->over == false && pos->good == true)
    {
        pos->good = false;
        pos->go = false;
        pos->val = pos->val + 1;
    }
}

int main()
{
    Sys_Var Variables;
    
    SysInit(&Variables);

    return 0;
}