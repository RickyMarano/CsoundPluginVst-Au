//
//  MidiLearnParameters.h
//  CsoundPlugin
//
//  Created by Riccardo Marano on 30/12/25.
//  Copyright © 2025 Ricky Marano. All rights reserved.
//

#ifndef MidiLearnParameters_h
#define MidiLearnParameters_h

#pragma once

// WINDOWS
//#include <JuceLibraryCode/JuceHeader.h>
// MAC
#include <JuceHeader.h>

class MidiLearnManager
{
public:
    MidiLearnManager(juce::AudioProcessorValueTreeState& state)
        : parameters(state) {}

    // Attiva il learn su un parametro
    void beginLearn(const juce::String& paramID)
    {
        parameterToLearn = paramID;
    }

    // Rimuove tutte le mappature di un parametro
    void unlearn(const juce::String& paramID)
    {
        parameterToUnlearn = paramID;
    }
    void cancelLearn()
    {
        parameterToLearn.clear();
    }
    bool isParameterMapped(const juce::String& paramID) const
    {
        for (juce::HashMap<int, juce::String>::Iterator it(ccToParam); it.next();)
        {
            if (it.getValue() == paramID)
                return true;
        }
        return false;
    }
    // Restituisce il primo CC associato al parametro, oppure -1 se non mappato
    int getCCForParameter(const juce::String& paramID) const
    {
        for (juce::HashMap<int, juce::String>::Iterator it(ccToParam); it.next();)
        {
            if (it.getValue() == paramID)
                return it.getKey();
        }
        return -1; // nessun CC assegnato
    }
    void processMidi (juce::MidiBuffer& midi)
    {
        // =======================
        // UNLEARN (NO MIDI NEEDED)
        // =======================
        if (!parameterToUnlearn.isEmpty())
        {
            juce::Array<int> keysToRemove;

            for (juce::HashMap<int, juce::String>::Iterator it (ccToParam); it.next();)
            {
                if (it.getValue() == parameterToUnlearn)
                    keysToRemove.add (it.getKey());
            }

            for (int cc : keysToRemove)
                ccToParam.remove (cc);

            parameterToUnlearn.clear();
        }

        // =======================
        // MIDI LOOP
        // =======================
        juce::MidiBuffer::Iterator it (midi);
        juce::MidiMessage msg;
        int pos;

        while (it.getNextEvent (msg, pos))
        {
            if (!msg.isController())
                continue;

            int cc = msg.getControllerNumber();
            float value01 = msg.getControllerValue() / 127.0f;

            // =======================
            // LEARN (FIRST CC ONLY)
            // =======================
            if (parameterToLearn.isNotEmpty())
            {
                // Se il CC non è mappato
                if (!ccToParam.contains (cc))
                {
                    //Assegna CC a Parametro
                    ccToParam.set (cc, parameterToLearn);
                }
                parameterToLearn.clear();
            }

            // =======================
            // APPLY
            // =======================
            if (ccToParam.contains (cc))
            {
                if (auto* p = parameters.getParameter (ccToParam[cc]))
                    p->setValueNotifyingHost (value01);
            }
        }
    }


    // Salva mappa in ValueTree per preset
    juce::ValueTree saveState()
    {
        juce::ValueTree midiMap("MidiMap");

        for (juce::HashMap<int, juce::String>::Iterator it(ccToParam); it.next();)
        {
            juce::ValueTree entry("Mapping");
            entry.setProperty("cc", it.getKey(), nullptr);
            entry.setProperty("param", it.getValue(), nullptr);
            midiMap.addChild(entry, -1, nullptr);
        }

        return midiMap;
    }

    // Carica mappa da ValueTree
    void loadState(const juce::ValueTree& tree)
    {
        ccToParam.clear();
        for (int i = 0; i < tree.getNumChildren(); ++i)
        {
            auto entry = tree.getChild(i);
            int cc = entry.getProperty("cc");
            juce::String param = entry.getProperty("param").toString();
            ccToParam.set(cc, param);
        }
    }

private:
    juce::AudioProcessorValueTreeState& parameters;

    juce::String parameterToLearn;
    juce::String parameterToUnlearn;

    juce::HashMap<int, juce::String> ccToParam;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiLearnManager)
};


#endif /* MidiLearnParameters_h */
