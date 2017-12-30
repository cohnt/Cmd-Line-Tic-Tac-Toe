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
        cout << "Player " << (xTurn ? "X" : "O") << ", it is your turn. Please select your space as two one-digit numbers, separated by nothing. Row first, indexing from one." << endl;
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
void checkForWin(gameBoard board) {
    //
}
void checkForTie(gameBoard board) {
    //
}

int main() {
    gameBoard board = emptyBoard();
    bool gameOver = false;
    bool xTurn = true;
    //int winner = 0; //1 for x, 2 for o, -1 for tie.
    while(!gameOver) {
        array<int, 2> choice = getChoice(xTurn, board);
        cout << choice[0] << choice[1] << endl;

        exit(0);

        board[choice[0]][choice[1]] = (xTurn ? xPiece : oPiece);
        checkForWin(board);
        checkForTie(board);

        gameOver = true;
    }

    return 0;
}