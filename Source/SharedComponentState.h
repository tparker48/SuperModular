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


class ModuleState {
public:
    ValueTree state;

    ModuleState(int id, String moduleType) : state("module") {
        state.setProperty("id", id, nullptr);
        state.setProperty("type", moduleType, nullptr);
    }

    ModuleState(XmlElement& xml) {
        state = ValueTree::fromXml(xml);
    }

    XmlElement* toXml() {
        return state.createXml().release();
    }
};

class PatchCableState {
public:
    ValueTree state;

    PatchCableState(int inputModuleId, int inputCVId, int outputModuleId, int outputCVId) : state("cable") {
        setState(inputModuleId, inputCVId, outputModuleId, outputCVId);
    }

    PatchCableState(XmlElement& xml) {
        state = ValueTree::fromXml(xml);
    }

    XmlElement* toXml() {
        return state.createXml().release();
    }

    void setState(int inputModuleId, int inputCVId, int outputModuleId, int outputCVId) {
        state.setProperty("input.module", inputModuleId, nullptr);
        state.setProperty("input.cv", inputCVId, nullptr);
        state.setProperty("output.module", outputModuleId, nullptr);
        state.setProperty("output.cv", outputCVId, nullptr);
    }

private:
};

class PluginState {
public:
    std::vector<ModuleState> moduleStates;
    std::vector<PatchCableState> cableStates;

    PluginState() {}

    PluginState(XmlElement* xml) {
        auto modules = xml->getChildByName("modules");
        auto cables = xml->getChildByName("cables");

        if (modules) {
            for (auto* element : modules->getChildIterator()) {
                moduleStates.push_back(ModuleState(*element));
            }
        }
        if (cables) {
            for (auto* element : cables->getChildIterator()) {
                cableStates.push_back(PatchCableState(*element));
            }
        }
    }

    XmlElement* toXml() {
        auto* xml = new XmlElement("SuperModularPluginState");

        auto modules = new XmlElement("modules");
        for (auto moduleState : moduleStates) {
            modules->addChildElement(moduleState.toXml());
        }

        auto cables = new XmlElement("cables");
        for (auto cableState : cableStates) {
            cables->addChildElement(cableState.toXml());
        }

        xml->addChildElement(modules);
        xml->addChildElement(cables);
        return xml;
    }
};

class SharedPluginState {
public:
    SharedPluginState() {
        shareFlags = 0b00000000;
        alreadyRead[0] = true;
        alreadyRead[1] = true;
    }

    void write(PluginState& pluginState) {
        // write to the no-read state
        auto writeIdx = getWriteIdx();
        states[writeIdx] = pluginState;
        alreadyRead[writeIdx] = false;

        auto flagToWaitFor = writeIdx;
        auto flagToWrite = (~writeIdx) & 0b00000001;
        while (!shareFlags.compareAndSetBool(flagToWrite, flagToWaitFor)) {}
    }

    void read(PluginState& pluginState) {
        startRead();
        auto readIdx = getReadIdx();
        if (!alreadyRead[readIdx]) {
            pluginState = states[readIdx];
            alreadyRead[readIdx] = true;
        }
        endRead();
    }

private:
    Atomic<char> shareFlags;
    PluginState states[2];
    bool alreadyRead[2];
    int lastReadFlag = 0;

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

class SharedPluginStateWriter {
public:
    SharedPluginStateWriter(SharedPluginState* sharedStatePtr): sharedState(sharedStatePtr) {}

    void addPatchCable(int inputModuleId, int inputCVId, int outputModuleId, int outputCVId) {
        localState.cableStates.push_back(PatchCableState(inputModuleId, inputCVId, outputModuleId, outputCVId));
        updateSharedState();
    }

    void removePatchCable(int inputModuleId, int inputCVId, int outputModuleId, int outputCVId) {
        std::remove(
            localState.cableStates.begin(), 
            localState.cableStates.end(), 
            PatchCableState(inputModuleId, inputCVId, outputModuleId, outputCVId));
        updateSharedState();
    }

    void addModule(ModuleState& module) {
        localState.moduleStates.push_back(module);
        updateSharedState();
    }

    void removeModule(int moduleId) {
        std::remove_if(
            localState.cableStates.begin(),
            localState.cableStates.end(),
            [moduleId](ModuleState m) { (int)m.state["id"] == moduleId; });
;        updateSharedState();

    }

private:
    SharedPluginState* sharedState = nullptr;
    PluginState localState;

    void updateSharedState() {
        sharedState->write(localState);
    }
};