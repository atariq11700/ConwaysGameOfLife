#include "console.h"

#undef min
#ifndef min
    #define min(a,b) ((a)<(b)?(a):(b))
#endif


namespace console {
    void ResizeConsole( HANDLE hConsole, SHORT xSize, SHORT ySize ) {   
        CONSOLE_SCREEN_BUFFER_INFO csbi; // Hold Current Console Buffer Info 
        BOOL bSuccess;   
        SMALL_RECT srWindowRect;         // Hold the New Console Size 
        COORD coordScreen;    
        
        bSuccess = GetConsoleScreenBufferInfo( hConsole, &csbi );
        
        // Get the Largest Size we can size the Console Window to 
        coordScreen = GetLargestConsoleWindowSize( hConsole );
        
        // Define the New Console Window Size and Scroll Position 
        srWindowRect.Right  = (SHORT)(min(xSize, coordScreen.X) - 1);
        srWindowRect.Bottom = (SHORT)(min(ySize, coordScreen.Y) - 1);
        srWindowRect.Left   = srWindowRect.Top = (SHORT)0;

        // Define the New Console Buffer Size    
        coordScreen.X = xSize;
        coordScreen.Y = ySize;
        
        // If the Current Buffer is Larger than what we want, Resize the 
        // Console Window First, then the Buffer 
        if( (DWORD)csbi.dwSize.X * csbi.dwSize.Y > (DWORD) xSize * ySize) {
            bSuccess = SetConsoleWindowInfo( hConsole, TRUE, &srWindowRect );
            bSuccess = SetConsoleScreenBufferSize( hConsole, coordScreen );
        }
        
        // If the Current Buffer is Smaller than what we want, Resize the 
        // Buffer First, then the Console Window 
        if( (DWORD)csbi.dwSize.X * csbi.dwSize.Y < (DWORD) xSize * ySize ) {
            bSuccess = SetConsoleScreenBufferSize( hConsole, coordScreen );
            bSuccess = SetConsoleWindowInfo( hConsole, TRUE, &srWindowRect );
        }
        
        // If the Current Buffer *is* the Size we want, Don't do anything! 
        return;
    }
}
