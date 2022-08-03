#pragma once

#include <dlfcn.h>
#include <array>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <iostream>

template<typename E, size_t NumSymbols>
class RePlexModule {
  using SymbolArray = std::array<std::pair<const char*, void*>, NumSymbols>;

 public:
  RePlexModule(SymbolArray& symbols) : m_symbols(symbols) {}
  static void LoadLibrary() { GetInstance().Load(); }
  static void ReloadLibrary() { GetInstance().Reload(); }

 protected:
  static E& GetInstance() {
    static E instance;
    return instance;
  }

  // return the path to the library on disk
  virtual const char* GetPath() const = 0;

  template<size_t index, typename Ret, typename... Args>
  Ret Execute(Args... args) {
    // Lookup the function address
    auto symbol = m_symbols.at(index);
    return reinterpret_cast<Ret (*)(Args...)>(symbol.second)(args...);
  }

  template<size_t index, typename T>
  T* GetVar() {
    static_assert(index >= 0 && index < NumSymbols, "Out of bounds symbol index");
    auto symbol = m_symbols[index];
    return static_cast<T*>(symbol.second);
  }

 private:
  void Load() {
    m_libHandle = dlopen(GetPath(), RTLD_NOW);
    if (m_libHandle) {
      LoadSymbols();
    } else {
      std::cout << "ERROR!" << std::endl;
    }
  }

  void Reload() {
    dlclose(m_libHandle);
    for (auto&& symbol: m_symbols) {
      symbol.second = nullptr;
    }
    Load();
  }

  void LoadSymbols() {
    for (auto&& symbol: m_symbols) {
      symbol.second = dlsym(m_libHandle, symbol.first);
    }
  }

  void* m_libHandle;

  SymbolArray& m_symbols;
};