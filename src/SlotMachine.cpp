#include "SlotMachine.hpp"

SlotMachine::SlotMachine() {}

SlotMachine::~SlotMachine() {}

void SlotMachine::Setup() {
  const std::string& symbol = CAppSettings::instance().get_SlashSymbol();
  const std::string& slot_machine_texture_path = CAppSettings::instance().get_SourceFolder() + symbol + "assets" + symbol + "slot_machine" + symbol + "slot-machine4.png";

  m_SlotMachineTexture.LoadTexture(slot_machine_texture_path);

  Width slotMachine_texture_width(816);
  Height slotMachine_texture_height(624);
  RectSize slotMachine_texture_size(slotMachine_texture_width,slotMachine_texture_height);

  m_SlotMachineTexture.set_srcRect(RectPosition(0, 0),RectSize(slotMachine_texture_width, slotMachine_texture_height));

  m_SlotMachineTexture.set_dstRect(RectPosition((CAppSettings::instance().get_WindowWidth() / 2) - (slotMachine_texture_width.GetWidth() / 2), CAppSettings::instance().get_WindowHeight() - slotMachine_texture_height.GetHeight()),slotMachine_texture_size);
  
  std::string symbols_dir_path = CAppSettings::instance().get_SourceFolder() + symbol + "assets" + symbol + "slot_machine" + symbol + "symbols";
  
  std::string symbols_path[4];
  
  int line_counter = 0;
  for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(symbols_dir_path)){
     symbols_path[line_counter] = dirEntry.path().string();
     line_counter++;
  }

  const int slot_symbol_srcTexture_width = 96;
  const int slot_symbol_srcTexture_heigth = 96;
  RectSize SlotSymbol_texture_size(slot_symbol_srcTexture_width, slot_symbol_srcTexture_heigth);
  
  const int col_size = 4;
  const int row_size = 3;

  int symbols_texture_index = 0;

  for (int i = 0; i < col_size * row_size; i++, symbols_texture_index++) {
    if (i != 0 && ((i & 3) == 0)) {
      symbols_texture_index = 0;
    }

    m_SlotSymbolsVec.push_back(std::make_shared<SlotSymbol>(symbols_path[symbols_texture_index], static_cast<SymbolType>(symbols_texture_index + 1)));
    m_SlotSymbolsVec.back()->GetTexture().set_srcRect(RectPosition(0, 0), SlotSymbol_texture_size);
  }

  const std::string& slot_background_texture_path = CAppSettings::instance().get_SourceFolder() + symbol + "assets" + symbol + "slot_machine" + symbol + "slot-machine5.png";
  m_SlotBackground.LoadTexture(slot_background_texture_path);

  m_SlotBackground.set_srcRect(RectPosition(0, 0), RectSize(Width(370), Height(210)));

  m_SlotBackground.set_dstRect(RectPosition(220, 320), RectSize(Width(372), Height(212)));

  uint32_t space_between_symbols = 0; // pixels
  uint32_t x_offset = m_SlotBackground.get_dstRect().x + 10;

  shuffle(m_SlotSymbolsVec, 3);

  const auto &isNthSymbol = [&](int read_index, int n) {
    return (read_index != 0 && ((read_index & n) == 0));
  };

  for (uint32_t i = 0; i < m_SlotSymbolsVec.size(); i++, space_between_symbols += 25) {
    int final_symbol_Ypos = 0;

    if (isNthSymbol(i, 3)) {
      x_offset += (m_SlotBackground.get_dstRect().w / 2 + 40) - slot_symbol_srcTexture_width;
    }

    if (isNthSymbol(i + 1, 3)) {
      int final_space_between_symbols = (space_between_symbols % col_size) + 20;
      final_symbol_Ypos = m_SlotBackground.get_dstRect().y + (final_space_between_symbols + (slot_symbol_srcTexture_heigth * (i % col_size)));
    } else {
      final_symbol_Ypos = m_SlotBackground.get_dstRect().y + ((space_between_symbols % col_size) + (slot_symbol_srcTexture_heigth * (i % col_size)));
    }

    m_SlotSymbolsVec[i]->GetTexture().set_dstRect(RectPosition(x_offset, final_symbol_Ypos), RectSize(slot_symbol_srcTexture_width, slot_symbol_srcTexture_heigth));
  }

  const std::string& notPulled_level_texture_path = CAppSettings::instance().get_SourceFolder() + symbol + "assets" + symbol + "slot_machine" + symbol + "slot-machine2.png";
  m_Lever[0].LoadTexture(notPulled_level_texture_path);
  m_Lever[0].set_srcRect(RectPosition(0, 0), RectSize(109, 276));

  const int lever_texture_width = 80;
  const int lever_texture_height = 247;

  m_Lever[0].set_dstRect(RectPosition((CAppSettings::instance().get_WindowWidth() - (m_Lever[0].get_srcRect().w)) - 20,(CAppSettings::instance().get_WindowHeight() - m_Lever[0].get_srcRect().h) - 20),RectSize(lever_texture_width, lever_texture_height));
  
  const std::string& Pulled_level_texture_path = CAppSettings::instance().get_SourceFolder() + symbol + "assets" + symbol + "slot_machine" + symbol + "slot-machine3.png";
  m_Lever[1].LoadTexture(Pulled_level_texture_path);

  const int lever1_texture_width = 92;
  const int lever1_texture_height = 60;

  m_Lever[1].set_srcRect(RectPosition(0, 0), RectSize(lever1_texture_width, lever1_texture_height));

  RectPosition lever1_pos((CAppSettings::instance().get_WindowWidth() - (m_Lever[1].get_srcRect().w)) - (lever1_texture_width / 2),
                         (m_SlotMachineTexture.get_dstRect().y + m_SlotMachineTexture.get_dstRect().h) - lever1_texture_height);

  m_Lever[1].set_dstRect(lever1_pos, RectSize(lever1_texture_width, lever1_texture_height));
}

