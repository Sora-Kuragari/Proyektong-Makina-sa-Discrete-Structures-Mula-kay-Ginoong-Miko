#include <stdio.h>
#include <string.h>
#define M_SIZE 3
#define TRUE 1
#define FALSE 0

/*
Applicable Sets
Set C = {x ∈ Z+ | x < 4}
Set N = {x ∈ Z+ ∪ {0} | x <= 16}
Set M = C × C
Set V = {TRUE, FALSE}
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

// Function prototypes
void Expand(Sys_Var *pos);
void Replace(Sys_Var *pos);

void SysInit(Sys_Var *sys) // Initializes all system variables according to specs
{
    int i, j;
    sys->good = FALSE;
    sys->go = TRUE;
    sys->start = TRUE;
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

int CountF(Sys_Var *sys) // Calculates how many cells are part of set F
{
    int count = 0;
    int i, j;

    for (i = 0; i < M_SIZE; i++)
    {
        for (j = 0; j < M_SIZE; j++)
        {
            if (sys->R[sys->xPos][sys->yPos] == FALSE && sys->B[sys->xPos][sys->yPos] == FALSE)
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
    if ((countF == 3 || sys->val >= 20 || sys->start == FALSE) && ((countR > 0 && countB == 0) || (countR == 0 && countB > 0)))
    {
        sys->over = TRUE;
    }
    else
    {
        sys->over = FALSE;
    }
}


void Remove(Sys_Var *pos){
    if (pos->go == TRUE){
        pos->R[pos->xPos][pos->yPos] = FALSE;
    }
    if (pos->go == FALSE){
        pos->B[pos->xPos][pos->yPos] = FALSE;
    }

    pos->S[pos->xPos][pos->yPos] = FALSE;
    pos->T[pos->xPos][pos->yPos] = FALSE;
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
            if (sys->R[sys->xPos][sys->yPos] == TRUE)
            {
                Rcount++;
            }
            if (sys->B[sys->xPos][sys->yPos] == TRUE)
            {
                Bcount++;   
            }
        }
    }

    if (sys->over == TRUE)
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

void changePos(Sys_Var *sys, int pos[])
{
    sys->xPos = pos[0];
    sys->yPos = pos[1];
}

void Expand(Sys_Var *pos)
{
    //(a, b) = pos
    int xy[2] = {pos->xPos, pos->yPos};
    //u, d, k, r ∈ M
    int u[2];
    int d[2];
    int k[2];
    int r[2];

    //u = (a − 1, b)
    u[0] = xy[0] - 1;
    u[1] = xy[1];
    //d = (a + 1, b)
    d[0] = xy[0] + 1;
    d[1] = xy[1];
    //k = (a, b − 1)
    k[0] = xy[0];
    k[1] = xy[1] - 1;
    //r = (a, b + 1)
    r[0] = xy[0];
    r[1] = xy[1] + 1;

    Remove(pos);
    if (pos->go == TRUE)
    {
        changePos(pos, u);
        Replace(pos);
    }
    
    if (pos->go == FALSE)
    {
        changePos(pos, d);
        Replace(pos);
    }
    
    changePos(pos, k);
    Replace(pos);
    
    changePos(pos, r);
    Replace(pos);
    
    changePos(pos, xy);
}

void Replace(Sys_Var *pos)
{
    pos->found = FALSE;

    //(go ∧ pos ∈ B) → (B = B − {pos} ∧ found = TRUE)
    if (pos->go == TRUE && pos->B[pos->xPos][pos->yPos] == TRUE)
    {
        pos->B[pos->xPos][pos->yPos] = FALSE;
        pos->found = TRUE;
    }
    //(go ∧ pos ∈ R) → found = TRUE
    if (pos->go == TRUE && pos->R[pos->xPos][pos->yPos] == TRUE)
    {
        pos->found = TRUE;
    }
    //(go ∧ pos̸∈ R) → (R = R ∪ {pos})
    if (pos->go == TRUE && pos->R[pos->xPos][pos->yPos] == FALSE)
    {
        pos->R[pos->xPos][pos->yPos] = TRUE;
    }
    //(¬go ∧ pos ∈ R) → (R = R − {pos} ∧ found = TRUE)
    if (pos->go == FALSE && pos->R[pos->xPos][pos->yPos] == TRUE)
    {
        pos->R[pos->xPos][pos->yPos] = FALSE;
        pos->found = TRUE;
    }
    //(¬go ∧ pos ∈ B) → found = TRUE
    if (pos->go == FALSE && pos->B[pos->xPos][pos->yPos] == TRUE)
    {
        pos->found = TRUE;
    }
    //(¬go ∧ pos̸∈ B) → (B = B ∪ {pos})
    if (pos->go == FALSE && pos->B[pos->xPos][pos->yPos] == FALSE)
    {
        pos->B[pos->xPos][pos->yPos] = TRUE;
    }
    //(found ∧ pos̸∈ S) → (S = S ∪ {pos} ∧ f ound = FALSE)
    if (pos->found == TRUE && pos->S[pos->xPos][pos->yPos] == FALSE)
    {
        pos->S[pos->xPos][pos->yPos] = TRUE;
        pos->found = FALSE;
    }
   //(found ∧ pos ∈ S ∧ pos̸∈ T ) → (T = T ∪ {pos} ∧ Expand(pos))
   if (pos->found == TRUE && pos->S[pos->xPos][pos->yPos] == TRUE && pos->T[pos->xPos][pos->yPos] == FALSE)
   {
        pos->T[pos->xPos][pos->yPos] = TRUE;
        Expand(pos);
   }
}


void Update(Sys_Var *pos)
{
    if (pos->xPos < 0 || pos->xPos >= M_SIZE || pos->yPos < 0 || pos->yPos >= M_SIZE) 
        {
            return;
        }

    pos->good = FALSE;

    if (pos->S[pos->xPos][pos->yPos] == FALSE)
    {
        pos->S[pos->xPos][pos->yPos] = TRUE;
        pos->good = TRUE;
    }

    if (pos->good == FALSE && pos->S[pos->xPos][pos->yPos] == TRUE && pos->T[pos->xPos][pos->yPos] == FALSE)
    {
        pos->T[pos->xPos][pos->yPos] = TRUE;
        Expand(pos);
    }
}

void NextPlayerMove(Sys_Var *pos)
{
    int countR = CountSet(pos->R);
    int countB = CountSet(pos->B);

    //(¬over ∧ start ∧ go) → (R = R ∪ {pos} ∧ S = S ∪ {pos} ∧ good = TRUE)
    if (pos->over == FALSE && pos->start == TRUE && pos->go == TRUE)
    {
        pos->R[pos->xPos][pos->yPos] = TRUE;
        pos->S[pos->xPos][pos->yPos] = TRUE;
        pos->good = TRUE;
    }
    //(¬over ∧ start ∧ ¬go) → (B = B ∪ {pos} ∧ S = S ∪ {pos} ∧ good = TRUE)
    if (pos->over == FALSE && pos->start == TRUE && pos->go == FALSE)
    {
        pos->B[pos->xPos][pos->yPos] = TRUE;
        pos->S[pos->xPos][pos->yPos] = TRUE;
        pos->good = TRUE;
    }
    //(¬over ∧ ¬start ∧ (go ∧ pos ∈ R ∨ ¬go ∧ pos ∈ B)) → (Update(pos) ∧ good = TRUE)
    if (pos->over == FALSE && pos->start == FALSE && ((pos->go == TRUE && pos->R[pos->xPos][pos->yPos] == TRUE) || (pos->go == FALSE && pos->B[pos->xPos][pos->yPos])))
    {
        Update(pos);
        pos->good = TRUE;
    }
    //(start ∧ |R| = 1 ∧ |B| = 1) → start = FALSE
    if (pos->start == TRUE && countR == 1 && countB == 1)
    {
        pos->start = FALSE;
    }
    //(¬over ∧ good) → (good = ¬good ∧ go = ¬go ∧ val = val + 1)
    if (pos->over == FALSE && pos->good == TRUE)
    {
        pos->good = FALSE;
        pos->go = FALSE;
        pos->val = pos->val + 1;
    }
}

void display(Sys_Var sys)
{
    char pos[3][3];
    int i, j;

    for (i = 0; i < M_SIZE; i++)
    {
        for (j = 0; j < M_SIZE; j++)
        {
            if (sys.R[i][j] == TRUE && sys.B[i][j] == FALSE)
            {
                pos[i][j] = 'R';
            } else if (sys.B[i][j] == TRUE && sys.R[i][j] == FALSE)
            {
                pos[i][j] = 'B';
            } else
            {
                pos[i][j] = ' ';
            }
        }
    }

    printf("+---+---+---+\n");
    printf("| %c | %c | %c |\n", pos[0][0], pos[1][0], pos[2][0]);
    printf("+---+---+---+\n");
    printf("| %c | %c | %c |\n", pos[0][1], pos[1][1], pos[2][1]);
    printf("+---+---+---+\n");
    printf("| %c | %c | %c |\n", pos[0][2], pos[1][2], pos[2][2]);
    printf("+---+---+---+\n");
}

int main()
{
    Sys_Var sys;
    SysInit(&sys);
    int x, y;

    do {

        if (sys.go == TRUE)
        {
            printf("Red's Turn:\n");
            do
            {
                printf("x = ");
                scanf("%d", &x);
            } while (x > 3 && x < 1);
            do
            {
                printf("y = ");
                scanf("%d", &y);
            } while (y > 3 && y < 1);
            
            sys.xPos = x-1;
            sys.yPos = y-1;
        } else
        {
            printf("Blue's Turn:\n");
            do
            {
                printf("x = ");
                scanf("%d", &x);
            } while (x > 3 && x < 1);
            do
            {
                printf("y = ");
                scanf("%d", &y);
            } while (y > 3 && y < 1);

            sys.xPos = x-1;
            sys.yPos = y-1;
        }
        display(sys);

        NextPlayerMove(&sys);

        CheckOver(&sys);

    } while (sys.over != TRUE);
    GameOver(&sys);

    printf("%s", sys.result);

    return 0;
}