#pragma once

inline void TnAssert(bool condition)
{
    if (!condition)
    {
        *((int*)(0)) = 0; // crash
    }
}

inline void ExitApplication(int exitCode)
{
#ifdef _DEBUG
    system("PAUSE");
#endif // DEBUG

    std::exit(exitCode);
}
