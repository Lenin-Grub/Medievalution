#pragma once
#include <iostream>
#include <string>
#include <codecvt>
#include <locale>

// Define the enum for icons
enum Icon 
{
    ICON_MIN          = 0xe900,
    ICON_MAX          = 0xe99d,
    ICON_EDIT         = 0xe930,
    ICON_END          = 0xe934,
    ICON_EMPTY_FILES  = 0xe935,
    ICON_REMOVE_FILES = 0xe936,
    ICON_ADD_FILES    = 0xe937,
    ICON_FILES        = 0xe938,
    ICON_FILTER       = 0xe939,
    ICON_FOUR_QUADS   = 0xe93F,
    ICON_INSTAGRAM    = 0xe947,
    ICON_STACK_FILES  = 0xe94A,
    ICON_MENU         = 0xe957,
    ICON_LOG_IN       = 0xe950,
    ICON_LOG_OUT      = 0xe951,
    ICON_MAP          = 0xe953,
    ICON_MONITOR      = 0xe95f,
    ICON_PAUSE        = 0xe965,
    ICON_UPDATE       = 0xe970,
    ICON_BEGIN        = 0xe972,
    ICON_PREV         = 0xe97f,
    ICON_SAVE         = 0xe976,
    ICON_SEARCH       = 0xe978,
    ICON_SETTINGS     = 0xe979,
    ICON_NEXT         = 0xe980,
    ICON_PLAY         = 0xe96B
    };


/// Function to get the Unicode string for a given icon enum
namespace ICON
{
    inline std::string iconToUnicode(Icon icon)
    {
        // Convert the enum value to a wide string
        wchar_t wideChar = static_cast<wchar_t>(icon);

        // Convert the wide string to a UTF-8 encoded string
        std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
        return converter.to_bytes(wideChar);
    }
}