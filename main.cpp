#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

#define endl '\n'
#define int long long

char matrix[5][5]{
        '#', 'O', 'O', 'O', '#',
        'X', '.', '.', '.', '.',
        'X', '.', '.', '.', '.',
        'X', '.', '.', '.', '.',
        '#', '.', '.', '.', '#'
};
#define BAD  0;
#define GOOD 1
#define JUMP 2;

struct Move {
    int row, column, value;
};

void displayMatrix() {

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; ++j) {
            if (matrix[i][j] == 'X') {
                SetConsoleTextAttribute(h, 4);
                cout << matrix[i][j] << ' ';
            } else if (matrix[i][j] == 'O') {
                SetConsoleTextAttribute(h, 2);
                cout << matrix[i][j] << ' ';
            } else {
                SetConsoleTextAttribute(h, 7);
                cout << matrix[i][j] << ' ';
            }
        }
        cout << endl;
    }
}

int isValidMovement(char mat[5][5], int x, int y, bool isMaximizing) {
    if (isMaximizing) {
        if (x == 4)
            return false;
        else {
            if (mat[x + 1][y] != '.' and x + 2 < 5 and mat[x + 2][y] == '.') {
                return JUMP; // move 2 down
            } else if (mat[x + 1][y] == '.') {
                return GOOD; // move one down
            } else
                return BAD; // can't move, the path is not clear
        }
    } else {
        if (y == 4)
            return false;
        else {
            if (mat[x][y + 1] == '.') {
                return GOOD; // move 2 right
            } else if (mat[x][y + 1] != '.' and y + 2 < 5 and mat[x][y + 2] == '.') {
                return JUMP;// move 1 right
            } else {
                return BAD;// can't move, the path is not clear
            }
        }
    }
}

int winner(char mat[5][5]) {
    if (mat[4][1] == 'O' and mat[4][2] == 'O' and mat[4][3] == 'O') {
        return 2; // computer wins
    } else if (mat[1][4] == 'X' and mat[2][4] == 'X' and mat[3][4] == 'X') {
        return 1; // player wins
    } else
        return 0;// no one wins ... the game is still going
}

int miniMax(char mat[5][5], int depth, bool isMaximizing) { // maximize the computer
    int score = winner(mat);
    if (score == 1) {
        return -10;
    } else if (score == 2) {
        return 100 - depth;
    } else {
        if (isMaximizing) {
            int best = -1000;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; ++j) {
                    if (mat[i][j] == 'O') {
                        int movement = isValidMovement(mat, i, j, isMaximizing);
                        if (movement) {
                            swap(mat[i][j], mat[i + movement][j]);
                            best = max(best, miniMax(mat, depth + 1, !isMaximizing));
                            swap(mat[i][j], mat[i + movement][j]);
                        } else
                            continue;
                    }
                }
            }
            return best;
        } else {
            int best = 1000;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; ++j) {
                    if (mat[i][j] == 'X') {
                        int movement = isValidMovement(mat, i, j, isMaximizing);
                        if (movement) {
                            swap(mat[i][j], mat[i][j + movement]);// make movement
                            best = min(best, miniMax(mat, depth, !isMaximizing));
                            swap(mat[i][j], mat[i][j + movement]);// undo the movement
                        } else
                            continue;
                    }
                }
            }
            return best;
        }
    }
}

Move findBestMove(char mat[5][5]) {
    Move bestMove;
    bestMove.row = -1;
    bestMove.column = -1;
    bestMove.value = -1;
    int bestVal = -INT_MAX;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (mat[i][j] == 'O') {
                int movement = isValidMovement(mat, i, j, 1);
                if (movement) {
                    swap(mat[i][j], mat[i + movement][j]);
                    int moveVal = miniMax(mat, 0, 0);
                    swap(mat[i][j], mat[i + movement][j]);
                    if (moveVal > bestVal and moveVal > 0) {
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

void solve() {
    bool player = true;
    while (true) {
        int score = winner(matrix);
        if (score == 1) {
            cout << "Player Wins !!" << endl;
            break;
        } else if (score == 2) {
            displayMatrix();
            cout << "Computer Wins !!" << endl;
            break;
        }
        if (player) {
            int row, column;
            player = !player;
            displayMatrix();
            cout << "Enter Row and Column values for 'X' :";
            cin >> row >> column;
            while (matrix[row][column] != 'X') {
                cout << "Enter Row and Column values for 'X' :";
                cin >> row >> column;
            }
            int movement = isValidMovement(matrix, row, column, false);
            if (movement) {
                swap(matrix[row][column], matrix[row][column + movement]);
            } else
                cout << "CANT MOVE" << endl;
        } else {
            player = !player;
            Move myMove = findBestMove(matrix);
            int row = myMove.row, col = myMove.column;
            if (myMove.row == -1 and myMove.column == -1) {
                cout << "Player Wins , Congratulations You Played Optimal" << endl;
                break;
            } else {
                swap(matrix[row][col], matrix[row + isValidMovement(matrix, row, col, true)][col]);
            }
            system("CLS");
        }
    }
    system("PAUSE");
};
// Praise Be To ALLAH ❤️

signed main() {
//    FastIO
    int t = 1;
//    cin >> t;
    while (t--) {
        solve();
    }
}



