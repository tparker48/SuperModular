/*
  ==============================================================================

    ModuleUI.h
    Created: 5 Apr 2025 7:25:39pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "CVJackComponent.h"
#include "PatchCableManager.h"
#include "PatchCable.h"
#include "ModuleGrid.h"

enum ComponentLabelStyle {
    TOP, BOTTOM, LEFT, RIGHT
};

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
    virtual void paintModule(Graphics& g) = 0;
    virtual void resized() = 0;

    void paintComponentLabel(Graphics& g, Component*, std::string, ComponentLabelStyle, int, Colour = Colour(20, 20, 20), int=14);

    CVJackComponent* getCvInputJack(int jackId) {
        if (jackId < 0 || jackId > cvIns.size()) {
            return nullptr;
        }
        return cvIns[jackId];
    }
    CVJackComponent* getCvOutputJack(int jackId) {
        if (jackId < 0 || jackId > cvOuts.size()) {
            return nullptr;
        }
        return cvOuts[jackId];
    }
    int getNumCVInputs() {
        return cvIns.size();
    }
    int getNumCVOutputs() {
        return cvOuts.size();
    }

    void paint(Graphics& g);

protected:
    std::vector<CVJackComponent*> cvIns, cvOuts;
    SharedStateWriter* stateWriter;

    // Colour bgCol = Colour(0xFFF5EFE4);
    // Colour textCol = Colour(0xFF60695C);
    // Colour knobCol = Colour(0xFF2E1F27);
    // Colour dotCol = Colour(0xFFF29E4C);
    // Colour cvCol = Colour(0xFF2E1F27);

    Colour bgCol = Colour(0xFF829191);
    Colour textCol = Colour(0xFFEFF7CF);
    Colour knobCol = Colour(0xFF2E1F27);
    Colour dotCol = Colour(0xFFF29E4C);
    Colour cvCol = Colour(0xFF2E1F27);

private:
    MODULE_ID id = -1;
    ModuleGrid* moduleGrid;
    ComponentDragger myDragger;
    ModuleDragRules moduleDragRules;
    PatchCableManager* cableManager;

    void shadeEdges(Graphics& g);
};

