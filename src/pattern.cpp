#include "pattern.h"



int Pattern::getSizeX() {
    return m_cells[0].size();
}

int Pattern::getSizeY() {
    return m_cells.size();
}

bool Pattern::getCell(int x, int y) {
    return m_cells[y][x];
}