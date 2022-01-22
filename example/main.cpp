#include <iostream>
#include <windows.h>
#include <WinUser.h>

#include "../src/utils/console.h"
#include "../src/cgol.h"

#include "../src/patterns/patternAcorn.h"
#include "../src/patterns/patternBlock.h"
#include "../src/patterns/patternBlinker.h"
#include "../src/patterns/patternGlider.h"
#include "../src/patterns/patternGGGun.h"
#include "../src/patterns/patternTest.h"


int main(int argc, const char** argv, const char** envp){
    ConwaysGameOfLife* game = new ConwaysGameOfLife(200,100);

    {
        PatternAcorn p;
        PatternBlock b;
        PatternBlinker f;
        PatternGlider g;
        PatternGGGun h;
        PatternTest t;
        game->addPattern(&g, 0,0);
        game->addPattern(&h, 5,5);
        game->addPattern(&h, 2,21);
        game->addPattern(&h, 75,145, ConwaysGameOfLife::Rotation::OneEighty);
        game->addPattern(&h, 12,134);
        game->addPattern(&h, 42,78, ConwaysGameOfLife::Rotation::TwoSeventyInverted);
        game->addPattern(&h, 12,97);
        game->addPattern(&h, 160,10, ConwaysGameOfLife::Rotation::Inverted);
        game->addPattern(&h, 157,72, ConwaysGameOfLife::Rotation::OneEighty);
        game->addPattern(&h, 120,30, ConwaysGameOfLife::Rotation::Ninety);
        game->addPattern(&h, 135,50, ConwaysGameOfLife::Rotation::NinetyInverted);


        
    }



    while(1){
        if (GetKeyState(VK_UP) & 0x8000) {
            DWORD currInterval = game->getSimThreadSleepInterval();
            if (currInterval <= 5000){
                game->setSimThreadSleepInterval(currInterval + 5);
                Sleep(100);
            }
        }
        if (GetKeyState(VK_RIGHT) & 0x8000) {
            game->setDisplayGeneration(!game->getDisplayGeneration());
            Sleep(50);
        }
        if (GetKeyState(VK_LEFT) & 0x8000) {
            if (game->isRunning()){
                game->stop();
            } else {
                game->start();
            }
            Sleep(100);
        }
        if (GetKeyState(VK_DOWN) & 0x8000) {
            DWORD currInterval = game->getSimThreadSleepInterval();
            if (currInterval >= 5){
                game->setSimThreadSleepInterval(currInterval - 5);
                Sleep(100);
            }
        }
        if (GetKeyState(VK_SPACE) & 0x8000) {
            game->end();
            break;
        }
    }

    int gen = game->getGeneration();
    printf("Game stopped. Total generations: %d\nPress enter to quit.\n", gen);  

    std::cin.get();
    delete game;
}