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

    ModuleUI(MODULE_ID id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter, int numCvIns, int numCvOuts) :
        id(id),
        moduleGrid(mg),
        moduleDragRules(mg),
        cableManager(cm),
        stateWriter(stateWriter)
    {
        for (int cvIdx = 0; cvIdx < numCvIns; cvIdx++) {
            auto newCv = new CVJack(CVInput, cvIdx, id, cm, stateWriter);
            addAndMakeVisible(newCv);
            cvIns.push_back(newCv);
        }
        for (int cvIdx = 0; cvIdx < numCvOuts; cvIdx++) {
            auto newCv = new CVJack(CVOutput, cvIdx, id, cm, stateWriter);
            addAndMakeVisible(newCv);
            cvOuts.push_back(newCv);
        }
    }

    ~ModuleUI() {
        for (auto cvIn : cvIns) {
            delete cvIn;
        }
        for (auto cvOut : cvOuts) {
            delete cvOut;
        }
    }
    virtual void startListeners() {}

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

    void mouseDown(const MouseEvent& e);
    void mouseUp(const MouseEvent& e);
    void mouseDrag(const MouseEvent& e);

    // Must implement per Module type
    virtual void paint(Graphics& g) = 0;
    virtual void resized() = 0;

    

    void applyState(ModuleState& state) {
        // Load Bounds
        auto bounds = state.getBounds();
        auto closest = moduleGrid->closestAvailablePosition(bounds);
        moduleGrid->placeModule(getId(), closest);
        stateWriter->moveModule(getId(), getBounds());

        // Wire CV inputs
        auto numCvInputs = state.getNumCvInputs();
        jassert(cvIns.size() == numCvInputs);
        for (int i = 0; i < state.getNumCvInputs(); i++) {
            auto connection = state.getInputCvConnection(i);
            if (connection.first == -1) continue;

            Component* targetComponent = moduleGrid->getModule(connection.first);
            auto targetModule = dynamic_cast<ModuleUI*>(targetComponent);
            if (!targetModule) continue;

            auto targetJack = targetModule->getCvOutputJack(connection.second);
            if (!targetJack) continue;

            targetJack->setConnection(cvIns[i]);
            cvIns[i]->setConnection(targetJack);
        }

        // Wire CV outputs
        auto numCvOutputs = state.getNumCvOutputs();
        jassert(cvOuts.size() == numCvOutputs);
        for (int i = 0; i < state.getNumCvOutputs(); i++) {
            auto connection = state.getOutputCvConnection(i);
            if (connection.first == -1) continue;

            Component* targetComponent = moduleGrid->getModule(connection.first);
            auto targetModule = dynamic_cast<ModuleUI*>(targetComponent);
            if (!targetModule) continue;

            auto targetJack = targetModule->getCvInputJack(connection.second);
            if (!targetJack) continue;

            targetJack->setConnection(cvOuts[i]);
            cvOuts[i]->setConnection(targetJack);
        }
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

