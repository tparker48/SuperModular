/*
  ==============================================================================

    SharedStateWriter.h
    Created: 12 Apr 2025 7:15:14pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CircularMessageBuffer.h"


class SharedStateWriter {
public:
    SharedStateWriter(CircularMessageBuffer* messageBufferPtr, SharedState* sharedStatePtr)
        : messageBuffer(messageBufferPtr), sharedState(sharedStatePtr)
    {
        sharedState->readFullState(localState);
    }

    bool reload(SharedState* sharedStatePtr) {
        return sharedStatePtr->readFullState(localState, true);
    }

    void startGroupUpdate();

    void sendGroupUpdate();

    void addModule(ModuleState newModule);

    void setModuleProperty(int moduleId, Identifier propertyName, var value);

    void setModuleProperties(int moduleId, std::vector<std::pair<Identifier, var>> properties);

    void moveModule(int moduleId, Rectangle<int> newBounds);

    void deleteModule(int moduleId);

    void addPatchCable(int inputModuleId, int inputCvId, int outputModuleId, int outputCvId);

    void removePatchCable(int inputModuleId, int inputCvId, int outputModuleId, int outputCvId);

    void setNumRows(int newNumRows);

    void saveStateToShared();

    PluginState& dumpLocalState() {
        return localState;
    }

private:
    PluginState localState;
    CircularMessageBuffer* messageBuffer;
    SharedState* sharedState;
    std::vector<StateChangeMessage> pendingUpdates;
    bool withinGroupUpdate = false;

    void sendUpdate(StateChangeMessage update);
    void sendUpdates(std::vector<StateChangeMessage> updates);
};
