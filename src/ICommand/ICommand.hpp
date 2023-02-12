#ifndef COMMAND_HPP
#define COMMAND_HPP
#include <vector>
#include <memory>
#include "SDL2/SDL.h"
struct SlotSymbol;

class ICommand {
public:
  virtual ~ICommand() {}
  virtual void BeforeExecute() = 0;
  virtual void AfterExecute() = 0;
  
  virtual bool Execute(std::vector<std::shared_ptr<SlotSymbol>>& slot_symbols) = 0;
};
#endif //! COMMAND_HPP