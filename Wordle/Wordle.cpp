#include <algorithm>
#include <ctime>
#include <cctype>
#include <fstream>
#include <iostream>

#include "FileGetter.h"
#include "Wordle.h"

#include <vector>

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

    cout << targetWord << endl;
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

        bool userInputtedValidWord = false;
        string userInputtedWord;

        while (!userInputtedValidWord)
        {
            cout << "Enter a 5 letter word" << endl;
            cin >> userInputtedWord;

            if (userInputtedWord.size() != 5) continue;

            userInputtedValidWord = true;
            for (const char character : userInputtedWord)
            {
                if (!isalpha(toupper(character)))
                {
                    userInputtedValidWord = false;
                    break;
                }
            }
        }

        for (int i = 0; i < WORD_SIZE; i++)
        {
            setGuessedCharToArray(i, userInputtedWord[i]);
        }
        cout << endl;

        for (int i = 0; i < static_cast<int>(targetWord.size()); i++)
        {
            WordCharacterData data{};
            data.character = targetWord[i];
            data.guessed = false;
            data.position = i;
            targetWordData.push_back(data);
        }

        for (int i = 0; i < WORD_SIZE; i++)
        {
            for (int j = 0; j < WORD_SIZE; j++)
            {
                if (getGuessedCharFromArray(i) == targetWordData[j].character && i == targetWordData[j].position)
                // same letter and same position
                {
                    if (targetWordData[j].guessed)
                    {
                        for (int r = 0;  r < WORD_SIZE; r++)
                        {
                            if (getUserScoreCharFromArray(r) == 'A')
                            {
                                setUserScoreCharToArray(r,'R');
                            }
                        }
                    }
                    targetWordData[j].guessed = true;
                    setUserScoreCharToArray(i, 'G');
                    break;
                }
                if (getGuessedCharFromArray(i) == targetWordData[j].character && !targetWordData[j].guessed)
                {
                    targetWordData[j].guessed = true;
                    setUserScoreCharToArray(i, 'A');
                    break;
                }
                setUserScoreCharToArray(i, 'R');
            }
        }
    }
}
