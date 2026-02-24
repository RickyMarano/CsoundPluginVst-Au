
// WINDOWS
#include <JuceLibraryCode/JuceHeader.h>
#include "CsoundOpcode.h"
#include "XmlCustomParse.h"
#include "CSoundFunzioniToken.h"
#include "MyCodeTokenizer.h"

// MAC
// #include <JuceHeader.h>
// #include "CsoundOpcode.h"
// #include "XmlCustomParse.h"
// #include "CSoundFunzioniToken.h"
// #include "MyCodeTokenizer.h"

class CsoundTokeniser : public MyCodeTokeniser
{
public:

    CsoundTokeniser() 
    {
      
    }
    ~CsoundTokeniser() {}

    // StringArray udoKeywords;
    //==============================================================================
    enum CsoundTokenTypes
    {
        keyword = 0,
        identifier,
        number,
        string,
        operatorToken,
        comment,
        preprocessor,
        error,
        eof,
        audioVar,
        controlVar,
        initVar,
        globalVar,
        stringVar,
        tag,
        instr,
        myOpcode
    };
   
    MyCodeEditorFont::MyColourAndFontScheme getDefaultColourScheme()
    {
        struct Type
        {
            const char* name;
            uint32 colour;
            Font font;
        };

        const Type types[] =
        {
            { "Keyword",       Colour (0xff0000ff).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::bold)  },  // blu
            { "Identifier",    Colour (0xff000000).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::plain) },  // nero 
            { "Number",        Colour (0xff000000).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::plain) },  // nero
            { "String",        Colour (0xFFA52A2A).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::italic)},  // marrone
            { "Operator",      Colour (0xFFA52A2A).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::bold)  },  // marrone
            { "Comment",       Colour (0xFFCCCCCC).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::italic)},  // grigio
            { "Preprocessor",  Colour (0xFFA52A2A).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::plain) },  // marrone
            { "Error",         Colour (0xFF000000).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::plain) },  // nero
            { "EOF",           Colour (0xFFCCCCCC).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::plain) },  // grigio chiaro
            { "AudioVar",      Colour (0xFF008000).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::bold)  },  // verde
            { "ControlVar",    Colour (0xFF006699).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::plain) },  // blu scuro
            { "InitVar",       Colour (0xff006400).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::plain) },  // verde 
            { "TableVar",      Colour (0xFFCCCCCC).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::bold)  },  // grgio 
            { "GlobalVar",     Colour (0xFFCC6600).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::plain) },  // arancione scuro
            { "StringVar",     Colour (0xFFCC0066).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::plain) },  // rosa acceso
            { "Tag"      ,     Colour (0xFFA52A2A).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::bold)  },  // marrone
            { "Instr",         Colour (0xFF8B0000).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::bold)  },  // rossoScuro
            { "MyOpcode",      Colour (0xff0000ff).getARGB(),juce::Font ("Courier New", 16.0f, juce::Font::bold)  },  // blu

        };

        MyCodeEditorFont::MyColourAndFontScheme cs;

        for (std::size_t i = 0; i < int (sizeof (types) / sizeof (types[0])); ++i) // (NB: numElementsInArray doesn't work here in GCC4.2)
            cs.set (types[i].name, Colour (types[i].colour),Font (types[i].font));

        return cs;
    }


