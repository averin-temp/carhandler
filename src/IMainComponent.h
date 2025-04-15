#pragma once

#include <string>
#include "AppState.h"

class IMainComponent
{
    public:
        virtual AppState getState() = 0;
};