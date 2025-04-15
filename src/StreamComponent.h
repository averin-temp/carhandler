#pragma once

#include "ApplicationComponent.h"
#include "IStreamComponent.h"

class StreamComponent : public ApplicationComponent, public IStreamComponent
{
public:
    StreamComponent(Application* pApp);
	void create();
	void destroy();
	void run();

    void startStream();
    void stopStream();

};