private:
    //==============================================================================
    StringArray getTokenTypes()
    {
        StringArray s;
        s.add ("Keyword");
        s.add ("Identifier");
        s.add ("Number");
        s.add ("String");
        s.add ("Operator");
        s.add ("Comment");
        s.add ("Preprocessor");
        s.add ("Error");
        s.add ("EOF");
        s.add ("AudioVar");
        s.add ("ControlVar");
        s.add ("InitVar");
        s.add ("TableVar");
        s.add ("GlobalVar");
        s.add ("StringVar");
        s.add("Tag");
        s.add("Instr");
        s.add("MyOpcode");
        
        /* Aggiungere un Token per creare le automationi quindi un sistema di controllo per le variabili i o K */
        
        return s;
    }

    //==============================================================================
    void skipQuotedString (CodeDocument::Iterator& source)
    {
        const juce_wchar quote = source.nextChar();

        for (;;)
        {
            const juce_wchar c = source.nextChar();

            if (c == quote || c == 0)
                break;

            if (c == '\\')
                source.skip();
        }
    }
    //==============================================================================
    void skipCSDTag (CodeDocument::Iterator& source) noexcept
    {
        for (;;)
        {
            const juce_wchar c = source.nextChar();

            if (c == 0 || (c == '>'))
                break;
        }
    }
    
    //==============================================================================
    bool isIdentifierStart (const char c)
    {
        // return CharacterFunctions::isLetter (c) || c == '_' || c == '@';
        
        return CsoundTokeniserFunctions::isIdentifierStart(c);
    }

    //==============================================================================
    bool isIdentifierBody (const char c)
    {
        // return CharacterFunctions::isLetter (c)|| CharacterFunctions::isDigit (c) || c == '_' || c == '@';
        
        return CsoundTokeniserFunctions::isIdentifierBody(c);
    }

    //==============================================================================
    bool isReservedKeyword (String::CharPointerType token,const int length) noexcept
    { 
        return CsoundTokeniserFunctions::isReservedKeyword(token,length);
    }
    //==============================================================================
    bool isReservedTag (String::CharPointerType token,const int length) noexcept
    {
        return CsoundTokeniserFunctions::isReservedTag(token,length);
    }
    //==============================================================================
    bool isReservedInstr (String::CharPointerType token,const int length) noexcept
    {
        return CsoundTokeniserFunctions::isReservedInstr(token,length);
    }
    //==============================================================================
    bool isReservedNewOpcode (String::CharPointerType token,const int length) noexcept
    {
        return CsoundTokeniserFunctions::isReservedNewOpcode(token,length);
    }
    //==============================================================================
    int parseIdentifier (CodeDocument::Iterator& source) noexcept
    {
        return CsoundTokeniserFunctions::parseIdentifier(source);
    }
    //==============================================================================
    int readNextToken (CodeDocument::Iterator& source)
    {
        return CsoundTokeniserFunctions::readNextToken(source);
    }
};

class CsoundCodeEditor : public MyCodeEditorFont,juce::Thread,juce::CodeDocument::Listener,juce::Timer,public juce::FileDragAndDropTarget
{
    class MyListBoxModel : public juce::ListBoxModel
    {
    public:
    
        MyListBoxModel(CsoundCodeEditor* csoundEdit):csoundEditor(csoundEdit)
        {         
            resetItem();
        }
        void addItem(const juce::String& text)
        {
            itemsFix.add(text);
        }
        void removeItem(const juce::String& text)
        {
        }
        int getNumRows() override
        {
            return itemsFilter.size();
        }
        
        void resetItem()
        {   
            itemsFix.clear();
            opcode.caricaMenuOpcode();
            
            
            //Nuovi Opcode
            // opcode.OpcodeList.add("monoampmidi");
            // opcode.OpcodeList.add("monocpsmidi");
            // opcode.OpcodeList.add("monoAdsr");
            // opcode.OpcodeList.add("mononotnum");
            // opcode.OpcodeList.add("mononotvel");
            // opcode.OpcodeList.add("mononoteactive");
            // opcode.OpcodeList.add("monopitchbend");
            // opcode.OpcodeList.add("monoaftertouch");
            // opcode.OpcodeList.add("monocc");
            
            
            for (auto& item : opcode.OpcodeList)
            {
                itemsFix.add(item);
            }
            
        }
        void resetItemFile (juce::String folderName)
        {
            itemsFile.clear();
            juce::File targetDir = directory.getChildFile (folderName);
        
            if (targetDir.isDirectory())
            {
                juce::DirectoryIterator iter (targetDir, false, "*", juce::File::findFilesAndDirectories);
                while (iter.next()) 
                {
                    juce::File f = iter.getFile();
                    juce::String name = f.getFileName();
                    
                    if (f.isDirectory())
                        name += "\\";
                
                    itemsFile.addIfNotAlreadyThere (name);
                }
            }
            else
            {
                juce::DirectoryIterator iter (directory, false, "*", juce::File::findDirectories);
                while (iter.next()) 
                {
                    juce::File f = iter.getFile();
                    juce::String dName = f.getFileName();
                    
                    if (folderName.isEmpty() || dName.startsWithIgnoreCase(folderName)) 
                    {
                        itemsFile.addIfNotAlreadyThere (dName + "\\");
                    }
                }
            }
        }
    

        // void returnKeyPressed (int lastRowSelected)
        // {
        //     String &text = itemsFilter.getReference(lastRowSelected);
        //     csoundEditor->replaceText(text);
        //     csoundEditor->autoCompleteListBox.setVisible(false);
        // }
       
        void returnKeyPressed (int lastRowSelected)
        {
            juce::String text = itemsFilter.getReference(lastRowSelected);
        
            if (text.trim() == "#include")
            {
                text = "#include \"\""; 
            }

            csoundEditor->replaceText(text);
            csoundEditor->autoCompleteListBox.setVisible(false);
        }

        void deleteKeyPressed (int lastRowSelected)
        {
            csoundEditor->grabKeyboardFocus();
        }
        
