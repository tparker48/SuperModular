/*
  ==============================================================================

    MixerProcessor.h
    Created: 20 Apr 2025 7:30:15pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"


class MixerProcessor : public ModuleProcessor {
public:
    MixerProcessor(int id) : ModuleProcessor(id, 5, 1) {
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBlock);
        l1 = l2 = l3 = l4 = l5 = 1.0;
        sl1.reset(sampleRate, 0.02);
        sl2.reset(sampleRate, 0.02);
        sl3.reset(sampleRate, 0.02);
        sl4.reset(sampleRate, 0.02);
        sl5.reset(sampleRate, 0.02);
    }

    void updateFromState(ModuleState moduleState) {
        auto newL1 = moduleState.state.getProperty("l1");
        auto newL2 = moduleState.state.getProperty("l2");
        auto newL3 = moduleState.state.getProperty("l3");
        auto newL4 = moduleState.state.getProperty("l4");
        auto newL5 = moduleState.state.getProperty("l5");
        if (!newL1.isVoid()) {
            l1 = newL1;
        }
        if (!newL2.isVoid()) {
            l2 = newL2;
        }
        if (!newL3.isVoid()) {
            l3 = newL3;
        }
        if (!newL4.isVoid()) {
            l4 = newL4;
        }
        if (!newL5.isVoid()) {
            l5 = newL5;
        }
    }

    void processSample() {
        sl1.setTargetValue(l1);
        sl2.setTargetValue(l2);
        sl3.setTargetValue(l3);
        sl4.setTargetValue(l4);
        sl5.setTargetValue(l5);
        getCVOutputJack(0)->write(
            (sl1.getNextValue() * getCVInputJack(0)->read() +
             sl2.getNextValue() * getCVInputJack(1)->read() +
             sl3.getNextValue() * getCVInputJack(2)->read() +
             sl4.getNextValue() * getCVInputJack(3)->read() +
             sl5.getNextValue() * getCVInputJack(4)->read()));
    }

private:
    double l1, l2, l3, l4, l5;
    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> sl1, sl2, sl3, sl4, sl5;
};

