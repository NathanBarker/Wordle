#include <ctime>
#include <cctype>
#include <fstream>
#include <iostream>

#include "FileGetter.h"
#include "Wordle.h"

int main()
{
    WordleGame wordleGameManager = WordleGame(FileGetter::GetAddressOfWordFile());
    return 0;
}

WordleGame::WordleGame(const string& file)
{
    setGameIsActive(true);
    GameLoop(SetTargetWord(file));
}

string WordleGame::SetTargetWord(const string& fileAddress) const
{
    ifstream file(fileAddress);
    if (!file.is_open())
    {
        cout << "Error opening file: " << fileAddress << endl;
        return "";
    }

    // Determine total number of lines in the file
    int numLines = 0;
    string line;
    while (getline(file, line))
    {
        ++numLines;
    }

    // Refresh stream? TODO: figure out why the text file needs closing and opening
    file.close();
    file.open(fileAddress);

    const auto seed = static_cast<unsigned>(time(nullptr));
    srand(seed);
    const int randomLineNum = rand() % numLines + 1;

    int currentLineNum = 0;
    while (getline(file, line))
    {
        ++currentLineNum;
        if (currentLineNum == randomLineNum)
        {
            file.close();
            return line;
        }
    }
    file.close();
    cout << "Error: Could not find random line." << endl;
    return "";
}

void WordleGame::GameLoop(const string& targetWord)
{
    // TODO: Stop while loop if the player has 0 attempts left or gets the correct word

    while (getGameIsActive())
    {
        cout << "Current Guessed Letters:" << endl;
        for (int i = 0; i < WORD_SIZE; i++)
        {
            cout << " [ " << getGuessedCharFromArray(i) << " ] ";
        }
        cout << endl;
        cout << "Results:" << endl;
        for (int i = 0; i < WORD_SIZE; i++)
        {
            cout << " [ " << getUserScoreCharFromArray(i) << " ] ";
        }
        cout << endl;

        bool userInputtedValidLetter = false;
        char userInputtedCharacter;
        while (!userInputtedValidLetter)
        {
            cout << "Enter a valid letter" << endl;
            cin >> userInputtedCharacter;
            userInputtedValidLetter = isalpha(toupper(userInputtedCharacter)) ? true : false;
        }
    }
}