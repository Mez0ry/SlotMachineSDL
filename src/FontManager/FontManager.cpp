#include "FontManager.hpp"

FontManager::FontManager() noexcept : m_Texture(nullptr), m_font(nullptr)
{
  
}

FontManager::~FontManager() noexcept
{
  DestroyTexture();
  if (m_font != nullptr)
  {
    TTF_CloseFont(m_font);
    m_font = nullptr;
  }
}

void FontManager::LoadFont(const char *path, int font_size)
{
  m_font = TTF_OpenFont(path, font_size);
  if (m_font == NULL)
  {

    std::cout << "Font can't be opened, Error: " << TTF_GetError() << '\n';
  }
}

void FontManager::DestroyTexture()
{
  if (m_Texture != nullptr)
  {
    SDL_DestroyTexture(m_Texture);
    m_Texture = nullptr;
  }
}

void FontManager::LoadTexture(const char *text, const SDL_Color &color)
{
  if (m_Texture)
    return;

  SDL_Surface *surface = TTF_RenderText_Solid(m_font, text, color);
  if (surface == NULL)
  {
    std::cout << "Can't get surface from font, Error: " << TTF_GetError() << '\n';
  }
  m_Texture = SDL_CreateTextureFromSurface(CSDLContext::instance().get_renderer(), surface);

  SDL_FreeSurface(surface);
  if (m_Texture == NULL)
  {
    std::cout << "Can't get font texture from surface , Error: " << TTF_GetError() << '\n';
  }
}

bool FontManager::CursorIsColliding(const Vec2 &cursor_pos)
{
  return (cursor_pos.x >= m_dst.x && cursor_pos.x <= m_dst.x + m_dst.w && cursor_pos.y >= m_dst.y && cursor_pos.y <= m_dst.y + m_dst.h);
}

void FontManager::set_dstRect(int x, int y, int width, int height)
{
  m_dst.x = x;
  m_dst.y = y;
  m_dst.w = width;
  m_dst.h = height;
}

void FontManager::RenderCopy()
{
  if (SDL_RenderCopy(CSDLContext::instance().get_renderer(), m_Texture, NULL, &m_dst) != EXIT_SUCCESS)
  {
    std::cout << "Failed to blit font surface, Error: " << SDL_GetError() << '\n';
  }
}

void FontManager::RenderTextOnTopOfAnother(SDL_Texture *back_texture, SDL_Rect &back_src, SDL_Rect &back_dst)
{
  SDL_SetTextureBlendMode(back_texture, SDL_BLENDMODE_BLEND);
  SDL_SetRenderTarget(CSDLContext::instance().get_renderer(), back_texture);

  SDL_RenderCopy(CSDLContext::instance().get_renderer(), m_Texture, NULL, &m_dst);

  SDL_SetRenderTarget(CSDLContext::instance().get_renderer(), 0);
  SDL_RenderCopy(CSDLContext::instance().get_renderer(), back_texture, &back_src, &back_dst);
}

void FontManager::set_dstWidthAndHeight(int width, int height)
{
  m_dst.w = width;
  m_dst.h = height;
}