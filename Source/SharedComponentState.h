/*
  ==============================================================================

    SharedComponentState.h
    Created: 9 Apr 2025 6:25:46pm
    Author:  Tom

  ==============================================================================

  This class is responsible for: 
   - Storing the full state of the plugin. All modules, module coordinates, patch 
   connections.
   - Allowing the processor thread and a single editor thread to both access this
   state in a thread-safe manner, without locking the processor thread. (Locking
   the editor thread for very short periods is fine).

   The class does this in the following way:
    - under the hood, there are actually two state objects
    - one object is readable, one object is writable
    - an atomic char is maintained as well,  first 2 bits are used as flags. 
    - Bit 0 denotes which object is for reading, and which is for writing
    - Bit 1 is set to 1 when a read is taking place, and 0 otherwise
    
    NOTE: design assumes only 1 processor (reader) and 1 editor (writer)
    NOTE: design assumes underlying architecture supports non-locking atomic chars,
        otherwise locking may take place
*/

#pragma once

#include <JuceHeader.h>


enum PLUGIN_STATE_MESSAGE_OP_CODE {
    ADD,
    UPDATE,
    DELETE
};


// The full state of a module
class ModuleState {
public:
    ValueTree state;

    ModuleState() {}

    ModuleState(int id) {
        state.setProperty("id", id, nullptr);
    }

    ModuleState(int id, String moduleType, Rectangle<int> bounds, int numCvInputs=0, int numCvOutputs=0) : state("module") {
        setBounds(bounds);
        state.setProperty("id", id, nullptr);
        state.setProperty("type", moduleType, nullptr);

        // init cv ports
        auto ins = ValueTree("cv_in");
        for (int i = 0; i < numCvInputs; i++) {
            auto cv = ValueTree(Identifier(std::to_string(i)));
            cv.setProperty("module", -1, nullptr);
            cv.setProperty("cv", -1, nullptr);
            ins.appendChild(cv, nullptr);
        }
        state.appendChild(ins, nullptr);

        // init cv ports
        auto outs = ValueTree("cv_out");
        for (int i = 0; i < numCvOutputs; i++) {
            auto cv = ValueTree(Identifier(std::to_string(i)));
            cv.setProperty("module", -1, nullptr);
            cv.setProperty("cv", -1, nullptr);
            ins.appendChild(cv, nullptr);
        }
        state.appendChild(outs, nullptr);
    }

    ModuleState(XmlElement& xml) {
        state = ValueTree::fromXml(xml);
    }

    XmlElement* toXml() {
        return state.createXml().release();
    }

    int getId() const {
        return state["id"];
    }

    void setBounds(Rectangle<int> bounds) {
        state.setProperty("x", bounds.getX(), nullptr);
        state.setProperty("y", bounds.getY(), nullptr);
        state.setProperty("w", bounds.getWidth(), nullptr);
        state.setProperty("h", bounds.getHeight(), nullptr);
    }

    Rectangle<int> getBounds() {
        return Rectangle<int>(state["x"], state["y"], state["w"], state["h"]);
    }
    std::pair<int, int> getInputCvConnection(int cvId) {
        return getCvConnection(cvId, "cv_in");
    }
    std::pair<int, int> getOutputCvConnection(int cvId) {
        return getCvConnection(cvId, "cv_out");
    }

    void setInputCvConnection(int cvId, int otherModuleId, int otherCvId) {
        setCvConnection(cvId, otherModuleId, otherCvId, "cv_in");
    }
    void setOutputCvConnection(int cvId, int otherModuleId, int otherCvId) {
        setCvConnection(cvId, otherModuleId, otherCvId, "cv_out");
    }

private:
    std::pair<int, int> getCvConnection(int cvId, Identifier cvTagName) {
        auto cvJacks = state.getChildWithName(cvTagName);
        if (!cvJacks.isValid()) {
            return { -1,-1 };
        }

        auto cvJack = cvJacks.getChildWithName(Identifier(std::to_string(cvId)));
        if (!cvJack.isValid()) {
            return { -1,-1 };
        }
        else {
            return { cvJack["module"], cvJack["cv"] };
        }
    }
    void setCvConnection(int cvId, int otherModuleId, int otherCvId, Identifier cvTagName) {
        auto cvJacks = state.getChildWithName(cvTagName);
        if (!cvJacks.isValid()) {
            return;
        }
        auto cvJack = cvJacks.getChildWithName(Identifier(std::to_string(cvId)));
        if (!cvJack.isValid()) {
            return;
        }

        cvJack.setProperty("module", otherModuleId, nullptr);
        cvJack.setProperty("cv", otherCvId, nullptr);
    }
};


// The full state of the plugin, maintained on the processor side
class PluginState {
public:
    std::vector<ModuleState> moduleStates;

    PluginState() {}

    PluginState(XmlElement* xml) {
        auto modules = xml->getChildByName("modules");

        if (modules) {
            for (auto* element : modules->getChildIterator()) {
                moduleStates.push_back(ModuleState(*element));
            }
        }
    }

    XmlElement* toXml() {
        auto* xml = new XmlElement("SuperModularPluginState");
        auto modules = new XmlElement("modules");
        for (auto moduleState : moduleStates) {
            modules->addChildElement(moduleState.toXml());
        }
        xml->addChildElement(modules);
        return xml;
    }

    ModuleState* getModule(int id) {
        auto module = std::find_if(moduleStates.begin(), moduleStates.end(), [id](ModuleState m) {return m.getId() == id; });
        if (module == moduleStates.end()) {
            return nullptr;
        }
        else {
            return &(*module);
        }
    }
    void removeModule(int id) {
        std::remove_if(
            moduleStates.begin(), 
            moduleStates.end(), 
            [id](ModuleState m) {return m.getId() == id; });
    }
};


