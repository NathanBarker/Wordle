#pragma once
#include <string>

using namespace std;

#define WORD_SIZE 5

class WordleGame
{
private:
    const int attempts = 6;
    bool gameIsActive = false;
    char lettersGuessed[5] = {' ',' ',' ',' ',' ',};
    char userScoreCard[5] = {' ',' ',' ',' ',' ',};

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

    char getUserScoreCharFromArray(const int& index) const
    {
        return userScoreCard[index];
    }

    string SetTargetWord(const string& file) const;
    void GameLoop(const string& targetWord);
};