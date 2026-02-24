#ifndef CSOUNDFUNZIONITOKEN_H
#define CSOUNDFUNZIONITOKEN_H

// WINDOWS
//#include <JuceLibraryCode/JuceHeader.h>
// MAC
#include <JuceHeader.h>
#include "CsoundOpcode.h"
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
        tableVar,
        globalVar,
        stringVar,
        tag,
        instr,
        myOpcode
    };



struct CsoundTokeniserFunctions
{
    // Verifica se un carattere può iniziare un identificatore
    static bool isIdentifierStart(juce_wchar c) noexcept
    {
        return juce::CharacterFunctions::isLetter(c) || c == '_';
    }

    // Verifica se un carattere può essere parte di un identificatore
    static bool isIdentifierBody(juce_wchar c) noexcept
    {
        return juce::CharacterFunctions::isLetterOrDigit(c) || c == '_';
    }

    // Verifica se una stringa è una parola chiave riservata di Csound
    static bool isReservedKeyword(String::CharPointerType token, int length) noexcept
    {
        StringArray udoKeywords;
        const char* const* k = nullptr;

        if (length < 2)
            return false;

        k = CsoundKeywords; 

        for (int i = 0; k != nullptr && k[i] != nullptr; ++i)
        {
            if (token.compare(CharPointer_ASCII(k[i])) == 0)
                return true;
        }

        for (auto& key : udoKeywords)
        {
            if (token == key)
                return true;
        }

        return false;
    }
    static bool isReservedTag(String::CharPointerType token, int length) noexcept
    {
        StringArray udoKeywords;
        const char* const* k = nullptr;

        if (length < 2)
            return false;

        k = CsoundTag; 

        for (int i = 0; k != nullptr && k[i] != nullptr; ++i)
        {
            if (token.compare(CharPointer_ASCII(k[i])) == 0)
                return true;
        }

        for (auto& key : udoKeywords)
        {
            if (token == key)
                return true;
        }

        return false;
    }
    static bool isReservedInstr(String::CharPointerType token, int length) noexcept
    {
        StringArray udoKeywords;
        const char* const* k = nullptr;

        if (length < 2)
            return false;

        k = CsoundInstr; 

        for (int i = 0; k != nullptr && k[i] != nullptr; ++i)
        {
            if (token.compare(CharPointer_ASCII(k[i])) == 0)
                return true;
        }

        for (auto& key : udoKeywords)
        {
            if (token == key)
                return true;
        }

        return false;
    }
    
    static bool isReservedNewOpcode(String::CharPointerType token, int length) noexcept
    {
        if (length < 2)
            return false;

        juce::String tokenStr(token, length);

        for (auto& key : CustomOpcodes)
        {
            if (tokenStr == key)
                return true;
        }
        return false;
    }

   
    static juce::String trimAngleBrackets(const juce::String& s) noexcept
    {
      int start = 0;
      int end = s.length() - 1;

     // rimuovo < all’inizio
     if (start <= end && s[start] == '<')
         ++start;
    
     // rimuovo > alla fine
     if (end >= start && s[end] == '>')
         --end;
     
     // se start > end, ritorna stringa vuota
     if (start > end)
         return {};

     return s.substring(start, end + 1);
}

// static bool isReservedTag(const juce::String& token) noexcept
// {
//     juce::String cleanToken = trimAngleBrackets(token);

//     for (int i = 0; CsoundTag[i] != nullptr; ++i)
//     {
//         juce::String cleanTag = trimAngleBrackets(CsoundTag[i]);
//         if (cleanToken == cleanTag)
//             return true;
//     }

//     return false;
// }
// static bool isReservedInstr(const juce::String& token) noexcept
// {
//     juce::String cleanToken = trimAngleBrackets(token);

//     for (int i = 0; CsoundInstr[i] != nullptr; ++i)
//     {
//         juce::String cleanTag = trimAngleBrackets(CsoundInstr[i]);
//         if (cleanToken == cleanTag)
//             return true;
//     }

//     return false;
// }
// static bool isReservedKeyword(const juce::String& token) noexcept
// {
//     juce::String cleanToken = trimAngleBrackets(token);

//     for (int i = 0; CsoundKeywords[i] != nullptr; ++i)
//     {
//         juce::String cleanTag = trimAngleBrackets(CsoundKeywords[i]);
//         if (cleanToken == cleanTag)
//             return true;
//     }

//     return false;
// }
    // Riconosce i prefissi di variabili Csound e restituisce il tipo appropriato
    static int classifyCsoundVariable(const juce::String& identifier)
    {
        if (identifier.startsWithChar('a'))
            return CsoundTokenTypes::audioVar;
        if (identifier.startsWithChar('k'))
            return CsoundTokenTypes::controlVar;
        if (identifier.startsWithChar('i'))
            return CsoundTokenTypes::initVar;
        if (identifier.startsWithIgnoreCase("f"))
            return CsoundTokenTypes::tableVar;
        if (identifier.startsWithIgnoreCase("g"))
            return CsoundTokenTypes::globalVar;
        if (identifier.startsWithChar('S') || identifier.startsWithIgnoreCase("gS"))
            return CsoundTokenTypes::stringVar;

        return CsoundTokenTypes::identifier;
    }
    
