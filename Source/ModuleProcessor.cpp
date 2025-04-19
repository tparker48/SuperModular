/*
  ==============================================================================

    Module.cpp
    Created: 14 Apr 2025 9:27:46pm
    Author:  Tom

  ==============================================================================
*/

#include "ModuleProcessor.h"

ModuleProcessor::ModuleProcessor(int id, int numCvInputs, int numCvOutputs)
    : id(id), blockSize(-1), sampleRate(-1.0) {
    for (int i = 0; i < numCvInputs; i++) {
        cvInputs.push_back(CVInputJack());
    }
    for (int i = 0; i < numCvOutputs; i++) {
        cvOutputs.push_back(CVOutputJack());
    }
}

void ModuleProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    this->sampleRate = sampleRate;
    blockSize = samplesPerBlock;
}

CVInputJack* ModuleProcessor::getCVInputJack(int inputId) {
    if(inputId < cvInputs.size() && inputId >= 0) {
        return &cvInputs[inputId];
    }
    else {
        return nullptr;
    }
}

CVOutputJack* ModuleProcessor::getCVOutputJack(int outputId) {
    if (outputId < cvOutputs.size() && outputId >= 0) {
        return &cvOutputs[outputId];
    }
    else {
        return nullptr;
    }

}

void ModuleProcessor::setCVInputJack(int inputId, CVOutputJack* output) {
    if (inputId < cvInputs.size() && inputId >= 0) {
        cvInputs[inputId].wirePtr(output->getPtr());
    }
}
