/*
  ==============================================================================

    DistortionProcessor.h
    Created: 1 May 2025 5:43:03pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"
#include "Distortion.h"

class DistortionProcessor : public ModuleProcessor {
public:

    DistortionProcessor(int id) : ModuleProcessor(id, 1, 1) {
        gain = 1.0;
        color = 1600.0;
        dryWet = 0.5;
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBlock);

        juce::dsp::ProcessSpec spec;
        spec.maximumBlockSize = 1;
        spec.sampleRate = sampleRate;
        spec.numChannels = 1;

        buffer = AudioBuffer<float>(1, 1);
        audioBlock = juce::dsp::AudioBlock<float>(buffer);

        distortion.prepare(spec);
    }

    void updateFromState(ModuleState moduleState) {
        auto newGain = moduleState.state["gain"];
        if (!newGain.isVoid()) {
            gain = newGain;
        }
        auto newColor = moduleState.state["color"];
        if (!newColor.isVoid()) {
            color = newColor;
        }
        auto newDryWet = moduleState.state["dryWet"];
        if (!newDryWet.isVoid()) {
            dryWet = newDryWet;
        }
        distortion.setParams(gain, color, dryWet);
    }

    void processSample() {
        buffer.setSample(0, 0, getCVInputJack(0)->read());
        distortion.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        getCVOutputJack(0)->write(buffer.getSample(0, 0));
    }

private:
    Distortion distortion;
    double gain, color, dryWet;

    AudioBuffer<float> buffer;
    juce::dsp::AudioBlock<float> audioBlock;
};

