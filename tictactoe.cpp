#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <array>
#include <math.h>
#include <cassert>
#include <functional>
#include <random>
#include <time.h>
#include <algorithm>

#include "ticTacToeGenome.h"

enum gamePiece {
    empty,
    xPiece,
    oPiece
};
enum gameResult {
    win,
    loss,
    tie
};
typedef std::array<std::array<gamePiece, 3>, 3> gameBoard;

const bool useTerminalHackyStuff = true;

const int numGenerations = 25;
const int creaturesPerGeneration = 20;
const int numToBreed = 20; //10 pairs, each with 10 offspring
// const std::string crossOverType = "single"; //Options: single, double. Could add others later.
const double mutationChance = 0.001; //0.5 is 50% chance for a single gene value to change
const int matchesPerCreaturePair = 5; //Each generation, between each creature, how many matches?
const std::vector<int> networkArchitecture = {18, 27, 9, 9};

void printGamePiece(gamePiece p) {
    switch(p) {
        case empty:
            std::cout << " ";
            break;
        case xPiece:
            std::cout << "X";
            break;
        case oPiece:
            std::cout << "O";
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
                std::cout << "|";
            }
        }
        std::cout << std::endl;
        if(i < 2) {
            std::cout << "-+-+-" << std::endl;
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
std::array<int, 2> getChoice(bool xTurn, gameBoard board) {
    bool valid = false;
    std::array<int, 2> finalChoice;
    while(!valid) {
        std::cout << "Player " << (xTurn ? "X" : "O") << ", it is your turn. Please select your space as two one-digit numbers, separated by nothing, with row first, and indexing from one." << std::endl;
        std::cout << "For example: 12 would give me the top middle space." << std::endl;
        std::string input;
        std::cin >> input;
        if(input.size() != 2u) {
            if(useTerminalHackyStuff) {
                printf("\033[2J\033[1;1H");
            }
            displayBoard(board);
            std::cout << "Wrong number of characters!" << std::endl << std::endl;
            continue;
        }
        try {
            std::array<int, 2> choice;
            choice[0] = stoi(input.substr(0, 1));
            choice[1] = stoi(input.substr(1, 2));
            if(choice[0] > 3 || choice[0] < 1 || choice[1] > 3 || choice[1] < 1) {
                if(useTerminalHackyStuff) {
                    printf("\033[2J\033[1;1H");
                }
                displayBoard(board);
                std::cout << "Invalid range!" << std::endl << std::endl;
                continue;
            }
            if(board[choice[0]-1][choice[1]-1] != empty) {
                if(useTerminalHackyStuff) {
                    printf("\033[2J\033[1;1H");
                }
                displayBoard(board);
                std::cout << "Space already picked!" << std::endl << std::endl;
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
            std::cout << "Invalid numbers!" << std::endl << std::endl;
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
void hotseatGameLoop(gameBoard &board, int &winner) {
    bool gameOver = false;
    bool xTurn = true;
    while(!gameOver) {
        if(useTerminalHackyStuff) {
            printf("\033[2J\033[1;1H");
        }
        displayBoard(board);
        std::array<int, 2> choice = getChoice(xTurn, board);
        std::cout << choice[0] << " " << choice[1] << std::endl;
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
std::vector<double> loadInputsFromBoard(gameBoard board) {
    std::vector<double> res;
    for(int i=0; i<3; ++i) {
        for(int j=0; j<3; ++j) {
            if(board[i][j] == empty) {
                res.push_back(0);
                res.push_back(0);
            }
            else if(board[i][j] == xPiece) {
                res.push_back(1);
                res.push_back(0);
            }
            else if(board[i][j] == oPiece) {
                res.push_back(0);
                res.push_back(1);
            }
        }
    }
    assert(res.size() == 18u);
    return res;
}
void aiVsPlayerGameLoop(gameBoard &board, int &winner, NeuralNetwork ai, bool computerGoesFirst) {
    //Make this better later.

    bool gameOver = false;
    bool xTurn = true;
    while(!gameOver) {
        if(useTerminalHackyStuff) {
            printf("\033[2J\033[1;1H");
        }
        displayBoard(board);
        std::array<int, 2> choice;
        if(computerGoesFirst == xTurn) {
            //Computer's turn.
            std::vector<double> inputs = loadInputsFromBoard(board);
            std::vector<double> outputs = ai.outputs(inputs);
            //We have weights. Now select the highest empty space.
            while(true) {
                double maxOutput = -1;
                int maxOutputIndex = -1;
                for(int i=0; i<int(outputs.size()); ++i) {
                    if(outputs[i] > maxOutput) {
                        maxOutput = outputs[i];
                        maxOutputIndex = i;
                    }
                }
                assert(maxOutputIndex != -1);
                std::cout << "Trying index " << maxOutputIndex << std::endl;
                std::array<int, 2> coords;
                coords[0] = maxOutputIndex / 3;
                coords[1] = maxOutputIndex % 3;
                std::cout << "Computed coords: " << coords[0] << " " << coords[1] << std::endl;
                if(board[coords[0]][coords[1]] == empty) {
                    choice = coords;
                    choice[0] += 1;
                    choice[1] += 1;
                    break;
                }
                else {
                    outputs[maxOutputIndex] = -2;
                }
            }
        }
        else {
            choice = getChoice(xTurn, board);
        }
        std::cout << choice[0] << " " << choice[1] << std::endl;
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
        std::cout << "X wins!" << std::endl;
    }
    else if(winner == 2) {
        std::cout << "O wins!" << std::endl;
    }
    else if(winner == -1) {
        std::cout << "It's a tie!" << std::endl;
    }
    else {
        std::cout << "Error." << std::endl;
    }
}

TicTacToeGenome singleCrossover(TicTacToeGenome parentA, TicTacToeGenome parentB) {
    std::vector<std::vector<std::vector<double> > > aWeights = parentA.getSigWeights();
    std::vector<std::vector<std::vector<double> > > bWeights = parentB.getSigWeights();

    std::vector<std::vector<std::vector<double> > > childWeights;

    int totalGenes = 0;
    for(int i=0; i<int(aWeights.size()); ++i) {
        for(int j=0; j<int(aWeights[i].size()); ++j) {
            totalGenes += int(aWeights[i][j].size());
        }
    }
    int flipIndex = rand() % totalGenes;

    int currentGeneNumber = 0;
    for(int i=0; i<int(aWeights.size()); ++i) {
        childWeights.push_back(std::vector<std::vector<double> >());
        childWeights[i].reserve(int(aWeights[i].size()));
        for(int j=0; j<int(aWeights[i].size()); ++j) {
            childWeights[i].push_back(std::vector<double>());
            childWeights[i][j].reserve(int(aWeights[i][j].size()));
            for(int k=0; k<int(aWeights[i][j].size()); ++k) {
                if(currentGeneNumber < flipIndex) {
                    childWeights[i][j].push_back(aWeights[i][j][k]);
                }
                else {
                    childWeights[i][j].push_back(bWeights[i][j][k]);
                }

                ++currentGeneNumber;
            }
        }
    }

    return TicTacToeGenome(parentA.getSigIds(), childWeights);
}
void mutate(TicTacToeGenome &genome, double mutationChance) {
    std::vector<std::vector<std::vector<int> > > ids = genome.getSigIds();
    for(int i=0; i<int(ids.size()); ++i) {
        for(int j=0; j<int(ids[i].size()); ++j) {
            for(int k=0; k<int(ids[i][j].size()); ++k) {
                if(double(rand())/double(RAND_MAX) < mutationChance) {
                    genome.setWeight(i, j, k, double(rand())/double(RAND_MAX));
                }
            }
        }
    }
}
std::vector<TicTacToeGenome> breed(TicTacToeGenome parentA, TicTacToeGenome parentB, int numOffspring, double mutationChance) {
    std::vector<TicTacToeGenome> offspring;
    offspring.reserve(numOffspring);

    for(int i=0; i<numOffspring; ++i) {
        TicTacToeGenome child;

        if(double(rand())/double(RAND_MAX) >= 0.5) {
            child = singleCrossover(parentA, parentB);
        }
        else {
            child = singleCrossover(parentB, parentA);
        }

        mutate(child, mutationChance);
        offspring.push_back(child);
    }

    return offspring;
}
std::vector<TicTacToeGenome> computeNextGeneration(std::vector<TicTacToeGenome> gen, std::vector<int> indexes, int numPerGen, double mutationChance) {
    assert(int(indexes.size()) > 0);
    assert(((2*numPerGen) / int(indexes.size())) == (double(2*numPerGen) / double(indexes.size())));
    assert(int(indexes.size()) % 2 == 0);

    int offspringPerCouple = 2*(numPerGen / int(indexes.size()));

    std::vector<TicTacToeGenome> newGen;
    newGen.reserve(numPerGen);
    for(int i=0; i<int(indexes.size()); i+=2) {
        std::vector<TicTacToeGenome> newCreatures = breed(gen[indexes[i]], gen[indexes[i+1]], offspringPerCouple, mutationChance);
        for(int j=0; j<int(newCreatures.size()); ++j) {
            newGen.push_back(newCreatures[j]);
        }
    }

    return newGen;
}
void loadRandomCreatures(std::vector<TicTacToeGenome> &generation, int num) {
    for(int i=0; i<num; ++i) {
        generation.push_back(TicTacToeGenome());
        generation[i].randomize(int(networkArchitecture.size()), networkArchitecture);
    }
}
gameResult playMatch(TicTacToeGenome p1, TicTacToeGenome p2) {
    NeuralNetwork ai1 = p1.generateNN();
    NeuralNetwork ai2 = p2.generateNN();

    gameBoard board = emptyBoard();
    bool xTurn = rand() % 2;
    while(true) {
        std::array<int, 2> choice;
        std::vector<double> inputs = loadInputsFromBoard(board);
        std::vector<double> outputs;
        if(xTurn) {
            outputs = ai1.outputs(inputs);
        }
        else {
            outputs = ai2.outputs(inputs);
        }
        //We have weights. Now select the highest empty space.
        while(true) {
            double maxOutput = -1;
            int maxOutputIndex = -1;
            for(int i=0; i<int(outputs.size()); ++i) {
                if(outputs[i] > maxOutput) {
                    maxOutput = outputs[i];
                    maxOutputIndex = i;
                }
            }
            assert(maxOutputIndex != -1);
            std::array<int, 2> coords;
            coords[0] = maxOutputIndex / 3;
            coords[1] = maxOutputIndex % 3;
            if(board[coords[0]][coords[1]] == empty) {
                choice = coords;
                choice[0] += 1;
                choice[1] += 1;
                break;
            }
            else {
                outputs[maxOutputIndex] = -2;
            }
        }
        board[choice[0]-1][choice[1]-1] = (xTurn ? xPiece : oPiece);
        bool won = checkForWin(board, xTurn);
        if(won) {
            return (xTurn ? win : loss);
        }
        else {
            bool tied = checkForTie(board);
            if(tied) {
                return tie;
            }
        }

        xTurn = !xTurn;
    }
}
std::vector<double> compete(std::vector<TicTacToeGenome> generation, int matchesPerPair) {
    // Return, for each creature, its fitness (100% - lossChance for now)

    std::vector<int> wins;
    std::vector<int> losses;
    std::vector<int> ties;
    std::vector<int> games;
    for(int i=0; i<int(generation.size()); ++i) {
        wins.push_back(0);
        losses.push_back(0);
        ties.push_back(0);
        games.push_back(0);
    }

    for(int i=0; i<int(generation.size()); ++i) {
        for(int j=i+1; j<int(generation.size()); ++j) {
            for(int k=0; k<matchesPerPair; ++k) {
                gameResult res = playMatch(generation[i], generation[j]);
                if(res == win) {
                    ++wins[i];
                    ++losses[j];
                }
                else if(res == loss) {
                    ++losses[i];
                    ++wins[j];
                }
                else {
                    ++ties[i];
                    ++ties[j];
                }
                ++games[i];
                ++games[j];
            }
        }
    }

    std::vector<double> fitness;
    for(int i=0; i<int(generation.size()); ++i) {
        double lossChance = double(losses[i])/double(games[i]);
        fitness.push_back(1.0 - lossChance);
    }
    return fitness;
}
std::vector<int> selectBest(std::vector<double> fitness, int num) {
    assert(num <= int(fitness.size()));
    std::vector<int> indexes;
    indexes.reserve(num);
    while(int(indexes.size()) < num) {
        int maxIndex = -1;
        double maxFitness = -1;
        for(int i=0; i<int(fitness.size()); ++i) {
            if(fitness[i] > maxFitness) {
                maxFitness = fitness[i];
                maxIndex = i;
            }
        }
        indexes.push_back(maxIndex);
        fitness[maxIndex] = -1;
    }
    return indexes;
}

int main() {
    srand(time(NULL));

    // Final version
    //Learning stage
    std::vector<TicTacToeGenome> currentGeneration;
    currentGeneration.reserve(creaturesPerGeneration);
    loadRandomCreatures(currentGeneration, creaturesPerGeneration);
    std::vector<double> fitness = compete(currentGeneration, matchesPerCreaturePair);
    std::vector<int> bestIndexes = selectBest(fitness, numToBreed);
    for(int i=0; i<numGenerations; ++i) {
        std::cout << "Generation " << i << std::endl;
        //currentGeneration[0].printGenome();

        currentGeneration = computeNextGeneration(currentGeneration, bestIndexes, creaturesPerGeneration, mutationChance);
        fitness = compete(currentGeneration, matchesPerCreaturePair);
        bestIndexes = selectBest(fitness, numToBreed);
        std::cout << bestIndexes[0];
        for(int i=1; i<int(bestIndexes.size()); ++i) {
            if(bestIndexes[i] < 10) {
                std::cout << " ";
            }
            std::cout << " " << bestIndexes[i];
        }
        std::cout << std::endl;
    }
    NeuralNetwork testNetwork = currentGeneration[bestIndexes[0]].generateNN();

    while(true) {
        gameBoard board = emptyBoard();
        int winner = 0; //1 for x, 2 for o, -1 for tie.
        std::cout << "Do you want to go first? (y/n) ";
        std::string uin;
        std::cin >> uin;
        assert(uin == "y" || uin == "n");
        bool compGoesFirst = (uin == "n");
        aiVsPlayerGameLoop(board, winner, testNetwork, compGoesFirst);

        std::cout << std::endl << std::endl << std::endl;
        currentGeneration[bestIndexes[0]].printGenome();
        std::cout << "Best ai" << std::endl;

        printWinner(board, winner);
    }

    // // Play against a random AI
    // TicTacToeGenome testGenome;
    // testGenome.randomize(3, std::vector<int>{18, 18, 9});
    // testGenome.printGenome();
    // NeuralNetwork testNetwork = testGenome.generateNN();
    // std::vector<double> inputs = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // std::vector<double> outputs = testNetwork.outputs(inputs);

    // gameBoard board = emptyBoard();
    // int winner = 0; //1 for x, 2 for o, -1 for tie.
    // std::cout << "Do you want to go first? (y/n) ";
    // std::string uin;
    // std::cin >> uin;
    // assert(uin == "y" || uin == "n");
    // bool compGoesFirst = (uin == "n");
    // aiVsPlayerGameLoop(board, winner, testNetwork, compGoesFirst);
    // printWinner(board, winner);

    // // Hotseat tic-tac-toe for two players on one computer
    // gameBoard board = emptyBoard();
    // int winner = 0; //1 for x, 2 for o, -1 for tie.
    // hotseatGameLoop(board, winner);
    // printWinner(board, winner);

    // // Random tic-tac-toe neural network genome and evaluation of an empty board.
    // TicTacToeGenome testGenome;
    // testGenome.randomize(3, std::vector<int>{18, 18, 9});
    // testGenome.printGenome();
    // NeuralNetwork testNetwork = testGenome.generateNN();
    // std::vector<double> inputs = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    // std::vector<double> outputs = testNetwork.outputs(inputs);

    // // Sample tic-tac-toe neural network genome
    // std::vector<NeuralNetworkLayer> ls;
    // NeuralNetworkLayer l1; //Inputs
    // NeuralNetworkLayer l2; //Hidden
    // NeuralNetworkLayer l3; //Outputs

    // //Useful things to have
    // std::vector<int> all18;
    // std::vector<double> all1; 
    // for(int i=0; i<18; ++i) {
    //     all18.push_back(i);
    //     all1.push_back(1.0);
    // }
    // std::function<double(double)> rectify = [](double x){ return log(1+exp(x)); };

    // for(int i=0; i<18; ++i) {
    //     std::vector<int> inputIds = {i};
    //     std::vector<double> inputWeights = {1};
    //     Neuron neuron(inputIds, inputWeights, rectify);
    //     l1.addNeuron(neuron);
    // }

    // for(int i=0; i<18; ++i) {
    //     std::vector<int> inputIds = all18;
    //     std::vector<double> inputWeights = all1;
    //     Neuron neuron(inputIds, inputWeights, rectify);
    //     l2.addNeuron(neuron);
    // }

    // for(int i=0; i<9; ++i) {
    //     std::vector<int> inputIds = all18;
    //     std::vector<double> inputWeights = all1;
    //     Neuron neuron(inputIds, inputWeights, rectify);
    //     l3.addNeuron(neuron);
    // }

    // ls.push_back(l1);
    // ls.push_back(l2);
    // ls.push_back(l3);
    // NeuralNetwork network(ls);

    // std::vector<double> inputs = all1;
    // std::vector<double> outputs = network.outputs(inputs);
    // std::cout << std::endl << "Output:" << std::endl;
    // for(int i=0; i<int(outputs.size()); ++i) {
    //     std::cout << outputs[i] << std::endl;
    // }


    // // Neural network test
    // std::vector<int> nIds1 = {0, 1, 2, 3};
    // std::vector<int> nIds2 = {0, 1, 2, 3};
    // std::vector<int> nIds3 = {0, 3};

    // std::vector<double> nWeights1 = {1, 1, 1, 1};
    // std::vector<double> nWeights2 = {0.5, 0.5, 0.5, 0.5};
    // std::vector<double> nWeights3 = {0.67, 0.33};

    // std::function<double(double)> func = [](double x){ return (x>0 ? x : 0); };

    // Neuron n1(nIds1, nWeights1, func);
    // Neuron n2(nIds2, nWeights2, func);
    // Neuron n3(nIds3, nWeights3, func);

    // std::vector<Neuron> ns = {n1, n2, n3};

    // NeuralNetworkLayer layer1(ns);

    // std::vector<int> l2NIDs = {0, 1, 2};
    // std::vector<double> l2NWeights = {1, 1, 0.25};

    // Neuron l2N(l2NIDs, l2NWeights, func);
    // std::vector<Neuron> l2 = {l2N};
    // NeuralNetworkLayer layer2(l2);

    // std::vector<NeuralNetworkLayer> layers = {layer1, layer2};
    // NeuralNetwork network(layers);

    // std::vector<double> inputs = {1, 2, 3, 4};
    // std::vector<double> outputs = network.outputs(inputs);

    // // Hotseat tic-tac-toe for two players on one computer
    // gameBoard board = emptyBoard();
    // int winner = 0; //1 for x, 2 for o, -1 for tie.
    // hotseatGameLoop(board, winner);
    // printWinner(board, winner);

    return 0;
}