// Contains a list of changes to make
class PluginStateUpdateMessage {
public:
    PLUGIN_STATE_MESSAGE_OP_CODE op;
    ModuleState state;

    PluginStateUpdateMessage(ModuleState& state, PLUGIN_STATE_MESSAGE_OP_CODE opCode) {
        op = opCode;
        this->state = state;
    }

    void applyMessage(PluginState& pluginState) {
        switch (op) {
        case ADD:
            pluginState.moduleStates.push_back(state);
            break;
        case UPDATE:
            std::replace_if(
                pluginState.moduleStates.begin(),
                pluginState.moduleStates.end(),
                [this](ModuleState other) { return state.getId() == other.getId(); },
                state
            );
            break;
        case DELETE:
            std::remove_if(
                pluginState.moduleStates.begin(),
                pluginState.moduleStates.end(),
                [this](ModuleState other) { return state.getId() == other.getId(); }
            );
            break;
        }
    }
};


// designed for message passing between threads, with emphasis on never locking up the reader
// used to signal module/patch updates from UI to processor.
class PluginStateMessageQueue {
public:

    void send_messages(std::vector<PluginStateUpdateMessage> messages) {
        auto writeIdx = getWriteIdx();

        // push messages to the no-read buffer
        messageBuffers[writeIdx].insert(
            messageBuffers[writeIdx].end(), 
            messages.begin(), 
            messages.end());

        auto flagToWaitFor = writeIdx;
        auto flagToWrite = (~writeIdx) & 0b00000001;
        while (!shareFlags.compareAndSetBool(flagToWrite, flagToWaitFor)) {}
    }

    void send_message(PluginStateUpdateMessage message) {
        std::vector<PluginStateUpdateMessage> messages;
        messages.push_back(message);
        send_messages(messages);
    }

    void recieve_messages(std::vector<PluginStateUpdateMessage> &messages) {
        startRead();

        // load message from the read buffer and clear it
        auto readIdx = getReadIdx();
        messages = messageBuffers[readIdx];
        messageBuffers[readIdx].clear();

        endRead();
    }

    void writeFullState(PluginState& state) {
        stateLock.lock();
        this->state = state;
        shouldReloadState = true;
        stateLock.unlock();

    }
    void readFullState(PluginState& state) {
        // call this on timer in GUI
        stateLock.lock();
        if (shouldReloadState) {
            state = this->state;
            shouldReloadState = false;
        }
        stateLock.unlock();
    }

private:
    Atomic<char> shareFlags = 0b00000000;
    std::vector<PluginStateUpdateMessage> messageBuffers[2];

    std::mutex stateLock;
    bool shouldReloadState = false;
    PluginState state;

    void startRead() {
        shareFlags += 0b00000010;
    }
    void endRead() {
        shareFlags -= 0b00000010;
    }
    char getWriteIdx() {
        return shareFlags.get() & 0b00000001;
    }
    char getReadIdx() {
        return (~shareFlags.get()) & 0b00000001;
    }
};


class PluginStateWriteHandler {
public:
    PluginStateWriteHandler(PluginStateMessageQueue* messageQueuePtr) : stateMessageQueue(messageQueuePtr) {
        messageQueuePtr->readFullState(localState);
    }

    void addModule(ModuleState newModule) {
        localState.moduleStates.push_back(newModule);

        std::vector<PluginStateUpdateMessage> messages({
            PluginStateUpdateMessage(newModule, ADD)
        });
    }

    void setModuleProperty(int moduleId, Identifier propertyName, var value) {
        auto module = localState.getModule(moduleId);
        if (module) {
            module->state.setProperty(propertyName, value, nullptr);
            stateMessageQueue->send_message(PluginStateUpdateMessage(*module, UPDATE));
        }
    }

    void moveModule(int moduleId, Rectangle<int> newBounds) {
        auto module = localState.getModule(moduleId);
        if (module) {
            module->setBounds(newBounds);
            stateMessageQueue->send_message(PluginStateUpdateMessage(*module, UPDATE));
        }
    }

    void deleteModule(int moduleId) {
        localState.removeModule(moduleId);
        stateMessageQueue->send_message(PluginStateUpdateMessage(ModuleState(moduleId), DELETE));
    }

    void addPatchCable(int inputModuleId, int inputCvId, int outputModuleId, int outputCvId) {
        auto inputModule = localState.getModule(inputModuleId);
        auto outputModule = localState.getModule(outputModuleId);

        inputModule->setInputCvConnection(inputCvId, outputModuleId, outputCvId);
        outputModule->setOutputCvConnection(outputCvId, inputModuleId, inputCvId);

        // create messages with new states
        std::vector<PluginStateUpdateMessage> messages({
            PluginStateUpdateMessage(*inputModule, UPDATE),
            PluginStateUpdateMessage(*outputModule, UPDATE)
        });

        stateMessageQueue->send_messages(messages);
    }

    void removePatchCable(int inputModuleId, int inputCvId, int outputModuleId, int outputCvId) {
        auto inputModule = localState.getModule(inputModuleId);
        auto outputModule = localState.getModule(outputModuleId);

        inputModule->setInputCvConnection(inputCvId, -1, -1);
        outputModule->setOutputCvConnection(outputCvId, -1, -1);

        // create messages with new states
        std::vector<PluginStateUpdateMessage> messages({
            PluginStateUpdateMessage(*inputModule, UPDATE),
            PluginStateUpdateMessage(*outputModule, UPDATE)
        });

        stateMessageQueue->send_messages(messages);
    }

    void saveStateToShared() {
        stateMessageQueue->writeFullState(localState);
    }

private:
    PluginState localState;
    PluginStateMessageQueue* stateMessageQueue;
};
