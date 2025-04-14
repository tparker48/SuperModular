/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================


SuperModularAudioProcessorEditor::SuperModularAudioProcessorEditor (SuperModularAudioProcessor& p, SharedPluginState* sharedStatePtr)
    : AudioProcessorEditor (&p), audioProcessor (p), sharedState(sharedStatePtr), stateWriter(sharedStatePtr) {
    initModuleFactoryMap(moduleFactories);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    double ratio = double(hpPerRow) / (double(numRows) * 5.0);
    double size = hpPerRow * 60.0;
    setSize(size, size / ratio);
    setLookAndFeel(&customLookAndFeel);

    addAndMakeVisible(cableManager.getDragCable());

    hpWidth = (float)getWidth() / (float)hpPerRow;
    moduleHeight = (float)getHeight() / (float)numRows;

    startTimerHz(2);
    /*
    auto modState1 = ModuleState(1, TestModule::typeId, ModuleBounds(0, 0, TestModule::hp * hpWidth, moduleHeight), 1, 1);
    auto modState2 = ModuleState(2, TestModule::typeId, ModuleBounds(100, 500, TestModule::hp * hpWidth, moduleHeight), 1, 1);
    stateWriter.addModule(modState1);
    stateWriter.addModule(modState2);
    stateWriter.addPatchCable(1, 0, 2, 0);
    */
    auto state = stateWriter.dumpLocalState();

    // first create all modules
    std::map<int, ModuleUIComponent*> modules;
    for (auto moduleState : state.moduleStates) {
        int id = moduleState.getId();
        int moduleTypeId = moduleState.getTypeId();
        modules[id] = moduleFactories[moduleTypeId](id, &moduleGrid, &cableManager, &stateWriter);
        modules[id]->setBounds(moduleState.getBounds());
        moduleGrid.addModule(id, modules[id]);
        addAndMakeVisible(modules[id]);
    }

    // after all modules exist, initialize them
    for (auto moduleState : state.moduleStates) {
        int id = moduleState.getId();
        modules[id]->applyState(moduleState);
    }

}

void SuperModularAudioProcessorEditor::timerCallback() {
    if (!stateWriter.reload(sharedState)) {
        return; // nothing to read
    }

    PluginState state = stateWriter.dumpLocalState();

    // reset mod grid
    moduleGrid.clearAllModules();

    // delete all current modules
    std::vector<ModuleUIComponent*> oldModules;
    for (auto child : getChildren()) {
        ModuleUIComponent* module = dynamic_cast<ModuleUIComponent*>(child);
        if (module) {
            oldModules.push_back(module);
        }
    }
    for (int i = 0; i < oldModules.size(); i++) {
        delete oldModules[i];
    }

    std::map<int, ModuleUIComponent*> modules;
    for (auto moduleState : state.moduleStates) {
        int id = moduleState.getId();
        int moduleTypeId = moduleState.getTypeId();
        modules[id] = moduleFactories[moduleTypeId](id, &moduleGrid, &cableManager, &stateWriter);
        modules[id]->setBounds(moduleState.getBounds());
        moduleGrid.addModule(id, modules[id]);
        addAndMakeVisible(modules[id]);
    }

    // after all modules exist, initialize them
    for (auto moduleState : state.moduleStates) {
        int id = moduleState.getId();
        modules[id]->applyState(moduleState);
    }
}

SuperModularAudioProcessorEditor::~SuperModularAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
  
    stateWriter.saveStateToShared();

    // delete all leftover modules
    std::vector<ModuleUIComponent*> modules;
    for (auto child : getChildren()) {
        ModuleUIComponent* module = dynamic_cast<ModuleUIComponent*>(child);
        if (module) {
            modules.push_back(module);
        }
    }
    for (int i = 0; i < modules.size(); i++) {
        delete modules[i];
    }
}

//==============================================================================
void SuperModularAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
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
        showPopupMenu();
    } 
}

void SuperModularAudioProcessorEditor::showPopupMenu() {
    // right click
    PopupMenu m;
    m.addItem(1, "TestModule");
    m.addItem(2, "Increment comp state");
    m.addItem(3, "Decrement comp state");
    m.showMenuAsync(PopupMenu::Options(),
        [this](int result)
        {
            switch (result) {
            case 1:
                addNewModule<TestModule>();
                break;
            case 2:

                break;
            case 3:

                break;
            default:
                break;
            }
        });

}

static int nextModuleId = 0;

template <typename M>
void SuperModularAudioProcessorEditor::addNewModule() {
    auto newModule = new M(nextModuleId++, &moduleGrid, &cableManager, &stateWriter);
    auto bounds = ModuleBounds(0, 0, hpWidth * M::hp, moduleHeight);
    newModule->setBounds(bounds);

    if (moduleGrid.addModule(newModule->getId(), newModule)) {
        bounds = newModule->getBounds();
        auto module = ModuleState(newModule->getId(), M::typeId, bounds, 1, 1);
        stateWriter.addModule(module);
        addAndMakeVisible(newModule);
    }
}
