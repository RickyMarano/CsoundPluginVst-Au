#ifndef MYPRESETMANAGER_H
#define MYPRESETMANAGER_H

// WINDOWS
//#include <JuceLibraryCode/JuceHeader.h>
//#include "Source/PluginProcessor.h"
// MAC
#include <JuceHeader.h>
#include "PluginProcessor.h"

class MyPresetManager  : public juce::Component, public juce::FileBrowserListener
{
public:
    //==============================================================================
    MyPresetManager(NewProjectAudioProcessor& p): processor(p)
    {
        addAndMakeVisible(presetBrowser);
        addAndMakeVisible(buttonApri);
        addAndMakeVisible(buttonSalva);
        addAndMakeVisible(buttonLoad);
        addAndMakeVisible(buttonNewFile);
        addAndMakeVisible(buttonDelete);
        addAndMakeVisible(presetEditorName);
        
        presetBrowser.setColour(juce::TreeView::backgroundColourId,juce::Colours::darkslategrey);
        presetBrowser.addListener(this);
        CaricaDirectory();
        
        buttonApri.onClick = [this] 
        { 
            juce::File userDocument = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory); // nome cartella
            juce::String path = userDocument.getFullPathName()+ "/CsoundPluginEffect";
            juce::File directory(path);
            
            if (!directory.exists())
            directory.createDirectory();
            
            juce::FileChooser myChooser("Seleziona la cartella dei preset",directory,"*.csd", true);
            juce::File specialDirectory = directory;
            
            if (myChooser.browseForDirectory())
            {
                specialDirectory = myChooser.getResult();

                presetList.setDirectory(specialDirectory, true, true);
                directoryThread.startThread();
                presetList.refresh();
            }

        };
        
        buttonSalva.onClick = [this]()
        {
            SavePreset();
        }; 
        buttonNewFile.onClick = [this]()
        {
            NewFile();
        }; 
        buttonLoad.onClick = [this]()
        {
            if(presetSelect.existsAsFile())
                LoadPreset(presetSelect);
        };
        buttonDelete.onClick = [this]()
        {
            if(presetSelect.existsAsFile())
               DeleteFile();
        };
    }

    ~MyPresetManager() override {}
    
    void CaricaDirectory()
    {
         juce::File userDocument = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory); // nome cartella
         juce::String path = userDocument.getFullPathName()+ "/CsoundPluginEffect";
         juce::File directory(path);
            
        if (!directory.exists())
            directory.createDirectory();
            
            juce::File specialDirectory = directory;
            
            presetList.setDirectory(specialDirectory, true, true);
            directoryThread.startThread();
            presetList.refresh();
    }
    void SavePreset()
    {
        if (!directoryThread.isThreadRunning())
                directoryThread.startThread();

            File targetDir = presetList.getDirectory();
            
            if (!targetDir.exists())
                targetDir.createDirectory();

            String presetName = presetEditorName.getText().trim();
            
            if (presetName.isEmpty())
            {
                juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::AlertIconType::WarningIcon,"Avviso","Inserire Nome");
                return;
            }

            File newFile = targetDir.getChildFile(presetName + ".csd");

            newFile.replaceWithText(processor.editor.documento->getAllContent());
            
            presetList.refresh();
    }

    void LoadPreset(const juce::File& preset)
    {
        processor.editor.documento->replaceAllContent(preset.loadFileAsString().toUTF8());
    }
    void DeleteFile()
    {
        presetSelect.deleteFile();
        presetList.refresh();
    }
    void NewFile()
    {
          String codeTemplate = R"(<CsoundSynthesizer>
<CsOptions>
-M0
</CsOptions>
<CsInstruments>
    
sr = 44100
ksmps = 128
nchnls = 4
0dbfs = 1


</CsInstruments>
<CsScore>
</CsScore>
</CsoundSynthesizer>)";
         
        processor.editor.documento->replaceAllContent(codeTemplate);
    }

    //==============================================================================
    void paint (juce::Graphics& g) override
    {
        
    }
    void resized() override
    {
       presetBrowser.setBounds(0,50,getWidth(),getHeight()-50);
       
       buttonApri.setBounds(0,0,getWidth()*0.2,25);
       buttonNewFile.setBounds(getWidth()*0.2,0,getWidth()*0.2,25);
       buttonSalva.setBounds(getWidth()*0.4,0,getWidth()*0.2,25);
       buttonLoad.setBounds(getWidth()*0.6,0,getWidth()*0.2,25);
       buttonDelete.setBounds(getWidth()*0.8,0,getWidth()*0.2,25);
       presetEditorName.setBounds(0,25,getWidth(),25);
    }
    
    void selectionChanged() override
    {
    
    }
    void fileClicked(const juce::File& file, const juce::MouseEvent&) override
    {
        presetSelect = file;
        presetEditorName.setText(presetSelect.getFileNameWithoutExtension());
    }

    void fileDoubleClicked(const juce::File& file) override
    {
        LoadPreset(file);
    }

    void browserRootChanged(const juce::File& newRoot) override
    {
        DBG("Cartella cambiata: " << newRoot.getFullPathName());
    }
    
private:
    juce::WildcardFileFilter presetFilter {"*.csd;", "*", "CsoundPluginEffect"};
    juce::TimeSliceThread directoryThread {"CsoundPluginEffect File Scanner Thread"};
    juce::DirectoryContentsList presetList {&presetFilter, directoryThread};
    juce::FileTreeComponent presetBrowser {presetList};
    NewProjectAudioProcessor &processor;
    juce::TextButton buttonApri{"Directory"},buttonSalva{"Salva"},buttonLoad{"Load"},buttonNewFile{"NewFile"},buttonDelete{"Delete"};
    juce::TextEditor presetEditorName;
    juce::File presetSelect;
};
#endif // MYPRESETMANAGER_H
