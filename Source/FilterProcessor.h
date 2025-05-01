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
        drive = 0.0;
    }


    void prepareToPlay(double sampleRate, int samplesPerBuffer) override {
        ModuleProcessor::prepareToPlay(sampleRate, samplesPerBuffer);

        juce::dsp::ProcessSpec spec;
        spec.maximumBlockSize = samplesPerBuffer;
        spec.sampleRate = sampleRate;
        spec.numChannels = 1;

        lpf.prepare(spec);
        lpf.setMode(juce::dsp::LadderFilterMode::LPF24);
        lpf.setEnabled(true);

        hpf.prepare(spec);
        hpf.setMode(juce::dsp::LadderFilterMode::HPF24);
        hpf.setEnabled(true);

        hzSmooth.reset(sampleRate, 0.01);
        hzSmooth.setCurrentAndTargetValue(hz);
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

    void processSample() {
        hzSmooth.setTargetValue(hz);
        auto smoothedHz = hzSmooth.getNextValue();

        lpf.setCutoffFrequencyHz(smoothedHz);
        lpf.setResonance(q);
        lpf.setDrive(drive);

        hpf.setCutoffFrequencyHz(smoothedHz);
        hpf.setResonance(q);
        hpf.setDrive(drive);

        auto buffer = AudioBuffer<float>(1, 1);
        juce::dsp::AudioBlock<float> audioBlock(buffer);

        auto in = getCVInputJack(0)->read();

        buffer.setSample(0, 0, in);
        lpf.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        getCVOutputJack(0)->write(buffer.getSample(0, 0));

        buffer.setSample(0, 0, in);
        hpf.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
        getCVOutputJack(1)->write(buffer.getSample(0, 0));
    }

private:
    juce::dsp::LadderFilter<float> lpf, hpf;
    SmoothedValue<double, ValueSmoothingTypes::Multiplicative> hzSmooth;
    double hz, q, drive;
};