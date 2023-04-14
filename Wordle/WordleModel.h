#pragma once

#include <vector>

using namespace std;

struct WordCharacterData
{
    char character;
    bool guessed;
    int position;
};

class WordleModel
{
private:
    int attempts = 6;

    vector<char> lettersGuessed = {' ', ' ', ' ', ' ', ' ',};
    vector<char> userScoreCard = {' ', ' ', ' ', ' ', ' ',};

    vector<WordCharacterData>* targetWordData = new vector<WordCharacterData>{
        {' ', false, 0},
        {' ', false, 1},
        {' ', false, 2},
        {' ', false, 3},
        {' ', false, 4}
    };

public:
    WordleModel() = default;

    int GetAttempts() const
    {
        return attempts;
    }

    void ReduceAttempts()
    {
        attempts--;
    }

    vector<char> getLettersGuessed()
    {
        return lettersGuessed;
    }

    vector<char>& getUserScoreCard()
    {
        return userScoreCard;
    }

    vector<WordCharacterData>* getTargetWordData() const
    {
        return targetWordData;
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
};
