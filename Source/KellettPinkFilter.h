/*
  ==============================================================================

    KellettPinkFilter.h
    Created: 3 Feb 2023 2:46:29pm
    
    -10dB/decade Filter to transform white noise into pink noise

    From:
    https://www.musicdsp.org/en/latest/Filters/76-pink-noise-filter.html?highlight=pink

    "If 'white' consists of uniform random numbers, such as those generated
    by the rand() function, 'pink' will have an almost gaussian level
    distribution."
  ==============================================================================
*/

#pragma once

class KellettPinkFilter
{
public:
    enum FilterMode {
        NORMAL,
        ECONOMY
    };

    KellettPinkFilter() {
        resetState();
    }

    float processSample(float whiteIn);

private:
    float processSampleNormal(float whiteIn);

    // An 'economy' version with accuracy of + / -0.5dB
    float processSampleEconomy(float whiteIn);

    void resetState()
    {
        for (int i = 0; i < 7; i++)
        {
            b[i] = 0.0;
        }
    }
    
    // filter states
    float b[7];

    // filter mode
    FilterMode mode;
};