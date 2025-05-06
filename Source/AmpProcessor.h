/*
  ==============================================================================

    AmpProcessor.h
    Created: 5 May 2025 9:28:15pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"

class AmpProcessor : public ModuleProcessor {
public:
    AmpProcessor(int id) : ModuleProcessor(id, 2, 1) {}

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBlock);
        gain = 1.0;
        gainSmooth.reset(sampleRate, 0.02);
    }

    void updateFromState(ModuleState moduleState) {
        auto newGain = moduleState.state.getProperty("gain");
        if (!newGain.isVoid()) {
            gain = newGain;
            gainSmooth.setTargetValue(gain);
        }

        auto newModAmt = moduleState.state.getProperty("modAmt");
        if (!newModAmt.isVoid()) {
            modAmt = newModAmt;
        }
    }

    void processSample() {
        effectiveGain = gainSmooth.getNextValue();
        effectiveGain = effectiveGain + (effectiveGain * modAmt *getCVInputJack(1)->read());
        effectiveGain = std::min(1.0f, effectiveGain);
        getCVOutputJack(0)->write(getCVInputJack(0)->read() * effectiveGain);
    }

private:
    float gain = 1.0;
    float modAmt = 0.3;
    float effectiveGain;
    SmoothedValue<float, ValueSmoothingTypes::Multiplicative> gainSmooth;
};
