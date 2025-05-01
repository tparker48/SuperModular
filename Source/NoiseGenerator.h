#pragma once

#include <JuceHeader.h>
#include<cstdlib>
#include <time.h>

#include "KellettPinkFilter.h"

class NoiseGenerator {
public:
    NoiseGenerator() : pinkFilter() { srand((unsigned) time(NULL)); }
    void processBlock(AudioSampleBuffer& buffer, int startSample, int numSamples, float gain);
    float generateSampleWhite();
    float generateSamplePink();

private:
    KellettPinkFilter pinkFilter;
};