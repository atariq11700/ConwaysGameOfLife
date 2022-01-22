#pragma once
#include <vector>

class Pattern {
public:
    bool getCell(int x, int y);
    int getSizeX();
    int getSizeY();

protected:
    std::vector<std::vector<bool>> m_cells;
    Pattern(){}
};