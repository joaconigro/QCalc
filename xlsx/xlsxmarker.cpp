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

#include "xlsxmarker_p.h"

#include <QIODevice>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

QT_BEGIN_NAMESPACE_XLSX

MarkerPrivate::MarkerPrivate(Marker *p) :
    q_ptr(p), enable(false)
{
}

MarkerPrivate::MarkerPrivate(Marker *p, const QColor& color) :
    q_ptr(p), xcolor(color), enable(true)
{
}

MarkerPrivate::MarkerPrivate(const MarkerPrivate * const mp) :
    xcolor(mp->xcolor), enable(mp->enable)
{
}

MarkerPrivate::~MarkerPrivate()
{
}

/*!
 * \class Marker
 * \inmodule QtXlsx
 * \brief Main class for the markers.
 */

/*!
  \enum Marker::MarkerType

  \value MT_Auto
  \value MT_Circle,
  \value MT_Dash,
  \value MT_Diamond,
  \value MT_Dot,
  \value MT_None,
  \value MT_Plus,
  \value MT_Square,
  \value MT_Star,
  \value MT_Triangle,
  \value MT_X
*/

/*!
 * \internal
 */

Marker::Marker(const QColor& color, MarkerType type, unsigned size)
    : d_ptr(new MarkerPrivate(this, color)), symbol(type), size(size)
{
}

Marker::Marker(MarkerType type, unsigned size)
    : d_ptr(new MarkerPrivate(this)), symbol(type), size(size)
{
}

/*!
 * Destroys the marker.
 */

Marker::~Marker()
{
}

QString Marker::getType() {
    switch (symbol) {
        case MT_Auto :
            return QLatin1String("auto");
        case MT_Circle :
            return QLatin1String("circle");
        case MT_Dash :
            return QLatin1String("dash");
        case MT_Diamond :
            return QLatin1String("diamond");
        case MT_Dot :
            return QLatin1String("dot");
        case MT_None :
            return QLatin1String("none");
        case MT_Plus :
            return QLatin1String("plus");
        case MT_Square :
            return QLatin1String("square");
        case MT_Star :
            return QLatin1String("star");
        case MT_Triangle :
            return QLatin1String("triangle");
        case MT_X :
            return QLatin1String("x");
        default :
            return QLatin1String("auto");
    }
}

void Marker::setColor(const QColor& color) {
    Q_D(Marker);
    d->setColor(color);
}

QString hexFromInt(int value)
{
    switch (value) {
        case 0 :
        case 1 :
        case 2 :
        case 3 :
        case 4 :
        case 5 :
        case 6 :
        case 7 :
        case 8 :
        case 9 :  return QString::number(value);
        case 10 : return QLatin1String("A");
        case 11 : return QLatin1String("B");
        case 12 : return QLatin1String("C");
        case 13 : return QLatin1String("D");
        case 14 : return QLatin1String("E");
        case 15 : return QLatin1String("F");
        default : return QLatin1String("");
    }
}

QString hexColor(const QColor& color)
{
    QString result = QLatin1String("");
    result += hexFromInt(color.red() / 16);
    result += hexFromInt(color.red() % 16);
    result += hexFromInt(color.green() / 16);
    result += hexFromInt(color.green() % 16);
    result += hexFromInt(color.blue() / 16);
    result += hexFromInt(color.blue() % 16);
    return result;
}

QString Marker::getColor() const
{
    Q_D(const Marker);
    return hexColor(d->getColor());
}

bool Marker::isCustomColor() const
{
    Q_D(const Marker);
    return d->isEnable();
}

/*!
 *
 */

ChartLinePrivate::ChartLinePrivate(ChartLine *p) :
    q_ptr(p), enable(false)
{
}

ChartLinePrivate::ChartLinePrivate(ChartLine *p, const QColor& color) :
    q_ptr(p), xcolor(color), enable(true)
{
}

ChartLinePrivate::ChartLinePrivate(const ChartLinePrivate * const mp) :
    xcolor(mp->xcolor), enable(mp->enable)
{
}

ChartLinePrivate::~ChartLinePrivate()
{
}

/*!
 * \class ChartLine
 * \inmodule QtXlsx
 * \brief Main class for the lines in chart.
 */

/*!
 * \internal
 */

ChartLine::ChartLine(const QColor& color, Marker marker, QString name, bool wide)
    : marker(marker), d_ptr(new ChartLinePrivate(this, color)), seriesName(name), wide(wide)
{
}

ChartLine::ChartLine(Marker marker, QString name, bool wide)
    : marker(marker), d_ptr(new ChartLinePrivate(this)), seriesName(name), wide(wide)
{
}

/*!
 * Destroys the chart line.
 */

ChartLine::~ChartLine()
{
}

void ChartLine::setName(const QString name)
{
    seriesName = name;
}

QString ChartLine::getName() const
{
    return seriesName;
}

void ChartLine::setLineWide(bool isWide)
{
    wide = isWide;
}

bool ChartLine::isLineWide() const
{
    return wide;
}

void ChartLine::setColor(const QColor& color) {
    Q_D(ChartLine);
    d->setColor(color);
}

QString ChartLine::getColor() const
{
    Q_D(const ChartLine);
    return hexColor(d->getColor());
}

bool ChartLine::isCustomColor() const
{
    Q_D(const ChartLine);
    return d->isEnable();
}

QT_END_NAMESPACE_XLSX
