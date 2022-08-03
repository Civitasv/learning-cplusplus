#pragma once

#include <dlfcn.h>
#include <array>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <iostream>

template <typename E, size_t NumSymbols>
class RePlexModule
{
public:
    RePlexModule(SymbolArray &symbols) : m_symbols(symbols) {}
    static void LoadLibrary() { GetInstance().Load(); }
    static void ReloadLibrary() { GetInstance().Reload(); }

protected:
    static E &GetInstance()
    {
        static E instance;
        return instance;
    }

    // return the path to the library on disk
    virtual const char *GetPath() const = 0;

    // return a reference to an array of C-strings of size NumSymbols.
    // Used when loading or reloading the library to lookup
    // the address of all exported symbols.
    virtual std::array<const char *, NumSymbols> &GetSymbolNames() const = 0;

    template <typename Ret, typename... Args>
    Ret Execute(const int index, Args... args)
    {
        // Lookup the function address
        auto symbol = m_symbols.at(index);
        return reinterpret_cast<Ret (*)(Args...)>(symbol.second)(args...);
    }

    template <typename T>
    T *GetVar(const int index)
    {
        static_assert(Index >= 0 && Index < NumSymbols, "Out of bounds symbol index");
        auto symbol = m_symbols[index];
        return static_cast<T *>(symbol.second);
    }

private:
    void Load()
    {
        m_libHandle = dlopen(GetPath(), RTLD_NOW);
        if (m_libHandle)
        {
            LoadSymbols();
        }
        else
        {
            std::cout << "ERROR!" << std::endl;
        }
    }

    void Reload()
    {
        dlclose(m_libHandle);
        m_symbols.clear();
        Load();
    }

    void LoadSymbols()
    {
        for (auto &&symbol : m_symbols)
        {
            symbol.second = dlsym(m_libHandle, symbol.first);
        }
    }

    void *m_libHandle;

    using SymbolArray = std::array<std::pair<const char *, void *>>;
    SymbolArray &m_symbols;
};