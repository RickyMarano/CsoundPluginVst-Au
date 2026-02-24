#pragma once

// WINDOWS
#include <JuceLibraryCode/JuceHeader.h>
#include "Source/PluginProcessor.h"
#include "MySlider.h"
#include "MidiLearnParameters.h"

// MAC
// #include <JuceHeader.h>
// #include "PluginProcessor.h"
// #include "MySlider.h"
// #include "MidiLearnParameters.h"

//==============================================================================
class SliderAutomazioni  : public juce::Component,juce::Timer
{
public:
    //==============================================================================
    SliderAutomazioni(NewProjectAudioProcessor& p): processor(p),midi(p.midiLearn),buttonMidiLearn("MIDI LEARN")
    {        
        addAndMakeVisible(viewPort);
        addAndMakeVisible(buttonMidiLearn);
        viewPort.setViewedComponent(&SliderContenitore,false);
        
        MidiLearn();
        deleteMidiLearn();
        
        CreateSlider();
        AttachmentParametro();
        
        buttonMidiLearn.onClick = [this]()
        {
            if (!isLearning)
            {
                buttonMidiLearn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::blue);
                isLearning = true;
                RemoveSelectParam();
                viewSliderOverly(isLearning);
                startTimer(100);
            }
            else
            {
                buttonMidiLearn.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::grey);
                processor.midiLearn.cancelLearn();
                isLearning = false;
                RemoveSelectParam();
                viewSliderOverly(isLearning);
                stopTimer();
            }
        };
    }

    ~SliderAutomazioni() override 
    {
        stopTimer();
    }
    
    void timerCallback() override
    {
        textSliderNumberCC();
    }
    //==============================================================================
    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::grey);
    }
    void viewSliderOverly(bool isVisible)
    {
        par01.setVisibleOverly(isVisible);
        par02.setVisibleOverly(isVisible);
        par03.setVisibleOverly(isVisible);
        par04.setVisibleOverly(isVisible);
        par05.setVisibleOverly(isVisible);
        par06.setVisibleOverly(isVisible);
        par07.setVisibleOverly(isVisible);
        par08.setVisibleOverly(isVisible);
        par09.setVisibleOverly(isVisible);
        par10.setVisibleOverly(isVisible);
        par11.setVisibleOverly(isVisible);
        par12.setVisibleOverly(isVisible);
        par13.setVisibleOverly(isVisible);
        par14.setVisibleOverly(isVisible);
        par15.setVisibleOverly(isVisible);
        par16.setVisibleOverly(isVisible);
        par17.setVisibleOverly(isVisible);
        par18.setVisibleOverly(isVisible);
        par19.setVisibleOverly(isVisible);
        par20.setVisibleOverly(isVisible);
        par21.setVisibleOverly(isVisible);
        par22.setVisibleOverly(isVisible);
        par23.setVisibleOverly(isVisible);
        par24.setVisibleOverly(isVisible);
        par25.setVisibleOverly(isVisible);
        par26.setVisibleOverly(isVisible);
        par27.setVisibleOverly(isVisible);
        par28.setVisibleOverly(isVisible);
        par29.setVisibleOverly(isVisible);
        par30.setVisibleOverly(isVisible);
        par31.setVisibleOverly(isVisible);
        par32.setVisibleOverly(isVisible);

    }
    void deleteMidiLearn()
    {
        par01.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par01"))
            {
                midi.unlearn("Par01");
                par01.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par02.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par02"))
            {
                midi.unlearn("Par02");
                par02.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par03.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par03"))
            {
                midi.unlearn("Par03");
                par03.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par04.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par04"))
            {
                midi.unlearn("Par04");
                par04.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par05.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par05"))
            {
                midi.unlearn("Par05");
                par05.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par06.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par06"))
            {
                midi.unlearn("Par06");
                par06.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par07.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par07"))
            {
                midi.unlearn("Par07");
                par07.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par08.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par08"))
            {
                midi.unlearn("Par08");
                par08.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par09.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par09"))
            {
                midi.unlearn("Par09");
                par09.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par10.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par10"))
            {
                midi.unlearn("Par10");
                par10.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par11.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par11"))
            {
                midi.unlearn("Par11");
                par11.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par12.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par12"))
            {
                midi.unlearn("Par12");
                par12.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par13.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par13"))
            {
                midi.unlearn("Par13");
                par13.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par14.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par14"))
            {
                midi.unlearn("Par14");
                par14.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par15.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par15"))
            {
                midi.unlearn("Par15");
                par15.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par16.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par16"))
            {
                midi.unlearn("Par16");
                par16.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par17.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par17"))
            {
                midi.unlearn("Par17");
                par17.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par18.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par18"))
            {
                midi.unlearn("Par18");
                par18.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par19.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par19"))
            {
                midi.unlearn("Par19");
                par19.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par20.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par20"))
            {
                midi.unlearn("Par20");
                par20.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par21.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par21"))
            {
                midi.unlearn("Par21");
                par21.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par22.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par22"))
            {
                midi.unlearn("Par22");
                par22.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par23.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par23"))
            {
                midi.unlearn("Par23");
                par23.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par24.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par24"))
            {
                midi.unlearn("Par24");
                par24.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par25.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par25"))
            {
                midi.unlearn("Par25");
                par25.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par26.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par26"))
            {
                midi.unlearn("Par26");
                par26.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par27.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par27"))
            {
                midi.unlearn("Par27");
                par27.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par28.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par28"))
            {
                midi.unlearn("Par28");
                par28.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par29.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par29"))
            {
                midi.unlearn("Par29");
                par29.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par30.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par30"))
            {
                midi.unlearn("Par30");
                par30.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par31.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par31"))
            {
                midi.unlearn("Par31");
                par31.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

        par32.comp.unLearn = [this]()
        {
            if (midi.isParameterMapped("Par32"))
            {
                midi.unlearn("Par32");
                par32.comp.setTextCC("NoMap");
                RemoveSelectParam();
            }
        };

    }
    
    void MidiLearn()
    {
        par01.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par01.comp.setSelect(true);

            if (!midi.isParameterMapped("Par01"))
                midi.beginLearn("Par01");
        };

        par02.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par02.comp.setSelect(true);

            if (!midi.isParameterMapped("Par02"))
                midi.beginLearn("Par02");
        };

        par03.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par03.comp.setSelect(true);

            if (!midi.isParameterMapped("Par03"))
                midi.beginLearn("Par03");
        };

        par04.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par04.comp.setSelect(true);

            if (!midi.isParameterMapped("Par04"))
                midi.beginLearn("Par04");
        };

        par05.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par05.comp.setSelect(true);

            if (!midi.isParameterMapped("Par05"))
                midi.beginLearn("Par05");
        };

        par06.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par06.comp.setSelect(true);

            if (!midi.isParameterMapped("Par06"))
                midi.beginLearn("Par06");
        };

        par07.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par07.comp.setSelect(true);

            if (!midi.isParameterMapped("Par07"))
                midi.beginLearn("Par07");
        };

        par08.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par08.comp.setSelect(true);

            if (!midi.isParameterMapped("Par08"))
                midi.beginLearn("Par08");
        };

        par09.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par09.comp.setSelect(true);

            if (!midi.isParameterMapped("Par09"))
                midi.beginLearn("Par09");
        };

        par10.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par10.comp.setSelect(true);

            if (!midi.isParameterMapped("Par10"))
                midi.beginLearn("Par10");
        };

        par11.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par11.comp.setSelect(true);

            if (!midi.isParameterMapped("Par11"))
                midi.beginLearn("Par11");
        };

        par12.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par12.comp.setSelect(true);

            if (!midi.isParameterMapped("Par12"))
                midi.beginLearn("Par12");
        };

        par13.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par13.comp.setSelect(true);

            if (!midi.isParameterMapped("Par13"))
                midi.beginLearn("Par13");
        };

        par14.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par14.comp.setSelect(true);

            if (!midi.isParameterMapped("Par14"))
                midi.beginLearn("Par14");
        };

        par15.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par15.comp.setSelect(true);

            if (!midi.isParameterMapped("Par15"))
                midi.beginLearn("Par15");
        };

        par16.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par16.comp.setSelect(true);

            if (!midi.isParameterMapped("Par16"))
                midi.beginLearn("Par16");
        };

        par17.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par17.comp.setSelect(true);

            if (!midi.isParameterMapped("Par17"))
                midi.beginLearn("Par17");
        };

        par18.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par18.comp.setSelect(true);

            if (!midi.isParameterMapped("Par18"))
                midi.beginLearn("Par18");
        };

        par19.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par19.comp.setSelect(true);

            if (!midi.isParameterMapped("Par19"))
                midi.beginLearn("Par19");
        };

        par20.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par20.comp.setSelect(true);

            if (!midi.isParameterMapped("Par20"))
                midi.beginLearn("Par20");
        };

        par21.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par21.comp.setSelect(true);

            if (!midi.isParameterMapped("Par21"))
                midi.beginLearn("Par21");
        };

        par22.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par22.comp.setSelect(true);

            if (!midi.isParameterMapped("Par22"))
                midi.beginLearn("Par22");
        };

        par23.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par23.comp.setSelect(true);

            if (!midi.isParameterMapped("Par23"))
                midi.beginLearn("Par23");
        };

        par24.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par24.comp.setSelect(true);

            if (!midi.isParameterMapped("Par24"))
                midi.beginLearn("Par24");
        };

        par25.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par25.comp.setSelect(true);

            if (!midi.isParameterMapped("Par25"))
                midi.beginLearn("Par25");
        };

        par26.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par26.comp.setSelect(true);

            if (!midi.isParameterMapped("Par26"))
                midi.beginLearn("Par26");
        };

        par27.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par27.comp.setSelect(true);

            if (!midi.isParameterMapped("Par27"))
                midi.beginLearn("Par27");
        };

        par28.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par28.comp.setSelect(true);

            if (!midi.isParameterMapped("Par28"))
                midi.beginLearn("Par28");
        };

        par29.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par29.comp.setSelect(true);

            if (!midi.isParameterMapped("Par29"))
                midi.beginLearn("Par29");
        };

        par30.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par30.comp.setSelect(true);

            if (!midi.isParameterMapped("Par30"))
                midi.beginLearn("Par30");
        };

        par31.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par31.comp.setSelect(true);

            if (!midi.isParameterMapped("Par31"))
                midi.beginLearn("Par31");
        };

        par32.comp.selectPar = [this]()
        {
            RemoveSelectParam();
            par32.comp.setSelect(true);

            if (!midi.isParameterMapped("Par32"))
                midi.beginLearn("Par32");
        };

    }
    
    void textSliderNumberCC()
    {
        if (midi.isParameterMapped("Par01"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par01"));
            par01.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par02"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par02"));
            par02.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par03"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par03"));
            par03.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par04"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par04"));
            par04.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par05"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par05"));
            par05.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par06"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par06"));
            par06.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par07"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par07"));
            par07.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par08"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par08"));
            par08.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par09"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par09"));
            par09.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par10"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par10"));
            par10.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par11"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par11"));
            par11.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par12"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par12"));
            par12.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par13"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par13"));
            par13.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par14"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par14"));
            par14.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par15"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par15"));
            par15.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par16"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par16"));
            par16.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par17"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par17"));
            par17.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par18"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par18"));
            par18.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par19"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par19"));
            par19.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par20"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par20"));
            par20.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par21"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par21"));
            par21.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par22"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par22"));
            par22.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par23"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par23"));
            par23.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par24"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par24"));
            par24.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par25"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par25"));
            par25.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par26"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par26"));
            par26.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par27"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par27"));
            par27.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par28"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par28"));
            par28.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par29"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par29"));
            par29.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par30"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par30"));
            par30.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par31"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par31"));
            par31.comp.setTextCC(CC);
        }
        if (midi.isParameterMapped("Par32"))
        {
            juce::String CC = juce::String(midi.getCCForParameter("Par32"));
            par32.comp.setTextCC(CC);
        }

    }
    void RemoveSelectParam()
    {
        par01.comp.setSelect(false);
        par02.comp.setSelect(false);
        par03.comp.setSelect(false);
        par04.comp.setSelect(false);
        par05.comp.setSelect(false);
        par06.comp.setSelect(false);
        par07.comp.setSelect(false);
        par08.comp.setSelect(false);
        par09.comp.setSelect(false);
        par10.comp.setSelect(false);
        par11.comp.setSelect(false);
        par12.comp.setSelect(false);
        par13.comp.setSelect(false);
        par14.comp.setSelect(false);
        par15.comp.setSelect(false);
        par16.comp.setSelect(false);
        par17.comp.setSelect(false);
        par18.comp.setSelect(false);
        par19.comp.setSelect(false);
        par20.comp.setSelect(false);
        par21.comp.setSelect(false);
        par22.comp.setSelect(false);
        par23.comp.setSelect(false);
        par24.comp.setSelect(false);
        par25.comp.setSelect(false);
        par26.comp.setSelect(false);
        par27.comp.setSelect(false);
        par28.comp.setSelect(false);
        par29.comp.setSelect(false);
        par30.comp.setSelect(false);
        par31.comp.setSelect(false);
        par32.comp.setSelect(false);

    }
    void resized() override
    {
        viewPort.setBounds(0,25,getLocalBounds().getWidth(),getLocalBounds().getHeight()-25);
        buttonMidiLearn.setBounds(0,0,200,25);

        par01.setBounds(50, 0,  250, 25);
        par02.setBounds(50, 25,  250, 25);
        par03.setBounds(50, 50,  250, 25);
        par04.setBounds(50, 75,  250, 25);
        par05.setBounds(50, 100, 250, 25);
        par06.setBounds(50, 125, 250, 25);
        par07.setBounds(50, 150, 250, 25);
        par08.setBounds(50, 175, 250, 25);
        par09.setBounds(50, 200, 250, 25);
        par10.setBounds(50, 225, 250, 25);
        par11.setBounds(50, 250, 250, 25);
        par12.setBounds(50, 275, 250, 25);
        par13.setBounds(50, 300, 250, 25);
        par14.setBounds(50, 325, 250, 25);
        par15.setBounds(50, 350, 250, 25);
        par16.setBounds(50, 375, 250, 25);
        par17.setBounds(50, 400, 250, 25);
        par18.setBounds(50, 425, 250, 25);
        par19.setBounds(50, 450, 250, 25);
        par20.setBounds(50, 475, 250, 25);
        par21.setBounds(50, 500, 250, 25);
        par22.setBounds(50, 525, 250, 25);
        par23.setBounds(50, 550, 250, 25);
        par24.setBounds(50, 575, 250, 25);
        par25.setBounds(50, 600, 250, 25);
        par26.setBounds(50, 625, 250, 25);
        par27.setBounds(50, 650, 250, 25);
        par28.setBounds(50, 675, 250, 25);
        par29.setBounds(50, 700, 250, 25);
        par30.setBounds(50, 725, 250, 25);
        par31.setBounds(50, 750, 250, 25);        
        par32.setBounds(50, 775, 250, 25);
        
        SliderContenitore.setBounds(0,0,300,825);

    }
    void AttachmentParametro()
    {
        par01Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par01", par01));
        par02Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par02", par02));
        par03Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par03", par03));
        par04Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par04", par04));
        par05Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par05", par05));
        par06Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par06", par06));
        par07Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par07", par07));
        par08Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par08", par08));
        par09Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par09", par09));
        par10Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par10", par10));
        par11Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par11", par11));
        par12Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par12", par12));
        par13Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par13", par13));
        par14Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par14", par14));
        par15Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par15", par15));
        par16Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par16", par16));
        par17Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par17", par17));
        par18Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par18", par18));
        par19Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par19", par19));
        par20Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par20", par20));
        par21Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par21", par21));
        par22Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par22", par22));
        par23Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par23", par23));
        par24Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par24", par24));
        par25Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par25", par25));
        par26Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par26", par26));
        par27Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par27", par27));
        par28Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par28", par28));
        par29Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par29", par29));
        par30Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par30", par30));
        par31Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par31", par31));
        par32Attachment.reset(new AudioProcessorValueTreeState::SliderAttachment(processor.parameters, "Par32", par32));
       
    }
    void CreateSlider()
    {
        SliderContenitore.addAndMakeVisible(par01);
        par01.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par01.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par01.setRange(0.0f, 1.0f);
        par01Label.setText("Par01", dontSendNotification);
        par01Label.attachToComponent(&par01,true);
        SliderContenitore.addAndMakeVisible(par01Label);

        SliderContenitore.addAndMakeVisible(par02);
        par02.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par02.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par02.setRange(0.0f, 1.0f);
        par02Label.setText("Par02", dontSendNotification);
        par02Label.attachToComponent(&par02,true);
        SliderContenitore.addAndMakeVisible(par02Label);

        SliderContenitore.addAndMakeVisible(par03);
        par03.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par03.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par03.setRange(0.0f, 1.0f);
        par03Label.setText("Par03", dontSendNotification);
        par03Label.attachToComponent(&par03,true);
        SliderContenitore.addAndMakeVisible(par03Label);

        SliderContenitore.addAndMakeVisible(par04);
        par04.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par04.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par04.setRange(0.0f, 1.0f);
        par04Label.setText("Par04", dontSendNotification);
        par04Label.attachToComponent(&par04,true);
        SliderContenitore.addAndMakeVisible(par04Label);
        
        SliderContenitore.addAndMakeVisible(par05);
        par05.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par05.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par05.setRange(0.0f, 1.0f);
        par05Label.setText("Par05", dontSendNotification);
        par05Label.attachToComponent(&par05,true);
        SliderContenitore.addAndMakeVisible(par05Label);

        SliderContenitore.addAndMakeVisible(par06);
        par06.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par06.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par06.setRange(0.0f, 1.0f);
        par06Label.setText("Par06", dontSendNotification);
        par06Label.attachToComponent(&par06,true);
        SliderContenitore.addAndMakeVisible(par06Label);

        SliderContenitore.addAndMakeVisible(par07);
        par07.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par07.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par07.setRange(0.0f, 1.0f);
        par07Label.setText("Par07", dontSendNotification);
        par07Label.attachToComponent(&par07,true);
        SliderContenitore.addAndMakeVisible(par07Label);

        SliderContenitore.addAndMakeVisible(par08);
        par08.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par08.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par08.setRange(0.0f, 1.0f);
        par08Label.setText("Par08", dontSendNotification);
        par08Label.attachToComponent(&par08,true);
        SliderContenitore.addAndMakeVisible(par08Label);

        SliderContenitore.addAndMakeVisible(par09);
        par09.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par09.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par09.setRange(0.0f, 1.0f);
        par09Label.setText("Par09", dontSendNotification);
        par09Label.attachToComponent(&par09,true);
        SliderContenitore.addAndMakeVisible(par09Label);

        SliderContenitore.addAndMakeVisible(par10);
        par10.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par10.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par10.setRange(0.0f, 1.0f);
        par10Label.setText("Par10", dontSendNotification);
        par10Label.attachToComponent(&par10,true);
        SliderContenitore.addAndMakeVisible(par10Label);

        SliderContenitore.addAndMakeVisible(par11);
        par11.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par11.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par11.setRange(0.0f, 1.0f);
        par11Label.setText("Par11", dontSendNotification);
        par11Label.attachToComponent(&par11,true);
        SliderContenitore.addAndMakeVisible(par11Label);

        SliderContenitore.addAndMakeVisible(par12);
        par12.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par12.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par12.setRange(0.0f, 1.0f);
        par12Label.setText("Par12", dontSendNotification);
        par12Label.attachToComponent(&par12,true);
        SliderContenitore.addAndMakeVisible(par12Label);

        SliderContenitore.addAndMakeVisible(par13);
        par13.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par13.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par13.setRange(0.0f, 1.0f);
        par13Label.setText("Par13", dontSendNotification);
        par13Label.attachToComponent(&par13,true);
        SliderContenitore.addAndMakeVisible(par13Label);
        
        SliderContenitore.addAndMakeVisible(par14);
        par14.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par14.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par14.setRange(0.0f, 1.0f);
        par14Label.setText("Par14", dontSendNotification);
        par14Label.attachToComponent(&par14,true);
        SliderContenitore.addAndMakeVisible(par14Label);

        SliderContenitore.addAndMakeVisible(par15);
        par15.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par15.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par15.setRange(0.0f, 1.0f);
        par15Label.setText("Par15", dontSendNotification);
        par15Label.attachToComponent(&par15,true);
        SliderContenitore.addAndMakeVisible(par15Label);

        SliderContenitore.addAndMakeVisible(par16);
        par16.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par16.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par16.setRange(0.0f, 1.0f);
        par16Label.setText("Par16", dontSendNotification);
        par16Label.attachToComponent(&par16,true);
        SliderContenitore.addAndMakeVisible(par16Label);

        SliderContenitore.addAndMakeVisible(par17);
        par17.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par17.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par17.setRange(0.0f, 1.0f);
        par17Label.setText("Par17", dontSendNotification);
        par17Label.attachToComponent(&par17,true);
        SliderContenitore.addAndMakeVisible(par17Label);

        SliderContenitore.addAndMakeVisible(par18);
        par18.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par18.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par18.setRange(0.0f, 1.0f);
        par18Label.setText("Par18", dontSendNotification);
        par18Label.attachToComponent(&par18,true);
        SliderContenitore.addAndMakeVisible(par18Label);

        SliderContenitore.addAndMakeVisible(par19);
        par19.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par19.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par19.setRange(0.0f, 1.0f);
        par19Label.setText("Par19", dontSendNotification);
        par19Label.attachToComponent(&par19,true);
        SliderContenitore.addAndMakeVisible(par19Label);

        SliderContenitore.addAndMakeVisible(par20);
        par20.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par20.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par20.setRange(0.0f, 1.0f);
        par20Label.setText("Par20", dontSendNotification);
        par20Label.attachToComponent(&par20,true);
        SliderContenitore.addAndMakeVisible(par20Label);

        SliderContenitore.addAndMakeVisible(par21);
        par21.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par21.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par21.setRange(0.0f, 1.0f);
        par21Label.setText("Par21", dontSendNotification);
        par21Label.attachToComponent(&par21,true);
        SliderContenitore.addAndMakeVisible(par21Label);

        SliderContenitore.addAndMakeVisible(par22);
        par22.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par22.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par22.setRange(0.0f, 1.0f);
        par22Label.setText("Par22", dontSendNotification);
        par22Label.attachToComponent(&par22,true);
        SliderContenitore.addAndMakeVisible(par22Label);

        SliderContenitore.addAndMakeVisible(par23);
        par23.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par23.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par23.setRange(0.0f, 1.0f);
        par23Label.setText("Par23", dontSendNotification);
        par23Label.attachToComponent(&par23,true);
        SliderContenitore.addAndMakeVisible(par23Label);
    
        SliderContenitore.addAndMakeVisible(par24);
        par24.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par24.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par24.setRange(0.0f, 1.0f);
        par24Label.setText("Par24", dontSendNotification);
        par24Label.attachToComponent(&par24,true);
        SliderContenitore.addAndMakeVisible(par24Label);

        SliderContenitore.addAndMakeVisible(par25);
        par25.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par25.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par25.setRange(0.0f, 1.0f);
        par25Label.setText("Par25", dontSendNotification);
        par25Label.attachToComponent(&par25,true);
        SliderContenitore.addAndMakeVisible(par25Label);

        SliderContenitore.addAndMakeVisible(par26);
        par26.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par26.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par26.setRange(0.0f, 1.0f);
        par26Label.setText("Par26", dontSendNotification);
        par26Label.attachToComponent(&par26,true);
        SliderContenitore.addAndMakeVisible(par26Label);
    
        SliderContenitore.addAndMakeVisible(par27);
        par27.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par27.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par27.setRange(0.0f, 1.0f);
        par27Label.setText("Par27", dontSendNotification);
        par27Label.attachToComponent(&par27,true);
        SliderContenitore.addAndMakeVisible(par27Label);

        SliderContenitore.addAndMakeVisible(par28);
        par28.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par28.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par28.setRange(0.0f, 1.0f);
        par28Label.setText("Par28", dontSendNotification);
        par28Label.attachToComponent(&par28,true);
        SliderContenitore.addAndMakeVisible(par28Label);

        SliderContenitore.addAndMakeVisible(par29);
        par29.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par29.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par29.setRange(0.0f, 1.0f);
        par29Label.setText("Par29", dontSendNotification);
        par29Label.attachToComponent(&par29,true);
        SliderContenitore.addAndMakeVisible(par29Label);

        SliderContenitore.addAndMakeVisible(par30);
        par30.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par30.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par30.setRange(0.0f, 1.0f);
        par30Label.setText("Par30", dontSendNotification);
        par30Label.attachToComponent(&par30,true);
        SliderContenitore.addAndMakeVisible(par30Label);
    
        SliderContenitore.addAndMakeVisible(par31);
        par31.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par31.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par31.setRange(0.0f, 1.0f);
        par31Label.setText("Par31", dontSendNotification);
        par31Label.attachToComponent(&par31,true);
        SliderContenitore.addAndMakeVisible(par31Label);

        SliderContenitore.addAndMakeVisible(par32);
        par32.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
        par32.setTextBoxStyle(Slider::TextBoxRight,false,75,25);
        par32.setRange(0.0f, 1.0f);
        par32Label.setText("Par32", dontSendNotification);
        par32Label.attachToComponent(&par32,true);
        SliderContenitore.addAndMakeVisible(par32Label);
    }

