﻿#include <iostream>
#include <string>

int main(int argc, char** argv) {
  std::cout << "Please: " << '\n';
  std::cout << "1. Encode" << '\n';
  std::cout << "2. Decode" << '\n';

  std::cout << "What do you want to do? ";

  int choice;

  std::cin >> choice;

  if (choice == 1) {
    std::string message;
    std::cout << "Please input the message that you want to encode: ";
    std::cin >> message;

    // TODO ENCODE
  } else if (choice == 2) {
    std::string path;
    std::cout << "Please input path of QR code that you want to decode: ";
    std::cin >> path;

    // TODO DECODE
  }

  return 0;
}