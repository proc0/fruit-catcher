#include "game.hpp"

using namespace std;

void Game::End() {
    state = END;
}

bool Game::isRunning(){
    return state != END;
}

void Game::Over() {
    state = OVER;
    timeEnd = GetTime();
}

void Game::Play() {
    state = PLAY;
    score = 0;
    lives = GAME_LIVES;
    timeStart = GetTime();
}

void Game::Update() {
    if(state != END){
        stage.Update();
        mousePosition = GetMousePosition();
    }

    if(state == OVER) {
        if(IsKeyPressed(KEY_R)){
            Play();
        }
    }

    if(state == START){
        display.UpdateStartMenu(mousePosition);

        if(display.isStartButtonClicked()){
            Play();
        }

        if(display.isQuitButtonClicked()){
            End();
        }
    }

    if(state == PLAY) {
        pot.Update();

        tuple<int, int> result = fruits.Update(pot);
        lives += get<0>(result);
        score += get<1>(result);

        if(lives <= 0) {
            Over();
        }
    }
}

void Game::Render() {
    if(state != END){
        stage.Render();
    }

    if(state == START) {
        display.DisplayStartMenu();
    }

    if(state == OVER) {
        display.DisplayGameOver(lives, score, timeEnd, timeStart);
    }

    if(state == PLAY) {
        pot.Render();
        fruits.Render();
        display.DisplayHUD(lives, score);
    }
}

