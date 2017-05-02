#include "ViewModel.h"
#include "qmath.h"

void ViewModel::genereteAvailableUTMZones()
{
    QString line;
    for (int i = 1; i < 61; i++){
        line = "Zona " + QString::number(i) + " N (" + QString::number((i*6-183)-3) + "º a " + QString::number((i*6-183)+3) + "º)";
        availableUTMZone.append(line);
        line = "Zona " + QString::number(i) + " S (" + QString::number((i*6-183)-3) + "º a " + QString::number((i*6-183)+3) + "º)";
        availableUTMZone.append(line);
    }
}

void ViewModel::genereteAvailableGKZones()
{
    QString line;
    for (int i = 1; i < 8; i++){
        line = "Faja " + QString::number(i) + " (" + QString::number(-75.0 + i * 3.0 - 3.0/2.0) + "º a " + QString::number(-75.0 + i * 3.0 + 3.0/2.0) + "º)";
        availableGKZone.append(line);
    }
}

ViewModel::ViewModel(QWidget *parent)
{
    availableSystem.insert(0, "Geográficas");
    availableSystem.insert(1, "UTM");
    availableSystem.insert(2, "Gauss Krüger");

    availableDatum.insert(0, "WGS84");
    availableDatum.insert(1, "Campo Inchauspe");

    convertPoint = new Coordinate();
    genereteAvailableUTMZones();
    genereteAvailableGKZones();

    //Conections
    connect(this->convertPoint, &Coordinate::xCalculated, this, &ViewModel::finalXChanged);
    connect(this->convertPoint, &Coordinate::yCalculated, this, &ViewModel::finalYChanged);
    connect(this->convertPoint, &Coordinate::zCalculated, this, &ViewModel::finalZChanged);
    connect(this, &ViewModel::initialDatumChanged, this->convertPoint, &Coordinate::setInitialDatum);
    connect(this, &ViewModel::finalDatumChanged, this->convertPoint, &Coordinate::setDestinationDatum);
    connect(this, &ViewModel::initialZoneChanged, this->convertPoint, &Coordinate::setInitialZone);
    connect(this, &ViewModel::finalZoneChanged, this->convertPoint, &Coordinate::setDestinationZone);

    connect(this, &ViewModel::initialSystemChanged, this->convertPoint, &Coordinate::setInitialSystem);
    connect(this, &ViewModel::finalSystemChanged, this->convertPoint, &Coordinate::setDestinationSystem);

}

QStringList ViewModel::availableSystems() const
{
    return availableSystem;
}

QStringList ViewModel::availableDatums() const
{
    return availableDatum;
}


void ViewModel::startConverter()
{

    if (setInputAsGeographic) {
        convertPoint->setInitialLongitude(initialX);
        convertPoint->setInitialLatitude(initialY);
        convertPoint->setInitialElevation(initialZ);
    } else {
        convertPoint->setInitialX(initialX);
        convertPoint->setInitialY(initialY);
        convertPoint->setInitialZ(initialZ);
    }
    convertPoint->convertCoordinate();
}

void ViewModel::initialXChanged(const QString xValue)
{

    QLocale qLoc;
    QChar decimalChar = qLoc.decimalPoint();
    double tempValue;
    bool ok;
    QString tempNumber;
    if (setInputAsGeographic){
        if (geographicFormat == GeographicFormat::GMS){
            tempValue = validateGeographicGMSCoordinate(xValue);
            ok = true;
        } else {
            tempNumber = xValue;
            tempNumber = tempNumber.replace('.', decimalChar);
            tempNumber = tempNumber.replace(',', decimalChar);
            tempValue = qLoc.toDouble(tempNumber, &ok);
        }
    } else {
        tempNumber = xValue;
        tempNumber = tempNumber.replace('.', decimalChar);
        tempNumber = tempNumber.replace(',', decimalChar);
        tempValue = qLoc.toDouble(tempNumber, &ok);
    }
    if (ok){
        if (tempValue != convertPoint->getInitialX()){
            convertPoint->setInitialX(tempValue);
            initialX = tempValue;

        }

    }

}

void ViewModel::initialYChanged(const QString yValue)
{

    QLocale qLoc;
    QChar decimalChar = qLoc.decimalPoint();
    QString tempNumber;
    double tempValue;
    bool ok;
    if (setInputAsGeographic){
        if (geographicFormat == GeographicFormat::GMS){
            tempValue = validateGeographicGMSCoordinate(yValue);
            ok = true;
        } else {
            tempNumber = yValue;
            tempNumber = tempNumber.replace('.', decimalChar);
            tempNumber = tempNumber.replace(',', decimalChar);
            tempValue = qLoc.toDouble(tempNumber, &ok);
        }
    } else {
        tempNumber = yValue;
        tempNumber = tempNumber.replace('.', decimalChar);
        tempNumber = tempNumber.replace(',', decimalChar);
        tempValue = qLoc.toDouble(tempNumber, &ok);
    }

    if (ok){
        if (tempValue != convertPoint->getInitialY()){
            convertPoint->setInitialY(tempValue);
            initialY = tempValue;
        }

    }
}

void ViewModel::initialZChanged(const QString zValue)
{

    QLocale qLoc;
    QChar decimalChar = qLoc.decimalPoint();
    QString tempNumber;
    double tempValue;
    bool ok;

    tempNumber = zValue;
    tempNumber = tempNumber.replace('.', decimalChar);
    tempNumber = tempNumber.replace(',', decimalChar);
    tempValue = qLoc.toDouble(tempNumber, &ok);

    if (ok){
        if (tempValue != convertPoint->getInitialZ()){
            convertPoint->setInitialZ(tempValue);
            initialZ = tempValue;
        }

    }
}

