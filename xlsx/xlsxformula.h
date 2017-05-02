/****************************************************************************
** Copyright (c) 2016 Golubchikov Mihail <https://github.com/rue-ryuzaki>
** All right reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/

#ifndef QXLSX_FORMULA_H
#define QXLSX_FORMULA_H

#include <QString>

#include "xlsxglobal.h"

#include "xlsxcellrange.h"
#include "xlsxcellreference.h"

QT_BEGIN_NAMESPACE_XLSX

class CellRange;
class CellReference;

class Q_XLSX_EXPORT Formula
{
public:
    Formula();
    Formula(const Formula & formula);
    ~Formula();

    QString toString() const;

    void clear();

    static Formula COUNTIF(const CellRange& range, const QString& condition);
    static Formula COUNTIF(const CellRange& range, const CellReference& cell);
    static Formula PRODUCT(const CellRange& range);
    static Formula SUM(const CellRange& range);

    static Formula brace(const Formula& formula);

    Formula operator+ (const Formula& rhs) const;
    Formula operator- (const Formula& rhs) const;
    Formula operator* (const Formula& rhs) const;
    Formula operator/ (const Formula& rhs) const;
    Formula operator+ (const QString& rhs) const;
    Formula operator- (const QString& rhs) const;
    Formula operator* (const QString& rhs) const;
    Formula operator/ (const QString& rhs) const;

    Formula& operator+=(const Formula& rhs);
    Formula& operator-=(const Formula& rhs);
    Formula& operator*=(const Formula& rhs);
    Formula& operator/=(const Formula& rhs);
    Formula& operator+=(const QString& rhs);
    Formula& operator-=(const QString& rhs);
    Formula& operator*=(const QString& rhs);
    Formula& operator/=(const QString& rhs);

    bool operator== (const Formula& rhs) const;
    bool operator!= (const Formula& rhs) const;
    Formula& operator=(const Formula& rhs);

private:
    Formula(const QString& string);

    void append(const QString& string);

    QString m_formula;
};

QT_END_NAMESPACE_XLSX

#endif // QXLSX_FORMULA_H
