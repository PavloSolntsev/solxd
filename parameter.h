/**************************************************************************
**
** This parameter.h file is part of SolXd software.
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

#ifndef PARAMETER_H
#define PARAMETER_H

/**
* \brief Complex object to store any numeric value
*
*
*/

template <class T> class Parameter
{
public:
    Parameter();
    Parameter(const T& value,  const bool key = 0);

    inline const T& value()const{return _value;}
    inline const bool  &key()const{return _key;}
    inline void set_value(const T& value){_value = value;}
    inline void set_key(const bool &key){_key = key;}

    Parameter<T> &operator=(const Parameter<T> &);
    bool operator==(const Parameter<T> &);
    bool operator!=(const Parameter<T> &);

protected:
    T _value;
    bool _key;
};

template <class T> Parameter<T> &operator+(const Parameter<T> &obj1,const Parameter<T> &obj2);
template <class T> Parameter<T> &operator-(const Parameter<T> &obj1,const Parameter<T> &obj2);
template <class T> Parameter<T> &operator*(const Parameter<T> &obj1,const Parameter<T> &obj2);
template <class T> Parameter<T> &operator/(const Parameter<T> &obj1,const Parameter<T> &obj2);

template <class T> Parameter<T> &operator+(const Parameter<T> &obj1,const T &obj2);
template <class T> Parameter<T> &operator-(const Parameter<T> &obj1,const T &obj2);
template <class T> Parameter<T> &operator*(const Parameter<T> &obj1,const T &obj2);
template <class T> Parameter<T> &operator/(const Parameter<T> &obj1,const T &obj2);

template <class T> Parameter<T> &operator+(const T &obj1,const Parameter<T> &obj2);
template <class T> Parameter<T> &operator-(const T &obj1,const Parameter<T> &obj2);
template <class T> Parameter<T> &operator*(const T &obj1,const Parameter<T> &obj2);
template <class T> Parameter<T> &operator/(const T &obj1,const Parameter<T> &obj2);


#endif // PARAMETER_H



