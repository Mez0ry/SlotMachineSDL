#ifndef SnakeGame_FONT_MANAGER_HPP
#define SnakeGame_FONT_MANAGER_HPP
#include "../CSDLContext/CSDLContext.hpp"

#include <bitset>
#include <string_view>
#include <iostream>
#include "../CustomTypes/Vector.hpp"

class FontManager
{
private:
  std::string_view m_FontPath;
  std::string_view m_Text;
public:
  FontManager() noexcept;
  ~FontManager() noexcept;

  void LoadFont(const char *path, int font_size);

  void set_Text(const char *text) { this->m_Text = text; }

  const SDL_Rect &get_dstRect() const { return m_dst; }
  void set_dstRect(const SDL_Rect &rect) { m_dst = std::move(rect); }
  void set_dstRect(int x, int y, int width, int height);
  void set_dstWidthAndHeight(int width, int height);

  void DestroyTexture();
  void LoadTexture(const char *text, const SDL_Color &color);

  [[nodiscard]] SDL_Texture *get_Texture() const { return m_Texture; }

  void RenderCopy();

  void RenderTextOnTopOfAnother(SDL_Texture* back_texture,SDL_Rect& back_src,SDL_Rect& back_dst);
  
  bool CursorIsColliding(const Vec2 &cursor_pos);
  bool isSelected() const { return m_Selected[0]; }
  void set_SelectStatus(bool state) { this->m_Selected[0] = state; }

private:
  TTF_Font *m_font;
  SDL_Texture *m_Texture;
  SDL_Rect m_dst;
  SDL_Color m_Color;
  std::bitset<1> m_Selected;
};
#endif //! SnakeGame_FONT_MANAGER_HPP