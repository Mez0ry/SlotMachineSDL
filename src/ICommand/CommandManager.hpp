#ifndef COMMAND_MANAGER_HPP
#define COMMAND_MANAGER_HPP
#include "ICommand.hpp"
#include <memory>
#include <queue>
class ICommand;

class CommandManager {
public:
  CommandManager() = default;
  ~CommandManager() = default;

  void AddCommand(std::shared_ptr<ICommand> cmd) { m_ActiveCmdQueue.push(cmd); }

  void BeforeExecute() {
    if (m_ActiveCmdQueue.empty())
      return;

    m_ActiveCmdQueue.front()->BeforeExecute();
  }

  void AfterExecute() {
    if (m_ActiveCmdQueue.empty())
      return;

    m_ActiveCmdQueue.front()->AfterExecute();
  }

  template <typename T>
  /**
   * @brief calls execute method on all commands
   * @param argument - data which will be passed in parameters of execute call
   * @return true when every method called otherwise false
   */
  bool ExecuteCommands(T &argument) {
    while (!m_ActiveCmdQueue.empty()) {
      if (m_ActiveCmdQueue.front()->Execute(argument)) {
        m_InactiveCmdQueue.push(m_ActiveCmdQueue.front());
        m_ActiveCmdQueue.pop();
      }else return false;
    }
    return true;
  }

  /**
   * @brief restores internal state
   */
  void restore() {
    if (m_InactiveCmdQueue.empty()) {
      return;
    }
    m_ActiveCmdQueue = std::move(m_InactiveCmdQueue);
  }
  bool isEmpty() const {return m_ActiveCmdQueue.empty();}
private:
  std::queue<std::shared_ptr<ICommand>> m_ActiveCmdQueue;
  std::queue<std::shared_ptr<ICommand>> m_InactiveCmdQueue;
};
#endif //! COMMAND_MANAGER_HPP