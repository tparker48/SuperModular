/*
  ==============================================================================

    AudioInputProcessor.h
    Created: 7 May 2025 11:03:06pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"


class AudioInputProcessor : public ModuleProcessor {
public:
    AudioInputProcessor(int id) : ModuleProcessor(id, 0, 3) {
    }


    void prepareToPlay(double sampleRate, int samplesPerBuffer) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBuffer);
        internalBuffer.setSize(2, samplesPerBuffer);
        readHead = 0;

    }

    void updateFromState(ModuleState moduleState) {
    }

    void processSample() {
        auto l = internalBuffer.getSample(0, readHead);
        auto r = internalBuffer.getSample(1, readHead);
        cvOutputs[0].write(l);
        cvOutputs[1].write(r);
        cvOutputs[2].write((l+r)*0.5);
        readHead++;
    }

    void processBlock(AudioBuffer<float>& buffer) {
        readHead = 0;
        mono = (buffer.getNumChannels() == 1);

        if (mono) {
            internalBuffer.copyFrom(0,0, buffer.getReadPointer(0), buffer.getNumSamples(), 1.0);
            internalBuffer.copyFrom(1, 0, buffer.getReadPointer(0), buffer.getNumSamples(), 1.0);
        }
        else {
            internalBuffer.copyFrom(0, 0, buffer.getReadPointer(0), buffer.getNumSamples(), 1.0);
            internalBuffer.copyFrom(1, 0, buffer.getReadPointer(1), buffer.getNumSamples(), 1.0);
        }
    }

private:
    AudioBuffer<float> internalBuffer;
    int readHead = 0;
    bool mono = false;
};