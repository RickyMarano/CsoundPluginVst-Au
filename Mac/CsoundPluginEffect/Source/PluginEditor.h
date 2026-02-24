 /*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

// WINDOWS 
//#include <JuceLibraryCode/JuceHeader.h>
//#include "PluginProcessor.h"
//#include "Component/PannelloContenitore.h"

// MAC
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PannelloContenitore.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public AudioProcessorEditor, public ComponentListener
{
public:
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void componentMovedOrResized (Component& component, bool wasMoved, bool wasResized) override;
    
    // void mouseDrag (const MouseEvent& event) override;
 
private:
    NewProjectAudioProcessor& processor;
    juce::ComponentBoundsConstrainer constrainer;
    PannelloContenitore pannelloComponenti;
    
    juce::TextButton buttonAutomazioni{"Automazioni"},buttonPreset{"Preset Manager"};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
