/****************************************************************************
** Copyright (c) 2013-2014 Debao Zhang <hello@debao.me>
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

#include "xlsxchart_p.h"
#include "xlsxworksheet.h"
#include "xlsxcellrange.h"
#include "xlsxutility_p.h"

#include <QIODevice>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

QT_BEGIN_NAMESPACE_XLSX

ChartPrivate::ChartPrivate(Chart *q, Chart::CreateFlag flag)
    :AbstractOOXmlFilePrivate(q, flag), chartType(static_cast<Chart::ChartType>(0))
{
}

ChartPrivate::~ChartPrivate()
{
}

/*!
 * \class Chart
 * \inmodule QtXlsx
 * \brief Main class for the charts.
 */

/*!
  \enum Chart::ChartType

  \value CT_Area
  \value CT_Area3D,
  \value CT_Line,
  \value CT_Line3D,
  \value CT_Scatter,
  \value CT_ScatterLine,
  \value CT_Pie,
  \value CT_Pie3D,
  \value CT_Doughnut,
  \value CT_Bar,
  \value CT_Bar3D,

  \omitvalue CT_Stock,
  \omitvalue CT_Radar,
  \omitvalue CT_OfPie,
  \omitvalue CT_Surface,
  \omitvalue CT_Surface3D,
  \omitvalue CT_Bubble
*/

/*!
 * \internal
 */
void InsertAxisXY(QList<QSharedPointer<XlsxAxis> > &axis)
{
    axis.append(QSharedPointer<XlsxAxis>(new XlsxAxis(XlsxAxis::T_Cat, XlsxAxis::Bottom, 0, 1)));
    axis.append(QSharedPointer<XlsxAxis>(new XlsxAxis(XlsxAxis::T_Val, XlsxAxis::Left, 1, 0)));
}

void InsertAxisZ(QList<QSharedPointer<XlsxAxis> > &axis)
{
    axis.append(QSharedPointer<XlsxAxis>(new XlsxAxis(XlsxAxis::T_Ser, XlsxAxis::Bottom, 2, 0)));
}

Chart::Chart(AbstractSheet *parent, CreateFlag flag)
    :AbstractOOXmlFile(new ChartPrivate(this, flag))
{
    d_func()->sheet = parent;
}

/*!
 * Destroys the chart.
 */
Chart::~Chart()
{
}

/*!
 * Add the data series which is in the range \a range of the \a sheet.
 */
void Chart::addSeries(const CellRange &range, AbstractSheet *sheet, ChartLine line)
{
    Q_D(Chart);
    if (!range.isValid())
        return;
    if (sheet && sheet->sheetType() != AbstractSheet::ST_WorkSheet)
        return;
    if (!sheet && d->sheet->sheetType() != AbstractSheet::ST_WorkSheet)
        return;

    QString sheetName = sheet ? sheet->sheetName() : d->sheet->sheetName();
    //In case sheetName contains space or '
    sheetName = escapeSheetName(sheetName);

    if (range.columnCount() == 1 || range.rowCount() == 1) {
        QSharedPointer<XlsxSeries> series = QSharedPointer<XlsxSeries>(new XlsxSeries);
        series->numberDataSource_numRef = sheetName + QLatin1String("!") + range.toString(true, true);
        series->line = line;
        d->seriesList.append(series);
    } else if (range.columnCount() < range.rowCount()) {
        //Column based series
        int firstDataColumn = range.firstColumn();
        QString axDataSource_numRef;
        if (d->chartType == CT_Scatter || d->chartType == CT_ScatterLine || d->chartType == CT_Bubble) {
            firstDataColumn += 1;
            CellRange subRange(range.firstRow(), range.firstColumn(), range.lastRow(), range.firstColumn());
            axDataSource_numRef = sheetName + QLatin1String("!") + subRange.toString(true, true);
        }

        for (int col = firstDataColumn; col <= range.lastColumn(); ++col) {
            CellRange subRange(range.firstRow(), col, range.lastRow(), col);
            QSharedPointer<XlsxSeries> series = QSharedPointer<XlsxSeries>(new XlsxSeries);
            series->axDataSource_numRef = axDataSource_numRef;
            series->numberDataSource_numRef = sheetName + QLatin1String("!") + subRange.toString(true, true);
            series->line = line;
            d->seriesList.append(series);
        }

    } else {
        //Row based series
        int firstDataRow = range.firstRow();
        QString axDataSource_numRef;
        if (d->chartType == CT_Scatter || d->chartType == CT_ScatterLine || d->chartType == CT_Bubble) {
            firstDataRow += 1;
            CellRange subRange(range.firstRow(), range.firstColumn(), range.firstRow(), range.lastColumn());
            axDataSource_numRef = sheetName + QLatin1String("!") + subRange.toString(true, true);
        }

        for (int row = firstDataRow; row <= range.lastRow(); ++row) {
            CellRange subRange(row, range.firstColumn(), row, range.lastColumn());
            QSharedPointer<XlsxSeries> series = QSharedPointer<XlsxSeries>(new XlsxSeries);
            series->axDataSource_numRef = axDataSource_numRef;
            series->numberDataSource_numRef = sheetName + QLatin1String("!") + subRange.toString(true, true);
            series->line = line;
            d->seriesList.append(series);
        }
    }
}

