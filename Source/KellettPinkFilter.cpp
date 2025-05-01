#include "KellettPinkFilter.h"

float KellettPinkFilter::processSample(float whiteIn)
{
    /*if (mode == FilterMode::NORMAL)
    {
        return processSampleNormal(whiteIn);
    }
    else if (mode == FilterMode::ECONOMY) 
    {
        return processSampleEconomy(whiteIn);
    }*/
    return processSampleNormal(whiteIn);
}

float KellettPinkFilter::processSampleNormal(float whiteIn)
{
    b[0] = 0.99886f * b[0] + whiteIn * 0.0555179f;
    b[1] = 0.99332f * b[1] + whiteIn * 0.0750759f;
    b[2] = 0.96900f * b[2] + whiteIn * 0.1538520f;
    b[3] = 0.86650f * b[3] + whiteIn * 0.3104856f;
    b[4] = 0.55000f * b[4] + whiteIn * 0.5329522f;
    b[5] = -0.7616f * b[5] - whiteIn * 0.0168980f;

    float pinkOut = b[0] + b[1] + b[2] + b[3] + b[4] + b[5] + b[6] + whiteIn * 0.5362f;
    b[6] = whiteIn * 0.115926f;
    return pinkOut;
}

float KellettPinkFilter::processSampleEconomy(float whiteIn)
{
    b[0] = 0.99765f * b[0] + whiteIn * 0.0990460f;
    b[1] = 0.96300f * b[1] + whiteIn * 0.2965164f;
    b[2] = 0.57000f * b[2] + whiteIn * 1.0526913f;

    float pinkOut = b[0] + b[1] + b[2] + whiteIn * 0.1848f;
    return pinkOut;
}