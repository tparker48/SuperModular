/*
  ==============================================================================

    ModuleUI.h
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

class ModuleUI : public Component {
public:
    static const int hp = 1;

    ModuleUI(
        MODULE_ID id, 
        ModuleGrid* mg, 
        PatchCableManager* cm, 
        SharedStateWriter* stateWriter, 
        int numCvIns, 
        int numCvOuts
    );

    ~ModuleUI();

    virtual void applyState(ModuleState& state);
    virtual void startListeners() {}

    MODULE_ID getId() { return id; }

    void parentHierarchyChanged();
    void mouseDown(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);

    // Must implement per Module type
    virtual void paint(Graphics& g) = 0;
    virtual void resized() = 0;

    CVJack* getCvInputJack(int jackId) {
        return cvIns[jackId];
    }
    CVJack* getCvOutputJack(int jackId) {
        return cvOuts[jackId];
    }
    int getNumCVInputs() {
        return cvIns.size();
    }
    int getNumCVOutputs() {
        return cvOuts.size();
    }

protected:
    std::vector<CVJack*> cvIns, cvOuts;
    SharedStateWriter* stateWriter;

private:
    MODULE_ID id = -1;
    ModuleGrid* moduleGrid;
    ComponentDragger myDragger;
    ModuleDragRules moduleDragRules;
    PatchCableManager* cableManager;
};

