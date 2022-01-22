#include "cgol.h"

#include <time.h>
#include "utils/console.h"
#include <string>


ConwaysGameOfLife::ConwaysGameOfLife(int sizeX, int sizeY)
    : m_cellsAscii(new char[sizeX * sizeY])
{
    m_sizeX = sizeX;
    m_sizeY = sizeY;
    for (int i = 0; i < m_sizeY; i++){
        m_cells.push_back(std::vector<bool>(m_sizeX));
    }

    m_hConOut = CreateFileA(
        "CONOUT$",
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    console::ResizeConsole(m_hConOut, m_sizeX, m_sizeY);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(m_hConOut, &csbi);
    m_oldBufferSize = csbi.dwSize; 
}

ConwaysGameOfLife::~ConwaysGameOfLife() {
    delete[] m_cellsAscii;
}

DWORD
ConwaysGameOfLife::getSimThreadSleepInterval(){
    m_sleepMutex.lock();
    DWORD val = m_simThreadSleep;
    m_sleepMutex.unlock();
    return val;
}

DWORD
ConwaysGameOfLife::setSimThreadSleepInterval(DWORD milliseconds){
    m_sleepMutex.lock();
    DWORD oldVal = m_simThreadSleep;
    m_simThreadSleep = milliseconds;
    m_sleepMutex.unlock();
    return oldVal;
}

bool
ConwaysGameOfLife::setDisplayGeneration(bool display){
    // m_gendMutex.lock();
    bool oldVal = m_displayGeneration;
    m_displayGeneration = display;
    // m_gendMutex.unlock();
    return oldVal;
}

bool
ConwaysGameOfLife::getDisplayGeneration(){
    return m_displayGeneration;
}

bool
ConwaysGameOfLife::isRunning() {
    m_runningMutex.lock();
    bool val = m_isRunning;
    m_runningMutex.unlock();
    return val;
}

bool
ConwaysGameOfLife::exitSignal() {
    m_exitMutex.lock();
    bool val = m_exit;
    m_exitMutex.unlock();
    return val;
}

bool
ConwaysGameOfLife::start() {
    m_runningMutex.lock();
    m_isRunning = true;
    m_runningMutex.unlock();
    if (m_firstRun){
        m_firstRun = false;
        m_simThread = std::thread(&ConwaysGameOfLife::simulation, this);
    }
    return true;
}

bool
ConwaysGameOfLife::stop() {
    m_runningMutex.lock();
    m_isRunning = false;
    m_runningMutex.unlock();
    return true;
}

bool
ConwaysGameOfLife::end() {
    setDisplayGeneration(0);
    stop();
    printCells();
    m_exitMutex.lock();
    m_exit = true;
    m_exitMutex.unlock();
    if (m_simThread.joinable()){
        m_simThread.join();
    }
    return true;
}

void
ConwaysGameOfLife::simulation() {
    while(!exitSignal()){
        while (isRunning()){
            // checkResize();
            printCells();
            update();
            m_sleepMutex.lock();
            DWORD sleepVal = m_simThreadSleep;
            m_sleepMutex.unlock();
            Sleep(sleepVal);
        }
        Sleep(100);
    }
}

void 
ConwaysGameOfLife::printCells() {
    
    memset(m_cellsAscii, ' ', m_sizeX * m_sizeY);
    m_cellsMutex.lock();
    for (int row = 0; row < m_cells.size(); row++){
        for (int col = 0; col < m_cells[0].size(); col++){
            if (m_cells[row][col]){
                m_cellsAscii[row * m_sizeX + col] = '*';
            }
        }
    }
    m_cellsMutex.unlock();

    DWORD dummy;
    WriteConsoleOutputCharacterA(m_hConOut, m_cellsAscii, m_sizeX * m_sizeY , {0,0}, &dummy);

    // m_gendMutex.lock();
    bool val = m_displayGeneration;
    // m_gendMutex.unlock();
    if (val){
        std::string infoString;
        std::string gen = std::to_string(m_generation);
        infoString += "Generation: ";
        infoString += gen;
        infoString += " Thread Delay: ";
        infoString += std::to_string(m_simThreadSleep);
        infoString += " millseconds";

        WriteConsoleOutputCharacterA(m_hConOut, infoString.c_str(), infoString.size(), {0,0}, &dummy);
        // SetConsoleCursorPosition(m_hConOut, {0,0});
        // printf("%d %d", m_generation, m_simThreadSleep);
    }
}

void
ConwaysGameOfLife::update() {
    m_genMutex.lock();
    m_generation++;
    m_genMutex.unlock();

    m_cellsMutex.lock();
    std::vector<std::vector<bool>> newGrid(m_cells);
    for (int row = 0; row < m_sizeY; row++){
        for (int col = 0; col < m_sizeX; col++){
            int numberOfNeighbors = 0;

            numberOfNeighbors += checkBound(row - 1, Axis::Y, Direction::UP) && checkBound(col - 1, Axis::X, Direction::LEFT) && m_cells[row-1][col-1];
            numberOfNeighbors += checkBound(row - 1, Axis::Y, Direction::UP) && m_cells[row-1][col];
            numberOfNeighbors += checkBound(row - 1, Axis::Y, Direction::UP) && checkBound(col + 1, Axis::X, Direction::RIGHT) && m_cells[row-1][col+1];

            numberOfNeighbors += checkBound(col - 1, Axis::X, Direction::LEFT) && m_cells[row][col-1];
            numberOfNeighbors += checkBound(col + 1, Axis::X, Direction::RIGHT) && m_cells[row][col+1];
            
            numberOfNeighbors += checkBound(row + 1, Axis::Y, Direction::DOWN) && checkBound(col - 1, Axis::X, Direction::LEFT) && m_cells[row+1][col-1];
            numberOfNeighbors += checkBound(row + 1, Axis::Y, Direction::DOWN) && m_cells[row+1][col];
            numberOfNeighbors += checkBound(row + 1, Axis::Y, Direction::DOWN) && checkBound(col + 1, Axis::X, Direction::RIGHT) && m_cells[row+1][col+1];

            switch(numberOfNeighbors){
                case 2: {
                    newGrid[row][col] = m_cells[row][col];
                }
                break;
                case 3: {
                    newGrid[row][col] = true;
                }
                break;
                default: {
                    newGrid[row][col] = false;
                }
            }
        }
    }
    m_cells = newGrid;
    m_cellsMutex.unlock();
}

bool 
ConwaysGameOfLife::addPattern(Pattern* pattern, int posX, int posY) {
    m_cellsMutex.lock();
    for (int row = 0; row < pattern->getSizeY(); row++) {
        for (int col = 0; col < pattern->getSizeX(); col++) {
            if (posY + row < m_sizeY && posY + row >= 0 && posX + col < m_sizeX && posX + col >= 0){
                m_cells[posY + row][posX + col] = pattern->getCell(col, row);
            }
        }
    }
    m_cellsMutex.unlock();
    printCells();
    return true;
}

bool
ConwaysGameOfLife::addPattern(Pattern* pattern, int posX, int posY, Rotation rotation) {
    switch(rotation){

        case Rotation::Zero: {
            m_cellsMutex.lock();
            for (int row = 0; row < pattern->getSizeY(); row++) {
                for (int col = 0; col < pattern->getSizeX(); col++) {
                    if (posY + row < m_sizeY && posY + row >= 0 && posX + col < m_sizeX && posX + col >= 0){
                        m_cells[posY + row][posX + col] = pattern->getCell(col, row);
                    }
                }
            }
            m_cellsMutex.unlock();
        }
        break;

        case Rotation::Inverted: {
            m_cellsMutex.lock();
            for (int col = 0; col < pattern->getSizeX(); col++){
                for (int row = 0; row < pattern->getSizeY(); row++){
                    if (posY + row < m_sizeY && posY + row >= 0 && posX + col < m_sizeX && posX + col >= 0){
                        m_cells[posY + row][posX + col] = pattern->getCell(pattern->getSizeX() - 1 - col, row);
                    }
                }
            }
            m_cellsMutex.unlock();
        }
        break;

        case Rotation::Ninety: {
            m_cellsMutex.lock();
            for (int col = 0; col < pattern->getSizeX(); col++){
                for (int row = 0; row < pattern->getSizeY(); row++){
                    if (posY + col < m_sizeY && posY + col >= 0 && posX + row < m_sizeX && posX + row >= 0){
                        m_cells[posY + col][posX + row] = pattern->getCell(pattern->getSizeX() - 1 - col, row);
                    }
                }
            }
            m_cellsMutex.unlock();
        }
        break;

        case Rotation::NinetyInverted: {
            m_cellsMutex.lock();
            for (int row = 0; row < pattern->getSizeY(); row++) {
                for (int col = 0; col < pattern->getSizeX(); col++) {
                    if (posY + col < m_sizeY && posY + col >= 0 && posX + row < m_sizeX && posX + row >= 0){
                        m_cells[posY + col][posX + row] = pattern->getCell(col,  row);
                    }
                }
            }
            m_cellsMutex.unlock();
        }
        break;

        case Rotation::OneEighty: {
            m_cellsMutex.lock();
            for (int row = 0; row < pattern->getSizeY(); row++) {
                for (int col = 0; col < pattern->getSizeX(); col++) {
                    if (posY + row < m_sizeY && posY + row >= 0 && posX + col < m_sizeX && posX + col >= 0){
                        m_cells[posY + row][posX + col] = pattern->getCell(pattern->getSizeX() - 1 - col, pattern->getSizeY() -1 - row);
                    }
                }
            }
            m_cellsMutex.unlock();
        }
        break;

        case Rotation::OneEightyInverted: {
            m_cellsMutex.lock();
            for (int col = 0; col < pattern->getSizeX(); col++){
                for (int row = 0; row < pattern->getSizeY(); row++){
                    if (posY + row < m_sizeY && posY + row >= 0 && posX + col < m_sizeX && posX + col >= 0){
                        m_cells[posY + row][posX + col] = pattern->getCell(col, pattern->getSizeY() - 1 - row);
                    }
                }
            }
            m_cellsMutex.unlock();
        }
        break;

        case Rotation::TwoSeventy: {
            m_cellsMutex.lock();
            for (int col = 0; col < pattern->getSizeX(); col++){
                for (int row = 0; row < pattern->getSizeY(); row++){
                    if (posY + col < m_sizeY && posY + col >= 0 && posX + row < m_sizeX && posX + row >= 0){
                        m_cells[posY + col][posX + row] = pattern->getCell(col, pattern->getSizeY() - 1 - row);
                    }
                }
            }
            m_cellsMutex.unlock();
        }
        break;

        case Rotation::TwoSeventyInverted: {
            m_cellsMutex.lock();
            for (int row = 0; row < pattern->getSizeY(); row++) {
                for (int col = 0; col < pattern->getSizeX(); col++) {
                    if (posY + col < m_sizeY && posY + col >= 0 && posX + row < m_sizeX && posX + row >= 0){
                        m_cells[posY + col][posX + row] = pattern->getCell(pattern->getSizeX() - 1 - col, pattern->getSizeY() -1 - row);
                    }
                }
            }
            m_cellsMutex.unlock();
        }
        break;
    }
    printCells();
    return true;
}

int 
ConwaysGameOfLife::getGeneration() {
    m_genMutex.lock();
    int val = m_generation;
    m_genMutex.unlock();
    return val;
}

bool
ConwaysGameOfLife::checkBound(int position, Axis axis, Direction direction) {
    if (axis == Axis::X){
        if (direction == Direction::LEFT){
            return position >= 0;
        }
        if (direction == Direction::RIGHT){
            return position < m_sizeX;
        }
    }
    if (axis == Axis::Y) {
        if (direction == Direction::UP){
            return position >= 0;
        }
        if (direction == Direction::DOWN){
            return position < m_sizeY;
        }
    }
    return false;
}