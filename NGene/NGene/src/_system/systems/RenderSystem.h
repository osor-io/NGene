#pragma once
#include "../System.h"

/**
System that handles all the rendering functionality in the application

@@TODO: Implement ImGUI rendering elements such as console, entity viewer, etc.
@@TODO: Implement access and rendering to all "renderable" components

*/
class RenderSystem : public System<RenderSystem> {
public:
    RenderSystem();
    ~RenderSystem();

    void update() override; 
    void startUp() override;
    void shutDown() override;

private:

};

