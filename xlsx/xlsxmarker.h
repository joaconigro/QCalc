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

#ifndef QXLSX_MARKER_H
#define QXLSX_MARKER_H

#include "xlsxglobal.h"
#include <QSharedPointer>

class QXmlStreamReader;
class QXmlStreamWriter;

QT_BEGIN_NAMESPACE_XLSX

class ChartLine;
class MarkerPrivate;
class Q_XLSX_EXPORT Marker
{
    Q_DECLARE_PRIVATE(Marker)

public:
    enum MarkerType {
        MT_Auto,
        MT_Circle,
        MT_Dash,
        MT_Diamond,
        MT_Dot,
        MT_None,
        MT_Plus,
        MT_Square,
        MT_Star,
        MT_Triangle,
        MT_X
    };

    Marker(const QColor& color, MarkerType type = MT_Auto, unsigned size = 5);
    Marker(MarkerType type = MT_Auto, unsigned size = 5);
    ~Marker();

    inline void setMarkerType(MarkerType type) { symbol = type; }
    inline void setMarkerSize(unsigned value) { size = value; }

    inline MarkerType markerType() const { return symbol; }
    inline unsigned   markerSize() const { return size; }

    QString getType();

    void setColor(const QColor& color);
    QString getColor() const;
    bool isCustomColor() const;

private:
    friend class ChartLine;

    MarkerPrivate * d_ptr;

    MarkerType symbol;     // <c:symbol>
    unsigned size;         // <c:size>

    //ChartShapeProperties // <c:spPr>
/* ChartShapeProperties
  \PresetGeometry,
  \CustomGeometry,
  \Transform2D
  \NoFill,        // <a::noFill>
  \SolidFill,     // <a:solidFill>
  \GradientFill,
  \BlipFill,
  \PatternFill,
  \Outline,
  \EffectList,    // <a::effectLst>
  \EffectDag,
  \Scene3DType,
  \Shape3DType,
  \ExtensionList
*/
    //ExtensionList        //<c:extLst>
};

class ChartLinePrivate;
class Q_XLSX_EXPORT ChartLine
{
    Q_DECLARE_PRIVATE(ChartLine)

public:
    ChartLine(const QColor& color, Marker marker = Marker(), QString name = QString(), bool wide = true);
    ChartLine(Marker marker = Marker(), QString name = QString(), bool wide = true);
    ~ChartLine();

    void setName(const QString);
    QString getName() const;
    void setLineWide(bool);
    bool isLineWide() const;
    void setColor(const QColor& color);
    QString getColor() const;
    bool isCustomColor() const;

    Marker  marker;

private:
    ChartLinePrivate * d_ptr;
    QString seriesName;
    bool    wide;
};

QT_END_NAMESPACE_XLSX

#endif // QXLSX_MARKER_H
