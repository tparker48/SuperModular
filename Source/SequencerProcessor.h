/*
  ==============================================================================

    SequencerProcessor.h
    Created: 26 Apr 2025 5:32:23pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"


class SequencerProcessor : public ModuleProcessor {
public:
    SequencerProcessor(int id) : ModuleProcessor(id, 1, 1) {
        rate = 1.0;
        position = 0.0;
        numSteps = MAX_STEPS;
        for (int i = 0; i < MAX_STEPS; i++) {
            controlSteps[i] = 0.0;
        }
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBlock);
    }

    void updateFromState(ModuleState moduleState) {
        auto newNumSteps = moduleState.state.getProperty("steps");
        if (!newNumSteps.isVoid()) {
            numSteps = newNumSteps;
        }
        auto newRate = moduleState.state.getProperty("rate");
        if (!newRate.isVoid()) {
            rate = ((double)newRate) / sampleRate;
        }
        for (int i = 0; i < MAX_STEPS; i++) {
            auto newStepLevel = moduleState.state.getProperty(Identifier("step"+std::to_string(i)));
            if (!newStepLevel.isVoid()) {
                controlSteps[i] = newStepLevel;
            }
        }
    }

    void processSample() {
        if (position >= 1.0) {
            position -= 1.0;
        }
        
        if (getCVInputJack(0)->read() >= 0.99){
            position = 0.0;
        }

        int posIdx = int(position * MAX_STEPS) % numSteps;
        getCVOutputJack(0)->write(controlSteps[posIdx]);
        position += rate;
    }

private:
    static const int MAX_STEPS = 8;
    double controlSteps[MAX_STEPS];
    int numSteps;
    double position, rate;
};

