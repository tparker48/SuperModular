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
        float totalConnections = (
            (int)getCVInputJack(0)->isConnected() +
            (int)getCVInputJack(1)->isConnected() +
            (int)getCVInputJack(2)->isConnected() +
            (int)getCVInputJack(3)->isConnected() +
            (int)getCVInputJack(4)->isConnected()
        );
        totalConnections = std::max(1.0f, totalConnections);
        getCVOutputJack(0)->write(
            (l1 * getCVInputJack(0)->read() +
             l2 * getCVInputJack(1)->read() +
             l3 * getCVInputJack(2)->read() +
             l4 * getCVInputJack(3)->read() +
             l5 * getCVInputJack(4)->read()) / totalConnections);
    }

private:
    double l1, l2, l3, l4, l5;
};

