/*
  ==============================================================================

    JoystickProcessor.h
    Created: 20 Apr 2025 5:00:46pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"


class JoystickProcessor : public ModuleProcessor {
public:
    JoystickProcessor(int id) : ModuleProcessor(id, 0, 2) {
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        xSmooth.reset(sampleRate, 0.02);
        xSmooth.setCurrentAndTargetValue(x);

        ySmooth.reset(sampleRate, 0.02);
        ySmooth.setCurrentAndTargetValue(y);
    }

    void updateFromState(ModuleState moduleState) {
        auto newX = moduleState.state.getProperty("joyX");
        auto newY = moduleState.state.getProperty("joyY");
        if (!newX.isVoid() && !newY.isVoid()) {
            x = newX;
            y = newY;
        }
    }

    void processSample() {
        xSmooth.setTargetValue(x);
        ySmooth.setTargetValue(y);
        getCVOutputJack(0)->write(xSmooth.getNextValue());
        getCVOutputJack(1)->write(ySmooth.getNextValue());
    }

private:
    float x = 0.0; 
    float y = 0.0;
    SmoothedValue<float, ValueSmoothingTypes::Linear> xSmooth, ySmooth;
};

