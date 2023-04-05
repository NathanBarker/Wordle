#pragma once
#include <string>
#include <vector>

using namespace std;

#define WORD_SIZE 5

struct WordCharacterData
{
    char character;
    bool guessed;
    int position;
};

class WordleGame
{
private:
    const int attempts = 6;
    bool gameIsActive = false;
    
    char lettersGuessed[5] = {' ', ' ', ' ', ' ', ' ',};
    char userScoreCard[5] = {' ', ' ', ' ', ' ', ' ',};

    vector<WordCharacterData> targetWordData;

public:
    WordleGame(const string& file);

    int GetAttempts() const
    {
        return attempts;
    }

    bool getGameIsActive() const
    {
        return gameIsActive;
    }

    void setGameIsActive(const bool active)
    {
        gameIsActive = active;
    }

    char getGuessedCharFromArray(const int& index) const
    {
        return lettersGuessed[index];
    }

    void setGuessedCharToArray(const int& index, const char& character)
    {
        lettersGuessed[index] = character;
    }

    char getUserScoreCharFromArray(const int& index) const
    {
        return userScoreCard[index];
    }

    void setUserScoreCharToArray(const int& index, const char& character)
    {
        userScoreCard[index] = character;
    }

    string SetTargetWord(const string& file) const;
    void GameLoop(const string& targetWord);
};
