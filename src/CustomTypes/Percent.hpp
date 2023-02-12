#ifndef HODER_PERCENT_HPP
#define HODER_PERCENT_HPP
#include <cstdint>
#include <tuple>

/**
 * @brief strong-type which represents percent
 */
struct Percent {
private:
  uint8_t m_percent;

public:
  Percent() = default;
  explicit Percent(uint8_t percent) : m_percent(percent) {}

  uint8_t get() const { return m_percent; }

public:
  Percent &operator-=(const Percent &rhs) {
    if (this == &rhs) {
      return *this;
    }

    this->m_percent -= rhs.m_percent;
    return (*this);
  }

  inline friend constexpr bool operator<(const Percent &lhs,
                                         const Percent &rhs) {
    return (std::tie(lhs.m_percent) < std::tie(rhs.m_percent));
  }

  inline friend constexpr bool operator>(const Percent &lhs,
                                         const Percent &rhs) {
    return (std::tie(lhs.m_percent) > std::tie(rhs.m_percent));
  }

  inline friend constexpr bool operator<=(const Percent &lhs,
                                         const Percent &rhs) {
    return (std::tie(lhs.m_percent) <= std::tie(rhs.m_percent));
  }

  inline friend constexpr bool operator>=(const Percent &lhs,
                                         const Percent &rhs) {
    return (std::tie(lhs.m_percent) >= std::tie(rhs.m_percent));
  }

  inline friend constexpr bool operator<(const Percent &lhs,
                                         int rhs) {
    return (std::tie(lhs.m_percent) < std::tie(rhs));
  }

  inline friend constexpr bool operator>(const Percent &lhs,
                                         int rhs) {
    return (std::tie(lhs.m_percent) > std::tie(rhs));
  }

};

#endif //!HODER_PERCENT_HPP