void Chart::addSeries(const CellRange &signature, const CellRange &values,
                      AbstractSheet *sheet, ChartLine line)
{
    Q_D(Chart);
    if (!values.isValid() || !signature.isValid())
        return;
    if (signature.columnCount() != 1 && signature.rowCount() != 1)
        return;
    if (sheet && sheet->sheetType() != AbstractSheet::ST_WorkSheet)
        return;
    if (!sheet && d->sheet->sheetType() != AbstractSheet::ST_WorkSheet)
        return;

    QString sheetName = sheet ? sheet->sheetName() : d->sheet->sheetName();
    //In case sheetName contains space or '
    sheetName = escapeSheetName(sheetName);

    CellRange subRange(signature.firstRow(), signature.firstColumn(), signature.lastRow(), signature.firstColumn());
    QString axDataSource_numRef = sheetName + QLatin1String("!") + subRange.toString(true, true);

    if (signature.columnCount() == 1) {
        if (signature.rowCount() == 1) {
            if (values.rowCount() == 1 || values.rowCount() == 1) {
                QSharedPointer<XlsxSeries> series = QSharedPointer<XlsxSeries>(new XlsxSeries);
                series->axDataSource_numRef = axDataSource_numRef;
                series->numberDataSource_numRef = sheetName + QLatin1String("!") + values.toString(true, true);
                series->line = line;
                d->seriesList.append(series);
            } else {
                return;
            }
        } else {
            if (values.rowCount() != signature.rowCount()) {
                return;
            }
            for (int col = values.firstColumn(); col <= values.lastColumn(); ++col) {
                CellRange subRange(values.firstRow(), col, values.lastRow(), col);
                QSharedPointer<XlsxSeries> series = QSharedPointer<XlsxSeries>(new XlsxSeries);
                series->axDataSource_numRef = axDataSource_numRef;
                series->numberDataSource_numRef = sheetName + QLatin1String("!") + subRange.toString(true, true);
                series->line = line;
                d->seriesList.append(series);
            }
        }
    } else {
        if (values.columnCount() != signature.columnCount()) {
            return;
        }
        for (int row = values.firstRow(); row <= values.lastRow(); ++row) {
            CellRange subRange(row, values.firstColumn(), row, values.lastColumn());
            QSharedPointer<XlsxSeries> series = QSharedPointer<XlsxSeries>(new XlsxSeries);
            series->axDataSource_numRef = axDataSource_numRef;
            series->numberDataSource_numRef = sheetName + QLatin1String("!") + subRange.toString(true, true);
            series->line = line;
            d->seriesList.append(series);
        }
    }
}

/*!
 * Set the type of the chart to \a type
 */
void Chart::setChartType(ChartType type)
{
    Q_D(Chart);
    d->chartType = type;
    d->axisList.clear();
    switch (type) {
        case Chart::CT_Bar :
        case Chart::CT_Bar3D :
            InsertAxisXY(d->axisList);
            break;
        case Chart::CT_Line :
            InsertAxisXY(d->axisList);
            break;
        case Chart::CT_Line3D :
            InsertAxisXY(d->axisList);
            InsertAxisZ(d->axisList);
            break;
        case Chart::CT_Scatter :
        case Chart::CT_ScatterLine :
            InsertAxisXY(d->axisList);
            break;
        case Chart::CT_Area :
        case Chart::CT_Area3D :
            InsertAxisXY(d->axisList);
            break;
        default :
            break;
    }
}

/*!
 * \internal
 *
 */
void Chart::setChartStyle(int id)
{
    Q_UNUSED(id)
    //!Todo
}

void Chart::setChartTytle(const QString &title)
{
    Q_D(Chart);
    d->title = title;
}

void Chart::setAxisMaxScope(AxisType type, double value)
{
    Q_D(Chart);
    int index = -1;
    switch (type) {
        case AT_X :
            Q_ASSERT(!d->axisList.isEmpty());
            index = 0;
            break;
        case AT_Y :
            Q_ASSERT(d->axisList.size() > 1);
            index = 1;
            break;
        case AT_Z :
            Q_ASSERT(d->axisList.size() == 3);
            index = 2;
            break;
    }
    if (index != -1)
        d->axisList[index]->maxValue.setValue(value);
}

void Chart::setAxisMinScope(AxisType type, double value)
{
    Q_D(Chart);
    int index = -1;
    switch (type) {
        case AT_X :
            Q_ASSERT(!d->axisList.isEmpty());
            index = 0;
            break;
        case AT_Y :
            Q_ASSERT(d->axisList.size() > 1);
            index = 1;
            break;
        case AT_Z :
            Q_ASSERT(d->axisList.size() == 3);
            index = 2;
            break;
    }
    if (index != -1)
        d->axisList[index]->minValue.setValue(value);
}

