/*
  ==============================================================================

    DelayProcessor.h
    Created: 1 May 2025 5:42:45pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"
#include "Delay.h"


class DelayProcessor : public ModuleProcessor {
public:
    DelayProcessor(int id) : ModuleProcessor(id, 2, 2) {
    }


    void prepareToPlay(double sampleRate, int samplesPerBuffer) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBuffer);

        juce::dsp::ProcessSpec spec;
        spec.maximumBlockSize = 2;
        spec.sampleRate = sampleRate;
        spec.numChannels = 1;

        delay.prepare(spec);
        delay.setParams(0.3, 0.3, 0.5);

        buffer = AudioBuffer<float>(1, 1);
        audioBlock = juce::dsp::AudioBlock<float>(buffer);
    }

    void updateFromState(ModuleState moduleState) {
        auto newLength = moduleState.state.getProperty("length");
        auto newFeedback = moduleState.state.getProperty("feedback");
        auto newDryWet = moduleState.state.getProperty("dryWet");
        if (!newLength.isVoid() && !newFeedback.isVoid() && !newDryWet.isVoid()) {
            length = newLength;
            feedback = newFeedback;
            dryWet = newDryWet;
        }
    }

    void preBlockProcessing() override {
        lengthx3 = length * 0.3;
    }

    void processSample() {
        auto modulatedLength = length + (lengthx3 * getCVInputJack(0)->read());
        delay.setParams(
            modulatedLength,
            feedback,
            dryWet
        );
        
        buffer.setSample(0, 0, getCVInputJack(1)->read() * inputGain);
        delay.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        getCVOutputJack(0)->write(buffer.getSample(0, 0));
        getCVOutputJack(1)->write(delay.getLastWetSample());
    }


private:
    Delay delay;
    double length, feedback, dryWet;
    double lengthx3;
    float inputGain = 0.95;

    AudioBuffer<float> buffer;
    juce::dsp::AudioBlock<float> audioBlock;
};