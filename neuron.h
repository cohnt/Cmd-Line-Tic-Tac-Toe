#ifndef NEURON_H
#define NEURON_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <array>
#include <math.h>
#include <cassert>
#include <functional>

class Neuron {
private:
    int numSigs;
    std::vector<int> sigIDs;
    std::vector<double> sigWeights;
    std::function<double(double)> phi;
    double sum(std::vector<double> sigVals) {
        assert(described);

        double total = 0;
        for(int i=0; i<int(sigIDs.size()); ++i) {
            total += sigVals[i] * sigWeights[i];
        }
        return total;
    }
    bool described;

public:
    Neuron() {
        //This is stupid. Don't use this.
        described = false;
    }
    Neuron(std::vector<int> signalIDs, std::vector<double> signalWeights) {
        assert(signalIDs.size() == signalWeights.size());

        sigIDs.reserve(int(signalIDs.size()));
        sigWeights.reserve(int(signalWeights.size()));

        for(int i=0; i<int(signalIDs.size()); ++i) {
            sigIDs.push_back(signalIDs[i]);
            sigWeights.push_back(signalWeights[i]);
        }

        phi = [](double x){ return log(1+exp(x)); }; //The (Continuous) Rectifier function is default.
    }
    Neuron(std::vector<int> signalIDs, std::vector<double> signalWeights, std::function<double(double)> transFunc) {
        assert(signalIDs.size() == signalWeights.size());

        sigIDs.reserve(int(signalIDs.size()));
        sigWeights.reserve(int(signalWeights.size()));

        for(int i=0; i<int(signalIDs.size()); ++i) {
            sigIDs.push_back(signalIDs[i]);
            sigWeights.push_back(signalWeights[i]);
        }

        phi = transFunc;

        described = true;
    }

    void newSig(int id, double weight) {
        described = true;

        sigIDs.push_back(id);
        sigWeights.push_back(weight);
    }
    bool delSig(int id) {
        for(int i=0; i<int(sigIDs.size()); ++i) {
            if(sigIDs[i] == id) {
                sigIDs.erase(sigIDs.begin() + i);
                sigWeights.erase(sigWeights.begin() + i);
                if(sigIDs.size() == 0u) {
                    described = false;
                }
                return true;
            }
        }
        return false; //Not in the list of signal ids.
    }
    std::vector<int> returnSigIds() {
        //
        return sigIDs;
    }
    void newPhi(std::function<double(double)> newFunc) {
        phi = newFunc;
        if(sigIDs.size() != 0u) {
            described = true;
        }
    }
    double output(std::vector<double> sigVals) {
        assert(described);
        assert(sigVals.size() == sigIDs.size());

        double total = Neuron::sum(sigVals);
        return phi(total);
    }
};

#endif