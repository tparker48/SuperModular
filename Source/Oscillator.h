/*
  ==============================================================================

    Oscillator.h
    Created: 8 May 2025 7:13:55pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cmath>

class Oscillator {
public:
    Oscillator() {
        phase = 0.0;
        phaseIncrement = 0.0;
        triAccumulator = 0.0;
    }

    void prepareToPlay(float sampleRate) {
        this->sampleRate = sampleRate;
        float cutoff = 5.0;
        triR = 1.0f - (2.0 * MathConstants<float>::pi * cutoff) / sampleRate;
        triLPAlpha = 1.0f - expf(-2.0f * MathConstants<float>::pi * 3000.0 / sampleRate);
    }

    void setHz(float hz) {
        phaseIncrement = hz / sampleRate;
    }

    void incrementPhase() {
        phase += phaseIncrement;
        if (phase >= 1.0) {
            phase -= 1.0;
        }
    }

    float getSawSample() {
        float saw = 2.0 * phase - 1.0;
        return saw - polyBlep(phase, phaseIncrement);
    }

    float getSquareSample() {
        float square = phase < 0.5 ? 1.0 : -1.0;
        square += polyBlep(phase, phaseIncrement);
        square -= polyBlep(std::fmod(phase + 0.5, 1.0), phaseIncrement);
        return square;
    }

    float getTriangleSample() {
        float square = getSquareSample();
        // integrate a bandlimited square
        triAccumulator += 0.5f * (square + lastSquare) * 4.0f * phaseIncrement;
        lastSquare = square;
        float tri = triAccumulator;

        // dc blocking filter
        float dc_blocked = tri - lastTriInput + triR * lastTriOutput;
        lastTriOutput = dc_blocked;
        lastTriInput = tri;

        return lastTriOutput * 0.9;        
    }

    float getPwmSample(float dutyCycle) {} // use two saw waves and shift phase of one to adjust duty cycle
    float getLfoTriangleSample() {}
    float getLfoSquareSample() {}
    float getLfoSawSample() {}

    float getSinSample() {
        return sin(MathConstants<float>::twoPi * phase);
    }

private:
    float phase, phaseIncrement;
    float sampleRate;

    float triAccumulator;
    float lastTriOutput = 0.0;
    float lastTriInput = 0.0;
    float triR;
    float lastSquare = 0.0;
    float lastLPOut = 0.0;
    float triLPAlpha = 0.0;

    float polyBlep(float t, float dt) {
        // left edge of a rising edge
        if (t < dt) {
            t = t / dt;
            return t + t - t * t - 1.0;
        }
        // right edge of the rising edge 
        if (t > 1.0 - dt) {
            t = (t - 1.0) / dt;
            return t * t + t + t + 1.0;
        }
        // do nothing in the middle
        return 0.0;
    }
};