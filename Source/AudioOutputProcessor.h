/*
  ==============================================================================

    AudioOutputProcessor.h
    Created: 16 Apr 2025 11:02:08pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"


class AudioOutputProcessor : public ModuleProcessor {
public:
    AudioOutputProcessor(int id) : ModuleProcessor(id, 2, 0) {
    }


    void prepareToPlay(double sampleRate, int samplesPerBuffer) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBuffer);
        internalBuffer.setSize(2, samplesPerBuffer);
        writeHead = 0;
    }

    void updateFromState(ModuleState moduleState) {
        auto newGain = moduleState.state.getProperty("gain");
        if (!newGain.isVoid()) {
            gain = newGain;
        }
    }

    void processSample() {
        internalBuffer.setSample(0, writeHead, cvInputs[0].read());
        internalBuffer.setSample(1, writeHead, cvInputs[1].read());
        writeHead++;
    }

    void processBlock(AudioBuffer<float>& buffer) {
        writeHead = 0;
        if (buffer.getNumChannels() == 1) {
            buffer.copyFrom(0, 0, internalBuffer.getReadPointer(0), internalBuffer.getNumSamples(), 0.5 * gain);
            buffer.copyFrom(0, 0, internalBuffer.getReadPointer(1), internalBuffer.getNumSamples(), 0.5 * gain);
        }
        else {
            auto test = buffer.getNumSamples();
            buffer.copyFrom(0, 0, internalBuffer.getReadPointer(0), internalBuffer.getNumSamples(), 1.0 * gain);
            buffer.copyFrom(1, 0, internalBuffer.getReadPointer(1), internalBuffer.getNumSamples(), 1.0 * gain);
        }
    }

public:
    AudioBuffer<float> internalBuffer;
    float gain = 1.0;
    int writeHead = 0;
};