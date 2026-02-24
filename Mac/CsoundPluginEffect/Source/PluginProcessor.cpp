#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
NewProjectAudioProcessor::NewProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input", AudioChannelSet::stereo(), true)
                      #endif
                       .withInput  ("Sidechain", AudioChannelSet::stereo(),false)
                       .withOutput ("Output",AudioChannelSet::stereo(), true)
                     #endif
                       ),play("Play"),stop("Stop"),parameters(*this,nullptr),midiLearn(parameters)
#endif
{   
    play.addListener(this);
    stop.addListener(this);
    CreaAutomazione();
    
    csoundConsoleThread = std::make_unique<CsoundConsoleThread>(*this);
    csoundConsoleThread->startThread();
    
    if(!editor.getDirectory().exists())
    editor.getDirectory().createDirectory();
}

NewProjectAudioProcessor::~NewProjectAudioProcessor()
{
    if (csoundConsoleThread)
    {
        csoundConsoleThread->signalThreadShouldExit();
        csoundConsoleThread->stopThread(500);
    }
    
    if(csound != nullptr)
    {
        result.store(-1);;
        csoundStopInt.store(-1);
        
        csoundStop(csound);
        csoundReset(csound);
        csoundCleanup(csound);
        csoundDestroy(csound);
        csound = nullptr;
    }
}

//==============================================================================
const String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NewProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
}

const String NewProjectAudioProcessor::getProgramName (int index)
{
    return String();
}

void NewProjectAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    bufferSize = samplesPerBlock;
}

void NewProjectAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else

    const auto& mainIn  = layouts.getMainInputChannelSet();
    const auto& mainOut = layouts.getMainOutputChannelSet();

    const bool hasSidechain = layouts.inputBuses.size() > 1;
    const auto sideIn = hasSidechain ? layouts.getChannelSet(true, 1)
                                     : juce::AudioChannelSet::disabled();
    
    if (mainOut != juce::AudioChannelSet::mono() &&
        mainOut != juce::AudioChannelSet::stereo())
        return false;
   
    if (mainIn != juce::AudioChannelSet::mono() &&
        mainIn != juce::AudioChannelSet::stereo())
        return false;

    if (hasSidechain && !sideIn.isDisabled())
    {
        if (sideIn != juce::AudioChannelSet::mono() &&
            sideIn != juce::AudioChannelSet::stereo())
            return false;
    }

    return true;
  #endif
}
#endif

void NewProjectAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,juce::MidiBuffer& midiMessages)
{
    midiLearn.processMidi(midiMessages);

    if (result.load() || csoundStopInt.load())
    {
        buffer.clear();
        return;
    }

    juce::ScopedNoDenormals noDenormals;

    const int numSamples  = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();
    
    const int mainCh = mainInput.getNumChannels();
    const int scCh   = sidechainInput.getNumChannels();
    const int totalCh = mainCh + scCh;

    juce::MidiBuffer::Iterator it(midiMessages);
    juce::MidiMessage currentMessage;
    int samplePosition;

    const int ksmps = csoundGetKsmps(csound);
    int csndIndex = currentCsIndex;
    
    if (!result.load() && !csoundStopInt.load())
    {
        mainInput.makeCopyOf(getBusBuffer(buffer,true,0));
        sidechainInput.makeCopyOf(getBusBuffer(buffer,true, 1));
                
        CSspin  = csoundGetSpin(csound);
        CSspout = csoundGetSpout(csound);

        for (int i = 0; i < numSamples; ++i)
        {
            while (it.getNextEvent(currentMessage, samplePosition))
            {
                if (samplePosition == i)
                    midiBuffer.addEvent(currentMessage, samplePosition);
            }

            it.setNextSamplePosition(0);
            
            if (csndIndex == 0)
            {
                try
                {
                    if (!result.load())
                        result.store(csoundPerformKsmps(csound));
                }
                catch (...)
                {
                    result.store(-1);
                }

                SetAutomazioneCound();
            }
            
            if (CSspin != nullptr)
            {
                if(numCsoundChannel == 4 && mainCh == 2 && scCh == 2) // Input Stereo + Sidechain Stereo
                {
                // =================== INPUT =================================

                    for (int ch = 0; ch < mainCh; ++ch)
                    {
                        MYFLT InBuffer = mainInput.getReadPointer(ch)[i];
                        CSspin[csndIndex * totalCh + ch] = InBuffer;
                    }
                // ===================SIDECHAIN INPUT =================================

                    for (int ch = 0; ch < scCh; ++ch)
                    {
                        MYFLT SideBuffer = sidechainInput.getReadPointer(ch)[i];
                        CSspin[csndIndex * totalCh + mainCh + ch] = SideBuffer;
                    }
                // =================== OUTPUT =================================

                    for (int ch = 0; ch < numCsoundChannel; ++ch)
                    {
                        if (CSspout != nullptr)
                        {
                            MYFLT OutBuffer = CSspout[csndIndex * numCsoundChannel + ch];
                            buffer.getWritePointer(ch)[i] = OutBuffer;
                        }
                    }
                }
                else if(numCsoundChannel == 2 && mainCh == 2)// Input Stereo No Sidechain
                {
                    for (int ch = 0; ch < mainCh; ++ch)
                    {
                        MYFLT InBuffer = mainInput.getReadPointer(ch)[i];
                        CSspin[csndIndex * mainCh + ch] = InBuffer;
                    }
                    
                    for (int ch = 0; ch < numCsoundChannel; ++ch)
                    {
                        if (CSspout != nullptr)
                        {
                            MYFLT OutBuffer = CSspout[csndIndex * numCsoundChannel + ch];
                            buffer.getWritePointer(ch)[i] = OutBuffer;
                        }
                    }
                }
                else if(numCsoundChannel == 2 && mainCh == 1 && scCh == 1)// Input Mono + Sidechain Mono
                {
                    for (int ch = 0; ch < mainCh; ++ch)
                    {
                        MYFLT InBuffer = mainInput.getReadPointer(ch)[i];
                        CSspin[csndIndex * numCsoundChannel + ch] = InBuffer;
                    }
                    
                    // ===================SIDECHAIN INPUT =================================

                    for (int ch = 0; ch < scCh; ++ch)
                    {
                        MYFLT SideBuffer = sidechainInput.getReadPointer(ch)[i];
                        CSspin[csndIndex * totalCh + mainCh + ch] = SideBuffer;
                    }
                    
                    for (int ch = 0; ch < numCsoundChannel; ++ch)
                    {
                        if (CSspout != nullptr)
                        {
                            MYFLT OutBuffer = CSspout[csndIndex * numCsoundChannel + ch];
                            buffer.getWritePointer(ch)[i] = OutBuffer;
                        }
                    }
                }
                else if(numCsoundChannel == 1 && mainCh == 1 )// Input Mono + Sidechain Mono
                {
                    for (int ch = 0; ch < numCsoundChannel; ++ch)
                    {
                        
                        MYFLT InBuffer = mainInput.getReadPointer(ch)[i];
                        CSspin[csndIndex * numCsoundChannel + ch] = InBuffer;
                        
                    }
                    
                    for (int ch = 0; ch < numCsoundChannel; ++ch)
                    {
                        if (CSspout != nullptr)
                        {
                            MYFLT OutBuffer = CSspout[csndIndex * numCsoundChannel + ch];
                            buffer.getWritePointer(ch)[i] = OutBuffer;
                        }
                    }
                }
                else
                {
                    buffer.clear();
                }
                
            }
            
            csndIndex++;

            if (csndIndex >= ksmps)
                csndIndex = 0;
        }

        currentCsIndex = csndIndex;
    }
}

//==============================================================================
bool NewProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void NewProjectAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    editor.save(destData);
}

void NewProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    editor.load(data,sizeInBytes);
}

void NewProjectAudioProcessor::buttonClicked(juce::Button *button)
{   
    if(button == &play)
    {
        StopCsound();
        StartCsound();      //Creare un modo per non buidare 2 Volte
        
        if(!result.load())
        {
            play.setColour(TextButton::textColourOffId,Colours::green);   
        }
    }
    
    if(button == &stop)
    {   
        StopCsound();
        
        if (csoundStopInt.load())
        {
            play.setColour(TextButton::textColourOffId,Colours::red);   
        }
    }
}

/*void NewProjectAudioProcessor::StartCsound()
{   
        csound = csoundCreate(0);
        
        if(csound != nullptr)
        {   
            SetHostCsound();
            
            if(csoundConsole.attivo)
            csoundSetMessageCallback(csound,&NewProjectAudioProcessor::messageCallback);
            
            result.store(csoundCompileCsdText(csound,editor.documento->getAllContent().toUTF8()));
            
            if (!result.load())
            {
                numCsoundChannel = csoundGetNchnls(csound);
                SetHostCsound();
                
                result.store(csoundStart(csound));
                SetAtomicPtr();
                csoundStopInt = 0;
            }
        }
}*/
void NewProjectAudioProcessor::StartCsound()
{
     csound = csoundCreate(0);
     
     if (!csound) return;

     SetHostCsound();

     if (csoundConsole.attivo)
         csoundSetMessageCallback(csound, &NewProjectAudioProcessor::messageCallback);

     compileThread = std::make_unique<CompileThread>(csound, editor.documento->getAllContent(), compileResult);
     compileThread->startThread();
    
     compileThread->waitForThreadToExit(-1);
    
    if (!compileResult.load())
    {
        numCsoundChannel = csoundGetNchnls(csound);
        
        result.store(csoundStart(csound));
        SetAtomicPtr();
        
        csoundStopInt.store(0);
    }
}

