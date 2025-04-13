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

    void addModule(ModuleState newModule);

    void setModuleProperty(int moduleId, Identifier propertyName, var value);

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
};
