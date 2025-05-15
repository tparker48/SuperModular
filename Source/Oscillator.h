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
#include <algorithm>

enum WAVETYPE {
    SIN,
    TRIANGLE,
    SAW,
    PULSE,
    SQUARE,
    LFOSAW,
    LFOTRI,
    LFOPULSE,
    LFOSQUARE,
};
const int NUM_WAVETYPES = 9;


class Oscillator {
public:
    Oscillator() {
        phase = 0.0;
        phaseIncrement = 0.0;
        triAccumulator = 0.0;
    }

    void prepareToPlay(float sampleRate) {
        this->sampleRate = sampleRate;
        float cutoff = 2.0;
        triR = 1.0f - (2.0 * MathConstants<float>::pi * cutoff) / sampleRate;
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

    void setPulseDutyCycle(float dutyCycle) {
        pulseDutyCycle = std::min(std::max(dutyCycle,0.10f), 0.90f);
    }

    float getSample(WAVETYPE wave) {
        switch (wave) {
        case SIN:
            return getSinSample();
            break;
        case TRIANGLE:
            return getTriangleSample();
            break;
        case SAW:
            return getSawSample();
            break;
        case SQUARE:
            return getSquareSample();
            break;
        case PULSE:
            return getPulseSample();
            break;
        case LFOSAW:
            return getNaiveSawSample();
            break;
        case LFOSQUARE:
            return getNaiveSquareSample();
            break;
        case LFOTRI:
            return getNaiveTriSample();
            break;
        case LFOPULSE:
            return getNaivePulseSample();
            break;
        }
    }

    float getSawSample() {
        return getNaiveSawSample()
                - polyBlep(phase, phaseIncrement);
    }

    float getSquareSample() {
        return getNaiveSquareSample()
                + polyBlep(phase, phaseIncrement)
                - polyBlep(std::fmod(phase + 0.5, 1.0), phaseIncrement);

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

    float getPulseSample() {
        float pulse = getNaivePulseSample();
        pulse += polyBlep(phase, phaseIncrement);
        pulse -= polyBlep(std::fmod(phase + (1.0-pulseDutyCycle), 1.0), phaseIncrement);
        return pulse;
    }

    float getNaiveSawSample() {
        return 2.0 * phase - 1.0;
    }
    float getNaiveSquareSample() {
        return phase < 0.5 ? 1.0 : -1.0;
    }
    float getNaiveTriSample() {
        return 4.0 * abs(phase - 0.5) - 1.0;
    }

    float getNaivePulseSample() {
        return phase < pulseDutyCycle ? 1.0 : -1.0;
    }

    float getSinSample() {
        return sin(MathConstants<float>::twoPi * phase);
    }

private:
    float phase, phaseIncrement;
    float sampleRate = 0.0;

    float triAccumulator = 0.0;
    float lastTriOutput = 0.0;
    float lastTriInput = 0.0;
    float triR;
    float lastSquare = 0.0;

    float lastSyncIn = 0.0;

    float pulseDutyCycle = 0.5;

    float nextBlep = 0.0;

    float polyBlep(float t, float dt) {
        if (t < dt) {
            t = t / dt;
            return t + t - t * t - 1.0;
        }
        if (t > 1.0 - dt) {
            t = (t - 1.0) / dt;
            return t * t + t + t + 1.0;
        }
        return 0.0;
    }
};