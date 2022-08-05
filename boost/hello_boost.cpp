#include <algorithm>
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <istream>
#include <iterator>

int main(int argc, const char **argv) {
  using namespace boost::lambda;
  typedef std::istream_iterator<int> in;

  std::for_each(in(std::cin), in(), std::cout << (_1 * 3) << " ");

  return 0;
}