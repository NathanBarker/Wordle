#pragma once

#include <string>
#include <vector>

#include "WordleModel.h"

using namespace std;

#define WORD_SIZE 5

class WordleGame
{
private:
    bool gameIsActive = false;
    
public:
    
    WordleGame(const string& file);

    WordleModel* model = nullptr;
    
    bool getGameIsActive() const
    {
        return gameIsActive;
    }

    void setGameIsActive(const bool active)
    {
        gameIsActive = active;
    }
    
    string SetTargetWord(const string& file) const;
    void GameLoop(const string& targetWord) const;
    static void PrintResults(const vector<char>& letters);
    static void PrintGameResults(const string& targetWord, const vector<char>& userGuessed);
    static bool RestartGame();
};