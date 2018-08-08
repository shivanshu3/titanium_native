#pragma once

inline void TnAssert(bool condition)
{
    if (!condition)
    {
        *((int*)(0)) = 0; // crash
    }
}