void Chart::setAxisMaxAuto(AxisType type)
{
    Q_D(Chart);
    int index = -1;
    switch (type) {
        case AT_X :
            Q_ASSERT(!d->axisList.isEmpty());
            index = 0;
            break;
        case AT_Y :
            Q_ASSERT(d->axisList.size() > 1);
            index = 1;
            break;
        case AT_Z :
            Q_ASSERT(d->axisList.size() == 3);
            index = 2;
            break;
    }
    if (index != -1)
        d->axisList[index]->maxValue.setAuto();
}

void Chart::setAxisMinAuto(AxisType type)
{
    Q_D(Chart);
    int index = -1;
    switch (type) {
        case AT_X :
            Q_ASSERT(!d->axisList.isEmpty());
            index = 0;
            break;
        case AT_Y :
            Q_ASSERT(d->axisList.size() > 1);
            index = 1;
            break;
        case AT_Z :
            Q_ASSERT(d->axisList.size() == 3);
            index = 2;
            break;
    }
    if (index != -1)
        d->axisList[index]->minValue.setAuto();
}

void Chart::enableAxisMajorGridLines(AxisType type, bool activate)
{
    Q_D(Chart);
    int index = -1;
    switch (type) {
        case AT_X :
            Q_ASSERT(!d->axisList.isEmpty());
            index = 0;
            break;
        case AT_Y :
            Q_ASSERT(d->axisList.size() > 1);
            index = 1;
            break;
        case AT_Z :
            Q_ASSERT(d->axisList.size() == 3);
            index = 2;
            break;
    }
    if (index != -1)
        d->axisList[index]->majorLines = activate;
}

void Chart::enableAxisMinorGridLines(AxisType type, bool activate)
{
    Q_D(Chart);
    int index = -1;
    switch (type) {
        case AT_X :
            Q_ASSERT(!d->axisList.isEmpty());
            index = 0;
            break;
        case AT_Y :
            Q_ASSERT(d->axisList.size() > 1);
            index = 1;
            break;
        case AT_Z :
            Q_ASSERT(d->axisList.size() == 3);
            index = 2;
            break;
    }
    if (index != -1)
        d->axisList[index]->minorLines = activate;
}

void Chart::setShowLegend(bool show, Pos pos)
{
    Q_D(Chart);
    d->legend.show = show;
    d->legend.pos  = pos;
}

/*!
 * \internal
 */

void solidFill(QXmlStreamWriter &writer, QString color)
{
    writer.writeStartElement(QStringLiteral("a:solidFill"));
    writer.writeStartElement(QStringLiteral("a:srgbClr"));
    writer.writeAttribute(QStringLiteral("val"), color);
    writer.writeEndElement();//a:srgbClr
    writer.writeEndElement();//a:solidFill
}

void noFill(QXmlStreamWriter &writer)
{
    writer.writeEmptyElement(QStringLiteral("a:noFill"));
}

void Chart::saveToXmlFile(QIODevice *device) const
{
    Q_D(const Chart);

    QXmlStreamWriter writer(device);

    writer.writeStartDocument(QStringLiteral("1.0"), true);
    writer.writeStartElement(QStringLiteral("c:chartSpace"));
    writer.writeAttribute(QStringLiteral("xmlns:c"), QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/chart"));
    writer.writeAttribute(QStringLiteral("xmlns:a"), QStringLiteral("http://schemas.openxmlformats.org/drawingml/2006/main"));
    writer.writeAttribute(QStringLiteral("xmlns:r"), QStringLiteral("http://schemas.openxmlformats.org/officeDocument/2006/relationships"));

    writer.writeStartElement(QStringLiteral("mc:AlternateContent"));
    writer.writeAttribute(QStringLiteral("xmlns:mc"), QStringLiteral("http://schemas.openxmlformats.org/markup-compatibility/2006"));
    writer.writeStartElement(QStringLiteral("mc:Choice"));
    writer.writeAttribute(QStringLiteral("Requires"), QStringLiteral("c14"));
    writer.writeAttribute(QStringLiteral("xmlns:c14"), QStringLiteral("http://schemas.microsoft.com/office/drawing/2007/8/2/chart"));
    writer.writeStartElement(QStringLiteral("c14:style"));
    writer.writeAttribute(QStringLiteral("val"), QStringLiteral("102"));
    writer.writeEndElement();//c14:style
    writer.writeEndElement();//mc:Choice
    writer.writeStartElement(QStringLiteral("mc:Fallback"));
    writer.writeStartElement(QStringLiteral("c:style"));
    writer.writeAttribute(QStringLiteral("val"), QStringLiteral("2"));
    writer.writeEndElement();//c:style
    writer.writeEndElement();//mc:Fallback
    writer.writeEndElement();//mc:AlternateContent

    d->saveXmlChart(writer);

    writer.writeEndElement();//c:chartSpace
    writer.writeEndDocument();
}

/*!
 * \internal
 */
bool Chart::loadFromXmlFile(QIODevice *device)
{
    Q_D(Chart);

    QXmlStreamReader reader(device);
    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("chart")) {
                if (!d->loadXmlChart(reader)) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool ChartPrivate::loadXmlChart(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("chart"));

    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("plotArea")) {
                if (!loadXmlPlotArea(reader)) {
                    return false;
                }
            } else if (reader.name() == QLatin1String("legend")) {
                if (!loadXmlLegend(reader)) {
                    return false;
                }
            }
        } else if (reader.tokenType() == QXmlStreamReader::EndElement &&
                   reader.name() == QLatin1String("chart")) {
            break;
        }
    }
    return true;
}

