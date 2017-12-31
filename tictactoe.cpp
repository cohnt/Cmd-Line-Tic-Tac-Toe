#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <array>
#include <math.h>
#include <cassert>
#include <functional>

#include "neuralNetwork.h"

using namespace std;

enum gamePiece {
    empty,
    xPiece,
    oPiece
};
typedef array<array<gamePiece, 3>, 3> gameBoard;

const bool useTerminalHackyStuff = true;

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
        if(input.size() != 2u) {
            if(useTerminalHackyStuff) {
                printf("\033[2J\033[1;1H");
            }
            displayBoard(board);
            cout << "Wrong number of characters!" << endl << endl;
            continue;
        }
        try {
            array<int, 2> choice;
            choice[0] = stoi(input.substr(0, 1));
            choice[1] = stoi(input.substr(1, 2));
            if(choice[0] > 3 || choice[0] < 1 || choice[1] > 3 || choice[1] < 1) {
                if(useTerminalHackyStuff) {
                    printf("\033[2J\033[1;1H");
                }
                displayBoard(board);
                cout << "Invalid range!" << endl << endl;
                continue;
            }
            if(board[choice[0]-1][choice[1]-1] != empty) {
                if(useTerminalHackyStuff) {
                    printf("\033[2J\033[1;1H");
                }
                displayBoard(board);
                cout << "Space already picked!" << endl << endl;
                continue;
            }
            finalChoice = choice;
            valid = true;
        }
        catch(const std::invalid_argument& err) {
            if(useTerminalHackyStuff) {
                printf("\033[2J\033[1;1H");
            }
            displayBoard(board);
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
        if(board[k][2-k] == player) {
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
void gameLoop(gameBoard &board, int &winner) {
    bool gameOver = false;
    bool xTurn = true;
    while(!gameOver) {
        if(useTerminalHackyStuff) {
            printf("\033[2J\033[1;1H");
        }
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
}
void printWinner(gameBoard board, int winner) {
    if(useTerminalHackyStuff) {
        printf("\033[2J\033[1;1H");
    }
    displayBoard(board);
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
}

int main() {
    vector<int> nIds1 = {0, 1, 2, 3};
    vector<int> nIds2 = {0, 1, 2, 3};
    vector<int> nIds3 = {0, 3};

    vector<double> nWeights1 = {1, 1, 1, 1};
    vector<double> nWeights2 = {0.5, 0.5, 0.5, 0.5};
    vector<double> nWeights3 = {0.67, 0.33};

    function<double(double)> func = [](double x){ return (x>0 ? x : 0); };

    Neuron n1(nIds1, nWeights1, func);
    Neuron n2(nIds2, nWeights2, func);
    Neuron n3(nIds3, nWeights3, func);

    vector<Neuron> ns = {n1, n2, n3};

    NeuralNetworkLayer layer(ns);

    vector<double> inputs = {1, 2, 3, 4};
    vector<double> outputs = layer.outputs(inputs);

    // gameBoard board = emptyBoard();
    // int winner = 0; //1 for x, 2 for o, -1 for tie.
    // gameLoop(board, winner);
    // printWinner(board, winner);

    return 0;
}