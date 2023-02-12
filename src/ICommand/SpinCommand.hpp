#ifndef SPIN_COMMAND_HPP
#define SPIN_COMMAND_HPP
//CustomTypes
#include "../CustomTypes/Range.hpp"
#include "../CustomTypes/Percent.hpp"

#include "../SlotSymbol.hpp"
#include "ICommand.hpp"

#include <math.h>
#include <random>
#include <exception>

typedef struct SpinModel {
  SpinModel(double timeout, const Range &range, double acceleration)
      : dTimeout(timeout), rStopRange(range), dAcceleration(acceleration) {}
  double dTimeout; // in seconds
  Range rStopRange;
  double dAcceleration;
};

class Spin : public ICommand {
private:
  using VecSymbolsType = std::vector<std::shared_ptr<SlotSymbol>>;

public:
  Spin(const SDL_Rect &rect) :  m_SpinAreaRect(rect) ,m_Acceleration(0.5), m_StartTime(0u), m_PreviousTime(0u),m_DeltaTime(0u){
    m_WinnersVec = GenerateWinners(3);
  }
  ~Spin() = default;

  void Setup(const SpinModel &spin_model) {
    m_Acceleration = spin_model.dAcceleration;
    m_Timeout.SetTimeout(spin_model.dTimeout * 1000);
    m_StopRange = spin_model.rStopRange;
    m_IsStopped = false;
  }

  void SetAcceleration(int acceleration) { m_Acceleration = acceleration; }

  void BeforeExecute() override { StartDeltaTime(); }

  void AfterExecute() override { EndDeltaTime(); }

  bool Execute(VecSymbolsType &slot_symbols) override {
    if (m_StartTime == 0 || m_PreviousTime == 0 || m_IsStopped)
      return false;
    
    if(m_IterationRange.IsEmpty()){
      m_IterationRange.SetRange(0,slot_symbols.size() / 3,4);
    }

    m_Timeout -= (m_DeltaTime > m_Timeout) ? 16 : m_DeltaTime;

    for (auto read_index = m_IterationRange.GetRange().getA(); read_index < m_IterationRange.GetRange().getB() && m_IterationRange.GetRange().getB() <= slot_symbols.size(); read_index++) {
      
      auto &proccessing_symbol = slot_symbols[read_index];

      ScrollVertically(proccessing_symbol);

      proccessing_symbol->Render();

      if (m_Timeout < 0) {
         
          if(!IsInStopRange(proccessing_symbol) || !IsWinnerSymbol(proccessing_symbol) ){
           continue;
          }
          
          if (m_IterationRange.GetRange().getB() == slot_symbols.size()) {
            m_IsStopped = true;
            return true;
          }

          m_IterationRange.Increase();
          m_Timeout.RestoreTimeout();
          return false;
        } 
      
    }
    return false;
  }

private:

  bool IsInStopRange(const std::shared_ptr<SlotSymbol>& proccessing_symbol) const {
    const TextureManager& text = proccessing_symbol->GetTexture();
    return (text.get_dstRect().y > m_StopRange.getA()  && text.get_dstRect().y + text.get_dstRect().h < m_StopRange.getB()) ? true : false;
  }

  bool IsWinnerSymbol(const std::shared_ptr<SlotSymbol>& proccessing_symbol) const {
    return (proccessing_symbol->GetType() == m_WinnersVec.at(m_IterationRange.GetReelIndex()));
  }

  void ScrollVertically(std::shared_ptr<SlotSymbol>& slot_symbol) {

    if (slot_symbol->GetTexture().get_dstRect().y < m_SpinAreaRect.y + m_SpinAreaRect.h) {
      slot_symbol->GetTexture().get_dstRect().y += m_Acceleration * m_DeltaTime;
    } else {
      slot_symbol->GetTexture().get_dstRect().y -= (m_SpinAreaRect.y + m_SpinAreaRect.h / 2);
    }
  }
  
  void StartDeltaTime() {
    m_StartTime = SDL_GetTicks();
    m_DeltaTime = std::abs((double)m_PreviousTime - m_StartTime);
  }