bool ChartPrivate::loadXmlPlotArea(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("plotArea"));

    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("layout")) {
                loadXmlLayout(reader);
            } else if (reader.name().endsWith(QLatin1String("Chart"))) {
                //For pieChart, barChart, ...
                loadXmlXxxChart(reader);
            } else if (reader.name().endsWith(QLatin1String("Ax"))) {
                //For valAx, catAx, serAx, dateAx
                loadXmlAxis(reader);
            }

        } else if (reader.tokenType() == QXmlStreamReader::EndElement &&
                   reader.name() == QLatin1String("plotArea")) {
            break;
        }
    }
    return true;
}

bool ChartPrivate::loadXmlXxxChart(QXmlStreamReader &reader)
{
    QStringRef name = reader.name();
    if (name == QLatin1String("pieChart"))          chartType = Chart::CT_Pie;
    else if (name == QLatin1String("pie3DChart"))   chartType = Chart::CT_Pie3D;
    else if (name == QLatin1String("barChart"))     chartType = Chart::CT_Bar;
    else if (name == QLatin1String("bar3DChart"))   chartType = Chart::CT_Bar3D;
    else if (name == QLatin1String("lineChart"))    chartType = Chart::CT_Line;
    else if (name == QLatin1String("line3DChart"))  chartType = Chart::CT_Line3D;
    else if (name == QLatin1String("scatterChart")) chartType = Chart::CT_Scatter;
    else if (name == QLatin1String("areaChart"))    chartType = Chart::CT_Area;
    else if (name == QLatin1String("area3DChart"))  chartType = Chart::CT_Area3D;
    else if (name == QLatin1String("doughnutChart"))chartType = Chart::CT_Doughnut;
    else qDebug() << "Cann't load chart: " <<name;

    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("ser")) {
                loadXmlSer(reader);
            } else if (reader.name() == QLatin1String("axId")) {
                loadXmlAxId(reader);
            }
        } else if (reader.tokenType() == QXmlStreamReader::EndElement
                   && reader.name() == name) {
            break;
        }
    }
    return true;
}

bool ChartPrivate::loadXmlSer(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("ser"));

    QSharedPointer<XlsxSeries> series = QSharedPointer<XlsxSeries>(new XlsxSeries);
    seriesList.append(series);

    while (!reader.atEnd() && !(reader.tokenType() == QXmlStreamReader::EndElement
                                && reader.name() == QLatin1String("ser"))) {
        if (reader.readNextStartElement()) {
            QStringRef name = reader.name();
            if (name == QLatin1String("cat") || name == QLatin1String("xVal")) {
                while (!reader.atEnd() && !(reader.tokenType() == QXmlStreamReader::EndElement
                                            && reader.name() == name)) {
                    if (reader.readNextStartElement()) {
                        if (reader.name() == QLatin1String("numRef")) {
                            series->axDataSource_numRef = loadXmlNumRef(reader);
                        }
                    }
                }
            } else if (name == QLatin1String("val") || name == QLatin1String("yVal")) {
                while (!reader.atEnd() && !(reader.tokenType() == QXmlStreamReader::EndElement
                                            && reader.name() == name)) {
                    if (reader.readNextStartElement()) {
                        if (reader.name() == QLatin1String("numRef")) {
                            series->numberDataSource_numRef = loadXmlNumRef(reader);
                        }
                    }
                }
            } else if (name == QLatin1String("extLst")) {
                while (!reader.atEnd() && !(reader.tokenType() == QXmlStreamReader::EndElement
                                            && reader.name() == name)) {
                    reader.readNextStartElement();
                }
            }
        }
    }

    return true;
}

QString ChartPrivate::loadXmlNumRef(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("numRef"));

    while (!reader.atEnd() && !(reader.tokenType() == QXmlStreamReader::EndElement
                                && reader.name() == QLatin1String("numRef"))) {
        if (reader.readNextStartElement()) {
            if (reader.name() == QLatin1String("f")) {
                return reader.readElementText();
            }
        }
    }

    return QString();
}

