# QT       += core gui 
# greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = lib
CONFIG += c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += warn_off
CONFIG += dll
QMAKE_EXTENSION_SHLIB = vst3
QMAKE_BUNDLE_EXTENSION = vst3

CONFIG(release, debug|release)
{
    DESTDIR     = build/Desktop_Qt_6_8_3_MSVC2022_64bit-release
    OBJECTS_DIR = build/Desktop_Qt_6_8_3_MSVC2022_64bit-release/release
    TARGET = CSoundPlugin
}
CONFIG(debug, debug|release)
{
    DESTDIR     = build/Desktop_Qt_6_8_3_MSVC2022_64bit-Debug
    OBJECTS_DIR = build/Desktop_Qt_6_8_3_MSVC2022_64bit-Debug/debug
    # Nome del progetto
    TARGET = CSoundPlugin
}

#Librerie WindowSDK
win32: LIBS += -lShell32 -lgdi32 -luser32 -lkernel32 -lcomctl32 -lcomdlg32 -limm32 -lole32 -loleaut32 -lopengl32 -lrpcrt4 -lshlwapi -luuid -lversion -lwininet -lwinmm -lws2_32 -lwsock32 -static #-lpthread

JUCE_DIR = JUCE
INCLUDEPATH += $$JUCE_DIR
INCLUDEPATH += $$JUCE_DIR/modules
INCLUDEPATH += $$JUCE_DIR/modules/juce_audio_processors/format_types/VST3_SDK

#Tutti i file cpp dei Modui + i File cpp del progetto

# Aggiungi la directory di JUCE
# JUCE_DIR = $$quote(C:/Users/User/Documents/AppProgrammazione/JUCE)

#Libreria CSOUND

win32: LIBS += -L$$PWD/Csound6_x64/lib -lcsound64

INCLUDEPATH += $$PWD/Csound6_x64/include/csound
DEPENDPATH += $$PWD/Csound6_x64/include/csound

# Per altre piattaforme, puoi configurare aggiustamenti specifici se necessario

# Per Windows, dovrai configurare le opzioni di linker per il multithreading
# Aggiungi eventuali librerie esterne se necessarie
# QMAKE_LFLAGS += -fvisibility=hidden -pthread
    
# QMAKE_CFLAGS         = $$QMAKE_CXXFLAGS
# QMAKE_CFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE
# QMAKE_CFLAGS_DEBUG   = $$QMAKE_CXXFLAGS_DEBUG

# Linker flags
# LIBS = -L$$DESTDIR 
# win32: QMAKE_LFLAGS += -static-libstdc++ -static-libgcc
# QMAKE_LFLAGS += 
# QMAKE_LFLAGS_DEBUG += -fvisibility=hidden

HEADERS += \
    JuceLibraryCode/AppConfig.h \
    JuceLibraryCode/JuceHeader.h \
    Source/CSoundFunzioniToken.h \
    Source/CodeEditor.h \
    Source/Component/CsoundConsole.h \
    Source/Component/CsoundConsoleText.h \
    Source/Component/MidiLearnParameters.h \
    Source/Component/MyPresetManager.h \
    Source/Component/MySlider.h \
    Source/Component/PannelloContenitore.h \
    Source/Component/SliderAutomazioni.h \
    Source/CsoundOpcode.h \
    Source/CustomCodeEditorFont/CSoundFunzioniToken.h \
    Source/CustomCodeEditorFont/CodeEditor.h \
    Source/CustomCodeEditorFont/CsoundOpcode.h \
    Source/CustomCodeEditorFont/Customcodeeditorfont.h \
    Source/CustomCodeEditorFont/MyCodeTokenizer.h \
    Source/CustomCodeEditorFont/XmlCustomParse.h \
    Source/CustomOpcode.h \
    Source/MyOpcode/MyOpcode.h \
    Source/PluginEditor.h \
    Source/PluginProcessor.h \
    Source/PopupMenuSearch.h \
    Source/XmlCustomParse.h 

SOURCES += \
    JuceLibraryCode/include_juce_audio_basics.cpp \
    JuceLibraryCode/include_juce_audio_devices.cpp \
    JuceLibraryCode/include_juce_audio_formats.cpp \
    JuceLibraryCode/include_juce_audio_plugin_client_AAX.cpp \
    JuceLibraryCode/include_juce_audio_plugin_client_RTAS_1.cpp \
    JuceLibraryCode/include_juce_audio_plugin_client_RTAS_2.cpp \
    JuceLibraryCode/include_juce_audio_plugin_client_RTAS_3.cpp \
    JuceLibraryCode/include_juce_audio_plugin_client_RTAS_4.cpp \
    JuceLibraryCode/include_juce_audio_plugin_client_RTAS_utils.cpp \
    JuceLibraryCode/include_juce_audio_plugin_client_Standalone.cpp \
    JuceLibraryCode/include_juce_audio_plugin_client_Unity.cpp \
    JuceLibraryCode/include_juce_audio_plugin_client_VST2.cpp \
    JuceLibraryCode/include_juce_audio_plugin_client_VST3.cpp \
    JuceLibraryCode/include_juce_audio_plugin_client_utils.cpp \
    JuceLibraryCode/include_juce_audio_processors.cpp \
    JuceLibraryCode/include_juce_audio_utils.cpp \
    JuceLibraryCode/include_juce_core.cpp \
    JuceLibraryCode/include_juce_cryptography.cpp \
    JuceLibraryCode/include_juce_data_structures.cpp \
    JuceLibraryCode/include_juce_events.cpp \
    JuceLibraryCode/include_juce_graphics.cpp \
    JuceLibraryCode/include_juce_gui_basics.cpp \
    JuceLibraryCode/include_juce_gui_extra.cpp \
    JuceLibraryCode/include_juce_opengl.cpp \
    Source/CustomCodeEditorFont/Customcodeeditorfont.cpp \
    Source/PluginEditor.cpp \
    Source/PluginProcessor.cpp
    
DISTFILES += \
    JuceLibraryCode/include_juce_audio_plugin_client_AU.r \
    JuceLibraryCode/include_juce_audio_plugin_client_RTAS.r
