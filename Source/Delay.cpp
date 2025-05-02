#include "Delay.h"

void Delay::prepare(const juce::dsp::ProcessSpec& spec) 
{
	sampleRate = spec.sampleRate;

	dryWet.prepare(spec);

	//delayBuffer.clear();
	delayBuffer.resize((sampleRate * 5.0f) + 2);

	writeHead = 0;
	eraserHead = 1;
	readHead = 0;
	readOffset = 0.0f;
	targetReadOffset = 0;

	timeSmoothed.reset(6000);
}

void Delay::setParams(float delayTime, float feedback, float wet)
{
	dryWet.setWetMixProportion(wet);
	this->feedback = feedback;

	timeSmoothed.setTargetValue(delayTime);
	targetReadOffset = timeSmoothed.getCurrentValue() * sampleRate;
}

void Delay::process(dsp::ProcessContextReplacing<float> context)
{
	dryWet.pushDrySamples(context.getInputBlock());

	auto inputBlock = context.getInputBlock();
	auto outputBlock = context.getOutputBlock();

	float delayOutSample;
	
	for (int sample = 0; sample < inputBlock.getNumSamples(); sample++)
	{ 
		delayBuffer[eraserHead] = 0.0;
		delayOutSample = interpolate(readHead); // delayBuffer[readHead];
		
		for (int channel = 0; channel < inputBlock.getNumChannels(); channel++)
		{	
			delayBuffer[writeHead] = inputBlock.getSample(channel, sample) + feedback * delayOutSample;
			outputBlock.setSample(channel, sample, delayOutSample);
		}
		incrementHeadPositions();
	}
	lastWetSample = outputBlock.getSample(0, outputBlock.getNumSamples() - 1);
	lastDrySample = inputBlock.getSample(0, inputBlock.getNumSamples()-1);
	dryWet.mixWetSamples(context.getOutputBlock());
}

float Delay::interpolate(float idx)
{
	int lo = (int)(idx);
	int hi = lo + 1;
	float hiAmt = idx - lo;
	float loAmt = 1.0f - hiAmt;
	return delayBuffer[lo];
	if (hi >= delayBuffer.size()) hi -= delayBuffer.size();
	return loAmt * delayBuffer[lo] + hiAmt * delayBuffer[hi];
}

// call me once per processed sample
void Delay::incrementHeadPositions()
{
	timeSmoothed.skip(1);

	if (readOffset != targetReadOffset)
	{
		auto diff = targetReadOffset - readOffset;
		auto diffSign = abs(diff) / diff;

		auto shift =  diff * 0.0005f;
		if (abs(shift) > 3.0) shift = diffSign * 3.0;
		if (abs(shift) < 0.5f) shift = diffSign * 0.5f;

		readOffset += shift;
		
		if (abs(diff) < 1.0) readOffset = targetReadOffset;
	}

	writeHead += 1;
	eraserHead += 1;
	readHead = writeHead - readOffset;

	if (writeHead >= delayBuffer.size()) writeHead -= delayBuffer.size();
	if (eraserHead >= delayBuffer.size()) eraserHead -= delayBuffer.size();
	if (readHead >= delayBuffer.size()) readHead -= delayBuffer.size();
	if (readHead < 0) readHead = delayBuffer.size() + readHead;
}

float Delay::getLastWetSample() {
	return lastWetSample;
}
float Delay::getLastDrySample() {
	return lastDrySample;
}