bool ChartPrivate::loadXmlAxis(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name().endsWith(QLatin1String("Ax")));
    QString name = reader.name().toString();

    XlsxAxis *axis = new XlsxAxis;
    if (name == QLatin1String("valAx"))
        axis->type = XlsxAxis::T_Val;
    else if (name == QLatin1String("catAx"))
        axis->type = XlsxAxis::T_Cat;
    else if (name == QLatin1String("serAx"))
        axis->type = XlsxAxis::T_Ser;
    else
        axis->type = XlsxAxis::T_Date;

    axisList.append(QSharedPointer<XlsxAxis>(axis));

    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("axPos")) {
                QXmlStreamAttributes attrs = reader.attributes();
                QStringRef pos = attrs.value(QLatin1String("val"));
                if (pos == QLatin1String("l"))
                    axis->axisPos = XlsxAxis::Left;
                else if (pos == QLatin1String("r"))
                    axis->axisPos = XlsxAxis::Right;
                else if (pos == QLatin1String("b"))
                    axis->axisPos = XlsxAxis::Bottom;
                else
                    axis->axisPos = XlsxAxis::Top;
            } else if (reader.name() == QLatin1String("axId")) {
                axis->axisId = reader.attributes().value(QLatin1String("val")).toString().toInt();
            } else if (reader.name() == QLatin1String("crossAx")) {
                axis->crossAx = reader.attributes().value(QLatin1String("val")).toString().toInt();
            }
        } else if (reader.tokenType() == QXmlStreamReader::EndElement
                   && reader.name() == name) {
            break;
        }
    }

    return true;
}

bool ChartPrivate::loadXmlAxId(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("axId"));
    QString name = reader.name().toString();
    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            // !Todo
        } else if (reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == name) {
            break;
        }
    }
    return true;
}

bool ChartPrivate::loadXmlLayout(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("layout"));
    QString name = reader.name().toString();
    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            // !Todo
        } else if (reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == name) {
            break;
        }
    }
    return true;
}

bool ChartPrivate::loadXmlLegend(QXmlStreamReader &reader)
{
    Q_ASSERT(reader.name() == QLatin1String("legend"));
    QString name = reader.name().toString();
    while (!reader.atEnd()) {
        reader.readNextStartElement();
        if (reader.tokenType() == QXmlStreamReader::StartElement) {
            if (reader.name() == QLatin1String("legendPos")) {
                QXmlStreamAttributes attrs = reader.attributes();
                QStringRef pos = attrs.value(QLatin1String("val"));
                if (pos == QLatin1String("l"))
                    legend.pos = Chart::Left;
                else if (pos == QLatin1String("r"))
                    legend.pos = Chart::Right;
                else if (pos == QLatin1String("b"))
                    legend.pos = Chart::Bottom;
                else
                    legend.pos = Chart::Top;
            }
        } else if (reader.tokenType() == QXmlStreamReader::EndElement && reader.name() == name) {
            break;
        }
    }
    return true;
}

void ChartPrivate::saveXmlChart(QXmlStreamWriter &writer) const
{
    writer.writeStartElement(QStringLiteral("c:chart"));
    if (!title.isEmpty()) {
        // write title
        writer.writeStartElement(QStringLiteral("c:title"));
        writer.writeStartElement(QStringLiteral("c:tx"));
        writer.writeStartElement(QStringLiteral("c:rich"));
        writer.writeEmptyElement(QStringLiteral("a:bodyPr"));
        writer.writeEmptyElement(QStringLiteral("a:lstStyle"));
        writer.writeStartElement(QStringLiteral("a:p"));
        writer.writeStartElement(QStringLiteral("a:pPr"));
        writer.writeEmptyElement(QStringLiteral("a:defRPr"));
        writer.writeEndElement(); //a:pPr
        writer.writeStartElement(QStringLiteral("a:r"));
        writer.writeStartElement(QStringLiteral("a:rPr"));
        writer.writeAttribute(QStringLiteral("lang"), QStringLiteral("en-US"));
        writer.writeEndElement(); //a:rPr
        writer.writeStartElement(QStringLiteral("a:t"));
        writer.writeCharacters(title);
        writer.writeEndElement(); //a:t
        writer.writeEndElement(); //a:r

        writer.writeStartElement(QStringLiteral("a:endParaRPr"));
        writer.writeAttribute(QStringLiteral("lang"), QStringLiteral("en-US"));
        writer.writeEndElement(); //a:endParaRPr

        writer.writeEndElement(); //a:p
        writer.writeEndElement(); //c:rich
        writer.writeEndElement(); //c:tx

        writer.writeStartElement(QStringLiteral("c:overlay"));
        writer.writeAttribute(QStringLiteral("val"), QStringLiteral("0"));
        writer.writeEndElement(); //c:overlay

        writer.writeEndElement(); //c:title

        writer.writeStartElement(QStringLiteral("c:autoTitleDeleted"));
        writer.writeAttribute(QStringLiteral("val"), QStringLiteral("0"));
        writer.writeEndElement(); //c:autoTitleDeleted
    }
    writer.writeStartElement(QStringLiteral("c:plotArea"));
    switch (chartType) {
        case Chart::CT_Pie :
        case Chart::CT_Pie3D :
            saveXmlPieChart(writer);
            break;
        case Chart::CT_Bar :
        case Chart::CT_Bar3D :
            saveXmlBarChart(writer);
            break;
        case Chart::CT_Line :
        case Chart::CT_Line3D :
            saveXmlLineChart(writer);
            break;
        case Chart::CT_Scatter :
        case Chart::CT_ScatterLine :
            saveXmlScatterChart(writer);
            break;
        case Chart::CT_Area :
        case Chart::CT_Area3D :
            saveXmlAreaChart(writer);
            break;
        case Chart::CT_Doughnut :
            saveXmlDoughnutChart(writer);
            break;
        default :
            break;
    }
    saveXmlAxes(writer);
    writer.writeEndElement(); //plotArea

    saveXmlLegend(writer);

    writer.writeEndElement(); //chart
}

