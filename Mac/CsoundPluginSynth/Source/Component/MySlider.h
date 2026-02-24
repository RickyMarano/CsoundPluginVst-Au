#ifndef MYSLIDER_H
#define MYSLIDER_H
// WINDOWS
//#include <JuceLibraryCode/JuceHeader.h>
// MAC
#include <JuceHeader.h>

class MySlider  : public juce::Slider
{
public:
    class SliderOverly : public juce::Component
    {
    public:
        SliderOverly()
        {
            addAndMakeVisible(label);
            label.setBounds(getLocalBounds());
            label.setJustificationType(juce::Justification::centred);
            label.setText("NoMap",juce::NotificationType::dontSendNotification);
            label.setInterceptsMouseClicks(false, false);
        };
        ~SliderOverly(){};
        
        std::function<void()> selectPar;
        std::function<void()> unLearn;

        void paint (juce::Graphics& g) override
        {
            if(isSelect())
            {
                g.setColour (juce::Colours::red.withAlpha (0.5f));
                g.fillRoundedRectangle (getLocalBounds().toFloat(), 10.0f);
            }
            else
            {
                g.setColour (juce::Colours::blue.withAlpha (0.5f));
                g.fillRoundedRectangle (getLocalBounds().toFloat(), 10.0f);
            }
        }
        void setTextCC(juce::String CC)
        {
            label.setText(CC,juce::NotificationType::dontSendNotification);
        }
        void resized() override
        {
            label.setBounds(getLocalBounds());
        }
        void mouseDown (const MouseEvent& event) override
        {
            selectPar();
            repaint();
        }
        bool keyPressed (const KeyPress& key) override
        {
            if(key == juce::KeyPress::backspaceKey)
            {
                unLearn();
                return true;
            }
            return false;
        }
        bool isSelect()
        {
            return select;
        }
        void setSelect(bool selected)
        {
            select = selected;
            setWantsKeyboardFocus(true);
            repaint();
        }
        
    private:
        bool select = false;
        bool prova = false;
        String namePar;
        
        juce::Label label;
    };
    
    //==============================================================================
    MySlider()
    {
        addChildComponent(comp);
        setScrollWheelEnabled (false);
    }
    ~MySlider() override {}

    //==============================================================================
    void paint (juce::Graphics& g) override
    {
        juce::Slider::paint(g);
    }
    void resized() override
    {
        juce::Slider::resized();
        comp.setBounds(getLocalBounds());
    }
    void setVisibleOverly(bool visible)
    {
        comp.setVisible(visible);
    }
    
    
public:
    SliderOverly comp;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MySlider)
};

#endif // MYSLIDER_H
