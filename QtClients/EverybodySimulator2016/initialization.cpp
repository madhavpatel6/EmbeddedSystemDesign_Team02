#include "initialization.h"

initialization::initialization(){

}

QJsonObject initialization::getConfig(QString in)
{
    if(cache[in] != QJsonObject()){
        return cache[in];
    }else{

        QByteArray fileData;
        QFile file(in);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return QJsonObject();

        while (!file.atEnd()) {
            fileData += file.readLine();
        }
        cache[in] = QJsonDocument::fromJson(fileData).object();

        return cache[in];
    }
}
bool initialization::changeResponse(QJsonObject in){
    if(cache["responses.json"] == QJsonObject()){
        getConfig("responses.json");
    }
    for(auto j = in.begin(); j != in.end(); j++){
        cache["responses.json"].insert(j.key(), j.value().toString());
    }
    return true;

}
