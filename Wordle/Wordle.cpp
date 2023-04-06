#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

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

    cout << targetWord << endl;
    while (getGameIsActive())
    {
        cout << "Current Guessed Letters:" << endl;
        PrintResults(lettersGuessed);

        cout << "Results:" << endl;
        PrintResults(userScoreCard);

        bool userInputtedValidWord = false;
        string userInputtedWord;

        while (!userInputtedValidWord)
        {
            cout << "Enter a 5 letter word" << endl;
            cin >> userInputtedWord;

            std::regex wordRegex("[a-zA-Z]{5}");
            userInputtedValidWord = regex_match(userInputtedWord, wordRegex) ? true : false;
        }

        for (int i = 0; i < WORD_SIZE; i++)
        {
            setGuessedCharToArray(i, userInputtedWord[i]);
        }

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
                        auto it = find(userScoreCard.begin(), userScoreCard.end(), 'A');
                        if (it != userScoreCard.end()) *it = 'R';
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

void WordleGame::PrintResults(const vector<char>& letters)
{
    for (int i = 0; i < WORD_SIZE; i++)
    {
        cout << " [ " << letters[i] << " ] ";
    }
    cout << endl;
}