void ViewModel::finalXChanged(const double xValue)
{
    QLocale qLoc;
    QString tempValue;
    if (setOutputAsGeographic) {
        if (geographicFormat == GeographicFormat::GMS){
            tempValue = setOutputAsGSM(xValue, false);
        } else {
            tempValue = qLoc.toString(xValue, 'f', 8);
        }
    } else {
        tempValue = qLoc.toString(xValue, 'f', 3);
    }

    emit updateFinalX(tempValue);
}

void ViewModel::finalYChanged(const double yValue)
{
    QLocale qLoc;
    QString tempValue;
    if (setOutputAsGeographic) {
        if (geographicFormat == GeographicFormat::GMS){
            tempValue = setOutputAsGSM(yValue, true);
        } else {
            tempValue = qLoc.toString(yValue, 'f', 8);
        }
    } else {
        tempValue = qLoc.toString(yValue, 'f', 3);
    }

    emit updateFinalY(tempValue);
}

void ViewModel::finalZChanged(const double zValue)
{
    QLocale qLoc;
    QString tempValue = qLoc.toString(zValue, 'f', 3);
    emit updateFinalZ(tempValue);
}

void ViewModel::selectedInputSystemChanged(const QString name)
{
    initialSystem = name;
    if (name == "Geográficas"){
        setInputAsGeographic = true;
    } else {
        setInputAsGeographic = false;
        if (name == "UTM"){
            emit updateInputZoneList(availableUTMZone);
        } else {
            emit updateInputZoneList(availableGKZone);
        }
    }
    emit initialSystemChanged(initialSystem);

}

void ViewModel::selectedOutputSystemChanged(const QString name)
{
    finalSystem = name;
    if (name == "Geográficas"){
        setOutputAsGeographic = true;
    } else {
        setOutputAsGeographic = false;
        if (name == "UTM"){
            emit updateOutputZoneList(availableUTMZone);
        } else {
            emit updateOutputZoneList(availableGKZone);
        }
    }
    emit finalSystemChanged(finalSystem);
}

void ViewModel::selectedInputDatumChanged(const QString name)
{
    emit initialDatumChanged(name);
}

void ViewModel::selectedOutputDatumChanged(const QString name)
{
    emit finalDatumChanged(name);
}

void ViewModel::selectedInputZoneChanged(const int region)
{
    CoordinateSystem::HemisphereType hem;
    if (initialSystem == "UTM"){
        double reg = floor(double(region) / 2.0 + 1.0);
        int mod = region % 2;
        if (mod == 0){
            hem = CoordinateSystem::HemisphereType::North;
        } else {
            hem = CoordinateSystem::HemisphereType::South;
        }
        emit initialZoneChanged(reg, hem);
    } else if (initialSystem == "Gauss Krüger") {
        double reg = double(region + 1);
        hem = CoordinateSystem::HemisphereType::South;
        emit initialZoneChanged(reg, hem);
    }

}

void ViewModel::selectedOutputZoneChanged(const int region)
{
    CoordinateSystem::HemisphereType hem;
    if (finalSystem == "UTM"){
        double reg = floor(double(region) / 2.0 + 1.0);
        int mod = region % 2;
        if (mod == 0){
            hem = CoordinateSystem::HemisphereType::North;
        } else {
            hem = CoordinateSystem::HemisphereType::South;
        }
        emit finalZoneChanged(reg, hem);
    } else if (finalSystem == "Gauss Krüger") {
        double reg = double(region + 1);
        hem = CoordinateSystem::HemisphereType::South;
        emit finalZoneChanged(reg, hem);
    }

}

void ViewModel::setGeographicFormat(const ViewModel::GeographicFormat format)
{
    geographicFormat = format;
}

double ViewModel::validateGeographicGMSCoordinate(const QString &value)
{

    QLocale qloc;
    QString resultValue;
    QChar signChar;
    QChar decimalChar = qloc.decimalPoint();
    resultValue.clear();
    foreach (QChar c, value) {
        if (c.isDigit()){
            resultValue.append(c);
        } else if (c == '.' || c == ',') {
            resultValue.append(c);
        } else if (c == 'S' || c == 's' || c == 'W' || c == 'w' || c == 'O' || c == 'o') {
            signChar = '-';
        } else if (c == 'N' || c == 'n' || c == 'E' || c == 'e') {
            signChar = ' ';
        } else {
            resultValue.append(' ');
        }
    }

    QStringList tempList = resultValue.split(' ', QString::SkipEmptyParts);
    double result = 0.0;

    for (int i = 0; i < tempList.length(); i++){
        QString tempNumber = tempList.at(i);
        tempNumber = tempNumber.replace('.', decimalChar);
        tempNumber = tempNumber.replace(',', decimalChar);
        result = result + qloc.toDouble(tempNumber) / pow(60.0, i);
    }

    if (signChar == '-'){
        result = result * (-1.0);
    }

    return result;

}

QString ViewModel::setOutputAsGSM(const double value, bool isLatitude)
{
    int degrees = trunc(value);
    double minutesWithDecimal = std::abs(value - degrees) * 60.0;
    int minutes = trunc(minutesWithDecimal);
    double seconds = (minutesWithDecimal - minutes) * 60.0;

    QChar letter;
    if (isLatitude) {
        if (degrees >= 0){
            letter = 'N';
        } else {
            letter = 'S';
        }
    } else {
        if (degrees >= 0){
            letter = 'E';
        } else {
            letter = 'W';
        }
    }

    QLocale qLoc;
    QString result = qLoc.toString(abs(degrees)) + "º " +
            qLoc.toString(minutes) + "\' " +
            qLoc.toString(seconds, 'f', 2) + "\'' " + letter;

    return result;
}
