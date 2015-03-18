/**************************************************************************
**
** This parameter.cpp file is part of SolXd software.
**
** Copyright (C) 2015 Pavlo Solntsev
**
** SolXd is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Foobar is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#include "parameter.h"

template <class T>
Parameter<T>::Parameter():_value(T()),_key(false)
{
}

template <class T>
Parameter<T>::Parameter(const T &value, const bool key):_value(value),_key(key)
{

}

template <class T>
Parameter<T> &Parameter<T>::operator=(const Parameter& value)
{
//    this->_value = value;
    swap(value);
    return *this;
}

template <class T>
bool Parameter<T>::operator==(const Parameter<T> &obj)
{
    return (this->_value == obj.value()) ? true : false;
}

template <class T>
bool Parameter<T>::operator!=(const Parameter<T> &obj)
{
    return !(*this == obj);
}

template <class T>
Parameter<T> &operator+(const Parameter<T> &obj1,const Parameter<T> &obj2)
{
    Parameter<T> res(obj1.value()+obj2.value());
    return res;
}

template <class T>
Parameter<T> &operator-(const Parameter<T> &obj1, const Parameter<T> &obj2)
{
    Parameter<T> res(obj1.value()-obj2.value());
    return res;

}

template <class T>
Parameter<T> &operator*(const Parameter<T> &obj1, const Parameter<T> &obj2)
{
    Parameter<T> res(obj1.value()*obj2.value());
    return res;
}

template <class T>
Parameter<T> &operator/(const Parameter<T> &obj1, const Parameter<T> &obj2)
{
    Parameter<T> res(obj1.value()/obj2.value());
    return res;
}

template <class T>
Parameter<T> &operator+(const Parameter<T> &obj1, const T &obj2)
{
    Parameter<T> res(obj1);

    res.set_value(res.value()+obj2);

    return res;
}

template <class T>
Parameter<T> &operator-(const Parameter<T> &obj1, const T &obj2)
{

}

template <class T>
Parameter<T> &operator*(const Parameter<T> &obj1, const T &obj2)
{

}

template <class T>
Parameter<T> &operator/(const Parameter<T> &obj1, const T &obj2)
{

}

template <class T>
Parameter<T> &operator+(const T &obj1, const Parameter<T> &obj2)
{

}

template <class T>
Parameter<T> &operator-(const T &obj1, const Parameter<T> &obj2)
{

}

template <class T>
Parameter<T> &operator*(const T &obj1, const Parameter<T> &obj2)
{

}

template <class T>
Parameter<T> &operator/(const T &obj1, const Parameter<T> &obj2)
{

}
