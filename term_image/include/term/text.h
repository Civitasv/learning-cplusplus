#pragma once

#include "style.h"

namespace term {
template <typename T>
class Text {
 public:
  Text() {}
  Text(T text) : text_(text) {}
  Text(T text, Style style) : text_(text), style_(style) {}

  Text &style(Style style) {
    style_ = style;
    return *this;
  }

  Text &text(T text) {
    text_ = text;
    return *this;
  }

  T text() { return text_; }

  friend std::ostream &operator<<(std::ostream &os, Text &val) {
    os << val.style_ << val.text_ << Decoration::From(Decoration::RESET);
    return os;
  }

 private:
  Style style_;
  T text_;
};
}  // namespace term