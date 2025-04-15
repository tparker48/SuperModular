/*
  ==============================================================================

    Module.h
    Created: 14 Apr 2025 9:27:46pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModuleState.h"

class CVInputJack {
public:
    float read() {
        if (internalPtr) {
            return *internalPtr;
        }
        else {
            return 0.0;
        }
    }
    void wirePtr(float* ptr) {
        internalPtr = ptr;
    }
    void clearPtr() {
        internalPtr = nullptr;
    }
private:
    float* internalPtr = nullptr;
};

class CVOutputJack {
public:
    void write(float val) {
        cvValue = val;
    }
    float* getPtr() {
        return &cvValue;
    }
private:
    float cvValue = 0.0;
};

class Module {
public:
    static const int typeId = -1;

    Module(int id): id(id), blockSize(-1), sampleRate(-1.0) {}

    void prepareToPlay(double sampleRate, int samplesPerBlock) {
        this->sampleRate = sampleRate;
        blockSize = samplesPerBlock;
    }
    
    virtual void processSample() = 0; // must implement!

    int getId() { return id; }

    CVInputJack* getCVInputJack(int inputId) {
        if(inputId < cvInputs.size()) {
            return &cvInputs[inputId];
        }
        else {
            return nullptr;
        }
    }

    CVOutputJack* getCVOutputJack(int outputId) {
        if (outputId < cvOutputs.size()) {
            return &cvOutputs[outputId];
        }
        else {
            return nullptr;
        }

    }

    void setCVInputJack(int inputId, CVOutputJack* output) {
        if (inputId < cvInputs.size()) {
            cvInputs[inputId].wirePtr(output->getPtr());
        }
    }

protected:
    std::vector<CVInputJack> cvInputs;
    std::vector<CVOutputJack> cvOutputs;
    double sampleRate;
    int blockSize;
private:
    int id = -1;
};

class AudioOutputModule : public Module {
public:
    static const int typeId = 0;

    AudioOutputModule(int id) : Module(id) {
        // Left channel input
        cvInputs.push_back(CVInputJack());
        // Right channel input
        cvInputs.push_back(CVInputJack());
    }

    void prepareToPlay(double sampleRate, int samplesPerBuffer) {
        Module::prepareToPlay(sampleRate, samplesPerBuffer);
        internalBuffer.setSize(2, samplesPerBuffer);
        writeHead = 0;
    }

    void processSample() {
        internalBuffer.setSample(0, writeHead, cvInputs[0].read());
        internalBuffer.setSample(1, writeHead, cvInputs[1].read());
        writeHead++;
    }

    void processBlock(AudioBuffer<float>& buffer) {
        writeHead = 0;


        if (buffer.getNumChannels() == 1) {
            // add L and R to mono channel
            buffer.copyFrom(0, 0, internalBuffer.getReadPointer(0), internalBuffer.getNumSamples(), 0.5);
            buffer.copyFrom(0, 0, internalBuffer.getReadPointer(1), internalBuffer.getNumSamples(), 0.5);
        }
        else {
            auto test = buffer.getNumSamples();
            buffer.copyFrom(0, 0, internalBuffer.getReadPointer(0), internalBuffer.getNumSamples(), 1.0);
            buffer.copyFrom(1, 0, internalBuffer.getReadPointer(1), internalBuffer.getNumSamples(), 1.0);
        }
    }

public:
    AudioBuffer<float> internalBuffer;
    int writeHead;
};

class OscillatorModule : public Module {
public:
    static const int typeId = 1;

    OscillatorModule(int id) : Module(id) {
        cvInputs.push_back(CVInputJack());
        cvOutputs.push_back(CVOutputJack());
    }

    void prepareToPlay(double sampleRate, int samplesPerBlock, float hz) {
        Module::prepareToPlay(sampleRate, samplesPerBlock);
        this->hz = hz;// 440.0;
        phase = 0.0;
        phaseIncrement = (hz * juce::MathConstants<float>::twoPi) / sampleRate;
    }

    void processSample() {
        cvOutputs[0].write(sin(phase));

        if (phase >= juce::MathConstants<float>::twoPi) {
            phase -= juce::MathConstants<float>::twoPi;
        }

        auto modVal = cvInputs[0].read();
        phase += phaseIncrement + (modVal*0.1);
    }
private:
    float hz;
    float phase, phaseIncrement;
};