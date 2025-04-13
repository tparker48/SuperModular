/*
  ==============================================================================

    ModuleState.h
    Created: 12 Apr 2025 7:14:56pm
    Author:  Tom

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

// The full state of a module
class ModuleState {
public:
    ModuleState() {}
    ModuleState(int id);
    ModuleState(int id, int typeId, Rectangle<int> bounds, int numCvInputs = 0, int numCvOutputs = 0);
    ModuleState(XmlElement& xml);

    XmlElement* toXml();

    int getId() const;
    int getTypeId() const;

    void setBounds(Rectangle<int> bounds);
    Rectangle<int> getBounds();

    int getNumCvInputs();
    int getNumCvOutputs();
    std::pair<int, int> getInputCvConnection(int cvId);
    std::pair<int, int> getOutputCvConnection(int cvId);
    void setInputCvConnection(int cvId, int otherModuleId, int otherCvId);
    void setOutputCvConnection(int cvId, int otherModuleId, int otherCvId);

    ValueTree state;
private:
    std::pair<int, int> getCvConnection(int cvId, Identifier cvTagName);
    void setCvConnection(int cvId, int otherModuleId, int otherCvId, Identifier cvTagName);
};