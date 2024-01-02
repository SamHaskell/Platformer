#pragma once

#include "core/defines.hpp"

/*
    Simple templated singleton class for engine subsystems. Based on templated singletons in Ogre.
    Initialisation and shutdown is left for the user to do manually for better control.

    To use, simply derive from this, specifying a unique templated type
    i.e. class Manager : public Singleton<Manager> { ... }
*/

template <typename T>
class Singleton
{
protected:
    static T *s_Instance;

public:
    Singleton()
    {
        NT_ASSERT(s_Instance == nullptr, "Cannot have more than one instance of a singleton.");
        s_Instance = static_cast<T *>(this);
    }

    ~Singleton()
    {
        NT_ASSERT(s_Instance != nullptr, "Attempted to destroy singleton instance where it does not exist.");
        s_Instance = nullptr;
    }

    static T &GetInstance()
    {
        return *s_Instance;
    }

    Singleton(const Singleton<T> &) = delete;
    Singleton &operator=(Singleton const &);
};

template <typename T>
T *Singleton<T>::s_Instance = nullptr;