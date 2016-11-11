#include "initialization.h"

QJsonObject initialization::getConfig()
{
    QByteArray fileData;
    QFile file(JSON_CONFIG_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QJsonObject();

    while (!file.atEnd()) {
        fileData += file.readLine();
    }

    return QJsonDocument::fromJson(fileData).object();

}
