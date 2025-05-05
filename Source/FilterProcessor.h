/*
  ==============================================================================

    FilterProcessor.h
    Created: 29 Apr 2025 11:48:28pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include "ModuleProcessor.h"
#include <JuceHeader.h>


class FilterProcessor : public ModuleProcessor {
public:
    FilterProcessor(int id) : ModuleProcessor(id, 2, 2) {
        hz = 4000.0;
        q = 0.0;
        drive = 1.0;
    }

    void prepareToPlay(double sampleRate, int samplesPerBuffer) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBuffer);

        juce::dsp::ProcessSpec spec;
        spec.maximumBlockSize = 1;
        spec.sampleRate = sampleRate;
        spec.numChannels = 1;

        lpf.prepare(spec);
        lpf.setMode(juce::dsp::LadderFilterMode::LPF24);
        lpf.setEnabled(true);

        hpf.prepare(spec);
        hpf.setMode(juce::dsp::LadderFilterMode::HPF24);
        hpf.setEnabled(true);

        hzSmooth.reset(sampleRate, 0.02);
        hzSmooth.setCurrentAndTargetValue(hz);

        buffer = AudioBuffer<float>(1, 1);
        audioBlock = juce::dsp::AudioBlock<float>(buffer);
    }

    void updateFromState(ModuleState moduleState) {
        auto newHz = moduleState.state["hz"];
        if (!newHz.isVoid()) {
            hz = newHz;
        }
        auto newQ = moduleState.state["q"];
        if (!newQ.isVoid()) {
            q = newQ;
        }
        auto newDrive = moduleState.state["drive"];
        if (!newDrive.isVoid()) {
            drive = newDrive;
        }

        auto newMode = moduleState.state["mode"];
        if (!newMode.isVoid()) {
            lpfMode = newMode;
        }
    }

    void preBlockProcessing() override {
        hzSmooth.setTargetValue(hz);
        hzSmooth.skip(blockSize);
        smoothedHzVal = hzSmooth.getCurrentValue();
        smoothedHzx8 = smoothedHzVal * 0.8;
    }

    void processSample() {
        if (!getCVInputJack(0)->isConnected()) {
            getCVOutputJack(0)->write(0.0);
            return;
        }

        auto modulatedHz = smoothedHzVal + (smoothedHzx8 * getCVInputJack(1)->read());

        lpf.setCutoffFrequencyHz(modulatedHz);
        lpf.setResonance(q);
        lpf.setDrive(drive);

        hpf.setCutoffFrequencyHz(modulatedHz);
        hpf.setResonance(q);
        hpf.setDrive(drive);

        auto in = getCVInputJack(0)->read();
        buffer.setSample(0, 0, in);

        if (lpfMode) {
            lpf.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
            getCVOutputJack(0)->write(buffer.getSample(0, 0));
        }
        else {
            hpf.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
            getCVOutputJack(1)->write(buffer.getSample(0, 0));
        }
    }

private:
    bool lpfMode = true;
    juce::dsp::LadderFilter<float> lpf, hpf;
    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> hzSmooth;
    double hz, q, drive;
    double smoothedHzx8, smoothedHzVal;
    AudioBuffer<float> buffer;
    juce::dsp::AudioBlock<float> audioBlock;
};