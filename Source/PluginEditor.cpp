/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================

static int nextModuleId = 0;

SuperModularAudioProcessorEditor::SuperModularAudioProcessorEditor (SuperModularAudioProcessor& p, SharedPluginState* sharedStatePtr)
    : AudioProcessorEditor (&p), audioProcessor (p), sharedState(sharedStatePtr), stateWriter(sharedStatePtr) {
    initModuleUIFactoryMap(moduleFactories);
    double ratio = double(hpPerRow) / (double(numRows) * 5.0);
    double size = hpPerRow * 60.0;
    hpWidth = (float)getWidth() / (float)hpPerRow;
    moduleHeight = (float)getHeight() / (float)numRows;
    
    setSize(size, size / ratio);
    setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(cableManager.getDragCable());

    loadState();
    startTimerHz(3);
}

SuperModularAudioProcessorEditor::~SuperModularAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    stateWriter.saveStateToShared();
    clearState();
}

void SuperModularAudioProcessorEditor::timerCallback() {
    if (stateWriter.reload(sharedState)) {
        clearState();
        loadState();
    }

    sharedState->switchWriteIdx();
}


void SuperModularAudioProcessorEditor::loadState() {
    auto state = stateWriter.dumpLocalState();
    auto maxId = -1;

    std::map<int, ModuleUI*> modules;
    for (auto moduleState : state.moduleStates) {
        int id = moduleState.getId();
        int moduleTypeId = moduleState.getTypeId();
        modules[id] = moduleFactories[(ModuleType)moduleTypeId](id, &moduleGrid, &cableManager, &stateWriter);
        modules[id]->setBounds(moduleState.getBounds());
        moduleGrid.addModule(id, modules[id]);
        addAndMakeVisible(modules[id]);
        maxId = std::max(id, maxId);
    }
    for (auto moduleState : state.moduleStates) {
        int id = moduleState.getId();
        modules[id]->applyState(moduleState);
        modules[id]->startListeners();
    }

    nextModuleId = maxId + 1;
}

void SuperModularAudioProcessorEditor::clearState() {
    moduleGrid.clearAllModules();

    std::vector<ModuleUI*> oldModules;
    for (auto child : getChildren()) {
        ModuleUI* module = dynamic_cast<ModuleUI*>(child);
        if (module) {
            oldModules.push_back(module);
        }
    }
    for (int i = 0; i < oldModules.size(); i++) {
        delete oldModules[i];
    }
}


//==============================================================================
void SuperModularAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour(ResizableWindow::backgroundColourId));


    for (int i = 0; i < numRows+1; i++) {
        int moduleHeight = getHeight() / numRows;
        int barH = getHeight() / 40;
        int barY = i * moduleHeight;

        g.setColour(Colour(0xff1f1f1f));
        g.fillRect(0, barY, getWidth(), barH);
        g.fillRect(0, barY + moduleHeight - barH, getWidth(), barH);

        if (i < numRows) {
            g.setColour(Colour(0xff050505));
            g.fillRect(0, barY - 1, getWidth(), 2);
        }

        int hpWidth = getWidth() / hpPerRow;
        int holeRadius = getHeight() / 400;
        int holeDiam = holeRadius * 2;
        int holeY = barY + barH / 2 - holeRadius;
        for (int j = 0; j < hpPerRow; j++) {
            g.fillEllipse(j * hpWidth + hpWidth/2, holeY, holeDiam, holeDiam);
            g.fillEllipse(j * hpWidth + hpWidth/2, holeY + moduleHeight - barH, holeDiam, holeDiam);
        }
    }
}

void SuperModularAudioProcessorEditor::resized()
{
    hpWidth = (float)getWidth() / (float)hpPerRow;
    moduleHeight = (float)getHeight() / (float)numRows;
    moduleGrid.setRackDimensions(numRows, moduleHeight, hpWidth, hpPerRow);
}

void SuperModularAudioProcessorEditor::mouseUp(const MouseEvent& e) {
    if (e.mods.isRightButtonDown()) {
        // right click
        showPopupMenu(e);
    } 
}

void SuperModularAudioProcessorEditor::showPopupMenu(const MouseEvent& e) {
    PopupMenu m;
    m.addItem(AudioOutput, "Audio Out");
    m.addItem(Oscillator, "Oscillator");
    m.addItem(Splitter, "Splitter");
    m.showMenuAsync(PopupMenu::Options(),
        [this, e](int result)
        {
            switch (result) {
            case AudioOutput:
                addNewModule<AudioOutputUI>(AudioOutput, e.getPosition());
                break;
            case Oscillator:
                addNewModule<OscillatorUI>(Oscillator, e.getPosition());
                break;
            case Splitter:
                addNewModule<SplitterUI>(Splitter, e.getPosition());
            default:
                break;
            }
        });
}

template <typename M>
void SuperModularAudioProcessorEditor::addNewModule(ModuleType typeId, Point<int> clickLocation) {
    if (typeId == AudioOutput) {
        auto localState = stateWriter.dumpLocalState();
        for (auto module : localState.moduleStates) {
            if((ModuleType)module.getTypeId() == AudioOutput) {
                return;
            }
        }
    }

    auto newModule = new M(nextModuleId++, &moduleGrid, &cableManager, &stateWriter);
    auto width = hpWidth * M::hp;
    auto x = clickLocation.getX() - (width / 2);
    auto y = clickLocation.getY() - (moduleHeight / 2);
    auto bounds = ModuleBounds(x, y, width, moduleHeight);
    newModule->setBounds(bounds);

    if (moduleGrid.addModule(newModule->getId(), newModule)) {
        bounds = newModule->getBounds();
        auto module = ModuleState(
            newModule->getId(), 
            typeId,
            bounds, 
            newModule->getNumCVInputs(),
            newModule->getNumCVOutputs()
        );
        stateWriter.addModule(module);
        newModule->startListeners();
        addAndMakeVisible(newModule);
    }
}
