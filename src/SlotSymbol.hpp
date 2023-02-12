#ifndef SLOT_SYMBOL_HPP
#define SLOT_SYMBOL_HPP
#include "TextureManager/TextureManager.hpp"

enum class SymbolType { 
  SEVEN_TYPE = 1, CHERRY_TYPE, BELL_TYPE, BAR_TYPE 
};

class SlotSymbol {
public:
  SlotSymbol(const std::string &texture_path, SymbolType type);
  ~SlotSymbol();

  void Render();

  [[nodiscard]] TextureManager &GetTexture() { return m_SymbolTexture; }
  SymbolType GetType() const {return m_SymbolType;}
  void SetType(SymbolType type) {m_SymbolType = type;}
private:
  TextureManager m_SymbolTexture;
  SymbolType m_SymbolType;
};
#endif //! SLOT_SYMBOL_HPP