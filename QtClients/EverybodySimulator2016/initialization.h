#ifndef INITIALIZATION_H
#define INITIALIZATION_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QJsonObject>
#include <QHash>
#include <QDebug>


class initialization
{
private:
    // this caches all json files that have been parsed (one for requests.json one for responses.json)
    QHash<QString, QJsonObject> cache;

public:
    initialization();
    QJsonObject getConfig(QString in);
    bool changeResponse(QJsonObject in);
};

#endif // INITIALIZATION_H
