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
        //This is stupid. Don't use this.
        described = false;
    }
    NeuralNetworkLayer(std::vector<Neuron> neurons) {
        described = true;

        nodes.reserve(int(neurons.size()));
        for(int i=0; i<int(neurons.size()); ++i) {
            nodes.push_back(neurons[i]);
        }
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

        for(int i=0; i<int(res.size()); ++i) {
            std::cout << res[i] << std::endl;
        }

        return res;
    }
};

class NeuralNetwork {
private:
    //
public:
    //
};

#endif