void ChartPrivate::saveXmlPieChart(QXmlStreamWriter &writer) const
{
    QString name = chartType == Chart::CT_Pie ? QStringLiteral("c:pieChart") : QStringLiteral("c:pie3DChart");

    writer.writeStartElement(name);

    //Do the same behavior as Excel, Pie prefer varyColors
    writer.writeEmptyElement(QStringLiteral("c:varyColors"));
    writer.writeAttribute(QStringLiteral("val"), QStringLiteral("1"));

    for (int i = 0; i < seriesList.size(); ++i)
        saveXmlSer(writer, seriesList[i].data(), i);

    writer.writeEndElement(); //pieChart, pie3DChart
}

void ChartPrivate::saveXmlBarChart(QXmlStreamWriter &writer) const
{
    QString name = chartType == Chart::CT_Bar ? QStringLiteral("c:barChart") : QStringLiteral("c:bar3DChart");

    writer.writeStartElement(name);

    writer.writeEmptyElement(QStringLiteral("c:barDir"));
    writer.writeAttribute(QStringLiteral("val"), QStringLiteral("col"));

    for (int i = 0; i < seriesList.size(); ++i)
        saveXmlSer(writer, seriesList[i].data(), i);

    if (axisList.isEmpty()) {
        //The order the axes??
        InsertAxisXY(const_cast<ChartPrivate*>(this)->axisList);
    }

    //Note: Bar3D have 2~3 axes
    Q_ASSERT(axisList.size() == 2 || (axisList.size() == 3 && chartType == Chart::CT_Bar3D));

    for (int i = 0; i < axisList.size(); ++i) {
        writer.writeEmptyElement(QStringLiteral("c:axId"));
        writer.writeAttribute(QStringLiteral("val"), QString::number(axisList[i]->axisId));
    }

    writer.writeEndElement(); //barChart, bar3DChart
}

void ChartPrivate::saveXmlLineChart(QXmlStreamWriter &writer) const
{
    QString name = chartType == Chart::CT_Line ? QStringLiteral("c:lineChart") : QStringLiteral("c:line3DChart");

    writer.writeStartElement(name);

    writer.writeEmptyElement(QStringLiteral("grouping"));

    for (int i = 0; i < seriesList.size(); ++i)
        saveXmlSer(writer, seriesList[i].data(), i);

    if (axisList.isEmpty()) {
        InsertAxisXY(const_cast<ChartPrivate*>(this)->axisList);
        if (chartType == Chart::CT_Line3D) {
            InsertAxisZ(const_cast<ChartPrivate*>(this)->axisList);
        }
    }

    Q_ASSERT((axisList.size() == 2 || chartType == Chart::CT_Line)
             || (axisList.size() == 3 && chartType == Chart::CT_Line3D));

    for (int i = 0; i < axisList.size(); ++i) {
        writer.writeEmptyElement(QStringLiteral("c:axId"));
        writer.writeAttribute(QStringLiteral("val"), QString::number(axisList[i]->axisId));
    }

    writer.writeEndElement(); //lineChart, line3DChart
}

void ChartPrivate::saveXmlScatterChart(QXmlStreamWriter &writer) const
{
    const QString name = QStringLiteral("c:scatterChart");

    writer.writeStartElement(name);

    writer.writeEmptyElement(QStringLiteral("c:scatterStyle"));

    for (int i = 0; i < seriesList.size(); ++i)
        saveXmlSer(writer, seriesList[i].data(), i);

    if (axisList.isEmpty()) {
        InsertAxisXY(const_cast<ChartPrivate*>(this)->axisList);
    }

    Q_ASSERT(axisList.size() == 2);

    for (int i = 0; i < axisList.size(); ++i) {
        writer.writeEmptyElement(QStringLiteral("c:axId"));
        writer.writeAttribute(QStringLiteral("val"), QString::number(axisList[i]->axisId));
    }

    writer.writeEndElement(); //c:scatterChart
}

