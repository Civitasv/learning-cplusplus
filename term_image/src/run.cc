#include "term/term_image.h"

int main() {
  using namespace term;

  TermImage term_image{};
  term_image.ShowImage("download.jpg");
  return 0;
}