        void paintListBoxItem(int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override
        {
            g.fillAll(rowIsSelected ? juce::Colours::grey : juce::Colours::whitesmoke);
    
            juce::Font font("Courier New", 16.0f, juce::Font::plain);
            g.setFont(font);
            g.setColour(juce::Colours::black);
            g.drawText(itemsFilter[rowNumber], 5, 0, width - 10, height,juce::Justification::centredLeft, true);
        }
    
        juce::StringArray itemsFix;
        juce::StringArray itemsDynamic;
        juce::StringArray itemsFile;
        juce::StringArray itemsFilter;
        
        juce::File userDocument = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory); // nome cartella
        juce::String path = userDocument.getFullPathName()+ "/CsoundPluginSynth/CustomOpcode";
        juce::File directory {path};
        
        OpcodeCsound opcode;
        CsoundCodeEditor *csoundEditor;
        
    };
    class MyListBox : public juce::ListBox,MyListBoxModel
    {
    public:
        MyListBox(CsoundCodeEditor* csoundEdit): MyListBoxModel(csoundEdit),editor(csoundEdit)
        {
             setModel(this);
             setColour(juce::ListBox::backgroundColourId,Colours::whitesmoke);
             getViewport()->setScrollBarsShown(false,false);
             
        }
        ~MyListBox()
        {
            
        }
        void cercaParola(const juce::String &currentWord)
        {
            itemsFilter.clear();
                        
            if(Stato_include == true)
            for (auto itemFile : itemsFile)
            {
                if (itemFile.trim().contains(currentWord))
                    itemsFilter.add(itemFile);
            }
            else
            {
                for (auto itemFix : itemsFix)
                {
                    if (itemFix.trim().contains(currentWord))
                        itemsFilter.add(itemFix);
                }
        
                for (auto itemDynamic : itemsDynamic)
                {
                    if (itemDynamic.trim().contains(currentWord))
                        itemsFilter.add(itemDynamic);
                }
            }
            
            updateContent();
        }
                
        void resetItemDynamic()
        {
            itemsDynamic.clear();
        }
        void updateFile(String &nameCartella)
        {
            resetItemFile(nameCartella);
        }
        
        void updateVariabile(const String &word, const StringArray &words)
        {
            String currentLine = editor->getCaretPos().getLineText().trim();
                
            if (word.startsWith("a") || word.startsWith("i") || word.startsWith("k") || word.startsWith("S") || word.startsWith("f") || word.startsWith("g"))
            {
                if (!itemsDynamic.contains(word))
                    itemsDynamic.addIfNotAlreadyThere(word);
            }
        }
        juce::String getIncludeFileContent(const juce::String& word,const juce::File& directory,const juce::StringArray& words)
        {
            int index = words.indexOf(word);

            bool isIncludeContext = (index > 0 && words[index - 1].trim() == "#include");
            
            if (!isIncludeContext)
            return {};

            juce::File fileToLoad = directory.getChildFile(word);
            
            if (!fileToLoad.existsAsFile())
            return {};
            
            return fileToLoad.loadFileAsString();
        }
           
        void updateVariabileOpcode(const String& word, const StringArray& words)
        {
            int index = words.indexOf(word);

            if (index > 0 && words[index - 1] == "opcode")
            {
                if (!itemsDynamic.contains(word)) 
                {  
                    itemsDynamic.addIfNotAlreadyThere(word);
                }
            }
        }
        
        void updateNewOpcode()
        {
            for(juce::String newOpcode : CustomOpcodes)
            if (!itemsDynamic.contains(newOpcode)) 
            {  
                itemsDynamic.addIfNotAlreadyThere(newOpcode);
            }
        }     

        int getHeightOttimizzato()
        {
            int altezza = getNumRows() * getRowHeight();
            
            if(altezza <= 150)   
                return altezza; 
            else
                return 150;
        }
        
        CsoundCodeEditor *editor;
        bool Stato_include = false;
    };
    
    // class ResizableInnerComponent : public Component
    // {
    // public:
    //     ResizableInnerComponent(CsoundCodeEditor *csoundEditor): editor(csoundEditor)
    //     {
    //         addAndMakeVisible(resizer = new ResizableBorderComponent(this, &constrainer));
    //         resizer->setBorderThickness(BorderSize<int>(0, 5, 0, 0));
    //     }
    //     void paint (Graphics& g)
    //     {
    //         g.fillAll(Colours::white);
            
    //         int larghezza = getBounds().getWidth()-5;
            
    //         int x = getLocalBounds().getX();
    //         int y = getLocalBounds().getY();
    //         int Width = getLocalBounds().getWidth();
    //         int Height = getLocalBounds().getHeight()+10;
            
