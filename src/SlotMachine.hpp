#ifndef SLOT_MACHINE_HPP
#define SLOT_MACHINE_HPP
#include "CAppSettings/CAppSettings.hpp"
#include "ICommand/CommandManager.hpp"
#include "ICommand/SpinCommand.hpp"
#include "SlotSymbol.hpp"
#include "TextureManager/TextureManager.hpp"

#include <filesystem>
#include <algorithm>
#include <memory>
#include <random>
#include <vector>

class SlotMachine {
public:
  SlotMachine();
  ~SlotMachine();

  void Setup();

  void InputHandler();
  void Update();
  void Render();

private:
  void shuffle(std::vector<std::shared_ptr<SlotSymbol>> &slot_symbols,int divider) {
    if (slot_symbols.empty())
      return;
    
    uint32_t end_index = slot_symbols.size() / divider;
    uint32_t offset = end_index;
    uint32_t start_index = 0;

    std::random_device dev;
    auto rng = std::default_random_engine{dev()};

    for (int i = 0; i < divider; i++) {
      std::shuffle(slot_symbols.begin() + start_index,slot_symbols.begin() + end_index - 1, rng);

      start_index += offset;
      end_index += offset;
    }
  }

  bool IsMouseButtonPressed(uint8_t button_index) {
    return (m_event.button.button == button_index);
  }

  SDL_Event m_event;
  TextureManager m_SlotMachineTexture;
  TextureManager m_SlotBackground;
  TextureManager m_Lever[2];

  std::vector<std::shared_ptr<SlotSymbol>> m_SlotSymbolsVec;
  CommandManager m_PullLeverCommands;
};
#endif //! SLOT_MACHINE_HPP