void SlotMachine::InputHandler() {
  while (SDL_PollEvent(&m_event)) {
    Vec2 cursor_pos;
    cursor_pos.x = m_event.motion.x;
    cursor_pos.y = m_event.motion.y;
    switch (m_event.type) {
    case SDL_QUIT: {
      
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      if (IsMouseButtonPressed(SDL_BUTTON_LEFT) &&
          m_Lever[0].CursorIsColliding(cursor_pos)) {

        if (!m_PullLeverCommands.isEmpty())
          return;

        Spin spin_command(m_SlotBackground.get_dstRect());
        double acceleration = 0.5;
        int center = m_SlotBackground.get_dstRect().y + (m_SlotBackground.get_dstRect().h / 2);
        spin_command.Setup(SpinModel(20, Range(center - (100 / 2), center + (100 / 2)), acceleration)); // 100 is symbol_texture_height + 4
        m_PullLeverCommands.AddCommand(std::make_shared<Spin>(spin_command));
      }

      break;
    } //! SDL_MOUSEBUTTONDOWN

    case SDL_MOUSEMOTION: {

      break;
    }

    } //! switch
  }
}

void SlotMachine::Update() {
  m_PullLeverCommands.BeforeExecute();
  m_PullLeverCommands.ExecuteCommands(m_SlotSymbolsVec);
  m_PullLeverCommands.AfterExecute();
}

void SlotMachine::Render() {
  bool lever_isPulled = (not m_PullLeverCommands.isEmpty()) ? true : false;

  m_SlotBackground.RenderTexture();
  for (auto &slot_symbol : m_SlotSymbolsVec) {
    slot_symbol->Render();
  }

  m_SlotMachineTexture.RenderTexture();
  if (not lever_isPulled) {
    m_Lever[0].RenderTexture();
  } else {
    m_Lever[1].RenderTexture();
  }
}
