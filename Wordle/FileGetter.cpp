#include "FileGetter.h"
#include <direct.h>

string FileGetter::GetAddressOfWordFile()
{
    const string stringCurrentDir = _getcwd(nullptr, 0);
    const string stringAddressExtension = "\\Words\\wordle_answers.txt";;
    return stringCurrentDir + stringAddressExtension;
}