#ifndef XMLCUSTOMPARSE_H
#define XMLCUSTOMPARSE_H

// WINDOWS
//#include <JuceLibraryCode/JuceHeader.h>

// MAC
#include <JuceHeader.h>

struct OpcodeCsound
{
#if JUCE_MAC
    void caricaMenuOpcode()
    {
            juce::File xmlFile("/Library/Audio/Plug-Ins/Components/CsoundPlugin.component/Contents/Resources/Resources/CsoundOpcodes.xml");
    
            if (xmlElement = juce::XmlDocument::parse(xmlFile))
            {
                juce::String opcodes = xmlElement->getTagName();            //XML BASE
                
                for(int i = 0;i<xmlElement->getNumChildElements();i++)
                {             
                   category = xmlElement->getChildElement(i)->getAttributeValue(0);
                   caricaCategory(i);                
                }
            }
    }
#elif JUCE_WINDOWS    
    void caricaMenuOpcode()
    {       
    
            juce::File pluginDirectory = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
            juce::File resourcesDirectory = pluginDirectory.getChildFile("Resources");
            juce::File xmlFile = resourcesDirectory.getChildFile("CsoundOpcodes.xml");
    
           // juce::File xml("C:/Users/User/Documents/Juce-QtCreator_(Progetti)/Progetti/Csound(Plugin)/CsoundOpcodes.xml");
    
            if (xmlElement = juce::XmlDocument::parse(xmlFile))
            {
                juce::String opcodes = xmlElement->getTagName();            //XML BASE
                
                for(int i = 0;i<xmlElement->getNumChildElements();i++)
                {             
                   category = xmlElement->getChildElement(i)->getAttributeValue(0);
                   caricaCategory(i);                
                }
            }
    }
#endif

    juce::String getStringSelect(int command)
    {
        return "category"; // XML BASE-CATEGORY
    }
    void caricaCategory(int iter)
    {
        PopupMenu Opcode;
        for(int i = 0; i < xmlElement->getChildElement(iter)->getNumChildElements();i++)
        {
            juce::String opcode = xmlElement->getChildElement(iter)->getChildElement(i)->getChildByName("synopsis")->getChildElementAllSubText("opcodeName","NULL");
            juce::String sintassi = xmlElement->getChildElement(iter)->getChildElement(i)->getChildByName("synopsis")->getAllSubText();
            
            sintassi.trim();            
            int startPos = sintassi.indexOf(opcode);
            int endPos = sintassi.getLastCharacter();
            
            juce::String sintassiOpcode = sintassi.substring(startPos,endPos);
            
            Opcode.addItem(item,opcode,true);
            itemNames.set(item,sintassi);
            
            OpcodeList.add(opcode);
            
            OpcodeSintassi.add(sintassiOpcode);

            item++;
        }
        menuOpcode.addSubMenu(category,Opcode,true);
    }
    juce::String caricaOpcode(int iter)
    {
        if (iter > 0)
        {
           return itemNames.getReference(iter); 
        }
    }
    
    PopupMenu menuOpcode;
    PopupMenu menuCategory;
    juce::StringArray OpcodeList;
    juce::StringArray OpcodeSintassi;
    
private:
    juce::String category;
    std::unique_ptr<juce::XmlElement> xmlElement;
    juce::HashMap<int, juce::String> itemNames;
    
    int item = 1;
};
#endif // XMLCUSTOMPARSE_H



/* xmla->getTagName() Leggi il nome del XML quindi L'elemento principale
   xmla->getChildElement(0)->getAttributeName(0); In Questo modo leggi il primo argomento dell'elemento(0) qundi del primo elemento;
   xmla->getChildElement(0)->getChildElement(0)->getChildElement(0)->getAllSubText(); Leggiamo dentro BASE-CATEGORY-OPCODE-DESC IN questo caso DESC è un elemento e non un Attributo quindi per leggere al suo interno dobbiamo usare getAllSubText();
   
    */
 // struct SceltaMenu : public ModalComponentManager::Callback 
    // {
    //     void modalStateFinished(int Num) override 
    //     {
    //         if(Num == 0)
    //         return;
            
    //         if(Num == 1)
    //         DBG("PrimoPasso ");
            
    //         if(Num == 2)
    //         DBG("SecondoPasso ");
    //     }
    // };
    
    // category = xmlElement->getChildElement(0)->getAttributeValue(0); // XML BASE-CATEGORY
    // category2 = xmlElement->getChildElement(1)->getAttributeValue(0);
    // juce::String opcode = xmlElement->getChildElement(0)->getChildElement(0)->getTagName(); // XML BASE-CATEGORY-OPCODE 
    // juce::String opcodeDesc = xmlElement->getChildElement(0)->getChildElement(0)->getChildElement(0)->getAllSubText(); // XML BASE-CATEGORY-OPCODE
