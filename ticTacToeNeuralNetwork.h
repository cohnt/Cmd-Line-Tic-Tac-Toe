#ifndef TIC_TAC_TOE_NEURAL_NETWORK_H
#define TIC_TAC_TOE_NEURAL_NETWORK_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <array>
#include <math.h>
#include <cassert>
#include <functional>
#include <random>

#include "neuralNetwork.h"

class TicTacToeNeuralNetwork {
private:
    //
public:
    //
};

class TicTacToeGenome {
private:
    std::vector<std::vector<std::vector<int> > > sigIds;
    std::vector<std::vector<std::vector<double> > > sigWeights;
    bool described;
public:
    TicTacToeGenome() {
        described = false;
    }
    TicTacToeGenome(std::vector<std::vector<std::vector<int> > > ids, std::vector<std::vector<std::vector<double> > > weights) {
        assert(ids.size() == weights.size());
        for(int i=0; i<int(ids.size()); ++i) {
            assert(ids[i].size() == weights[i].size());
        }
        assert(ids[0].size() == 18u);
        assert(ids[int(ids.size()-1)].size() == 9u);

        for(int i=0; i<int(ids.size()); ++i) {
            sigIds.push_back(std::vector<std::vector<int> >());
            sigWeights.push_back(std::vector<std::vector<double> >());
            for(int j=0; j<int(ids[i].size()); ++j) {
                sigIds[i].push_back(ids[i][j]);
                sigWeights[i].push_back(weights[i][j]);
            }
        }
        described = true;
    }

    void randomize(int numLayers, std::vector<int> numNeurons) {
        sigIds.resize(0);
        sigWeights.resize(0);
        for(int i=0; i<numLayers; ++i) {
            std::vector<std::vector<int> > layerIds;
            std::vector<std::vector<double> > layerWeights;
            layerIds.reserve(numNeurons[i]);
            layerWeights.reserve(numNeurons[i]);
            for(int j=0; j<numNeurons[i]; ++j) {
                layerIds.push_back(std::vector<int>());
                layerWeights.push_back(std::vector<double>());
                int max = i > 0 ? numNeurons[i-1] : numNeurons[0];
                if(i == 0) {
                    layerIds[j].push_back(j);
                    layerWeights[j].push_back(1);
                }
                else {
                    for(int k=0; k<max; ++k) {
                        layerIds[j].push_back(k);
                        layerWeights[j].push_back(double(rand())/double(RAND_MAX));
                    }
                }
            }
            sigIds.push_back(layerIds);
            sigWeights.push_back(layerWeights);
        }
    }
    std::vector<std::vector<std::vector<int> > > getSigIds() {
        //
        return sigIds;
    }
    std::vector<std::vector<std::vector<double> > > getSigWeights() {
        //
        return sigWeights;
    }
    double getWeight(int layer, int neur, int id) {
        //
        return sigWeights[layer][neur][id];
    }
    void printGenome() {
        for(int i=0; i<int(sigIds.size()); ++i) {
            std::cout << "Layer " << i << ":" << std::endl;
            for(int j=0; j<int(sigIds[i].size()); ++j) {
                std::cout << "\tNeuron " << j << ":" << std::endl;
                for(int k=0; k<int(sigIds[i][j].size()); ++k) {
                    std::cout << "\t\t" << sigIds[i][j][k] << "->" << sigWeights[i][j][k] << std::endl;
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
    }
    // TicTacToeNeuralNetwork generateNN() {
    //     //
    // }
};

#endif