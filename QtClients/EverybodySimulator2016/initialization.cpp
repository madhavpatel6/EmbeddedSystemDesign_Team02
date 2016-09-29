#include "initialization.h"

QJsonObject initialization::getConfig(QString in)
{
    QByteArray fileData;
    QFile file(in);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QJsonObject();

    while (!file.atEnd()) {
        fileData += file.readLine();
    }

    return QJsonDocument::fromJson(fileData).object();
}
