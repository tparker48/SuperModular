/*
  ==============================================================================

    CVJack.h
    Created: 6 Apr 2025 4:31:38pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PatchCable.h"

enum CVJackType {
    CVInput,
    CVOutput
};

class CVJack : public juce::Component {
public:
    CVJack(CVJackType type);

    CVJackType getType() { return jackType; }

    bool isConnected() {
        return connection != nullptr;

    }

    CVJack* getConnection() {
        return connection;
    }

    void setConnection(CVJack* other) {
        connection = other;
        refreshCablePosition();
    }

    void refreshCablePosition() {
        if (jackType == CVInput && connection != nullptr) {
            cable.setPoints(this, connection);
        }
    }

    void clearConnection() {
        if (connection != nullptr) {
            connection->cable.setBounds(-1, -1, -1, -1);
            connection->connection = nullptr;
        }
        cable.setBounds(-1, -1, -1, -1);
        connection = nullptr;
    }

    void mouseDown(const juce::MouseEvent& e);
    void mouseUp(const juce::MouseEvent& e);
    void mouseDrag(const juce::MouseEvent& e);
    juce::Line<float> getPatchLine();
    void paint(juce::Graphics& g);

private:
    CVJackType jackType;
    CVJack* connection = nullptr;
    PatchCable cable;
};