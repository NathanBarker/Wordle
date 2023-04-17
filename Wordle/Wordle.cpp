#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>
#include <Windows.h>

#include "FileGetter.h"
#include "Wordle.h"
#include "WordleModel.h"

int main()
{
    WordleGame wordleGameController = WordleGame(FileGetter::GetAddressOfWordFile());
    return 0;
}

WordleGame::WordleGame(const string& file)
{
    setGameIsActive(true);
    while (gameIsActive)
    {
        model = new WordleModel();
        GameLoop(SetTargetWord(file));
        gameIsActive = RestartGame();
    }
}

bool WordleGame::RestartGame()
{
    cout << "Press Y to restart";
    char userPrompt;
    cin >> userPrompt;
    return toupper(userPrompt) == 'Y' ? true : false;
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

void WordleGame::GameLoop(const string& targetWord) const
{
    cout << targetWord << endl;
    while (getGameIsActive())
    {
        bool hasUserWon = true;
        for (int i = 0; i < WORD_SIZE; i++)
        {
            if (!model->getOutcomeData()->at(i).gotCharacter || !model->getOutcomeData()->at(i).gotPosition)
            {
                hasUserWon = false;
            }
        }

        if (model->GetAttempts() == 0 || hasUserWon)
        {
            PrintGameResults(targetWord, model->getLettersGuessed());
            return;
        }

        cout << "Current Guessed Letters:" << endl;
        PrintResults(*model->getOutcomeData());

        bool isUserInputValid = false;
        string userInputtedWord;

        while (!isUserInputValid)
        {
            cout << "Enter a 5 letter word" << endl;
            cin >> userInputtedWord;

            regex wordRegex("[a-zA-Z]{5}");
            isUserInputValid = regex_match(userInputtedWord, wordRegex) ? true : false;

            if (!isUserInputValid) cout << "Invalid input" << endl;
        }

        model->ReduceAttempts();

        for (int i = 0; i < WORD_SIZE; i++)
        {
            model->setGuessedCharToArray(i, userInputtedWord[i]);
        }

        vector<AnswerCharacterData>* wordCharacterData = model->getAnswerData();

        if (wordCharacterData == nullptr)
        {
            cout << "wordCharacterData corrupted, please restart the game";
            return;
        }
        wordCharacterData->clear();

        for (int i = 0; i < static_cast<int>(targetWord.size()); i++)
        {
            AnswerCharacterData data{};
            data.character = targetWord[i];
            data.position = i;
            data.isRead = false;
            wordCharacterData->emplace_back(data);
        }

        vector<userOutcomeData>* outcomeData = model->getOutcomeData();

        if (outcomeData == nullptr)
        {
            cout << "outcomeData corrupted, please restart the game";
            return;
        }
        outcomeData->clear();

        for (int i = 0; i < static_cast<int>(model->getLettersGuessed().size()); i++)
        {
            userOutcomeData data{};
            data.character = model->getLettersGuessed()[i];
            data.gotPosition = false;
            data.gotCharacter = false;
            data.position = i;
            outcomeData->emplace_back(data);
        }

        for (int i = 0; i < WORD_SIZE; i++)
        {
            if (model->getInputtedCharAtIndex(i) == wordCharacterData->at(i).character)
            {
                wordCharacterData->at(i).isRead = true;
                vector<userOutcomeData> repeatedCharacters;
                auto it = copy_if(model->getOutcomeData()->begin(), model->getOutcomeData()->end(),
                                  back_inserter(repeatedCharacters),
                                  [&](const userOutcomeData& outcomeCharacterData)
                                  {
                                      return outcomeCharacterData.character == wordCharacterData->at(i).character &&
                                          !outcomeCharacterData.gotPosition && outcomeCharacterData.gotCharacter;
                                  });

                for (const userOutcomeData& data : repeatedCharacters)
                {
                    outcomeData->at(data.position).gotCharacter = false;
                }

                outcomeData->at(i).gotPosition = true;
                outcomeData->at(i).gotCharacter = true;
            }

            for (int j = 0; j < WORD_SIZE; j++)
            {
                if (model->getInputtedCharAtIndex(i) == wordCharacterData->at(j).character)
                {
                    auto it = find_if(model->getOutcomeData()->begin(), model->getOutcomeData()->end(),
                                      [&](const userOutcomeData& outcomeCharacterData)
                                      {
                                          return outcomeCharacterData.character == wordCharacterData->at(j).character &&
                                              !wordCharacterData->at(j).isRead && !outcomeCharacterData.gotCharacter;
                                      });
                    if (it != model->getOutcomeData()->end())
                    {
                        wordCharacterData->at(j).isRead = true;
                        it->gotCharacter = true;
                        break;
                    }
                }
            }
        }
    }
}

void WordleGame::PrintResults(const vector<userOutcomeData>& lettersData)
{
    const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < WORD_SIZE; i++)
    {
        // 32 ASCII == ' '
        if (toupper(lettersData[i].character) == 32) continue;

        const char letterToOutput = lettersData[i].character;

        if (lettersData[i].gotCharacter && lettersData[i].gotPosition)
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
        }
        if (lettersData[i].gotCharacter && !lettersData[i].gotPosition)
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
        }
        if (!lettersData[i].gotCharacter && !lettersData[i].gotPosition)
        {
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
        }

        cout << " [ " << letterToOutput << " ] ";
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }
    cout << endl;
}

void WordleGame::PrintGameResults(const string& targetWord, const vector<char>& userGuessed)
{
    const string word(userGuessed.begin(), userGuessed.end());
    if (targetWord == word)
    {
        cout << "You got the secret word!" << endl;
        cout << "Target word" << " " << targetWord << endl;
    }
    else
    {
        cout << "You failed :(" << endl;
        cout << "Target word" << " " << targetWord << endl;
    }
}