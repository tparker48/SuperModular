/*
  ==============================================================================

    SharedStateWriter.h
    Created: 12 Apr 2025 7:15:14pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SharedPluginState.h"


class SharedStateWriter {
public:
    SharedStateWriter(SharedPluginState* sharedStatePtr) : stateMessageQueue(sharedStatePtr) {
        sharedStatePtr->readFullState(localState);
    }

    bool reload(SharedPluginState* sharedStatePtr) {
        return sharedStatePtr->readFullStateIfNew(localState);
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

    void saveStateToShared();

    PluginState& dumpLocalState() {
        return localState;
    }

private:
    PluginState localState;
    SharedPluginState* stateMessageQueue;
    std::vector<StateChangeMessage> pendingUpdates;
    bool withinGroupUpdate = false;

    void sendUpdate(StateChangeMessage update);
    void sendUpdates(std::vector<StateChangeMessage> updates);
};
