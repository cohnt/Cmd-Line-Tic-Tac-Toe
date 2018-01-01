#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <array>
#include <math.h>
#include <cassert>
#include <functional>

#include "neuron.h"

class NeuralNetworkLayer {
private:
    std::vector<Neuron> nodes;
    bool described;

public:
    NeuralNetworkLayer() {
        //
        described = false;
    }
    NeuralNetworkLayer(std::vector<Neuron> neurons) {
        described = true;

        nodes.reserve(int(neurons.size()));
        for(int i=0; i<int(neurons.size()); ++i) {
            nodes.push_back(neurons[i]);
        }
    }

    void addNeuron(Neuron n) {
        nodes.push_back(n);
        described = true;
    }
    std::vector<double> outputs(std::vector<double> sigVals) {
        assert(described);

        std::vector<double> res;
        for(int i=0; i<int(nodes.size()); ++i) {
            // std::cout << "Calculating output for neuron " << i << std::endl;

            std::vector<double> specificSignalVals;
            std::vector<int> specificSignalIds = nodes[i].returnSigIds();

            // std::cout << "Needed input signal ids:";
            // for(int j=0; j<int(specificSignalIds.size()); ++j) {
            //     std::cout << " " << specificSignalIds[j];
            // }
            // std::cout << std::endl;

            specificSignalVals.reserve(int(specificSignalIds.size()));
            for(int j=0; j<int(specificSignalIds.size()); ++j) {
                specificSignalVals.push_back(sigVals[specificSignalIds[j]]);
            }

            // std::cout << "Actual signal values:";
            // for(int j=0; j<int(specificSignalVals.size()); ++j) {
            //     std::cout << " " << specificSignalVals[j];
            // }
            // std::cout << std::endl;

            res.push_back(nodes[i].output(specificSignalVals));
        }

        // for(int i=0; i<int(res.size()); ++i) {
        //     std::cout << res[i] << std::endl;
        // }

        return res;
    }
};

class NeuralNetwork {
private:
    std::vector<NeuralNetworkLayer> layers;
    bool described;

public:
    NeuralNetwork() {
        //
        described = false;
    }
    NeuralNetwork(std::vector<NeuralNetworkLayer> nnls) {
        described = true;

        layers.reserve(int(nnls.size()));
        for(int i=0; i<int(nnls.size()); ++i) {
            layers.push_back(nnls[i]);
        }
    }

    void addLayer(NeuralNetworkLayer l) {
        layers.push_back(l);
        described = true;
    }
    std::vector<double> outputs(std::vector<double> inVals) {
        assert(described);

        std::vector<double> currentIn = inVals;
        std::vector<double> currentOut;
        for(int i=0; i<int(layers.size()); ++i) {
            // std::cout << std::endl << "Layer " << i << std::endl;
            currentOut = layers[i].outputs(currentIn);
            currentIn = currentOut;
        }
        return currentOut;
    }
};

#endif