    //         manuale.setBounds(x+5,y,Width-5,Height);
    //         resizer->setBounds(x,y,Width,Height+5);
            
    //         addAndMakeVisible(manuale);
    //     }
    //     void resized() override
    //     {
    //         int x = getLocalBounds().getX();
    //         int y = getLocalBounds().getY();
    //         int Width = getLocalBounds().getWidth();
    //         int Height = getLocalBounds().getHeight()+10;
            
    //         manuale.setBounds(x+5,y,Width-5,Height);
    //         resizer->setBounds(x,y,Width,Height+5);
                  
    //     }
    //     void viewManuale(String URL)
    //     {
    //         manuale.goToURL(URL);
    //     }
    //     void visibilityChanged() override
    //     {
    //         if(isVisible())
    //         {
    //             DBG("Visible");
    //             editor->setAlpha(0.8);
    //         }
    //         else if (!isVisible())
    //         {   
    //             editor->setAlpha(1.0);
    //             DBG("Invisibile");
    //         } 
    //     }
        
    // private:
    //     ScopedPointer<ResizableBorderComponent> resizer;
    //     ComponentBoundsConstrainer constrainer;
    //     juce::WebBrowserComponent manuale;
    //     CsoundCodeEditor *editor;
    // };

public:
    CsoundCodeEditor(CodeDocument& document,CsoundTokeniser* codeTokeniser): MyCodeEditorFont(document,codeTokeniser),autoCompleteListBox(this),/*resizaComp(this),*/juce::Thread("Parsing")
    {   
        setColour(MyCodeEditorFont::backgroundColourId, Colours::white);
        setColour(MyCodeEditorFont::lineNumberTextId,Colours::black);
        setColour(MyCodeEditorFont::lineNumberBackgroundId,Colours::whitesmoke);
        
        setScrollbarThickness(15);
        
        document.addListener(this);
        // addMouseListener(this,false);
        setInterceptsMouseClicks(true,false);
                
        opcode.caricaMenuOpcode();
        addChildComponent(autoCompleteListBox);
        // addChildComponent(resizaComp);
        autoCompleteListBox.setRowHeight(20);
        scrollToLine(30);

    }
    ~CsoundCodeEditor()
    {
        if(isThreadRunning())
            stopThread(100);
    }
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override 
    {
        return true;
    }
    
    void fileDragEnter (const juce::StringArray& files, int x, int y) override
    {
        grabKeyboardFocus();
    }
    
    void fileDragMove (const juce::StringArray& files, int x, int y) override
    {
        auto pos = getPositionAt (x, y);
        moveCaretTo (pos, false);
        repaint(); 
    }
    
    void filesDropped (const juce::StringArray& files, int x, int y) override 
    {
        auto pos = getPositionAt (x, y);
        moveCaretTo (pos, false);
        
        for (auto path : files) 
        {
            juce::File file(path);
            juce::String absolutePath = file.getFullPathName().replaceCharacter('\\', '/');
            
            insertTextAtCaret(absolutePath);
        }
        
        grabKeyboardFocus();
    }
    
    //=============================================== AutoCompletamento ====================================================================== 
    
    juce::String testoDigitatoRigha()
    {
        return getCaretPos().getLineText();
    }
    juce::String testoDigitatoParola()
    {
        const auto caret = getCaretPos();
        const auto& doc = getDocument();
    
        const int lineNumber = caret.getLineNumber();
        const juce::String lineText = doc.getLine(lineNumber);
        const int caretInLine = caret.getIndexInLine();
    
        if (caretInLine == 0)
            return {};
    
        if (!juce::CharacterFunctions::isLetterOrDigit(lineText[caretInLine - 1]) || lineText[caretInLine - 1] == '#')
            return {};
    
        int start = caretInLine;
        while (start > 0 && (juce::CharacterFunctions::isLetterOrDigit(lineText[start - 1]) || lineText[start - 1] == '#'))        
        {
            --start;
        }

        int end = caretInLine;
        while (end < lineText.length() && juce::CharacterFunctions::isLetterOrDigit(lineText[end]))
        {
            ++end;
        }

        return lineText.substring(start, end);
    }
    
    static void moveIteratorTo (CodeDocument::Iterator& it, int targetPos)
    {
        while (! it.isEOF() && it.getPosition() < targetPos)
            it.nextChar();
    }

    // void replaceWordUnderCaret (const juce::String& text)
    // {
    //     auto& doc = getDocument();
    //     auto caretPos = getCaretPos();
    //     const int caretIndex = caretPos.getPosition();
    
