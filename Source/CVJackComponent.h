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
#include "SharedStateWriter.h"

enum CVJackType {
    CVInput,
    CVOutput
};

class CVJackComponent : public Component {
public:
    CVJackComponent(CVJackType type, int id, int parentId, PatchCableManager* cm, SharedStateWriter* stateWriter);

    int getId() { return jackId; }
    int getModuleId() { return moduleId; }

    CVJackType getType() { return jackType; }


    bool isConnected() {
        return connection != nullptr;
    }

    CVJackComponent* getConnection() {
        return connection;
    }

    void setConnection(CVJackComponent* other) {
        connection = other;
        refreshCablePosition();
        cable.setRandomColor();
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
    void paint(Graphics& g);

private:
    int jackId, moduleId;
    CVJackType jackType;
    CVJackComponent* connection = nullptr;
    PatchCableManager* cableManager;
    PatchCable cable;
    SharedStateWriter* stateWriter;
};