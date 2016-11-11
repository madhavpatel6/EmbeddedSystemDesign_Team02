#ifndef INITIALIZATION_H
#define INITIALIZATION_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QJsonObject>

#define JSON_CONFIG_FILE "config.json"

class initialization
{
public:
    static QJsonObject getConfig();
};

#endif // INITIALIZATION_H