    //     // ===============================
    //     // Trova INIZIO parola
    //     // ===============================
    //     CodeDocument::Iterator itBack (doc);
    //     moveIteratorTo (itBack, caretIndex);
    
    //     int startIndex = caretIndex;
    
    //     while (startIndex > 0)
    //     {
    //         itBack.previousChar();
    //         juce_wchar c = itBack.peekNextChar();
    
    //         if (! (juce::CharacterFunctions::isLetterOrDigit(c) || c == '_'))
    //             break;
    
    //         startIndex = itBack.getPosition();
    //     }
    
    //     auto startPos = CodeDocument::Position (doc, startIndex);
    
    //     // ===============================
    //     // Trova FINE parola
    //     // ===============================
    //     CodeDocument::Iterator itFwd (doc);
    //     moveIteratorTo (itFwd, caretIndex);
    
    //     int endIndex = caretIndex;
    
    //     while (! itFwd.isEOF())
    //     {
    //         juce_wchar c = itFwd.nextChar();
    
    //         if (! (juce::CharacterFunctions::isLetterOrDigit(c) || c == '_'))
    //             break;
    
    //         endIndex = itFwd.getPosition();
    //     }
    
    //     auto endPos = CodeDocument::Position (doc, endIndex);
    
    //     // ===============================
    //     // Sostituzione
    //     // ===============================
    //     doc.deleteSection (startPos, endPos);
    //     doc.insertText (startPos, text);
    
    //     moveCaretTo (startPos.movedBy (text.length()), false);
    // }    
    
    void replaceWordUnderCaret (const juce::String& text)
    {
        auto& doc = getDocument();
        const int totalLength = doc.getAllContent().length();
        
        // Otteniamo la posizione attuale del cursore come oggetto Position
        auto caretPos = getCaretPos();
        const int caretIndex = caretPos.getPosition();
    
        // 1. Trova l'inizio della parola
        // Inizializziamo l'iteratore passando l'oggetto Position (caretPos)
        CodeDocument::Iterator itBack (caretPos);
        int startIndex = caretIndex;
    
        while (startIndex > 0)
        {
            juce_wchar c = itBack.previousChar();
            
            if (! (juce::CharacterFunctions::isLetterOrDigit(c) || c == '#' || c == '_'))
                break;
    
            startIndex = itBack.getPosition();
        }
    
        // 2. Trova la fine della parola
        // Inizializziamo un altro iteratore partendo sempre dal cursore
        CodeDocument::Iterator itFwd (caretPos);
        int endIndex = caretIndex;
    
        while (endIndex < totalLength)
        {
            juce_wchar c = itFwd.nextChar();
    
            if (! (juce::CharacterFunctions::isLetterOrDigit(c) || c == '_'))
                break;
    
            endIndex = itFwd.getPosition();
        }
    
        // 3. Esegui la sostituzione
        CodeDocument::Position startPos (doc, startIndex);
        CodeDocument::Position endPos (doc, endIndex);
    
        doc.deleteSection (startPos, endPos);
        doc.insertText (startPos, text);
    
        // 4. Riposiziona il cursore alla fine del nuovo testo
        moveCaretTo (CodeDocument::Position (doc, startIndex + text.length()), false);
    }
    
    void replaceText(String &text)
    {    
        if(testoDigitatoParola().isEmpty())
        {         
            insertTextAtCaret(text);
        }
        else
        {
            replaceWordUnderCaret(text);
        }
    }
    bool hitTest (int x, int y) override
    {
        auto childBounds = autoCompleteListBox.getBounds(); // nello spazio del genitore
        
        if(childBounds.contains(x,y) && autoCompleteListBox.isVisible())
            return false;
        // else if(resizaComp.isVisible() && !helpBounds.contains(x,y))
        //     return false;
        else
            return true;       
    }

    void isIncludeLine()
    {
        juce::String lineText = getCaretPos().getLineText().trim();
        
        if (lineText.startsWith("#include \""))
        {
            autoCompleteListBox.Stato_include = true;
    
            juce::String folderName = lineText.fromFirstOccurrenceOf ("\"", false, false)
                                              .upToFirstOccurrenceOf ("\"", false, false);
            
             // if (folderName.isNotEmpty())
            {
                autoCompleteListBox.updateFile(folderName); 
                autoCompleteListBox.updateContent();            
            }
        }
        else
        {
            autoCompleteListBox.Stato_include = false;
        }
    }

