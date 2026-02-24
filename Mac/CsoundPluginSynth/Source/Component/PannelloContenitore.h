#ifndef PANNELLOCONTENITORE_H
#define PANNELLOCONTENITORE_H

// WINDOWS
//#include <JuceLibraryCode/JuceHeader.h>
//#include "MyPresetManager.h"
//#include "SliderAutomazioni.h"

// MAC
#include <JuceHeader.h>
#include "MyPresetManager.h"
#include "SliderAutomazioni.h"

class PannelloContenitore  : public juce::Component
{
public:
    //==============================================================================
    PannelloContenitore(NewProjectAudioProcessor& p): processor(p),sliderAutomazioni(p),myPresetManager(p)
    {   
        addChildComponent(myPresetManager);
        addChildComponent(sliderAutomazioni);     
   
    }
    ~PannelloContenitore() override {}

    //==============================================================================
    void paint (juce::Graphics& g) override
    {
       
    }

    void resized() override
    {
        myPresetManager.setBounds(getLocalBounds());
        sliderAutomazioni.setBounds(getLocalBounds());  
    }

public:
    NewProjectAudioProcessor &processor;
    MyPresetManager myPresetManager;
    SliderAutomazioni sliderAutomazioni;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PannelloContenitore)

};

#endif // PANNELLOCONTENITORE_H
