#ifndef LIEU_MEM_HPP
#define LIEU_MEM_HPP

#include <memory>

template<class T, class... A>
std::unique_ptr<T> make_unique(A&&... a)
{
    return std::unique_ptr<T>(new T(std::forward<A>(a)...));
}

#endif //LIEU_MEM_HPP