void ChartPrivate::saveXmlAreaChart(QXmlStreamWriter &writer) const
{
    QString name = chartType == Chart::CT_Area ? QStringLiteral("c:areaChart") : QStringLiteral("c:area3DChart");

    writer.writeStartElement(name);

    writer.writeEmptyElement(QStringLiteral("grouping"));

    for (int i = 0; i < seriesList.size(); ++i)
        saveXmlSer(writer, seriesList[i].data(), i);

    if (axisList.isEmpty()) {
        InsertAxisXY(const_cast<ChartPrivate*>(this)->axisList);
    }

    //Note: Area3D have 2~3 axes
    Q_ASSERT(axisList.size() == 2 || (axisList.size() == 3 && chartType == Chart::CT_Area3D));

    for (int i = 0; i < axisList.size(); ++i) {
        writer.writeEmptyElement(QStringLiteral("c:axId"));
        writer.writeAttribute(QStringLiteral("val"), QString::number(axisList[i]->axisId));
    }

    writer.writeEndElement(); //lineChart, line3DChart
}

void ChartPrivate::saveXmlDoughnutChart(QXmlStreamWriter &writer) const
{
    QString name = QStringLiteral("c:doughnutChart");

    writer.writeStartElement(name);

    writer.writeEmptyElement(QStringLiteral("c:varyColors"));
    writer.writeAttribute(QStringLiteral("val"), QStringLiteral("1"));

    for (int i = 0; i < seriesList.size(); ++i)
        saveXmlSer(writer, seriesList[i].data(), i);

    writer.writeStartElement(QStringLiteral("c:holeSize"));
    writer.writeAttribute(QStringLiteral("val"), QString::number(50));

    writer.writeEndElement();
}

void ChartPrivate::saveXmlSer(QXmlStreamWriter &writer, XlsxSeries *ser, int id) const
{
    writer.writeStartElement(QStringLiteral("c:ser"));
    writer.writeEmptyElement(QStringLiteral("c:idx"));
    writer.writeAttribute(QStringLiteral("val"), QString::number(id));
    writer.writeEmptyElement(QStringLiteral("c:order"));
    writer.writeAttribute(QStringLiteral("val"), QString::number(id));

    if (!ser->line.getName().isEmpty()) {
        writer.writeStartElement(QStringLiteral("c:tx"));
        writer.writeStartElement(QStringLiteral("c:strRef"));
        writer.writeStartElement(QStringLiteral("c:f"));
        writer.writeCharacters(ser->line.getName());
        writer.writeEndElement();//c:f
        writer.writeEndElement();//c:strRef
        writer.writeEndElement();//c:tx
    }

    writer.writeStartElement(QStringLiteral("c:spPr"));
    writer.writeStartElement(QStringLiteral("a:ln"));
    if (!ser->line.isLineWide()) {
        writer.writeAttribute(QStringLiteral("w"), QStringLiteral("6480")); // 6480 - small, 28440 - wide
    }
    // write line color
    if (chartType == Chart::CT_Scatter) {
        noFill(writer);
        if (ser->line.marker.markerType() == Marker::MT_None) {
            ser->line.marker.setMarkerType(Marker::MT_Auto);
        }
    }
    if (chartType == Chart::CT_ScatterLine && ser->line.isCustomColor()) {
        solidFill(writer, ser->line.getColor());
    }
    writer.writeEndElement();//a:ln
    writer.writeEndElement();//c:spPr

    if (chartType == Chart::CT_Scatter || chartType == Chart::CT_ScatterLine
            || chartType == Chart::CT_Line || chartType == Chart::CT_Line3D) {
        writer.writeStartElement(QStringLiteral("c:marker"));
        writer.writeStartElement(QStringLiteral("c:symbol"));
        writer.writeAttribute(QStringLiteral("val"), ser->line.marker.getType());
        writer.writeEndElement();//c:symbol
        writer.writeStartElement(QStringLiteral("c:size"));
        writer.writeAttribute(QStringLiteral("val"), QString::number(ser->line.marker.markerSize()));
        writer.writeEndElement();//c:size
        // write marker color
        if (ser->line.marker.isCustomColor()) {
            writer.writeStartElement(QStringLiteral("c:spPr"));
            solidFill(writer, ser->line.marker.getColor());
            writer.writeEndElement();//c:spPr
        }
        writer.writeEndElement();//c:marker
    }

    if (!ser->axDataSource_numRef.isEmpty()) {
        if (chartType == Chart::CT_Scatter || chartType == Chart::CT_ScatterLine
                || chartType == Chart::CT_Bubble)
            writer.writeStartElement(QStringLiteral("c:xVal"));
        else
            writer.writeStartElement(QStringLiteral("c:cat"));
        writer.writeStartElement(QStringLiteral("c:numRef"));
        writer.writeTextElement(QStringLiteral("c:f"), ser->axDataSource_numRef);
        writer.writeEndElement();//c:numRef
        writer.writeEndElement();//c:cat or c:xVal
    }

    if (!ser->numberDataSource_numRef.isEmpty()) {
        if (chartType == Chart::CT_Scatter || chartType == Chart::CT_ScatterLine
                || chartType == Chart::CT_Bubble)
            writer.writeStartElement(QStringLiteral("c:yVal"));
        else
            writer.writeStartElement(QStringLiteral("c:val"));
        writer.writeStartElement(QStringLiteral("c:numRef"));
        writer.writeTextElement(QStringLiteral("c:f"), ser->numberDataSource_numRef);
        writer.writeEndElement();//c:numRef
        writer.writeEndElement();//c:val or c:yVal
    }

    writer.writeEndElement();//c:ser
}