private:
    
     NewProjectAudioProcessor& processor;
     juce::Component SliderContenitore;
     juce::Viewport viewPort;
    
     MidiLearnManager &midi;
     bool isLearning = false;

        
    juce::TextButton buttonMidiLearn;
     
//==================================================== Slider ==========================================================================

     MySlider par01;            juce::Label par01Label;
     MySlider par02;            juce::Label par02Label;
     MySlider par03;            juce::Label par03Label;
     MySlider par04;            juce::Label par04Label;
     MySlider par05;            juce::Label par05Label;
     MySlider par06;            juce::Label par06Label;
     MySlider par07;            juce::Label par07Label;
     MySlider par08;            juce::Label par08Label;
     MySlider par09;            juce::Label par09Label;
     MySlider par10;            juce::Label par10Label;
     MySlider par11;            juce::Label par11Label;
     MySlider par12;            juce::Label par12Label;
     MySlider par13;            juce::Label par13Label;
     MySlider par14;            juce::Label par14Label;
     MySlider par15;            juce::Label par15Label;
     MySlider par16;            juce::Label par16Label;
     MySlider par17;            juce::Label par17Label;
     MySlider par18;            juce::Label par18Label;
     MySlider par19;            juce::Label par19Label;
     MySlider par20;            juce::Label par20Label;
     MySlider par21;            juce::Label par21Label;
     MySlider par22;            juce::Label par22Label;
     MySlider par23;            juce::Label par23Label;
     MySlider par24;            juce::Label par24Label;
     MySlider par25;            juce::Label par25Label;
     MySlider par26;            juce::Label par26Label;
     MySlider par27;            juce::Label par27Label;
     MySlider par28;            juce::Label par28Label;
     MySlider par29;            juce::Label par29Label;
     MySlider par30;            juce::Label par30Label;
     MySlider par31;            juce::Label par31Label;
     MySlider par32;            juce::Label par32Label;

 //==================================================== SliderAttachment ==========================================================================
   
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par01Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par02Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par03Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par04Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par05Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par06Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par07Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par08Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par09Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par10Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par11Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par12Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par13Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par14Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par15Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par16Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par17Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par18Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par19Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par20Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par21Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par22Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par23Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par24Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par25Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par26Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par27Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par28Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par29Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par30Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par31Attachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> par32Attachment;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderAutomazioni)
};