    juce::StringArray getAllIncludedFiles()
    {
        auto& doc = getDocument();
        juce::StringArray includedFiles;

        int totalLines = doc.getNumLines();
    
        for (int i = 0; i < totalLines; ++i)
        {
            juce::String line = doc.getLine(i).trim();
    
            if (line.startsWith("#include"))
            {
                int quoteStart = line.indexOfChar('"');
                int quoteEnd = line.lastIndexOfChar('"');

                if (quoteStart >= 0 && quoteEnd > quoteStart)
                {
                    juce::String filename = line.substring(quoteStart + 1, quoteEnd).trim();
                    includedFiles.addIfNotAlreadyThere(filename);
                }
            }   
        }
        return includedFiles;
    }

    juce::String loadIncludeFileContent(const juce::String& filename)
    {
        if (filename.isEmpty())
            return {};
                    
        juce::File file = directory.getChildFile(filename);
    
        if (!file.existsAsFile())
            return {};
    
        return file.loadFileAsString();
    }
    
    juce::File getDirectory()
    {
        return directory;
    }
    
    void codeDocumentTextInserted(const String& newText, int insertIndex) override
    {   
        {
            const juce::ScopedLock sl(lock);
            previousDocument = getDocument().getAllContent();
        }
        
           startTimer(2000);
    }

    void codeDocumentTextDeleted(int startIndex, int endIndex) override
    {
        {
            const juce::ScopedLock sl(lock);
            previousDocument = getDocument().getAllContent();
        }
        
           startTimer(2000); 
    }
    
    void timerCallback() override
    {
        stopTimer();
        
        if (!isThreadRunning())
        startThread();
    }
    
    void run() override
    {        
        juce::String mainText;
        {   
            const juce::ScopedLock sl(lock);
            mainText = previousDocument;
        }
            
        juce::SortedSet<juce::String> newOpcodesSet;
        juce::SortedSet<juce::String> uniqueWordsSet;
    
        const juce::StringArray includedFiles = getAllIncludedFiles();
    
            auto processContent = [&](const juce::String& content, bool filterOpcodeBlocks)    
            {
                if (content.isEmpty()) return;
    
                juce::StringArray lines;
                lines.addLines(content);
                bool isInsideOpcode = false;
    
                for (auto& line : lines)
                {
                    juce::String trimmedLine = line.trimStart();
                    
                    if (trimmedLine.isEmpty() || trimmedLine.startsWith(";") || trimmedLine.startsWith("//"))
                    continue;
                if (filterOpcodeBlocks && trimmedLine.startsWith("endop"))
                {
                    isInsideOpcode = false;
                    continue;
                }
        
                juce::StringArray tokens;
                tokens.addTokens(trimmedLine, " \t\n\r\\+-*/%=<>!&|^~?:.,()[]{}", "\"");

                for (int i = 0; i < tokens.size(); ++i)
                {
                    const juce::String& t = tokens[i];
                    if (t.startsWithChar('"') || t.length() < 2) continue; 
        
                    if (t == "opcode")
                    {
                        if (i + 1 < tokens.size())
                            newOpcodesSet.add(tokens[i + 1]);
                        
                        if (filterOpcodeBlocks) 
                        {
                            isInsideOpcode = true;
                            break; 
                        }
                    }

                    if (!isInsideOpcode)
                    {
                        const juce_wchar c = t[0];
                        if (c == 'a' || c == 'i' || c == 'k' || c == 'S' || c == 'f' || c == 'g')
                            uniqueWordsSet.add(t);
                    }
                }
            }       
        };

        for (const auto& fileName : includedFiles)
            processContent(loadIncludeFileContent(fileName), true);

        processContent(mainText, false);
    
        for (const auto& fileName : includedFiles)
            processContent(loadIncludeFileContent(fileName),true);
    
            processContent(mainText,false);
    
        for (auto* keyword : CsoundKeywords)
        {
            juce::String kw (keyword);
            
            int indexToRemove = uniqueWordsSet.indexOf (kw);
            
            if (indexToRemove != -1)
                uniqueWordsSet.remove (indexToRemove);
        }
    
        {
            const juce::ScopedLock sl(lock);
            CustomOpcodes.clear();
            
            for (int i = 0; i < newOpcodesSet.size(); ++i)
                CustomOpcodes.add(newOpcodesSet[i]);
        }
        
        autoCompleteListBox.resetItemDynamic();
        
        for (int i = 0; i < newOpcodesSet.size(); ++i)
            autoCompleteListBox.updateVariabileOpcode(newOpcodesSet[i], mainText);
        
        autoCompleteListBox.updateNewOpcode();
    
        for (int i = 0; i < uniqueWordsSet.size(); ++i)
            autoCompleteListBox.updateVariabile(uniqueWordsSet[i], mainText);
    }             

