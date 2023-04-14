#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

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
    while (getGameIsActive())
    {
        bool hasUserWon = true;
        for (int i = 0; i < WORD_SIZE; i++)
        {
            if (model->getUserScoreCharFromArray(i) != 'G')
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
        PrintResults(model->getLettersGuessed());

        cout << "Results:" << endl;
        PrintResults(model->getUserScoreCard());

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

        vector<WordCharacterData>* wordCharacterData = model->getTargetWordData();

        if (wordCharacterData != nullptr)
        {
            wordCharacterData->clear();
        }

        for (int i = 0; i < static_cast<int>(targetWord.size()); i++)
        {
            WordCharacterData data{};
            data.character = targetWord[i];
            data.guessed = false;
            data.position = i;
            wordCharacterData->emplace_back(data);
        }

        for (int i = 0; i < WORD_SIZE; i++)
        {
            for (int j = 0; j < WORD_SIZE; j++)
            {
                if (model->getGuessedCharFromArray(i) == wordCharacterData->at(j).character && i ==
                    wordCharacterData->at(j).position)
                // same letter and same position
                {
                    if (wordCharacterData->at(j).guessed)
                    {
                        auto it = find(model->getUserScoreCard().begin(), model->getUserScoreCard().end(), 'A');
                        if (it != model->getUserScoreCard().end()) *it = 'R';
                    }
                    wordCharacterData->at(j).guessed = true;
                    model->setUserScoreCharToArray(i, 'G');
                    break;
                }
                if (model->getGuessedCharFromArray(i) == wordCharacterData->at(j).character &&
                    !wordCharacterData->at(j).guessed)
                {
                    wordCharacterData->at(j).guessed = true;
                    model->setUserScoreCharToArray(i, 'A');
                    break;
                }
                model->setUserScoreCharToArray(i, 'R');
            }
        }
    }
}

void WordleGame::PrintResults(const vector<char>& letters)
{
    for (int i = 0; i < WORD_SIZE; i++)
    {
        // 32 ASCII == ' '
        if (toupper(letters[i]) == 32) continue;
        cout << " [ " << letters[i] << " ] ";
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
        cout << "Target word" << targetWord << endl;
    }
}