/*
  ==============================================================================

    ModuleProcessor.h
    Created: 14 Apr 2025 9:27:46pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModuleState.h"

class CVInputJack {
public:
    float read() {
        if (isConnected()) {
            return *internalPtr;
        }
        else {
            return 0.0;
        }
    }
    void wirePtr(float* ptr) {
        internalPtr = ptr;
    }
    void clearPtr() {
        internalPtr = nullptr;
    }
    bool isConnected() {
        return internalPtr != nullptr;
    }
private:
    float* internalPtr = nullptr;
};


class CVOutputJack {
public:
    void write(float val) {
        cvValue = val;
    }
    float* getPtr() {
        return &cvValue;
    }
private:
    float cvValue = 0.0;
};


class ModuleProcessor {
public:
    ModuleProcessor(int id, int numCvInputs, int numCvOutputs);
    virtual ~ModuleProcessor() {}
    virtual void prepareToPlay(double sampleRate, int samplesPerBlock);

    virtual void updateFromState(ModuleState state) = 0;
    virtual void processSample() = 0;

    int getId() { return id; }
    void setCVInputJack(int inputId, CVOutputJack* output);
    CVInputJack* getCVInputJack(int inputId);
    CVOutputJack* getCVOutputJack(int outputId);

protected:
    std::vector<CVInputJack> cvInputs;
    std::vector<CVOutputJack> cvOutputs;
    double sampleRate;
    int blockSize;
private:
    int id = -1;
};



