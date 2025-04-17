/*
  ==============================================================================

    ModuleState.cpp
    Created: 12 Apr 2025 7:14:56pm
    Author:  Tom

  ==============================================================================
*/

#include "ModuleState.h"

ModuleState::ModuleState(int id) : state("module") {
    state.setProperty("id", id, nullptr);
}

ModuleState::ModuleState(int id, int typeId, Rectangle<int> bounds, int numCvInputs, int numCvOutputs) : state("module") {
    setBounds(bounds);
    state.setProperty("id", id, nullptr);
    state.setProperty("type", typeId, nullptr);
    state.setProperty("num_cv_in", numCvInputs, nullptr);
    state.setProperty("num_cv_out", numCvOutputs, nullptr);

    // init cv ports
    auto ins = ValueTree("cv_in");
    for (int i = 0; i < numCvInputs; i++) {
        auto cv = ValueTree(Identifier("cv_in."+std::to_string(i)));
        cv.setProperty("module", -1, nullptr);
        cv.setProperty("cv", -1, nullptr);
        ins.appendChild(cv, nullptr);
    }
    state.appendChild(ins, nullptr);

    // init cv ports
    auto outs = ValueTree("cv_out");
    for (int i = 0; i < numCvOutputs; i++) {
        auto cv = ValueTree(Identifier("cv_out."+std::to_string(i)));
        cv.setProperty("module", -1, nullptr);
        cv.setProperty("cv", -1, nullptr);
        outs.appendChild(cv, nullptr);
    }
    state.appendChild(outs, nullptr);
}

ModuleState::ModuleState(XmlElement& xml) {
    state = ValueTree::fromXml(xml);
}

XmlElement* ModuleState::toXml() {
    return state.createXml().release();
}

int ModuleState::getId() const {
    return state["id"];
}

int ModuleState::getTypeId() const {
    return state["type"];
}

void ModuleState::setBounds(Rectangle<int> bounds) {
    state.setProperty("x", bounds.getX(), nullptr);
    state.setProperty("y", bounds.getY(), nullptr);
    state.setProperty("w", bounds.getWidth(), nullptr);
    state.setProperty("h", bounds.getHeight(), nullptr);
}

Rectangle<int> ModuleState::getBounds() {
    return Rectangle<int>(state["x"], state["y"], state["w"], state["h"]);
}

int ModuleState::getNumCvInputs() {
    return state["num_cv_in"];
}
int ModuleState::getNumCvOutputs() {
    return state["num_cv_out"];
}

std::pair<int, int> ModuleState::getInputCvConnection(int cvId) {
    return getCvConnection(cvId, "cv_in");
}
std::pair<int, int> ModuleState::getOutputCvConnection(int cvId) {
    return getCvConnection(cvId, "cv_out");
}

void ModuleState::setInputCvConnection(int cvId, int otherModuleId, int otherCvId) {
    setCvConnection(cvId, otherModuleId, otherCvId, "cv_in");
}
void ModuleState::setOutputCvConnection(int cvId, int otherModuleId, int otherCvId) {
    setCvConnection(cvId, otherModuleId, otherCvId, "cv_out");
}

std::pair<int, int> ModuleState::getCvConnection(int cvId, Identifier cvTagName) {
    auto cvJacks = state.getChildWithName(cvTagName);
    if (!cvJacks.isValid()) {
        return { -1,-1 };
    }

    auto cvJack = cvJacks.getChildWithName(Identifier(cvTagName + "." + std::to_string(cvId)));
    if (!cvJack.isValid()) {
        return { -1,-1 };
    }

    else {
        return { cvJack["module"], cvJack["cv"] };
    }
}

void ModuleState::setCvConnection(int cvId, int otherModuleId, int otherCvId, Identifier cvTagName) {
    auto cvJacks = state.getChildWithName(cvTagName);
    if (!cvJacks.isValid()) {
        return;
    }

    auto cvJack = cvJacks.getChildWithName(cvTagName+"."+Identifier(std::to_string(cvId)));
    if (!cvJack.isValid()) {
        return;
    }

    cvJack.setProperty("module", otherModuleId, nullptr);
    cvJack.setProperty("cv", otherCvId, nullptr);
}
