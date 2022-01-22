#pragma once
#include <iostream>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <vector>


#include "pattern.h"

class ConwaysGameOfLife {
public:
    enum class Rotation: int16_t {
        Zero = 0,
        Inverted = -1,
        Ninety = 90,
        NinetyInverted = -90,
        OneEighty = 180,
        OneEightyInverted = -180,
        TwoSeventy = 270,
        TwoSeventyInverted = -270,
    };

public:
    ConwaysGameOfLife(int sizeX = 100, int sizeY = 100);
    ~ConwaysGameOfLife();
    bool addPattern(Pattern* pattern, int posX, int posY);
    bool addPattern(Pattern* pattern, int posX, int posY, Rotation rotation);
    bool isRunning();
    bool start();
    bool stop();
    bool end();
    bool setDisplayGeneration(bool display);
    bool getDisplayGeneration();
    int getGeneration();
    DWORD getSimThreadSleepInterval();
    DWORD setSimThreadSleepInterval(DWORD milliseconds);


private:
    enum class Axis: uint8_t {
        X,
        Y
    };

    enum class Direction: uint8_t {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

private:
    void printCells();
    void update();
    void simulation();
    void checkResize();
    void handleResize();
    bool exitSignal();
    bool checkBound(int position, Axis axis, Direction direction);

    bool m_firstRun = true;
    bool m_displayGeneration = false;
    
    HANDLE m_hConOut;

    int m_sizeX;
    int m_sizeY;
    int m_generation = 0;

    COORD m_oldBufferSize;
    
    std::thread m_simThread;
    
    std::mutex m_genMutex;
    std::mutex m_cellsMutex;
    std::mutex m_runningMutex;
    std::mutex m_exitMutex;
    std::mutex m_sleepMutex;

    std::vector<std::vector<bool>> m_cells;

    bool m_isRunning = false;
    bool m_exit = false;

    DWORD m_simThreadSleep = 60;

    char* const m_cellsAscii;
};