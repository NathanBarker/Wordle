#include "Wordle.h"
#include <direct.h>
#include <fstream>

int main()
{
    WordleGame wordleGameManager = WordleGame(FileGetter::GetAddressOfWordFile());
    return 0;
}

WordleGame::WordleGame(const string& file)
{
}

string FileGetter::GetAddressOfWordFile()
{
    
    const char* currentDir = _getcwd(nullptr, 0);
    const char* addressExtension = "\\Words\\wordle_answers.txt";
    const string stringCurrentDir = currentDir;
    const string stringAddressExtension = addressExtension;
    return stringCurrentDir + stringAddressExtension;
}
