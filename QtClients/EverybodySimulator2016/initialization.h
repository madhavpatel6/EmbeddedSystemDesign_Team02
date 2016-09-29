#ifndef INITIALIZATION_H
#define INITIALIZATION_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QJsonObject>


class initialization
{
public:
    static QJsonObject getConfig(QString in);
};

#endif // INITIALIZATION_H
