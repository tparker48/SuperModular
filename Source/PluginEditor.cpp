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
    g.fillAll(Colour(0xff121212));

    for (int i = 0; i < numRows+1; i++) {
        int moduleHeight = getHeight() / numRows;
        int barH = getHeight() * 0.02;
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
        for (int j = 0; j < hpPerRow * 2; j++) {
            g.fillEllipse(j * hpWidth/2 + hpWidth/4, holeY, holeDiam, holeDiam);
            g.fillEllipse(j * hpWidth/2 + hpWidth/4, holeY + moduleHeight - barH, holeDiam, holeDiam);
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
    m.addItem(Splitter, "Splitter");
    m.addItem(Mixer, "Mixer");
    m.addItem(Amp, "Amp");
    m.addItem(OscillatorModule, "Oscillator");
    m.addItem(OldOscillatorModule, "OldOscillator");
    m.addItem(Noise, "Noise");
    m.addItem(Filter, "Filter");
    m.addItem(DelayModule, "Delay");
    m.addItem(DistortionModule, "Distortion");
    m.addItem(Sequencer, "Sequencer");
    m.addItem(Clock, "Clock");
    m.addItem(JoystickModule, "Joystick");
    m.showMenuAsync(PopupMenu::Options(),
        [this, e](int result)
        {
            switch (result) {
            case AudioOutput:
                addNewModule<AudioOutputUI>(AudioOutput, e.getPosition());
                break;
            case OscillatorModule:
                addNewModule<OscillatorUI>(OscillatorModule, e.getPosition());
                break;
            case OldOscillatorModule:
                addNewModule<OldOscillatorUI>(OldOscillatorModule, e.getPosition());
                break;
            case Noise:
                addNewModule<NoiseUI>(Noise, e.getPosition());
                break;
            case Filter:
                addNewModule<FilterUI>(Filter, e.getPosition());
                break;
            case DelayModule:
                addNewModule<DelayUI>(DelayModule, e.getPosition());
                break;
            case DistortionModule:
                addNewModule<DistortionUI>(DistortionModule, e.getPosition());
                break;
            case Splitter:
                addNewModule<SplitterUI>(Splitter, e.getPosition());
                break;
            case Mixer:
                addNewModule<MixerUI>(Mixer, e.getPosition());
                break;
            case Amp:
                addNewModule<AmpUI>(Amp, e.getPosition());
                break;
            case JoystickModule:
                addNewModule<JoystickUI>(JoystickModule, e.getPosition());
                break;
            case Sequencer:
                addNewModule<SequencerUI>(Sequencer, e.getPosition());
                break;
            case Clock:
                addNewModule<ClockUI>(Clock, e.getPosition());
                break;
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
