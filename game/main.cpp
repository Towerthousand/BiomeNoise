#include "commons.hpp"
#include "Scene.hpp"

int main() {
    Log::setFlags(Log::Timestamp | Log::StandardOut);
    ContextSettings cs;
    cs.versionMajor = 4;
    cs.versionMinor = 5;
    Game* g = new Game(Window::getFullscreenModes()[0], cs);
    Scene* s = new Scene();
    s->addTo(g);
    g->run();
    delete g;
    return 0;
}
