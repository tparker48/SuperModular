#pragma once
#pragma once
#include <JuceHeader.h>

class Distortion {
public:
    Distortion() { }
    void prepare(const juce::dsp::ProcessSpec& spec);
    void setParams(float gain, float color, float dryWet);
    void process(juce::dsp::ProcessContextReplacing<float> context);

private:
    bool isPrepared = false;

    enum
    {
        bpfIdx,
        preGainIdx,
        waveshaperIdx,
        postGainIdx,
    };

    juce::SmoothedValue<float> smoothedGain;

    juce::dsp::ProcessorChain<
        juce::dsp::StateVariableTPTFilter<float>,
        juce::dsp::Gain<float>,
        juce::dsp::WaveShaper<float>,
        juce::dsp::Gain<float>> processorChain;
    
    juce::dsp::DryWetMixer<float> dryWet;
};
