#ifndef __XSCOPE_H__
#define __XSCOPE_H__


template<typename T>
class xscope{
    xscope( T& obj ):m_o(obj){

    }
    ~xscope(){

    }
    T& m_o;
};

template<typename T>
xscope<T> & operator<<(xscope<T>& os, const T& obj)
{
    return os<<obj;
}

template<typename T>
xscope<T>& operator>>(xscope<T>& is, T& obj)
{
    return os>>obj;
}

#endif