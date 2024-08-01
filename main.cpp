#include <windows.h>
#include <bits/stdc++.h>

using namespace std;

#define endl '\n'
#define int long long

char matrix[5][5]{
        '#', 'O', 'O', 'O', '#',
        'X', '.', '.', '.', '.',
        'X', '.', '.', '.', '.',
        'X', '.', '.', '.', '.',
        '#', '.', '.', '.', '#'};

enum Movement
{
    BAD = 0,
    GOOD = 1,
    JUMP = 2
};
enum Winner{
    COMPUTER = 2,
    PLAYER = 1,
    NONE = 0
};
struct Move
{
    int row = -1, column = -1, value = -1;
};

void displayMatrix()
{

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; ++j)
        {
            if (matrix[i][j] == 'X')
            {
                SetConsoleTextAttribute(h, 4);
                cout << matrix[i][j] << ' ';
            }
            else if (matrix[i][j] == 'O')
            {
                SetConsoleTextAttribute(h, 2);
                cout << matrix[i][j] << ' ';
            }
            else
            {
                SetConsoleTextAttribute(h, 7);
                cout << matrix[i][j] << ' ';
            }
        }
        cout << endl;
    }
}

int isValidPlayerMovement(int x, int y)
{
    if (y == 4)
        return false;
    else
    {
        if (matrix[x][y + 1] == '.')
        {
            return GOOD; // move 2 right
        }
        else if (matrix[x][y + 1] != '.' and y + 2 < 5 and matrix[x][y + 2] == '.')
        {
            return JUMP; // move 1 right
        }
        else
        {
            return BAD; // can't move, the path is not clear
        }
    }
}
int isValidComputerMovement(int x, int y)
{
    if (x == 4)
        return false;
    else
    {
        if (matrix[x + 1][y] != '.' and x + 2 < 5 and matrix[x + 2][y] == '.')
        {
            return JUMP; // move 2 down
        }
        else if (matrix[x + 1][y] == '.')
        {
            return GOOD; // move one down
        }
        else
            return BAD; // can't move, the path is not clear
    }
}

int hasWinner()
{
    if (matrix[4][1] == 'O' and matrix[4][2] == 'O' and matrix[4][3] == 'O')
    {
        return COMPUTER; // computer wins
    }
    else if (matrix[1][4] == 'X' and matrix[2][4] == 'X' and matrix[3][4] == 'X')
    {
        return PLAYER; // player wins
    }
    else
        return NONE; // no one wins ... the game is still going
}
int miniMax(int depth, bool isMaximizing)
{
    int score = hasWinner();
    if (score == 1)
    {
        return -1e9;
    }
    else if (score == 2)
    {
        return 1e9 - depth;
    }
    else
    {
        if (isMaximizing)
        {
            int best = -1e9;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; ++j)
                {
                    if (matrix[i][j] == 'O')
                    {
                        int movement = isValidComputerMovement( i, j);
                        if (movement)
                        {
                            swap(matrix[i][j], matrix[i + movement][j]);
                            best = max(best, miniMax( depth + 1, !isMaximizing));
                            swap(matrix[i][j], matrix[i + movement][j]);
                        }
                        else
                            continue;
                    }
                }
            }
            return best;
        }
        else
        {
            int best = 1e9;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; ++j)
                {
                    if (matrix[i][j] == 'X')
                    {
                        int movement = isValidPlayerMovement( i, j);
                        if (movement)
                        {
                            swap(matrix[i][j], matrix[i][j + movement]); // make movement
                            best = min(best, miniMax(depth, !isMaximizing));
                            swap(matrix[i][j], matrix[i][j + movement]); // undo the movement
                        }
                        else
                            continue;
                    }
                }
            }
            return best;
        }
    }
}

Move findBestMove()
{
    Move bestMove;

    int bestVal = -1e9;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (matrix[i][j] == 'O')
            {
                int movement = isValidComputerMovement( i, j);
                if (movement)
                {
                    swap(matrix[i][j], matrix[i + movement][j]);
                    int moveVal = miniMax( 0, false);
                    swap(matrix[i][j], matrix[i + movement][j]);

                    if (moveVal > bestVal and moveVal > 0)
                    {
                        bestMove.row = i;
                        bestMove.column = j;
                        bestVal = moveVal;
                    }
                }
            }
        }
    }
    return bestMove;
}
bool computerTurn()
{
    Move myMove = findBestMove();
    int row = myMove.row, col = myMove.column;
    if (myMove.row == -1 and myMove.column == -1)
    {
        cout << "Player Wins , Congratulations You Played Optimal" << endl;
        return false;
    }
    else
    {
        swap(matrix[row][col], matrix[row + isValidComputerMovement(row, col)][col]);
        return true;
    }
}
void playerTurn()
{
    int row, column;
    displayMatrix();
    cout << "Enter Row and Column values for 'X' :";
    cin >> row >> column;
    while (matrix[row][column] != 'X')
    {
        cout << "Enter Row and Column values for 'X' :";
        cin >> row >> column;
    }
    int movement = isValidPlayerMovement( row, column);
    if (movement)
    {
        swap(matrix[row][column], matrix[row][column + movement]);
    }
    else
        cout << "CANT MOVE" << endl;
}
bool boolHasWinner()
{
    int score = hasWinner();
    if (score == 1)
    {
        cout << "Player Wins !!" << endl;
        return true;
    }
    else if (score == 2)
    {
        displayMatrix();
        cout << "Computer Wins !!" << endl;
        return true;
    }
    return false;
}
void solve()
{
    bool player = true;
    while (true)
    {
        if (boolHasWinner())
            break;

        if (player)
        {
            playerTurn();
        }
        else
        {
            bool canMove = computerTurn();
            if (not canMove)
                break;
            system("CLS");
        }
        player = !player;
    }
    system("PAUSE");
};
// Praise Be To ALLAH ❤️

signed main()
{
    //    FastIO
    int t = 1;
    //    cin >> t;
    while (t--)
    {
        solve();
    }
}