    // Parsifica un identificatore e lo classifica come keyword, var, o identifier
    template <typename Iterator>
    static int parseIdentifier(Iterator& source) noexcept
    {
        int length = 0;
        
        juce::String::CharPointerType::CharType buffer[100] = {};
        juce::String::CharPointerType writer(buffer);

        while (isIdentifierBody(source.peekNextChar()))
        {
            juce_wchar c = source.nextChar();
            if (length < 99)
                writer.write(c);
            ++length;
        }

        if (length > 0)
        {
            writer.writeNull();
            juce::String identifier(buffer);
            
            if (isReservedTag(juce::String::CharPointerType(buffer),length))
                return CsoundTokenTypes::tag;
            
            if (isReservedInstr(juce::String::CharPointerType(buffer), length))
                return CsoundTokenTypes::instr;

            if (isReservedKeyword(juce::String::CharPointerType(buffer), length))
                return CsoundTokenTypes::keyword;
            
            if (isReservedNewOpcode(juce::String::CharPointerType(buffer), length))
            {
                return CsoundTokenTypes::myOpcode;
            } 
                    
            // Creare una funzione con gli altri tipi di Keywod 

            return classifyCsoundVariable(identifier);
        }

        return CsoundTokenTypes::identifier;
    }
    
    // template <typename Iterator>
    // static int parseIdentifier(Iterator& source) noexcept
    // {
    //     juce::String identifier;
    
    //     while (isIdentifierBody(source.peekNextChar()))
    //         identifier += source.nextChar();
                
    //     if (identifier.isNotEmpty())
    //     {
    //         if (isReservedTag(identifier))
    //             return CsoundTokenTypes::tag;
                
    //         if(isReservedInstr(identifier))
    //             return CsoundTokenTypes::instr;
    
    //         if (isReservedKeyword(identifier))
    //             return CsoundTokenTypes::keyword;
    
    //         return classifyCsoundVariable(identifier);
    //     }

    //     return CsoundTokenTypes::identifier;
    // }

    // Parsifica un numero
    template <typename Iterator>
    static int parseNumber(Iterator& source)
    {   
        bool foundDigit = false;

        if (source.peekNextChar() == '-')
            source.skip();

        while (juce::CharacterFunctions::isDigit(source.peekNextChar()) || source.peekNextChar() == '.')
        {
            foundDigit = true;   
            source.skip();
            
            if(source.peekNextChar() == 'd')
            {
                source.skip();
                if(source.peekNextChar() == 'b')
                {
                    source.skip();
                    if(source.peekNextChar() == 'f')
                    {
                        source.skip();
                        if(source.peekNextChar() == 's')
                        {
                            source.skip();
                            return CsoundTokenTypes::instr;
                        }
                    }
                }
                
            }
        } 
        return foundDigit ? CsoundTokenTypes::number : CsoundTokenTypes::error;
    }

    // Salta i commenti
    template <typename Iterator>
    static void skipComment(Iterator& source) noexcept
    {
        while (!source.isEOF())
        {
            juce_wchar c = source.nextChar();
            if (c == '\n' || c == '\r')
                break;
        }
    }
    
    template <typename Iterator>
    static void skipBlockComment(Iterator& source) noexcept
    {
        while (!source.isEOF())
        {
            juce_wchar c = source.nextChar();
    
            if (c == '*' && source.peekNextChar() == '/')
            {
                source.skip();
                break;
            }
        }
    }

    static bool isOperatorChar(juce_wchar c) noexcept
    {
    // Lista simboli operatore/punteggiatura Csound comune
    constexpr const char operators[] = "+-*/%=<>!&|^~?:.,()[]{}";

    for (char op : operators)
        if (c == op)
            return true;

    return false;
    }
    // Legge e classifica il prossimo token
    template <typename Iterator>
    static int readNextToken(Iterator& source)
    {
        source.skipWhitespace();
        juce_wchar firstChar = source.peekNextChar();
        juce_wchar secondChar = source.peekNextChar();
        
        if (firstChar == 0)
            return CsoundTokenTypes::eof;
        
        if (firstChar == '/')
        {
            source.skip();
            if (!source.isEOF() && source.peekNextChar() == '*')
            {
                source.skip(); 
                skipBlockComment(source);
                return CsoundTokenTypes::comment;
            }
            else
            {
                return CsoundTokenTypes::operatorToken;
            }
        }

        if (firstChar == ';')
        {
            skipComment(source);
            return CsoundTokenTypes::comment;
        }
        
        if (isOperatorChar(firstChar))
        {
            source.skip();
            return CsoundTokenTypes::operatorToken;
        }

        if (isIdentifierStart(firstChar))
            return parseIdentifier(source);
        
        if (juce::CharacterFunctions::isDigit(firstChar) || firstChar == '-')
            return parseNumber(source);

        if (firstChar == '\"')
        {
            source.skip(); // Skip "
            while (!source.isEOF() && source.nextChar() != '\"') {}
            return CsoundTokenTypes::string;
        }

        if (firstChar == '#')
        {
            source.skipToEndOfLine();
            return CsoundTokenTypes::preprocessor;
        }
        
        source.skip(); // Carattere non riconosciuto
        return CsoundTokenTypes::error;
    }
    static inline bool expectOpcodeName = false;
};

#endif // CSOUNDFUNZIONITOKEN_H
