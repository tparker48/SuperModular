#include "Distortion.h"

void Distortion::prepare(const juce::dsp::ProcessSpec& spec)
{
    auto& bpf = processorChain.template get<bpfIdx>();
    bpf.setType(juce::dsp::StateVariableTPTFilterType::bandpass);
    bpf.setResonance(0.25f);

    auto& waveshaper = processorChain.template get<waveshaperIdx>();
    waveshaper.functionToUse = [](float x)
    {
        return std::tanh(x);
    };

    auto& preGain = processorChain.template get<preGainIdx>();
    preGain.setRampDurationSeconds(0.1f);

    auto& postGain = processorChain.template get<postGainIdx>();
    postGain.setGainLinear(0.8f);

    dryWet.prepare(spec);
    processorChain.prepare(spec);

    isPrepared = true;
}

void Distortion::process(dsp::ProcessContextReplacing<float> context)
{
    jassert(isPrepared);

    dryWet.pushDrySamples(context.getInputBlock());
    processorChain.process(context);
    dryWet.mixWetSamples(context.getOutputBlock());

    smoothedGain.skip(context.getInputBlock().getNumSamples());
}

void Distortion::setParams(float gain, float color, float dryWet)
{
    // TODO figure out how color should work
    auto& bpf = processorChain.template get<bpfIdx>();
    bpf.setCutoffFrequency(color);

    smoothedGain.setTargetValue(gain);
    auto& preGain = processorChain.template get<preGainIdx>();
    preGain.setGainDecibels(smoothedGain.getCurrentValue());
    
    this->dryWet.setWetMixProportion(dryWet);
}