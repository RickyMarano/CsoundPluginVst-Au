// WINDOWS
#include "Customcodeeditorfont.h"
// MAC
// #include "Customcodeeditorfont.h"

namespace juce
{

class MyCodeTokeniser
{
public:
    MyCodeTokeniser() = default;
    virtual ~MyCodeTokeniser() = default;


    virtual int readNextToken (CodeDocument::Iterator& source) = 0;

    /** Returns a suggested syntax highlighting colour scheme. */
    virtual MyCodeEditorFont::MyColourAndFontScheme getDefaultColourScheme() = 0;

private:
    JUCE_LEAK_DETECTOR (MyCodeTokeniser)
};

} // namespace juce
