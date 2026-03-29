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
            if (sys->R[i][j] == FALSE && sys->B[i][j] == FALSE)
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
    if (countF == 3 || sys->val >= 20 || (!sys->start && ((countR > 0 && countB == 0) || (countR == 0 && countB > 0))))
    {
        sys->over = TRUE;
    }
    else
    {
        sys->over = FALSE;
    }
}


void Remove(Sys_Var *pos){ //Removes Current Location depending on whose turn it is
    if (pos->go == TRUE){ //Red's Turn
        pos->R[pos->xPos][pos->yPos] = FALSE;
    }
    if (!pos->go){ //Blue's Turn
        pos->B[pos->xPos][pos->yPos] = FALSE;
    }

    pos->S[pos->xPos][pos->yPos] = FALSE;
    pos->T[pos->xPos][pos->yPos] = FALSE;
}

void GameOver(Sys_Var *sys) //Checks how many cells Red and Blue occupies, then decides who wins by how many are taken
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

void changePos(Sys_Var *sys, int pos[])  //Updates Position
{
    sys->xPos = pos[0];
    sys->yPos = pos[1];
}

void Expand(Sys_Var *pos) //Expands Red or Blue's occupied spaces, and removes their original position
{
    //(a, b) = pos
    int xy[2] = {pos->xPos, pos->yPos};
    //u, d, k, r ∈ M
    int u[2];
    int d[2];
    int k[2];
    int r[2];

    //u = (a − 1, b) To the Left
    u[0] = xy[0] - 1;
    u[1] = xy[1];
    //d = (a + 1, b) To the Right
    d[0] = xy[0] + 1;
    d[1] = xy[1];
    //k = (a, b − 1) Down
    k[0] = xy[0];
    k[1] = xy[1] - 1;
    //r = (a, b + 1) Up
    r[0] = xy[0];
    r[1] = xy[1] + 1;

    Remove(pos);
    if(u[0] >= 0 && u[0] <= 2 && u[1] >= 0 && u[1] <= 2) 
    {
        if (pos->go == TRUE)
        {
            changePos(pos, u);
            Replace(pos);
        }
    }
    
    if(d[0] >= 0 && d[0] <= 2 && d[1] >= 0 && d[1] <= 2)
    {
        if (pos->go == FALSE)
        {
            changePos(pos, d);
            Replace(pos);
        }
    }

    if(k[0] >= 0 && k[0] <= 2 && k[1] >= 0 && k[1] <= 2)
    {
        changePos(pos, k);
        Replace(pos);
    }

    if(r[0] >= 0 && r[0] <= 2 && r[1] >= 0 && r[1] <= 2)
    {
        changePos(pos, r);
        Replace(pos);
    }   
    
    changePos(pos, xy);
}

void Replace(Sys_Var *pos) //
{
    pos->found = FALSE; //Flag

    //(go ∧ pos ∈ B) → (B = B − {pos} ∧ found = TRUE)
    if (pos->go == TRUE && pos->B[pos->xPos][pos->yPos] == TRUE) //Red's Turn
    {
        pos->B[pos->xPos][pos->yPos] = FALSE; //Occupies what was Blue's Cell
        pos->found = TRUE;
    }
    //(go ∧ pos ∈ R) → found = TRUE
    if (pos->go == TRUE && pos->R[pos->xPos][pos->yPos] == TRUE) //Red's Turn
    {
        pos->found = TRUE; //Cell remains occupied by Red
    }
    //(go ∧ pos̸∈ R) → (R = R ∪ {pos})
    if (pos->go == TRUE && pos->R[pos->xPos][pos->yPos] == FALSE) //Red's Turn
    {
        pos->R[pos->xPos][pos->yPos] = TRUE; //Red occupies cell
    }
    //(¬go ∧ pos ∈ R) → (R = R − {pos} ∧ found = TRUE)
    if (!pos->go && pos->R[pos->xPos][pos->yPos] == TRUE) //Blue's Turn
    {
        pos->R[pos->xPos][pos->yPos] = FALSE; //Occupies what was Red's Cell
        pos->found = TRUE;
    }
    //(¬go ∧ pos ∈ B) → found = TRUE
    if (!pos->go && pos->B[pos->xPos][pos->yPos] == TRUE) //Blue's Turn
    {
        pos->found = TRUE; //Cell remains occupied by Blue
    }
    //(¬go ∧ pos̸∈ B) → (B = B ∪ {pos})
    if (!pos->go && pos->B[pos->xPos][pos->yPos] == FALSE) //Blue's Turn
    {
        pos->B[pos->xPos][pos->yPos] = TRUE; //Blue occupies cell
    }
    //(found ∧ pos̸∈ S) → (S = S ∪ {pos} ∧ f ound = FALSE)
    if (pos->found == TRUE && pos->S[pos->xPos][pos->yPos] == FALSE)
    {
        pos->S[pos->xPos][pos->yPos] = TRUE; //Records position as "found"
        pos->found = FALSE;
    }
   //(found ∧ pos ∈ S ∧ pos̸∈ T ) → (T = T ∪ {pos} ∧ Expand(pos))
   if (pos->found == TRUE && pos->S[pos->xPos][pos->yPos] == TRUE && pos->T[pos->xPos][pos->yPos] == FALSE)
   {
        pos->T[pos->xPos][pos->yPos] = TRUE; //Checks position's neighbors (Left, Right, Down, Up)
        Expand(pos);
   }
}


