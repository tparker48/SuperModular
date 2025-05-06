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
    }

    void preBlockProcessing() override {
        hzSmooth.setTargetValue(hz);
    }

    void processSample() {
        if (!getCVInputJack(0)->isConnected()) {
            getCVOutputJack(0)->write(0.0);
            return;
        }

        hzSmooth.skip(1);

        auto modulatedHz = hzSmooth.getCurrentValue() + (hzSmooth.getCurrentValue() * 0.8 * getCVInputJack(1)->read());

        lpf.setCutoffFrequencyHz(modulatedHz);
        lpf.setResonance(q);
        lpf.setDrive(drive);

        hpf.setCutoffFrequencyHz(modulatedHz);
        hpf.setResonance(q);
        hpf.setDrive(drive);

        buffer.setSample(0, 0, getCVInputJack(0)->read());
        lpf.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        getCVOutputJack(0)->write(buffer.getSample(0, 0));

        buffer.setSample(0, 0, getCVInputJack(0)->read());
        hpf.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        getCVOutputJack(1)->write(buffer.getSample(0, 0));
    }

private:
    double hz, q, drive;

    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> hzSmooth;

    juce::dsp::LadderFilter<float> lpf, hpf;
    AudioBuffer<float> buffer;
    juce::dsp::AudioBlock<float> audioBlock;
};