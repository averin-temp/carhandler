#pragma once

#include <string>

class IStreamComponent
{
    public:
        virtual void startStream() = 0;
        virtual void stopStream() = 0;
};