void Update(Sys_Var *pos) 
{
    pos->good = FALSE; //Invalid Position

    if (pos->S[pos->xPos][pos->yPos] == FALSE) //If cell is not in S, add it to S and Declare good as true
    {
        pos->S[pos->xPos][pos->yPos] = TRUE;
        if (pos->good == TRUE){
            pos->good = FALSE;
        }
        else{
            pos->good = TRUE;
        }
    }

    if (!pos->good && pos->S[pos->xPos][pos->yPos] == TRUE && pos->T[pos->xPos][pos->yPos] == FALSE) //(¬good ∧ pos ∈ S ∧ pos ̸∈ T) → (T = T ∪ {pos} ∧ Expand(pos))
    {
        pos->T[pos->xPos][pos->yPos] = TRUE;
        Expand(pos);
    }
}

void NextPlayerMove(Sys_Var *pos)
{
    //(¬over ∧ start ∧ go) → (R = R ∪ {pos} ∧ S = S ∪ {pos} ∧ good = TRUE)
    if (!pos->over && pos->start && pos->go) //Red's Turn
    {
        pos->R[pos->xPos][pos->yPos] = TRUE;
        pos->S[pos->xPos][pos->yPos] = TRUE;
        pos->good = TRUE;
    }
    //(¬over ∧ start ∧ ¬go) → (B = B ∪ {pos} ∧ S = S ∪ {pos} ∧ good = TRUE)
    if (!pos->over && pos->start && !pos->go) //Blue's Turn
    {
        pos->B[pos->xPos][pos->yPos] = TRUE;
        pos->S[pos->xPos][pos->yPos] = TRUE;
        pos->good = TRUE;
    }
    //(¬over ∧ ¬start ∧ (go ∧ pos ∈ R ∨ ¬go ∧ pos ∈ B)) → (Update(pos) ∧ good = TRUE)
    if (!pos->over && !pos->start && ((pos->go == TRUE && pos->R[pos->xPos][pos->yPos] == TRUE) || (!pos->go && pos->B[pos->xPos][pos->yPos])))
    {
        Update(pos);
        pos->good = TRUE;
    }

    int countR = CountSet(pos->R);
    int countB = CountSet(pos->B);
    
    //(start ∧ |R| = 1 ∧ |B| = 1) → start = FALSE
    if (pos->start == TRUE && countR == 1 && countB == 1)
    {
        pos->start = FALSE;
    }
    //(¬over ∧ good) → (good = ¬good ∧ go = ¬go ∧ val = val + 1)
    if (!pos->over && pos->good)
    {
        pos->good = !pos->good;
        
        pos->go = !pos->go;

        pos->val = pos->val + 1;
    }
}

char status(Sys_Var sys, int x, int y)
{
    char res;
    if (sys.S[x][y] == TRUE)
    {
        res = '!';
    } else
    {
        if (sys.R[x][y] == FALSE && sys.B[x][y] == FALSE)
        {
            res = ' ';
        } else
        {
            res = '.';
        }
    }

    return res;
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

    // Board
    printf("================\n");
    printf("*    Turn %02d   *\n", sys.val+1);
    printf("+----+----+----+\n");
    printf("| %c%c | %c%c | %c%c |\n", pos[0][0], visual(sys, 0, 0),  pos[1][0], visual(sys, 1, 0), pos[2][0], visual(sys, 2, 0));
    printf("+----+----+----+\n");
    printf("| %c%c | %c%c | %c%c |\n", pos[0][1], visual(sys, 0, 1), pos[1][1], visual(sys, 1, 1), pos[2][1], visual(sys, 2, 1));
    printf("+----+----+----+\n");
    printf("| %c%c | %c%c | %c%c |\n", pos[0][2], visual(sys, 0, 2), pos[1][2], visual(sys, 1, 2), pos[2][2], visual(sys, 2, 2));
    printf("+----+----+----+\n");
}

int main()
{
    Sys_Var sys;
    SysInit(&sys);
    int x, y;

    display(sys);

    do {
        
        if (sys.go == TRUE)
        {
            printf("Red's Turn:\n");
            do
            {
                printf("x = ");
                scanf("%d", &x);
            } while (x > 3 || x < 1);
            do
            {
                printf("y = ");
                scanf("%d", &y);
            } while (y > 3 || y < 1);
            
            sys.xPos = x-1;
            sys.yPos = y-1;
        } else
        {
            printf("Blue's Turn:\n");
            do
            {
                printf("x = ");
                scanf("%d", &x);
            } while (x > 3 || x < 1);
            do
            {
                printf("y = ");
                scanf("%d", &y);
            } while (y > 3 || y < 1); 

            sys.xPos = x-1;
            sys.yPos = y-1;
        }

        NextPlayerMove(&sys);

        CheckOver(&sys);

        display(sys);

    } while (sys.over != TRUE);
    GameOver(&sys);

    printf("%s", sys.result);

    return 0;
}