void ChartPrivate::saveXmlAxes(QXmlStreamWriter &writer) const
{
    for (int i = 0; i < axisList.size(); ++i) {
        XlsxAxis *axis = axisList[i].data();
        QString name;
        switch (axis->type) {
            case XlsxAxis::T_Cat :  name = QStringLiteral("c:catAx"); break;
            case XlsxAxis::T_Val :  name = QStringLiteral("c:valAx"); break;
            case XlsxAxis::T_Ser :  name = QStringLiteral("c:serAx"); break;
            case XlsxAxis::T_Date : name = QStringLiteral("c:dateAx"); break;
            default : break;
        }

        QString pos;
        switch (axis->axisPos) {
            case XlsxAxis::Top :    pos = QStringLiteral("t"); break;
            case XlsxAxis::Bottom : pos = QStringLiteral("b"); break;
            case XlsxAxis::Left :   pos = QStringLiteral("l"); break;
            case XlsxAxis::Right :  pos = QStringLiteral("r"); break;
            default : break;
        }

        writer.writeStartElement(name);
        writer.writeEmptyElement(QStringLiteral("c:axId"));
        writer.writeAttribute(QStringLiteral("val"), QString::number(axis->axisId));

        writer.writeStartElement(QStringLiteral("c:scaling"));
        writer.writeEmptyElement(QStringLiteral("c:orientation"));
        writer.writeAttribute(QStringLiteral("val"), QStringLiteral("minMax"));
        if (!axis->maxValue.isAuto()) {
            writer.writeStartElement(QStringLiteral("c:max"));
            writer.writeAttribute(QStringLiteral("val"), QString::number(axis->maxValue.getValue()));
            writer.writeEndElement();//c:max
        }
        if (!axis->minValue.isAuto()) {
            writer.writeStartElement(QStringLiteral("c:min"));
            writer.writeAttribute(QStringLiteral("val"), QString::number(axis->minValue.getValue()));
            writer.writeEndElement();//c:min
        }
        writer.writeEndElement();//c:scaling

        writer.writeEmptyElement(QStringLiteral("c:axPos"));
        writer.writeAttribute(QStringLiteral("val"), pos);

        if (axis->majorLines) {
            writer.writeStartElement(QStringLiteral("c:majorGridlines"));
            writer.writeStartElement(QStringLiteral("c:spPr"));
            writer.writeStartElement(QStringLiteral("a:ln"));
            writer.writeStartElement(QStringLiteral("a:solidFill"));
            writer.writeStartElement(QStringLiteral("a:srgbClr"));
            writer.writeAttribute(QStringLiteral("val"), QStringLiteral("b3b3b3"));
            writer.writeEndElement();//a:srgbClr
            writer.writeEndElement();//a:solidFill
            writer.writeEndElement();//a:ln
            writer.writeEndElement();//c:spPr
            writer.writeEndElement();//c:majorGridlines
        }
        if (axis->minorLines) {
            writer.writeStartElement(QStringLiteral("c:minorGridlines"));
            writer.writeStartElement(QStringLiteral("c:spPr"));
            writer.writeStartElement(QStringLiteral("a:ln"));
            writer.writeStartElement(QStringLiteral("a:solidFill"));
            writer.writeStartElement(QStringLiteral("a:srgbClr"));
            writer.writeAttribute(QStringLiteral("val"), QStringLiteral("dddddd"));
            writer.writeEndElement();//a:srgbClr
            writer.writeEndElement();//a:solidFill
            writer.writeEndElement();//a:ln
            writer.writeEndElement();//c:spPr
            writer.writeEndElement();//c:minorGridlines
        }
        writer.writeEmptyElement(QStringLiteral("c:crossAx"));
        writer.writeAttribute(QStringLiteral("val"), QString::number(axis->crossAx));

        writer.writeEndElement();//name
    }
}

void ChartPrivate::saveXmlLegend(QXmlStreamWriter &writer) const
{
    if (legend.show) {
        writer.writeStartElement(QStringLiteral("c:legend"));
        writer.writeStartElement(QStringLiteral("c:legendPos"));
        QString pos;
        switch (legend.pos) {
            case Chart::Top :    pos = QStringLiteral("t"); break;
            case Chart::Bottom : pos = QStringLiteral("b"); break;
            case Chart::Left :   pos = QStringLiteral("l"); break;
            case Chart::Right :  pos = QStringLiteral("r"); break;
            default : break;
        }
        writer.writeAttribute(QStringLiteral("val"), pos);
        writer.writeEndElement();//c:legendPos
        writer.writeStartElement(QStringLiteral("c:overlay"));
        writer.writeAttribute(QStringLiteral("val"), QStringLiteral("0"));
        writer.writeEndElement();//c:overlay
        writer.writeEndElement();//c:legend
    }
}

QT_END_NAMESPACE_XLSX