    void resized() override
    {

        MyCodeEditorFont::resized();
                
        int puntoX = getBounds().getWidth()-getBounds().getWidth()/2;
        int puntoY = 0;
        int larghezza = getBounds().getWidth()/2;
        int altezza = getBounds().getHeight()-10;
        
        // resizaComp.setBounds(puntoX,puntoY,larghezza,altezza);        
        // manuale.setBounds(getWidth()/2,0,getWidth()/2,getHeight());
    }
    void paint (Graphics& g)
    {
        MyCodeEditorFont::paint(g);
    }
    bool keyPressed(const juce::KeyPress& key) override
    {  
        if(key == juce::KeyPress::escapeKey)
        {
            autoCompleteListBox.setVisible(false);
            // resizaComp.setVisible(false);
            repaint();
            grabKeyboardFocus();
        }
        
        // if(resizaComp.isVisible())
        //     return true;
        
        MyCodeEditorFont::keyPressed(key); // Richiamo della Funzione Originale
        
        autoCompleteListBox.cercaParola(testoDigitatoParola());
        
        int altezza = autoCompleteListBox.getHeightOttimizzato();
        juce::Rectangle<int> pos = getCaretRectangle();
        autoCompleteListBox.setBounds(pos.getCentreX(),pos.getCentreY(),300,altezza);
        
        if(autoCompleteListBox.getModel()->getNumRows()<=0)
        {
            autoCompleteListBox.setVisible(false);
        }
        
        if(key == juce::KeyPress::backspaceKey)
        {
            if(autoCompleteListBox.getModel()->getNumRows()<=0)
            {
                autoCompleteListBox.setVisible(false);
            }
        }
         
        if(key == juce::KeyPress(key.F1Key))
        {
            auto saveCaretPosition = getCaretPos();
            
            if(autoCompleteListBox.isVisible())
            {
                autoCompleteListBox.setVisible(false);
                
                moveCaretTo(saveCaretPosition,false);
                grabKeyboardFocus();
            }
            else
            {                   
                // updateAuto(); 
                
                isIncludeLine();                
                startThread();
                
                autoCompleteListBox.cercaParola(testoDigitatoParola());
               
                juce::Rectangle<int> pos = getCaretRectangle();
                int altezza = autoCompleteListBox.getHeightOttimizzato();
                
                if(getCaretPos().getLineNumber() < getNumLinesOnScreen()/2)
                    autoCompleteListBox.setBounds(pos.getCentreX(),pos.getCentreY(),300,altezza);
                else
                    autoCompleteListBox.setBounds(pos.getCentreX(),pos.getCentreY()-altezza,300,altezza);
              
                autoCompleteListBox.setVisible(true);
                autoCompleteListBox.grabKeyboardFocus();
            }
        }
        return true;
        
    }
    void mouseDown (const MouseEvent& e) override
    {
        // if(resizaComp.isVisible())
        // return;
        
        grabKeyboardFocus();
        MyCodeEditorFont::mouseDown(e);
        autoCompleteListBox.setVisible(false);
    }
    void mouseDrag (const MouseEvent& e) override
    {
        // if(resizaComp.isVisible())
        // return;
        
        MyCodeEditorFont::mouseDrag(e);
    }
    void mouseUp (const MouseEvent& e) override
    {
        // if(resizaComp.isVisible())
        // return;
        
        MyCodeEditorFont::mouseUp(e); 
    }
    void mouseDoubleClick (const MouseEvent& e) override
    {
        // if(resizaComp.isVisible())
        // return;
        
        MyCodeEditorFont::mouseDoubleClick(e); 
    } 

