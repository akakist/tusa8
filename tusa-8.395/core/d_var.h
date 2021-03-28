#ifndef D_VAR__H
#define D_VAR__H
template <class T>
class d_var
{
    T __container;
public:
    d_var() {}
    operator const T&()const {
        return __container;
    }
    const T* operator ->() const {
        return  &__container;
    }
    T& dbset(const T& c) {
        __container=c;
        __changed=true;
        return __container;
    }
    const T& get() const {
        return __container;
    }
};

#endif