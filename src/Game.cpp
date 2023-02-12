#include "Game.hpp"
Game::Game()
{
    CSDLContext::instance().init();
    
    m_background.set_srcRect(RectPosition(0,0),RectSize(1200,675));
    m_background.LoadTexture("/home/mezory/Documents/Repositories/SDL_SlotMachine/src/assets/background.jpg");
    m_background.set_dstRect(RectPosition(0,0),RectSize(800,800));
    m_bGameLoop = true;
}

Game::~Game()
{
    
}

void Game::Play()
{
    m_SlotMachine.Setup();

    while (m_bGameLoop)
    {
        SDL_RenderClear(CSDLContext::instance().get_renderer());
        m_frameStart = SDL_GetTicks();

        InputHandler();
        Update();
        Render();

        SDL_RenderPresent(CSDLContext::instance().get_renderer());
        m_frameTime = m_frameStart - m_frameStart;

        if (m_frameDelay > m_frameTime)
        {
            SDL_Delay(m_frameDelay - m_frameTime);
        }
    }
}

void Game::InputHandler()
{
   
    m_SlotMachine.InputHandler();
}

void Game::Update()
{
    m_SlotMachine.Update();
}

void Game::Render()
{
  m_background.RenderTexture();
  m_SlotMachine.Render();
}
