#include "game.hpp"

using namespace std;

void Game::End() {
    state = END;
}

void Game::Over() {
    state = OVER;
    timeEnd = GetTime();
}

void Game::Play() {
    score = 0;
    lives = GAME_LIVES;
    state = PLAY;
    timeStart = GetTime();
}

void Game::Start() {
    state = START;
}

void Game::Update() {
    if(state != END){
        stage.Update();
        mousePosition = GetMousePosition();
    }
    
    if(state == NONE) {
        Start();
    }

    if(state == OVER) {
        if(IsKeyPressed(KEY_R)){
            Play();
        }
    }

    if(state == START){
        display.UpdateStartMenu(mousePosition);
        if(display.startButtonState == display.ButtonState::CLICKED){
            Play();
        }

        if(display.quitButtonState == display.ButtonState::CLICKED){
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

