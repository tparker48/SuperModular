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
#include "PatchCableManager.h"
#include "SharedComponentState.h"

enum CVJackType {
    CVInput,
    CVOutput
};

class CVJack : public Component {
public:
    CVJack(CVJackType type, int id, int parentId, PatchCableManager* cm, PluginStateWriteHandler* stateWriter);

    int getId() { return jackId; }
    int getModuleId() { return moduleId; }

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

    void mouseDown(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);
    Line<float> getPatchLine();
    void paint(Graphics& g);

private:
    int jackId, moduleId;
    CVJackType jackType;
    CVJack* connection = nullptr;
    PatchCableManager* cableManager;
    PatchCable cable;
    PluginStateWriteHandler* stateWriter;
};