#ifndef TIC_TAC_TOE_GENOME
#define TIC_TAC_TOE_GENOME

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

class TicTacToeGenome {
private:
    std::vector<std::vector<std::vector<int> > > sigIds;
    std::vector<std::vector<std::vector<double> > > sigWeights;
    bool described;

public:
    TicTacToeGenome() {
        //
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
    NeuralNetwork generateNN() {
        assert(sigIds.size() > 1u);
        assert(sigIds[0].size() == 18u);
        assert(sigIds.back().size() == 9u);
        //May need more later?

        std::vector<NeuralNetworkLayer> layers;
        std::function<double(double)> transFunction = [](double x){ return log(1+exp(x)); };
        for(int i=0; i<int(sigIds.size()); ++i) {
            std::vector<Neuron> neurons;
            for(int j=0; j<int(sigIds[i].size()); ++j) {
                std::vector<int> ids;
                std::vector<double> weights;
                ids.reserve(int(sigIds[i][j].size()));
                weights.reserve(int(sigIds[i][j].size()));
                for(int k=0; k<int(sigIds[i][j].size()); ++k) {
                    ids.push_back(sigIds[i][j][k]);
                    weights.push_back(sigWeights[i][j][k]);
                }
                neurons.push_back(Neuron(ids, weights, transFunction));
            }
            layers.push_back(NeuralNetworkLayer(neurons));
        }
        return NeuralNetwork(layers);
    }
};

#endif