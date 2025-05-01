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
    AudioOutputProcessor(int id) : ModuleProcessor(id, 3, 0) {
    }


    void prepareToPlay(double sampleRate, int samplesPerBuffer) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBuffer);
        internalBuffer.setSize(2, samplesPerBuffer);
        writeHead = 0;

        gainSmooth.reset(sampleRate, 0.01);
        gainSmooth.setCurrentAndTargetValue(gain);
    }

    void updateFromState(ModuleState moduleState) {
        auto newGain = moduleState.state.getProperty("gain");
        if (!newGain.isVoid()) {
            gain = newGain;
        }
    }

    void processSample() {

        if (cvInputs[2].isConnected()) {
            internalBuffer.setSample(0, writeHead, cvInputs[2].read());
            internalBuffer.setSample(1, writeHead, cvInputs[2].read());
        }
        else {
            internalBuffer.setSample(0, writeHead, cvInputs[0].read());
            internalBuffer.setSample(1, writeHead, cvInputs[1].read());
        }
        writeHead++;
    }

    void processBlock(AudioBuffer<float>& buffer) {
        writeHead = 0;

        auto lastGain = gainSmooth.getCurrentValue();
        gainSmooth.setTargetValue(gain);
        gainSmooth.skip(blockSize);
        auto nextGain = gainSmooth.getNextValue();

        if (buffer.getNumChannels() == 1) {
            buffer.copyFrom(0, 0, internalBuffer.getReadPointer(0), internalBuffer.getNumSamples(), 0.5);
            buffer.copyFrom(0, 0, internalBuffer.getReadPointer(1), internalBuffer.getNumSamples(), 0.5);
        }
        else {
            buffer.copyFrom(0, 0, internalBuffer.getReadPointer(0), internalBuffer.getNumSamples(), 1.0);
            buffer.copyFrom(1, 0, internalBuffer.getReadPointer(1), internalBuffer.getNumSamples(), 1.0);
        }
        buffer.applyGainRamp(0, buffer.getNumSamples(), lastGain, nextGain);
    }

private:
    AudioBuffer<float> internalBuffer;
    double gain = 1.0;
    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> gainSmooth;
    int writeHead = 0;
};