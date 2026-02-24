/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED
// WINDOWS
//#include <JuceLibraryCode/JuceHeader.h>
//#include "CustomCodeEditorFont/CodeEditor.h"
//#include "Component/CsoundConsole.h"
//#include <csound.hpp>
//#include "Component/MidiLearnParameters.h"

// MAC
#include <JuceHeader.h>
#include "CodeEditor.h"
#include "CsoundConsole.h"
#include <csound.hpp>
#include "MidiLearnParameters.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public AudioProcessor, juce::Button::Listener
{
    class CsoundConsoleThread : public juce::Thread
    {
    public:
        CsoundConsoleThread(NewProjectAudioProcessor& p)
            : juce::Thread("CsoundConsoleThread"), processor(p)
        {
        }
    
        ~CsoundConsoleThread() override
        {
            signalThreadShouldExit();
            stopThread(500);
        }
    
        // Questo thread legge i messaggi dal FIFO e aggiorna la GUI
        void run() override
        {
            using namespace juce;
    
        while (!threadShouldExit())
        {
            StringArray messagesToPrint;
    
            // Legge tutti i messaggi disponibili dalla FIFO
            {
                ScopedLock lock(processor.csoundMessageLock);
    
                if (!processor.csoundMessageFifo.isEmpty())
                {
                    messagesToPrint.addArray(processor.csoundMessageFifo);
                    processor.csoundMessageFifo.clear();
                }
            }
    
            // Se ci sono messaggi, aggiorna la GUI in blocco
            if (!messagesToPrint.isEmpty())
            {
                String combinedMessage = messagesToPrint.joinIntoString("\n",0,20);
    
                MessageManager::callAsync([this, combinedMessage]()
                {
                    processor.csoundConsole.insertTextAtCaret(combinedMessage);
                });
            }
    
            // Frequenza di aggiornamento console (100 ms per ridurre il carico)
            wait(100);
        }
    }
    private:
        NewProjectAudioProcessor& processor;
    };
    
    class CompileThread : public juce::Thread
    {
    public:
        CompileThread(CSOUND* cs, const String& txt, std::atomic<int>& outResult)
            : juce::Thread("CsoundCompile", 1), csound(cs), text(txt), result(outResult)
        {}

        void run() override
        {
            int r = csoundCompileCsdText(csound, text.toUTF8());
            result.store(r);
        }

    private:
        CSOUND* csound;
        String text;
        std::atomic<int>& result;
    };
    
    
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void buttonClicked (Button*) override;
    void StartCsound();
    void StopCsound();
    
    void SetHostCsound();
    void CreaAutomazione();
    void SetAutomazioneCound();
    void SetAtomicPtr();

    static void messageCallback(CSOUND* cs, int attr, const char* format, va_list args)
    {
      char buf[2048];
      vsnprintf(buf, sizeof(buf), format, args);
    
      auto* processor = static_cast<NewProjectAudioProcessor*>(csoundGetHostData(cs));
      if (!processor) return;
    
      juce::ScopedLock lock(processor->csoundMessageLock);
      processor->csoundMessageFifo.add(buf);
    }
    
//Csound API functions for deailing with midi input
    static int OpenMidiInputDevice (CSOUND* csnd, void** userData, const char* devName);
    static int OpenMidiOutputDevice (CSOUND* csnd, void** userData, const char* devName);
    static int ReadMidiData (CSOUND* csound, void* userData, unsigned char* mbuf, int nbytes);
    static int WriteMidiData (CSOUND* csound, void* userData, const unsigned char* mbuf, int nbytes);

    
public:
    TextButton play,stop;
    CSoundEditorComponent editor;
    ResizableConsoleComponent csoundConsole;
    MidiBuffer midiBuffer = {};
    MidiBuffer midiOutputBuffer;
    AudioProcessorValueTreeState parameters;
    MidiLearnManager midiLearn;
    
    // FIFO messaggi Csound
    juce::StringArray csoundMessageFifo;
    juce::CriticalSection csoundMessageLock;

    // thread dedicato
    std::unique_ptr<CsoundConsoleThread> csoundConsoleThread;
    
    MYFLT *CSspin = nullptr;
    MYFLT *CSspout = nullptr;
    
    juce::AudioBuffer<float> mainInput;
    juce::AudioBuffer<float> sidechainInput;
    
    int numCsoundChannel = 0;

private:

    CSOUND* csound;
    
    std::unique_ptr<CompileThread> compileThread;

    std::atomic<int> result { -1 };
    std::atomic<int> compileResult{-1};
    std::atomic<int> csoundStopInt { -1 };
    
    //int result = -1;
    //int csoundStopInt = -1;
    
    int bufferSize = 0;
    int currentCsIndex = 0;
    
    
    //============================ AUTOMAZIONI ====================================
    
    static constexpr int numParams = 32;
    std::atomic<float*> parAtomics[numParams];
    MYFLT* csoundParamPtrs[numParams];

    //==============================================================================

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
