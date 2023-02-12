#include "SlotSymbol.hpp"

SlotSymbol::SlotSymbol(const std::string& texture_path,SymbolType type) : m_SymbolType(type)
{
    m_SymbolTexture.LoadTexture(texture_path);
}

SlotSymbol::~SlotSymbol()
{
    
}

void SlotSymbol::Render()
{
    m_SymbolTexture.RenderTexture();
}
