#pragma once
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>

#define SET_ICON(...) ICON::getStr(__VA_ARGS__).c_str()
#define SET_ICON_TEXT(icon, text) (ICON::getStr(icon) + " " + text).c_str()

enum class Icon 
{
    // Used only for init ranged.
    MIN             = 0xe900,
    MAX             = 0xe9A2,

    // Icons.
    CLOUD_LOAD      = 0xe92C,
    EDIT            = 0xe930,
    END             = 0xe934,
    EMPTY_FILES     = 0xe935,
    REMOVE_FILES    = 0xe936,
    ADD_FILES       = 0xe937,
    FILES           = 0xe938,
    FILTER          = 0xe939,
    FOUR_QUADS      = 0xe93F,
    INSTAGRAM       = 0xe947,
    STACK_FILES     = 0xe94A,
    DOT_MENU        = 0xe94D,
    MENU            = 0xe957,
    LOG_IN          = 0xe950,
    LOG_OUT         = 0xe951,
    MAP             = 0xe953,
    LOCATION        = 0xe954,
    SELECT          = 0xe955,
    UNWRAP          = 0xe956,
    MONITOR         = 0xe95f,
    PAUSE           = 0xe965,
    PLAY            = 0xe96B,
    ON_OFF          = 0xe96D,
    UPDATE          = 0xe970,
    LOOP            = 0xe971,
    BEGIN           = 0xe972,
    UNDO            = 0xe973,
    REDO            = 0xe974,
    PREV            = 0xe97f,
    SAVE            = 0xe976,
    SEARCH          = 0xe978,
    SETTINGS        = 0xe979,
    NEXT            = 0xe980,
    TOOL            = 0xe989,
    DUMP_EMPTY      = 0xe98A,
    DUMP_FULL       = 0xe98B,
    OPEN_WITHIN     = 0xe99E,
    PALETTE         = 0xe99F,
    FILL            = 0xe9A0,
    BRUSH           = 0xe9A1,
    FLIP_HORiZONTAL = 0xe9A2
    };


/// Function to get the Unicode string for a given icon enum
namespace ICON
{
    inline std::string getStr(Icon icon)
    {
        wchar_t w_char = static_cast<wchar_t>(icon);
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(w_char);
    }

    inline const char* getChar(Icon icon)
    {
        static std::string str = getStr(icon);
        return str.c_str();
    }
}

