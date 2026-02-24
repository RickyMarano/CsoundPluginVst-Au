#ifndef CSOUNDCONSOLE_H
#define CSOUNDCONSOLE_H

// WINDOWS
#include <JuceLibraryCode/JuceHeader.h>
// MAC
// #include <JuceHeader.h>

class CsoundConsole  : public juce::TextEditor
{
public:
    CsoundConsole()
    {
        setMultiLine(true);
        setReadOnly(true);
                
        setColour(juce::TextEditor::ColourIds::textColourId,juce::Colours::white);
        setColour(juce::TextEditor::ColourIds::backgroundColourId,juce::Colours::black);
        
        setScrollBarThickness(10);
        
        setFont(juce::Font ("Courier New", 12.0f, juce::Font::plain));
    };
    ~CsoundConsole(){};

    //==============================================================================
    void paint (juce::Graphics&g) override
    {
        juce::TextEditor::paint(g);
    }
    void resized() override
    {
       juce::TextEditor::resized();
    }
private:
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CsoundConsole)
};
class ResizableConsoleComponent : public juce::Component,TextButton::Listener
{
public:

    // std::function<void()> onResized;   //Serve a creare una Funzione che a sua volta può pure chiamare una Funzione Del Padre tipo eritarietà 
    
    ResizableConsoleComponent()
    {   
        addAndMakeVisible(resizer = new juce::ResizableBorderComponent(this,&compConteiner));
        addAndMakeVisible(manuale);
        addAndMakeVisible(attivaConsole);
        addAndMakeVisible(clear);
        
        attivaConsole.setColour(TextButton::buttonColourId,Colours::green);
        
        clear.addListener(this);
        attivaConsole.addListener(this);
        
        manuale.insertTextAtCaret("Csound Console");
        
        resizer->setBorderThickness(juce::BorderSize<int>(20, 0, 0, 0));
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::white);
    }
    
    void buttonClicked (Button* button)
    {
        if (button == &attivaConsole)
        {
            if(attivo)
            {
                attivo = false;
                attivaConsole.setColour(TextButton::buttonColourId,Colours::red);
                attivaConsole.setButtonText("Console Disattivata");
            }
            else
            {
                attivo = true;
                attivaConsole.setColour(TextButton::buttonColourId,Colours::green);
                attivaConsole.setButtonText("Console Attivata");
                
            }
        }
        if(button == &clear)
        {
            manuale.setText("",false);
        }
    }
    
    void resized() override
    {
        auto bounds = getLocalBounds();

        auto resizerHeight = 25;
        
        resizer->setBounds(bounds.removeFromTop(resizerHeight));
        manuale.setBounds(bounds);
        attivaConsole.setBounds(0,0,100,20);
        clear.setBounds(100,0,100,20);
        
        // if (onResized)
        //     onResized();
    }

    void insertTextAtCaret(const juce::String& s)
    {
        manuale.insertTextAtCaret(s);
    }
    
public:
    juce::ScopedPointer<juce::ResizableBorderComponent> resizer;
    juce::ComponentBoundsConstrainer compConteiner;
    TextButton clear {"Clear"};
    TextButton attivaConsole{"Console Attivata"};
    
    bool attivo = true;
    
    CsoundConsole manuale;
};


#endif // CSOUNDCONSOLE_H
