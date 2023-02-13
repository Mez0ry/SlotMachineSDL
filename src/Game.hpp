#ifndef _GAME_HPP
#define _GAME_HPP
#include "CSDLContext/CSDLContext.hpp"
#include "SlotMachine.hpp"

class Game
{
private:
    uint32_t m_fps = 120;
    uint32_t m_frameDelay = 1500 / m_fps;
    uint32_t m_frameStart;
    uint32_t m_frameTime;

public:
    Game();
    ~Game();
    void Play();

private:
    void InputHandler();
    void Update();
    void Render();

    bool m_bGameLoop = false;
    SDL_Event m_event;
private:
    SlotMachine m_SlotMachine;
    Texture m_background;
};
#endif //!_GAME_HPP