  void EndDeltaTime() { m_PreviousTime = SDL_GetTicks(); }

  /**
   * @brief generates winners
   * @return vector of winners otherwise empty vector
  */
  std::vector<SymbolType> GenerateWinners(int size){
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(1,4);
    
    std::vector<SymbolType> winners;

    for(int i = 0;i<size;i++){
      winners.push_back(static_cast<SymbolType>(dist6(rng)));
    }

    return winners;
  }

private:
  Range m_SymbolProccessingRange;

private:
  /** @brief strong-type which represents timeout*/
  class Timeout {
  private:
    double m_Timeout;
    double m_TimeoutOnCreate;

  public:
    Timeout() = default;
    explicit Timeout(double timeout)
        : m_Timeout(timeout), m_TimeoutOnCreate(timeout) {}

    ~Timeout() = default;

    /** @brief restores internal state to moment which it where created*/
    void RestoreTimeout() { m_Timeout = m_TimeoutOnCreate; }

    void SetTimeout(double timeout) {
      this->m_Timeout = timeout;
      m_TimeoutOnCreate = timeout;
    }
    Timeout &operator-=(const Timeout &rhs) {
      if (this == &rhs) {
        return *this;
      }

      this->m_Timeout -= rhs.m_Timeout;
      return (*this);
    }

    Timeout &operator-=(uint32_t rhs) {
      this->m_Timeout -= rhs;
      return (*this);
    }

    inline friend constexpr bool operator<(const Timeout &lhs,
                                           const Timeout &rhs) {
      return (lhs.m_Timeout < rhs.m_Timeout);
    }

    inline friend constexpr bool operator>(const Timeout &lhs,
                                           const Timeout &rhs) {
      return (lhs.m_Timeout > rhs.m_Timeout);
    }

    inline friend constexpr bool operator>(uint32_t lhs,
                                           const Timeout &rhs) {
      return (lhs > rhs.m_Timeout);
    }

    inline friend constexpr bool operator<=(const Timeout &lhs,
                                            const Timeout &rhs) {
      return (lhs.m_Timeout <= rhs.m_Timeout);
    }

    inline friend constexpr bool operator>=(const Timeout &lhs,
                                            const Timeout &rhs) {
      return (lhs.m_Timeout >= rhs.m_Timeout);
    }

    inline friend constexpr bool operator<(const Timeout &lhs, int rhs) {
      return (lhs.m_Timeout < rhs);
    }

    inline friend constexpr bool operator>(const Timeout &lhs, int rhs) {
      return (lhs.m_Timeout > rhs);
    }
  };

  class IteratingRange{
    private:
    uint32_t m_offset;
    Range m_Range; // current range
    uint32_t m_ReelsIndex;
    public:
    IteratingRange() : m_offset(0),m_Range(0,0), m_ReelsIndex(0){}
    explicit IteratingRange(uint32_t start_index,uint32_t end_index, uint32_t offset) : m_offset(offset),m_Range(start_index,end_index),m_ReelsIndex(0) { }
    ~IteratingRange() = default;

    void SetRange(uint32_t start_index,uint32_t end_index, uint32_t offset){
      m_Range = Range(start_index,end_index);
      m_offset = offset;
    }

    void Increase(){
      m_Range = Range(m_Range.getA() + m_offset ,m_Range.getB() + m_offset);
      m_ReelsIndex++;
    }

    bool IsEmpty(){return (m_offset == 0 || m_Range.getB() == 0) ?  true : false;}

    const Range& GetRange() const {return m_Range;}
    uint32_t GetReelIndex() const {return m_ReelsIndex;}
  };
private:
  std::vector<SymbolType> m_WinnersVec;

  Timeout m_Timeout;
  Range m_StopRange;
  IteratingRange m_IterationRange;
  SDL_Rect m_SpinAreaRect;
  
  double m_Acceleration;
  uint32_t m_StartTime;
  uint32_t m_PreviousTime;
  uint32_t m_DeltaTime;

  bool m_IsStopped;
};

#endif //! SPIN_COMMAND_HPP
