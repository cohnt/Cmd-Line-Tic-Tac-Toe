#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <array>
#include <math.h>
#include <cassert>

using namespace std;

enum gamePiece {
    empty,
    xPiece,
    oPiece
};
typedef array<array<gamePiece, 3>, 3> gameBoard;

const bool useConsoleHack = true;

void printGamePiece(gamePiece p) {
    switch(p) {
        case empty:
            cout << " ";
            break;
        case xPiece:
            cout << "X";
            break;
        case oPiece:
            cout << "O";
            break;
        default:
            assert(1 == 0);
            break;
    }
}
void displayBoard(gameBoard board) {
    for(int i=0; i<3; ++i) {
        for(int j=0; j<3; ++j) {
            printGamePiece(board[i][j]);
            if(j < 2) {
                cout << "|";
            }
        }
        cout << endl;
        if(i < 2) {
            cout << "-+-+-" << endl;
        }
    }
}
gameBoard emptyBoard() {
    gameBoard foo;
    for(int i=0; i<3; ++i) {
        for(int j=0; j<3; ++j) {
            foo[i][j] = empty;
        }
    }
    return foo;
}
array<int, 2> getChoice(bool xTurn, gameBoard board) {
    bool valid = false;
    array<int, 2> finalChoice;
    while(!valid) {
        cout << "Player " << (xTurn ? "X" : "O") << ", it is your turn. Please select your space as two one-digit numbers, separated by nothing, with row first, and indexing from one." << endl;
        cout << "For example: 12 would give me the top middle space." << endl;
        string input;
        cin >> input;
        if(input.size() < 2u) {
            cout << "Not enough characters!" << endl << endl;
            continue;
        }
        try {
            array<int, 2> choice;
            choice[0] = stoi(input.substr(0, 1));
            choice[1] = stoi(input.substr(1, 2));
            if(choice[0] > 3 || choice[0] < 1 || choice[1] > 3 || choice[1] < 1) {
                cout << "Invalid range!" << endl << endl;
                continue;
            }
            if(board[choice[0]-1][choice[1]-1] != empty) {
                cout << "Space already picked!" << endl << endl;
                continue;
            }
            finalChoice = choice;
            valid = true;
        }
        catch(const std::invalid_argument& err) {
            cout << "Invalid numbers!" << endl << endl;
            continue;
        }
    }
    return finalChoice;
}
bool checkForWin(gameBoard board, bool xTurn) {
    gamePiece player = xTurn ? xPiece : oPiece;

    //Horizontal
    for(int i=0; i<3; ++i) {
        int rowTotal = 0;
        for(int j=0; j<3; ++j) {
            if(board[i][j] == player) {
                ++rowTotal;
            }
        }
        if(rowTotal == 3) {
            return true;
        }
    }

    //Vertical
    for(int j=0; j<3; ++j) {
        int colTotal = 0;
        for(int i=0; i<3; ++i) {
            if(board[i][j] == player) {
                ++colTotal;
            }
        }
        if(colTotal == 3) {
            return true;
        }
    }

    //Diagonals
    int diagTotal = 0;
    for(int k=0; k<3; ++k) {
        if(board[k][k] == player) {
            ++diagTotal;
        }
    }
    if(diagTotal == 3) {
        return true;
    }

    diagTotal = 0;
    for(int k=0; k<3; ++k) {
        if(board[k][k] == player) {
            ++diagTotal;
        }
    }
    if(diagTotal == 3) {
        return true;
    }

    //Not a win
    return false;
}
bool checkForTie(gameBoard board) {
    for(int i=0; i<3; ++i) {
        for(int j=0; j<3; ++j) {
            if(board[i][j] == empty) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    gameBoard board = emptyBoard();
    bool gameOver = false;
    bool xTurn = true;
    int winner = 0; //1 for x, 2 for o, -1 for tie.
    while(!gameOver) {
        displayBoard(board);
        array<int, 2> choice = getChoice(xTurn, board);
        cout << choice[0] << " " << choice[1] << endl;
        board[choice[0]-1][choice[1]-1] = (xTurn ? xPiece : oPiece);
        bool won = checkForWin(board, xTurn);
        if(won) {
            winner = xTurn ? 1 : 2;
            gameOver = true;
        }
        else {
            bool tied = checkForTie(board);
            if(tied) {
                winner = -1;
                gameOver = true;
            }
        }

        xTurn = !xTurn;
    }

    if(winner == 1) {
        cout << "X wins!" << endl;
    }
    else if(winner == 2) {
        cout << "O wins!" << endl;
    }
    else if(winner == -1) {
        cout << "It's a tie!" << endl;
    }
    else {
        cout << "Error." << endl;
    }

    return 0;
}