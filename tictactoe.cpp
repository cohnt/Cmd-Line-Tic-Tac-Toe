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

int main() {
    gameBoard board = emptyBoard();
    displayBoard(board);

    return 0;
}