    void addPopupMenuItems (PopupMenu& menuToAddTo,const MouseEvent* mouseClickEvent) override
    {
        MyCodeEditorFont::addPopupMenuItems(menuToAddTo,mouseClickEvent); // Richiamo della Funzione Originale
        menuToAddTo.addSeparator();
        menuToAddTo.addItem(2000,"Help Opcode",true);
        menuToAddTo.addSubMenu("OPCODES",opcode.menuOpcode,true);            // Cose che aggiungiamo alla funzione Originale
    }

#if JUCE_MAC
    void performPopupMenuAction(int commandID) override
    {      
        if(commandID < 2000)
        {
            juce::String opcodeSintax = opcode.caricaOpcode(commandID);
            insertTextAtCaret(opcodeSintax);
            return;
        }
        else if(commandID == 2000)
        {
            String opcode = getTextInRange(getHighlightedRegion());
            StringArray opcodes(CsoundKeywords);
            
            if(opcodes.contains(opcode))
            {
                // int puntoX = getBounds().getWidth()-(getBounds().getWidth()/2);
                // int puntoY = 0;
                // int larghezza = getBounds().getWidth()/2;
                // int altezza = getBounds().getHeight()-10;
                
                // resizaComp.setBounds(puntoX,puntoY,larghezza,altezza);
                
                juce::String pluginDirectory = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory().getParentDirectory().getFullPathName();
                
                juce::String opcodeHelp("/Library/Audio/Plug-Ins/Components/CsoundPlugin.component/Contents/Resources/Resources/Manual/" + opcode + ".html");
                
                //resizaComp.viewManuale(opcodeHelp);
                
                 //resizaComp.viewManuale("C:/Users/User/Documents/Juce-QtCreator_(Progetti)/Progetti/RikyCodeEditor/html/"+ opcode + ".html");
                //resizaComp.setVisible(true);
                
                juce::URL(opcodeHelp).launchInDefaultBrowser();
            }
            else 
            {
                resizaComp.setVisible(false);   
            }
            
            return;
        }
        
        MyCodeEditorFont::performPopupMenuAction(commandID);    
    }
#elif JUCE_WINDOWS
     void performPopupMenuAction(int commandID) override
    {      
        if(commandID < 2000)
        {
            juce::String opcodeSintax = opcode.caricaOpcode(commandID);
            insertTextAtCaret(opcodeSintax);
            return;
        }
        else if(commandID == 2000)
        {
            String opcode = getTextInRange(getHighlightedRegion());
            StringArray opcodes(CsoundKeywords);
            
            if(opcodes.contains(opcode))
            {                  
                // int puntoX = getBounds().getWidth()-(getBounds().getWidth()/2);
                // int puntoY = 0;
                // int larghezza = getBounds().getWidth()/2;
                // int altezza = getBounds().getHeight()-10;
                
                // resizaComp.setBounds(puntoX,puntoY,larghezza,altezza);
                
                juce::File pluginDirectory = juce::File::getSpecialLocation(juce::File::currentApplicationFile).getParentDirectory();
                juce::File resourcesDirectory = pluginDirectory.getChildFile("Resources");
                juce::File xmlFile = resourcesDirectory.getChildFile("Manuale");
                juce::File opcodeHelp =  xmlFile.getChildFile(opcode +".html");
                String pathOpcode = opcodeHelp.getFullPathName();
                
                // DBG(pathOpcode);
                // resizaComp.viewManuale(pathOpcode);
                
                // // resizaComp.viewManuale("C:/Users/User/Documents/Juce-QtCreator_(Progetti)/Progetti/RikyCodeEditor/html/"+ opcode + ".html");
                // resizaComp.setVisible(true);
                
                juce::URL(pathOpcode).launchInDefaultBrowser();
            }
        }
        MyCodeEditorFont::performPopupMenuAction(commandID);    
    }
#endif

// ==========================================================  MY CODE ===========================================================================================
private:
    OpcodeCsound opcode;
    
    juce::File userDocument = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory); // nome cartella
    juce::String path = userDocument.getFullPathName()+ "/CsoundPluginSynth/CustomOpcode";
    juce::File directory {path};
    
    StringArray variableNamesToShow,variableNames;
    MyListBox autoCompleteListBox;
    // ResizableInnerComponent resizaComp;
    juce::CriticalSection lock;
    String previousDocument;
    
    int lastCaretPos;
};

class CSoundEditorComponent : public juce::Component
{
public:
    CSoundEditorComponent()
    { 
        documento = std::make_unique<juce::CodeDocument>();
        evidenziatore = std::make_unique<CsoundTokeniser>();
        editor = std::make_unique<CsoundCodeEditor>(*documento,evidenziatore.get());
        
        addAndMakeVisible(*editor);
       
        String codeTemplate = R"(<CsoundSynthesizer>
<CsOptions>
-M0
</CsOptions>
<CsInstruments>
    
sr = 44100
ksmps = 128
nchnls = 2
0dbfs = 1


</CsInstruments>
<CsScore>
</CsScore>
</CsoundSynthesizer>)";

    editor->insertTextAtCaret(codeTemplate);
    }

    void resized() override
    {
        editor->setBounds(getLocalBounds());
    }
    
    void save(juce::MemoryBlock& destData)
    {
        juce::MemoryOutputStream stream(destData,true);
        stream.writeString(documento->getAllContent());
    };
    
    void load(const void* data, int sizeInBytes)
    {
        if (data == nullptr || sizeInBytes <= 0)
            return;
        
        MemoryInputStream stream(data, sizeInBytes, false);
        documento->replaceAllContent(stream.readString());
    };
    juce::File getDirectory()
    {
       return editor->getDirectory();
    }
    
    
public:
    std::unique_ptr<juce::CodeDocument> documento;
    std::unique_ptr<CsoundTokeniser> evidenziatore;
    std::unique_ptr<CsoundCodeEditor> editor;
};