void NewProjectAudioProcessor::StopCsound()
{
    if(csound != nullptr && !result.load() && !csoundStopInt.load())
    {
        result.store(-1);
        csoundStopInt.store(-1);
        
        csoundStop(csound);
        csoundReset(csound);
        csoundCleanup(csound);
        csoundDestroy(csound);
        csound = nullptr;
    }
}

void NewProjectAudioProcessor::SetHostCsound()
{
    csoundSetHostData(csound,this);
    csoundSetHostImplementedAudioIO(csound,1,0);
    csoundSetHostImplementedMIDIIO(csound,true);
    csoundSetExternalMidiInOpenCallback(csound,OpenMidiInputDevice);
    csoundSetExternalMidiReadCallback(csound,ReadMidiData);
    csoundSetExternalMidiOutOpenCallback(csound,OpenMidiInputDevice);
    csoundSetExternalMidiWriteCallback(csound, WriteMidiData);
    
    juce::String incdir = editor.getDirectory().getFullPathName(); // Windows
    String csoundOptions =  "--env:INCDIR=" + incdir;
    
    csoundSetOption(csound,csoundOptions.toRawUTF8());
}

void NewProjectAudioProcessor::CreaAutomazione()
{
    juce::NormalisableRange<float> range(0.0f,1.0f);

    for (int i = 0; i < numParams; ++i)
    {   
        String name;
        
        if(i<9)
        {
            name = "Par0" + String(i+1);
        }
        else
        {
            name = "Par" + String(i+1);
        }
        parameters.createAndAddParameter(name,name,name,range,0.5,nullptr,nullptr);
    }
}

void NewProjectAudioProcessor::SetAutomazioneCound()
{
    for (int i = 0; i < numParams; ++i)
    {
        if (!parAtomics[i] || !csoundParamPtrs[i])
            continue; // Skip se puntatore invalido

        float value = *parAtomics[i];
        *csoundParamPtrs[i] = value;
    }
}

void NewProjectAudioProcessor::SetAtomicPtr()
{
    for (int i = 0; i < numParams; ++i)
    {    
        String name;
        
        if(i<9)
        {
            name = "Par0" + String(i+1);
        }
        else
        {
            name = "Par" + String(i+1);
        }
        parAtomics[i] = parameters.getRawParameterValue(name);
        csoundGetChannelPtr(csound,&csoundParamPtrs[i],name.toRawUTF8(),CSOUND_CONTROL_CHANNEL | CSOUND_INPUT_CHANNEL);
    }
}

//======================== CSOUND MIDI FUNCTIONS ================================
int NewProjectAudioProcessor::OpenMidiInputDevice (CSOUND* csound, void** userData, const char* /*devName*/)
{
    *userData = csoundGetHostData(csound);
    return 0;
}

//==============================================================================
// Reads MIDI input data from host, gets called every time there is MIDI input to our plugin
//==============================================================================
int NewProjectAudioProcessor::ReadMidiData (CSOUND* /*csound*/, void* userData,unsigned char* mbuf, int nbytes)
{
    auto* midiData = static_cast<NewProjectAudioProcessor*>(userData);

    if (!userData)
    {
        return 0;
    }

    int cnt = 0;

    if (!midiData->midiBuffer.isEmpty() && cnt <= (nbytes - 3))
    {
        MidiMessage message (0xf4, 0, 0, 0);
        MidiBuffer::Iterator i (midiData->midiBuffer);
        int messageFrameRelativeTothisProcess;

        while (i.getNextEvent (message, messageFrameRelativeTothisProcess))
        {
            const uint8* data = message.getRawData();
            *mbuf++ = *data++;

            if(message.isChannelPressure() || message.isProgramChange())
            {
                *mbuf++ = *data++;
                cnt += 2;
            }
            else
            {
                *mbuf++ = *data++;
                *mbuf++ = *data++;
                cnt  += 3;
            }
        }

        midiData->midiBuffer.clear();
        
    }

    
    return cnt;

}

//==============================================================================
// Opens MIDI output device, adding -QN to your CsOptions will causes this method to be called
// as soon as your plugin loads
//==============================================================================
int NewProjectAudioProcessor::OpenMidiOutputDevice (CSOUND* csound, void** userData, const char* /*devName*/)
{
    *userData = csoundGetHostData (csound);
    return 0;
}

//==============================================================================
// Write MIDI data to plugin's MIDI output. Each time Csound outputs a midi message this
// method should be called. Note: you must have -Q set in your CsOptions
//==============================================================================
int NewProjectAudioProcessor::WriteMidiData (CSOUND* /*csound*/, void* _userData,const unsigned char* mbuf, int nbytes)
{
    auto* userData = static_cast<NewProjectAudioProcessor*>(_userData);

    if (!userData)
    {
        return 0;
    }

    MidiMessage message (mbuf, nbytes, 0);
    userData->midiOutputBuffer.addEvent (message, 0);
    return nbytes;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}
