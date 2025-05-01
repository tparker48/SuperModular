#include "NoiseGenerator.h"

void NoiseGenerator::processBlock(AudioSampleBuffer& buffer, int startSample, int numSamples, float gain)
{
    for (int sample = 0; sample < numSamples; sample++)
	{
		for (int channel = 0; channel < buffer.getNumChannels(); channel++)
		{
			buffer.setSample(channel, sample, generateSamplePink() * gain);
		}
	}
}

float NoiseGenerator::generateSampleWhite() {
    return 2.0f * (((float)rand()) / RAND_MAX) - 1.0;
}

float NoiseGenerator::generateSamplePink() {
    float whiteIn = generateSampleWhite();
    float pinkOut = pinkFilter.processSample(whiteIn);
    return pinkOut;
}