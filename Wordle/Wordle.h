#pragma once
#include <string>

using namespace std;

class WordleGame
{
private:
    const int attempts = 6;
    string* currentDirectory = nullptr;

public:
    int GetAttempts() const
    {
        return attempts;
    }

    WordleGame(const string& file);
};

class FileGetter
{
public:
    static string GetAddressOfWordFile();
};
