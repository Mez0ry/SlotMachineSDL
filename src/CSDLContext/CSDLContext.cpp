#include "CSDLContext.hpp"

CSDLContext::CSDLContext() {}

CSDLContext::~CSDLContext()
{
  SDL_FreeSurface(m_windowSurface);
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);

  SDL_Quit();
  TTF_Quit();
  IMG_Quit();
  Mix_Quit();
}

void CSDLContext::init()
{
  
  if (SDL_Init(SDL_INIT_EVERYTHING) != EXIT_SUCCESS)
  {
    std::cerr << "Failed to initialized SDL" << '\n';
  }
 
  if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0)
  {
     std::cerr << "Failed to initialized SDL_Image" << '\n';
  }
 
  if (TTF_Init() != EXIT_SUCCESS)
  {
     std::cerr << "Failed to initialized SDL_TTF" << '\n';
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != EXIT_SUCCESS)
  {
     std::cerr << "Failed to initialized SDL_Mixer" << '\n';
  }
  
  uint32_t window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

  if(CAppSettings::instance().get_FullScreen()){
    window_flags += SDL_WINDOW_FULLSCREEN;
  }

  m_window = SDL_CreateWindow("SlotMachine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CAppSettings::instance().get_WindowWidth(),CAppSettings::instance().get_WindowHeight(), window_flags);
  
  
  if (m_window == NULL)
  {
    std::cerr << "Failed to create window" << '\n';
  }
  
  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

  if (m_renderer == NULL)
  {
   std::cerr << "Failed to create Renderer" << '\n';
  }
  
  m_windowSurface = SDL_GetWindowSurface(m_window);
  
  if (m_windowSurface == NULL)
  {
     std::cerr << "Failed to get window surface" << '\n';
  }

}

CSDLContext &CSDLContext::instance()
{
  static CSDLContext instance;
  return instance;
}