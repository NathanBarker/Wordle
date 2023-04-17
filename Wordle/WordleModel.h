#pragma once

#include <vector>

using namespace std;

struct AnswerCharacterData
{
    char character;
    int position;
    bool isRead;
};

struct userOutcomeData
{
    char character;
    bool gotPosition;
    bool gotCharacter;
    int position;
};

class WordleModel
{
private:
    int attempts = 6;

    vector<char> lettersGuessed = {' ', ' ', ' ', ' ', ' ',};

    vector<AnswerCharacterData>* targetWordData = new vector<AnswerCharacterData>{
        {' ', 0, false},
        {' ', 1, false},
        {' ', 2, false},
        {' ', 3, false},
        {' ', 4, false}
    };

    vector<userOutcomeData>* outcomeData = new vector<userOutcomeData>{
        {' ', false, false, 0},
        {' ', false, false, 0},
        {' ', false, false, 0},
        {' ', false, false, 0},
        {' ', false, false, 0}
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

    vector<AnswerCharacterData>* getAnswerData() const
    {
        return targetWordData;
    }

    vector<userOutcomeData>* getOutcomeData() const
    {
        return outcomeData;
    }

    char getInputtedCharAtIndex(const int& index) const
    {
        return lettersGuessed[index];
    }

    void setGuessedCharToArray(const int& index, const char& character)
    {
        lettersGuessed[index] = character;
    }
};