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
        getCVOutputJack(0)->write(x);
        getCVOutputJack(1)->write(y);
    }

private:
    float x, y;
};

