/*
  ==============================================================================

    ModuleUI.cpp
    Created: 5 Apr 2025 7:25:39pm
    Author:  Tom

  ==============================================================================
*/

#include "ModuleUI.h"

ModuleUI::ModuleUI(MODULE_ID id, ModuleGrid* mg, PatchCableManager* cm, SharedStateWriter* stateWriter, int numCvIns, int numCvOuts) :
    id(id),
    moduleGrid(mg),
    moduleDragRules(mg),
    cableManager(cm),
    stateWriter(stateWriter)
{
    for (int cvIdx = 0; cvIdx < numCvIns; cvIdx++) {
        auto newCv = new CVJackComponent(CVInput, cvIdx, id, cm, stateWriter);
        addAndMakeVisible(newCv);
        cvIns.push_back(newCv);
    }
    for (int cvIdx = 0; cvIdx < numCvOuts; cvIdx++) {
        auto newCv = new CVJackComponent(CVOutput, cvIdx, id, cm, stateWriter);
        addAndMakeVisible(newCv);
        cvOuts.push_back(newCv);
    }
}

ModuleUI::~ModuleUI() {
    for (auto cvIn : cvIns) {
        delete cvIn;
    }
    for (auto cvOut : cvOuts) {
        delete cvOut;
    }
}

void ModuleUI::parentHierarchyChanged() {
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

void ModuleUI::mouseDown(const MouseEvent& e) {
    myDragger.startDraggingComponent(this, e);
    moduleGrid->yankModule(id);
}

void ModuleUI::mouseUp(const MouseEvent& e) {
    if (e.mods.isRightButtonDown()) {
        // right click
        PopupMenu m;
        m.addItem(1, "delete");
        m.showMenuAsync(PopupMenu::Options(),
            [this](int result)
            {
                if (result == 1)
                {
                    for (auto cvIn : cvIns) {
                        if (cvIn->isConnected()) {
                            auto other = cvIn->getConnection();
                            cvIn->clearConnection();
                            other->clearConnection();
                            stateWriter->removePatchCable(
                                getId(),
                                cvIn->getId(),
                                other->getModuleId(),
                                other->getId()
                            );
                        }
                    }
                    for (auto cvOut : cvOuts) {
                        if (cvOut->isConnected()) {
                            auto other = cvOut->getConnection();
                            cvOut->clearConnection();
                            other->clearConnection();
                            stateWriter->removePatchCable(
                                other->getModuleId(),
                                other->getId(),
                                getId(),
                                cvOut->getId()
                            );
                        }
                    }

                    stateWriter->deleteModule(getId());
                    moduleGrid->removeModule(getId());
                    delete this;
                }
            });
    }
    else {
        // left click
        moduleGrid->placeModule(id, getBounds());
        stateWriter->moveModule(getId(), getBounds());
    }

}

void ModuleUI::mouseDrag(const MouseEvent& e) {
    myDragger.dragComponent(this, e, &moduleDragRules);

    for (auto child : getChildren()) {
        auto jack = dynamic_cast<CVJackComponent*>(child);
        if (jack) {
            jack->refreshCablePosition();
            if (jack->isConnected()) {
                jack->getConnection()->refreshCablePosition();
            }
        }
    }
}

void ModuleUI::applyState(ModuleState& state) {
    // Load Bounds
    auto bounds = state.getBounds();
    if (moduleGrid->moduleIsPlaced(getId())) {
        moduleGrid->yankModule(getId());
    }
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