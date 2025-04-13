/*
  ==============================================================================

    SharedStateWriter.cpp
    Created: 12 Apr 2025 7:15:14pm
    Author:  Tom

  ==============================================================================
*/

#include "SharedStateWriter.h"

void SharedStateWriter::addModule(ModuleState newModule) {
    localState.moduleStates.push_back(newModule);

    stateMessageQueue->send_update(
        StateChangeMessage(newModule, ADD)
    );

}

void SharedStateWriter::setModuleProperty(int moduleId, Identifier propertyName, var value) {
    auto module = localState.getModule(moduleId);
    if (module) {
        module->state.setProperty(propertyName, value, nullptr);
        stateMessageQueue->send_update(StateChangeMessage(*module, UPDATE));
    }
}

void SharedStateWriter::moveModule(int moduleId, Rectangle<int> newBounds) {
    auto module = localState.getModule(moduleId);
    if (module) {
        module->setBounds(newBounds);
        // The processor doesn't really care where modules are, so no need to send
        //stateMessageQueue->send_update(StateChangeMessage(*module, UPDATE));
    }
}

void SharedStateWriter::deleteModule(int moduleId) {
    localState.removeModule(moduleId);
    stateMessageQueue->send_update(StateChangeMessage(ModuleState(moduleId), DELETE));
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

    stateMessageQueue->send_updates(messages);
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
    stateMessageQueue->send_updates(messages);
}

void SharedStateWriter::saveStateToShared() {
    stateMessageQueue->writeFullState(localState);
}