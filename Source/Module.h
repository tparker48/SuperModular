/*
  ==============================================================================

    Module.h
    Created: 5 Apr 2025 7:25:39pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "CVJack.h"
#include "PatchCableManager.h"
#include "PatchCable.h"
#include "ModuleGrid.h"


class ModuleDragRules : public juce::ComponentBoundsConstrainer {
public:
    void checkBounds(
        juce::Rectangle< int >& bounds,
        const juce::Rectangle< int >& previousBounds,
        const juce::Rectangle< int >& limits,
        bool sTop, bool sLeft, bool sBottom, bool sRight) {
        auto moduleGrid = ModuleGrid::getInstance();
        ModuleBounds newBounds = moduleGrid->closestAvailablePosition(bounds);
        if (newBounds.getX() == -1) {
            bounds = previousBounds;
        }
        else {
            bounds = newBounds;
        }
    }
};

class Module : public juce::Component {
public:
    static const int hp = 1;

    Module() : id(nextModuleId) { nextModuleId++;}
    MODULE_ID getId() { return id; }

    void parentHierarchyChanged() {
        // When addAndMakeVisible() is called on this module,
        // transfer ownership of all owned PatchCables to parent.
        // this is necessary so patch cables can be drawn outside of this module
        auto parent = getParentComponent();
        if (parent == nullptr) {
            return;
        }
        for (auto child : getChildren()) {
            for (auto grandchild : child->getChildren()) {
                auto cable = dynamic_cast<PatchCable*>(grandchild);
                if (cable) {
                    parent->addAndMakeVisible(cable);
                    child->removeChildComponent(cable);
                }
            }
        }
    }

    void mouseDown(const juce::MouseEvent& e);
    void mouseUp(const juce::MouseEvent& e);
    void mouseDrag(const juce::MouseEvent& e);

    // Must implement per Module type
    virtual void paint(juce::Graphics& g) = 0;
    virtual void resized() = 0;
    
private:
    MODULE_ID id = -1;
    juce::ComponentDragger myDragger;
    ModuleDragRules moduleDragRules;
};

class TestModule : public Module {
public:
    static const int hp = 4;

    TestModule() : Module(), jack1(CVInput), jack2(CVOutput) {
        addAndMakeVisible(jack1);
        addAndMakeVisible(jack2);
    }

    void paint(juce::Graphics& g) override {
        switch (getId() % 3) {
        case 0:
            g.setColour(juce::Colours::red);
            break;
        case 1:
            g.setColour(juce::Colours::blue);
            break;
        case 2:
            g.setColour(juce::Colours::green);
            break;
        }
        
        g.fillAll();
    }

    void resized() override {
        jack1.setBounds(10, 10, 25, 25);
        jack2.setBounds(getWidth() - 10 - 25, 10, 25, 25);
    }
    
private:
    CVJack jack1, jack2;
};

class TestModule2 : public TestModule {
public:
    TestModule2() : TestModule(), njack1(CVInput), njack2(CVOutput) {
        addAndMakeVisible(njack1);
        addAndMakeVisible(njack2);
    }

    void paint(juce::Graphics& g) override {
        g.setColour(juce::Colours::cyan);
        g.fillAll();
    }

    void resized() override {
        TestModule::resized();
        njack1.setBounds(10, getHeight()-10-25, 25, 25);
        njack2.setBounds(getWidth() - 10 - 25, getHeight()-10-25, 25, 25);
    }

private:
    CVJack njack1, njack2;
};