/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p): AudioProcessorEditor(&p),processor(p),pannelloComponenti(p)
{
    setResizable(true,true);
    addMouseListener(this,true);
    processor.csoundConsole.addComponentListener(this);
    
    setSize(600,500);
    
    addAndMakeVisible(processor.editor);
    addAndMakeVisible(processor.play);
    addAndMakeVisible(processor.stop);
    addAndMakeVisible(processor.csoundConsole);
    addChildComponent(pannelloComponenti);
    
    addAndMakeVisible(buttonAutomazioni);
    addAndMakeVisible(buttonPreset);
    
    buttonAutomazioni.onClick = [this]()
    {
        if(!pannelloComponenti.isVisible())
        {
            pannelloComponenti.setVisible(true);
            pannelloComponenti.sliderAutomazioni.setVisible(true);
        }
        else
        {
            pannelloComponenti.setVisible(false);
            pannelloComponenti.sliderAutomazioni.setVisible(false);
            pannelloComponenti.myPresetManager.setVisible(false);
        }
        repaint();
    }; 
    
    buttonPreset.onClick = [this]()
    {
        if(!pannelloComponenti.isVisible())
        {
            pannelloComponenti.setVisible(true);
            pannelloComponenti.myPresetManager.setVisible(true);
        }
        else
        {
            pannelloComponenti.setVisible(false);
            pannelloComponenti.sliderAutomazioni.setVisible(false);
            pannelloComponenti.myPresetManager.setVisible(false);
        }
    
        repaint();
    }; 
    
    // addAndMakeVisible(sliderAutomazioni);

    // processor.csoundConsole.onResized = [this](){resized();};
    
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
    processor.csoundConsole.removeComponentListener(this);
}
//==============================================================================
void NewProjectAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::black);
    
    processor.play.setBounds(0,0,100,50);
    processor.stop.setBounds(100,0,100,50);
    buttonAutomazioni.setBounds(200,0,100,50);
    buttonPreset.setBounds(300,0,100,50);
    
    if(pannelloComponenti.isVisible())
        {
            processor.editor.setBounds(10,60,getWidth() - 340,processor.csoundConsole.getY() - 60);
            pannelloComponenti.setBounds(processor.editor.getWidth()+10,60,320,processor.csoundConsole.getY() - 60);
        }
        else
        {
            processor.editor.setBounds(10,60,getWidth() - 20,processor.csoundConsole.getY() - 60);
        }
}

void NewProjectAudioProcessorEditor::resized()
{
    processor.csoundConsole.compConteiner.setMaximumHeight(getHeight()-200);
    processor.csoundConsole.compConteiner.setMinimumHeight(90);
    
    processor.csoundConsole.setBounds(10,getHeight()-100,getWidth()-20,90);
    processor.editor.setBounds(10,60,getWidth() - 340,processor.csoundConsole.getY() - 60);
    pannelloComponenti.setBounds(processor.editor.getWidth()+10,60,320,processor.csoundConsole.getY() - 60);
}

void NewProjectAudioProcessorEditor::componentMovedOrResized(Component &component, bool wasMoved, bool wasResized)
{
    if (&component == &processor.csoundConsole && wasResized)
    {
        if(pannelloComponenti.isVisible())
        {
            processor.editor.setBounds(10,60,getWidth() - 340,processor.csoundConsole.getY() - 60);
            // sliderAutomazioni.setBounds(processor.editor.getWidth(),60,320,processor.csoundConsole.getY() - 60);
            pannelloComponenti.setBounds(processor.editor.getWidth()+10,60,320,processor.csoundConsole.getY() - 60);
        }
        else
        {
            processor.editor.setBounds(10,60,getWidth() - 20,processor.csoundConsole.getY() - 60);
        }
    }
    repaint();
}

