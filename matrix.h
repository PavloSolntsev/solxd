/**************************************************************************
**
** This matrix.h file is part of SolXd software.
**
** Copyright (C) 2015 pavlo
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

#ifndef MATRIX_H
#define MATRIX_H

class Matrix
{
public:
    Matrix();
private:
    double m11,m12,m13,
           m21,m22,m23,
           m31,m32,m33;

};

#endif // MATRIX_H
