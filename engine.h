#ifndef ENGINE_H
#define ENGINE_H


class Engine
{
public:
    Engine();

    //Her lages alle texturene, materialene, script

    void gameLoop(float deltatime);
    void render();
    bool isPlaying{false};
};

#endif // ENGINE_H
