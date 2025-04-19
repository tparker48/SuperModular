/*
  ==============================================================================

    SharedPluginStateTest.h
    Created: 18 Apr 2025 6:55:25pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SharedPluginState.h"

class SharedPluginStateExposed : public SharedPluginState {
public:
    void startRead() { SharedPluginState::startRead(); }
    void endRead() {SharedPluginState::endRead();}
    char getWriteIdx() {return SharedPluginState::getWriteIdx();}
    char getReadIdx() {return SharedPluginState::getReadIdx();}
    Atomic<char> getShareFlags() { return shareFlags; }
};

class SharedPluginStateTest : public UnitTest {
public:
    SharedPluginStateTest() : UnitTest("SharedPluginState Test") {}

    virtual void runTest() override {
        SharedPluginState sharedState;

        beginTest("Basic Message Passing");
        std::vector<StateChangeMessage> messagesReceived;

        sharedState.recieve_updates(messagesReceived);
        expect(messagesReceived.size() == 0);

        sharedState.send_update(StateChangeMessage(ModuleState(1), ADD));
        sharedState.recieve_updates(messagesReceived);
        expect(messagesReceived.size() == 1);

        beginTest("Multiple Updates, One Message");
        sharedState.send_updates({
            StateChangeMessage(ModuleState(1), DELETE),
            StateChangeMessage(ModuleState(1), UPDATE)
        });
        sharedState.recieve_updates(messagesReceived);
        expect(messagesReceived.size() == 2);

        beginTest("Multiple Updates, MultipleMessages");
        sharedState.send_updates({
            StateChangeMessage(ModuleState(1), DELETE),
            StateChangeMessage(ModuleState(1), UPDATE)
        });
        sharedState.send_updates({
            StateChangeMessage(ModuleState(1), DELETE),
            StateChangeMessage(ModuleState(1), UPDATE)
        });
        sharedState.send_updates({
            StateChangeMessage(ModuleState(1), DELETE),
            StateChangeMessage(ModuleState(1), UPDATE)
        });

        // q1: 4 messages, q2: 2 messages
        // ^ read queue    ^ write queue
        // reader only gets the 4 messages in readPtr queue
        sharedState.recieve_updates(messagesReceived);
        expect(messagesReceived.size() == 4);

        // q1: 0 messages, q2: 2 messages
        // ^ read queue    ^ write queue
        // further attempts from reader get 0 messages fomr readPtr queue
        for (int i = 0; i < 10; i++) {
            sharedState.recieve_updates(messagesReceived);
            expect(messagesReceived.size() == 0);
        }

        sharedState.switchWriteIdx();
        // q1: 0 messages, q2: 2 messages
        // ^ write queue   ^ read queue
        sharedState.recieve_updates(messagesReceived);
        expect(messagesReceived.size() == 2);

        beginTest("Atomic Share Flag Get/Set");
        SharedPluginStateExposed sharedStateExp;

        // write idx != read idx
        expect(sharedStateExp.getReadIdx() != sharedStateExp.getWriteIdx());
        sharedStateExp.switchWriteIdx();
        expect(sharedStateExp.getReadIdx() != sharedStateExp.getWriteIdx());
        sharedStateExp.send_update(StateChangeMessage(ModuleState(1), ADD));
        expect(sharedStateExp.getReadIdx() != sharedStateExp.getWriteIdx());
        sharedStateExp.recieve_updates(messagesReceived);
        expect(sharedStateExp.getReadIdx() != sharedStateExp.getWriteIdx());

        // read flag set properly
        sharedStateExp.startRead();
        expect(((sharedStateExp.getShareFlags().get() & 0b00000010) >> 1) == 1);
        sharedStateExp.endRead();
        expect(((sharedStateExp.getShareFlags().get() & 0b00000010) >> 1) == 0);

        // readwrite idx swapped properly
        auto startingReadIdx = sharedStateExp.getReadIdx();
        sharedStateExp.switchWriteIdx();
        auto endingReadIdx = sharedStateExp.getReadIdx();
        expect(startingReadIdx != endingReadIdx);

        beginTest("Full State Read/Write");
        SharedPluginState sharedStateFullTest;
        PluginState writeState;
        PluginState readState;

        sharedStateFullTest.readFullState(readState);
        expect(readState.moduleStates.size() == 0);

        writeState.moduleStates.push_back(ModuleState(77));
        writeState.moduleStates.push_back(ModuleState(78));
        writeState.moduleStates.push_back(ModuleState(79));
        writeState.moduleStates.push_back(ModuleState(80));
        sharedStateFullTest.writeFullState(writeState, true);
        sharedStateFullTest.readFullStateIfNew(readState);
        expect(readState.moduleStates.size() == 4);
        expect(readState.getModule(77) != nullptr);
        expect(readState.getModule(78) != nullptr);
        expect(readState.getModule(79) != nullptr);
        expect(readState.getModule(80) != nullptr);
        expect(readState.getModule(0) == nullptr);

        PluginState blankState;
        sharedStateFullTest.readFullStateIfNew(blankState);
        expect(blankState.moduleStates.size() == 0);

        sharedStateFullTest.writeFullState(writeState, false);
        sharedStateFullTest.readFullStateIfNew(blankState);
        expect(blankState.moduleStates.size() == 0);

        // multi-thread interaction is not unit tested
    }
};