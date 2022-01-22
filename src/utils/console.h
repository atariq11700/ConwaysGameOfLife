#pragma once

#include <iostream>
#include <windows.h>

namespace console {
    void ResizeConsole(HANDLE hConsole, SHORT xSize, SHORT ySize);
}