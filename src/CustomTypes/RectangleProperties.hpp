#ifndef TEXTURE_PROPERTIES_HPP
#define TEXTURE_PROPERTIES_HPP
#include <cstdint>
#include <utility>

class Width {
private:
  int w_;

public:
  explicit Width(int width) : w_(width) {}
  explicit Width(const Width &width) : w_(width.GetWidth()) {}

  int GetWidth() const { return w_; }
public:
  Width(Width&& other){
    this->w_ = other.w_;
    other.w_ = 0;
  }

};

class Height {
private:
  int h_;

public:
  explicit Height(int height) : h_(height) {}
  explicit Height(const Height &height) : h_(height.GetHeight()) {}

  int GetHeight() const { return h_; }
public:
  Height(Height&& other){
    this->h_ = other.h_;
    other.h_ = 0;
  }
 

};

class RectSize {
private:
  int w_;
  int h_;

public:
  explicit RectSize(int w, int h) : w_(w), h_(h) {}
  explicit RectSize(const Width& w, const Height& h) : w_(w.GetWidth()), h_(h.GetHeight()) {}
  int GetWidth() const { return w_; }
  int GetHeight() const { return h_; }
};

class RectPosition {
private:
  int x_, y_;

public:
  explicit RectPosition(int x, int y) : x_(x), y_(y) {}

  int GetX() const { return x_; }
  int GetY() const { return y_; }
};
 
#endif //! TEXTURE_PROPERTIES_HPP