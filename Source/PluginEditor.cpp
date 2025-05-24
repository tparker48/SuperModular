/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MenuLookAndFeel.h"

//==============================================================================

static int nextModuleId = 0;

SuperModularAudioProcessorEditor::SuperModularAudioProcessorEditor (SuperModularAudioProcessor& p, SharedPluginState* sharedStatePtr)
    : AudioProcessorEditor (&p), audioProcessor (p), sharedState(sharedStatePtr), stateWriter(sharedStatePtr) {
    initModuleUIFactoryMap(moduleFactories);
    double size = 1200;
    headerOffset = getHeight() * 0.03;
    aspectRatio = double(hpPerRow) / (double(numRows)*5.0);
    setSize(size, size / aspectRatio);

    hpWidth = (float)getWidth() / (float)hpPerRow;
    moduleHeight = (float)(getHeight() - headerOffset) / (float)numRows;
    setResizeLimits(1000, 1000 / aspectRatio, 1800, 1800/aspectRatio);
    getConstrainer()->setFixedAspectRatio(aspectRatio);
    setResizable(false, true);

    setLookAndFeel(&customLookAndFeel);
    addAndMakeVisible(cableManager.getDragCable());

    addAndMakeVisible(plusRowsButton);
    addAndMakeVisible(minusRowsButton);
    plusRowsButton.setButtonText("+");
    minusRowsButton.setButtonText("-");

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
        modules[id]->setBounds(moduleGrid.getBoundsFromRackPosition(moduleState.getBounds()));
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
    g.setColour(Colour(0xff202020));
    g.fillAll();

    //g.setColour(Colours::white);
    //g.fillRect(Rectangle<float>(0, 0, getWidth(), headerOffset));
    g.setColour(Colours::white);
    g.fillRect(Rectangle<float>(0, 0, getWidth(), headerOffset));

    for (int i = 0; i < numRows+1; i++) {
        int barH = moduleHeight * 0.08;
        int barY = i * moduleHeight + headerOffset;

        auto checker = ImageCache::getFromMemory(BinaryData::checker3_png, BinaryData::checker3_pngSize);
        checker = checker.getClippedImage(Rectangle<int>(0, barY, getWidth(), barH));
        g.drawImage(checker, Rectangle<float>(0, barY, getWidth(), barH), RectanglePlacement::doNotResize, false);
        g.drawImage(checker, Rectangle<float>(0, barY + moduleHeight - barH, getWidth(), barH), RectanglePlacement::doNotResize, false);

        g.setColour(Colours::black);
        g.drawRect(Rectangle<float>(-5, barY, getWidth()+10, barH), 1.0);
        g.drawRect(Rectangle<float>(-5, barY + moduleHeight - barH, getWidth()+10, barH), 1.0);
    }

    auto buttonWidth = plusRowsButton.getWidth();
    auto center = (plusRowsButton.getX() + minusRowsButton.getX()) / 2 + buttonWidth / 2;
    g.setColour(Colours::black);
    g.drawText("Rows", Rectangle<float>(center - buttonWidth, 0, 2 * buttonWidth, buttonWidth), Justification::centred, true);
}

void SuperModularAudioProcessorEditor::resized()
{
    headerOffset = 23;//getHeight() * 0.035;
    hpWidth = round((float)getWidth() / (float)hpPerRow);
    moduleHeight = (float)(getHeight() - headerOffset) / (float)numRows;

    moduleGrid.setRackDimensions(numRows, moduleHeight, hpWidth, hpPerRow, headerOffset);
    moduleGrid.resized();

    auto buttonSize = headerOffset;
    minusRowsButton.setBounds(0, 0, buttonSize, buttonSize);
    plusRowsButton.setBounds(buttonSize*3, 0, buttonSize, buttonSize);
}

void SuperModularAudioProcessorEditor::mouseUp(const MouseEvent& e) {
    if (e.mods.isRightButtonDown()) {
        // right click
        showPopupMenu(e);
    } 
}

void SuperModularAudioProcessorEditor::showPopupMenu(const MouseEvent& e) {
    PopupMenu m;
    m.setLookAndFeel(&menuLookAndFeel);
    m.addItem(AudioOutput, "Audio Output");
    m.addItem(AudioInput, "Audio Input");
    m.addItem(Splitter, "Splitter");
    m.addItem(Mixer, "Mixer");
    m.addItem(Amp, "Amp");
    m.addItem(OscillatorModule, "Oscillator");
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
            case AudioInput:
                addNewModule<AudioInputUI>(AudioInput, e.getPosition());
                break;
            case OscillatorModule:
                addNewModule<OscillatorUI>(OscillatorModule, e.getPosition());
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
    else if (typeId == AudioInput) {
        auto localState = stateWriter.dumpLocalState();
        for (auto module : localState.moduleStates) {
            if ((ModuleType)module.getTypeId() == AudioInput) {
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
            moduleGrid.getRackPosition(bounds), 
            newModule->getNumCVInputs(),
            newModule->getNumCVOutputs()
        );
        stateWriter.addModule(module);
        newModule->startListeners();
        addAndMakeVisible(newModule);
    }
}
