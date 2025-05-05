#pragma once
#include <JuceHeader.h>
#include <vector>

class Delay
{
public:
	void prepare(const juce::dsp::ProcessSpec &spec);
	void setParams(float delayTime, float feedback, float wet);
	void process(juce::dsp::ProcessContextReplacing<float> context);

	// only valid after process was called
	float getLastWetSample();
	float getLastDrySample();

private:
	void incrementHeadPositions();
	float interpolate(float idx);

	juce::dsp::DryWetMixer<float> dryWet;

	std::vector<float> delayBuffer;

	juce::SmoothedValue<float> timeSmoothed;

	int writeHead;
	int eraserHead, eraserOffset;

	float readOffset;
	int targetReadOffset;

	int targetReadHead;
	float readHead;

	float sampleRate;
	float feedback;

	float lastWetSample = 0.0;
	float lastDrySample = 0.0;
};
