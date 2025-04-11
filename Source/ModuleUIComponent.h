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


class ModuleDragRules : public ComponentBoundsConstrainer {
public:
    ModuleDragRules(ModuleGrid* mg) : ComponentBoundsConstrainer(), moduleGrid(mg) {}

    void checkBounds(
        Rectangle< int >& bounds,
        const Rectangle< int >& previousBounds,
        const Rectangle< int >& limits,
        bool sTop, bool sLeft, bool sBottom, bool sRight) {
        ModuleBounds newBounds = moduleGrid->closestAvailablePosition(bounds);
        if (newBounds.getX() == -1) {
            bounds = previousBounds;
        }
        else {
            bounds = newBounds;
        }
    }
private:
    ModuleGrid* moduleGrid;
};

class ModuleUIComponent : public Component {
public:
    static const int hp = 1;

    ModuleUIComponent(MODULE_ID id, ModuleGrid* mg, PatchCableManager* cm, PluginStateWriteHandler* stateWriter) :
        id(id), 
        moduleGrid(mg),
        moduleDragRules(mg),
        cableManager(cm)
    {}

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

    void mouseDown(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);

    // Must implement per Module type
    virtual void paint(Graphics& g) = 0;
    virtual void resized() = 0;
    
private:
    MODULE_ID id = -1;
    ModuleGrid* moduleGrid;
    ComponentDragger myDragger;
    ModuleDragRules moduleDragRules;
    PatchCableManager* cableManager;
};

class TestModule : public ModuleUIComponent {
public:
    static const int hp = 4;

    TestModule(int id, ModuleGrid* mg, PatchCableManager* cm, PluginStateWriteHandler* stateWriter) : 
        ModuleUIComponent(id, mg, cm, stateWriter),
        jack1(CVInput, 1, id, cm, stateWriter),
        jack2(CVOutput, 2, id, cm, stateWriter) {
        addAndMakeVisible(jack1);
        addAndMakeVisible(jack2);
    }

    void paint(Graphics& g) override {
        switch (getId() % 3) {
        case 0:
            g.setColour(Colours::red);
            break;
        case 1:
            g.setColour(Colours::blue);
            break;
        case 2:
            g.setColour(Colours::green);
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