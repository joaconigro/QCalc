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

#ifndef QXLSX_MARKER_P_H
#define QXLSX_MARKER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Xlsx API.  It exists for the convenience
// of the Qt Xlsx.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include "xlsxmarker.h"

#include <QSharedPointer>
#include <QColor>

class QXmlStreamReader;
class QXmlStreamWriter;

namespace QXlsx {

class MarkerPrivate
{
    Q_DECLARE_PUBLIC(Marker)

public:
    MarkerPrivate(Marker *p);
    MarkerPrivate(Marker *p, const QColor& color);
    MarkerPrivate(const MarkerPrivate * const);
    ~MarkerPrivate();

    void setColor(const QColor& color) {
        enable = true;
        xcolor = color;
    }
    QColor getColor() const { return xcolor; }
    bool   isEnable() const { return enable; }

    Marker *q_ptr;

private:

    QColor  xcolor;
    bool    enable;
};

class ChartLinePrivate
{
    Q_DECLARE_PUBLIC(ChartLine)

public:
    ChartLinePrivate(ChartLine *p);
    ChartLinePrivate(ChartLine *p, const QColor& color);
    ChartLinePrivate(const ChartLinePrivate * const mp);

    ~ChartLinePrivate();

    void setColor(const QColor& color) {
        enable = true;
        xcolor = color;
    }
    QColor getColor() const { return xcolor; }
    bool   isEnable() const { return enable; }

private:
    ChartLine *q_ptr;

    QColor  xcolor;
    bool    enable;
};

} // namespace QXlsx

#endif // QXLSX_MARKER_P_H
