/*
  ==============================================================================

    SharedStateWriter.cpp
    Created: 12 Apr 2025 7:15:14pm
    Author:  Tom

  ==============================================================================
*/

#include "SharedStateWriter.h"

void SharedStateWriter::startGroupUpdate() {
    withinGroupUpdate = true;
}

void SharedStateWriter::sendGroupUpdate() {
    stateMessageQueue->send_updates(pendingUpdates);
    pendingUpdates.clear();
    withinGroupUpdate = false;
}

void SharedStateWriter::addModule(ModuleState newModule) {
    localState.moduleStates.push_back(newModule);
    sendUpdate(StateChangeMessage(newModule, ADD));
    saveStateToShared();
}

void SharedStateWriter::setModuleProperty(int moduleId, Identifier propertyName, var value) {
    auto module = localState.getModule(moduleId);
    if (module) {
        module->state.setProperty(propertyName, value, nullptr);
        sendUpdate(StateChangeMessage(*module, UPDATE));
        saveStateToShared();
    }
}

void SharedStateWriter::setModuleProperties(int moduleId, std::vector<std::pair<Identifier, var>> properties) {
    auto module = localState.getModule(moduleId);
    if (module) {
        for (auto prop : properties) {
            module->state.setProperty(prop.first, prop.second, nullptr);
        }
        sendUpdate(StateChangeMessage(*module, UPDATE));
        saveStateToShared();
    }
}

void SharedStateWriter::moveModule(int moduleId, Rectangle<int> newBounds) {
    auto module = localState.getModule(moduleId);
    if (module) {
        module->setBounds(newBounds);
        saveStateToShared();
    }
}

void SharedStateWriter::deleteModule(int moduleId) {
    localState.removeModule(moduleId);
    sendUpdate(StateChangeMessage(ModuleState(moduleId), DELETE));
    saveStateToShared();
}

void SharedStateWriter::addPatchCable(int inputModuleId, int inputCvId, int outputModuleId, int outputCvId) {
    auto inputModule = localState.getModule(inputModuleId);
    auto outputModule = localState.getModule(outputModuleId);

    inputModule->setInputCvConnection(inputCvId, outputModuleId, outputCvId);
    outputModule->setOutputCvConnection(outputCvId, inputModuleId, inputCvId);

    // create messages with new states
    std::vector<StateChangeMessage> messages({
        StateChangeMessage(*inputModule, UPDATE),
        StateChangeMessage(*outputModule, UPDATE)
        });

    sendUpdates(messages);
    saveStateToShared();
}

void SharedStateWriter::removePatchCable(int inputModuleId, int inputCvId, int outputModuleId, int outputCvId) {
    auto inputModule = localState.getModule(inputModuleId);
    auto outputModule = localState.getModule(outputModuleId);

    inputModule->setInputCvConnection(inputCvId, -1, -1);
    outputModule->setOutputCvConnection(outputCvId, -1, -1);

    // create messages with new states
    std::vector<StateChangeMessage> messages({
        StateChangeMessage(*inputModule, UPDATE),
        StateChangeMessage(*outputModule, UPDATE)
        });
    sendUpdates(messages);
    saveStateToShared();
}

void SharedStateWriter::saveStateToShared() {
    stateMessageQueue->writeFullState(localState, false);
}



void SharedStateWriter::sendUpdate(StateChangeMessage update) {
    if (withinGroupUpdate) {
        pendingUpdates.push_back(update);
    }
    else {
        stateMessageQueue->send_update(update);
    }
}

void SharedStateWriter::sendUpdates(std::vector<StateChangeMessage> updates) {
    if (withinGroupUpdate) {
        pendingUpdates.insert(pendingUpdates.end(), updates.begin(), updates.end());
    }
    else {
        stateMessageQueue->send_updates(updates);
    }
}