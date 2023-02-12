#ifndef HODER_RANGE_HPP
#define HODER_RANGE_HPP

/**
 * @brief strong-type which represents Range
 */
struct Range {
private:
  int m_a, m_b;

public:
  Range() = default;
  explicit Range(int a, int b) : m_a(a), m_b(b) {}

  int getA() const { return m_a; }
  int getB() const { return m_b; }
};

#endif //! HODER_RANGE_HPP