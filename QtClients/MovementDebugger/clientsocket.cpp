// clientsocket.cpp

#include "clientsocket.h"
#include "mainwindow.h"

ClientSocket::ClientSocket(QObject *parent) :
    QObject(parent)
{
    numOfErrors = 0;
    socket = new QTcpSocket();

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    isConnected = false;
    isClear = true;
    sendData = true;
    v_lineColor = WHITE;
    v_threshold = 512;
}


void ClientSocket::connectToHost(QString ip,int port){
    if (isConnected) {
        socket->disconnectFromHost();
    } else {
        socket->disconnectFromHost();
        socket->connectToHost(ip,port);
    }
}

QTcpSocket* ClientSocket::getClient(){
    return socket;
}

void ClientSocket::movementRequested(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"R1_Movement\"" + request_end, SEARCHERMOVER);
}

void ClientSocket::lineLocationRequested(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"LineLocation\"" + request_end, SEARCHERMOVER);
}

void ClientSocket::sendForwardCommand(int distance){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Forward\":" + "\"" + QString::number(distance) + "\"" + request_end, SEARCHERMOVER);
    qDebug() << request_begin + "\"Forward\":" + "\"" + QString::number(distance) + "\"" + request_end;
}

void ClientSocket::sendBackCommand(int distance){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Back\":" + "\"" + QString::number(distance) + "\"" + request_end, SEARCHERMOVER);
    qDebug() << request_begin + "\"Back\":" + "\"" + QString::number(distance) + "\"" + request_end;
}

void ClientSocket::sendLeftCommand(int degrees){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Left\":" + "\"" + QString::number(degrees) + "\"" + request_end, SEARCHERMOVER);
    qDebug() << request_begin + "\"Left\":" + "\"" + QString::number(degrees) + "\"" + request_end;
}

void ClientSocket::sendRightCommand(int degrees){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Right\":" + "\"" + QString::number(degrees) + "\"" + request_end, SEARCHERMOVER);
    qDebug() << request_begin + "\"Right\":" + "\"" + QString::number(degrees) + "\"" + request_end;
}

void ClientSocket::sendStartCommand(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Start\"" + request_end, SEARCHERMOVER);
    qDebug() << request_begin + "\"Start\"" + request_end;
}

void ClientSocket::sendStopCommand(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[";
    QString request_end = "]}";
    SendJSONRequestToSocket(request_begin + "\"Stop\"" + request_end, SEARCHERMOVER);
    qDebug() << request_begin + "\"Stop\"" + request_end;
}

void ClientSocket::sendInitialData(QString mode, QString position, QString numTargets, QString numVertices, QString vertices) {
    QString response_begin = "{\"type\":\"Response\",\"InitialData\":{";
    QString response_end = "}}";
    SendJSONResponseToSocket(response_begin + mode + "," + position + "," + numTargets + "," + numVertices + "," + vertices + response_end, SEARCHERMOVER);
    qDebug() << response_begin + mode + "," + position + "," + numTargets + "," + numVertices + "," + vertices + response_end;
}

void ClientSocket::sendCorrectedPosition(QString x, QString y, QString orientation)
{
    QString response_begin = "{\"type\":\"Response\",\"R1_Location\":{";
    QString response_end = "}}";
    SendJSONResponseToSocket(response_begin + "\"x\":\"" + x + "\",\"y\":\"" + y + "\",\"orientation\":\"" + orientation + "\"" + response_end, SEARCHERMOVER);
    qDebug() << response_begin + "\"x\":\"" + x + "\",\"y\":\"" + y + "\",\"orientation\":\"" + orientation + "\"" + response_end;
}

void ClientSocket::sendLineTuning(QString lineColor, QString threshold)
{
    QString response_begin = "{\"type\":\"Response\",\"LineTuning\":{";
    QString response_end = "}}";
    SendJSONResponseToSocket(response_begin + "\"lineColor\":\"" + lineColor + "\",\"threshold\":\"" + threshold + "\"" + response_end, SEARCHERMOVER);
    qDebug() << response_begin + "\"lineColor\":\"" + lineColor + "\",\"threshold\":\"" + threshold + "\"" + response_end;
}

void ClientSocket::updateLineTuning(int lineColor, int threshold)
{
    v_lineColor = lineColor;
    v_threshold = threshold;
}

void ClientSocket::sendClear(bool send){
    sendData = send;
    isClear = true;
}

void ClientSocket::sendObstacle(bool send){
    sendData = send;
    isClear = false;
}

void ClientSocket::connected()
{
    qDebug() << "Client connected event";
    isConnected = true;
    emit serverIsConnectedSignal(true);
}

void ClientSocket::disconnected()
{
    qDebug() << "Client disconnected";
    isConnected = false;
    emit serverIsConnectedSignal(false);
}

int  ClientSocket::send(QByteArray words){
    return socket->write(words);
}

void ClientSocket::readyRead()
{
    //qDebug() << "ClientSocket::readyRead()";
    QByteArray array = socket->readAll();
    char buffer[MAXMESSAGESIZE];
    char source, messageCount;
    bool isError;
    for(int i = 0; i < array.size(); i++) {
        bool isCompleted = ParseMessage(array[i], buffer, &source, &messageCount, &isError);
        if(isCompleted) {
            QJsonDocument doc(QJsonDocument::fromJson(buffer));
            QJsonObject json = doc.object();
            QString type = json["type"].toString();

            if(type == QStringLiteral("Response")) {
                if(json.contains(QStringLiteral("R1_Movement")) ||
                        json.contains(QStringLiteral("LineLocation"))) {
                    HandleResponse(json);
                }
            }
            else if(type == QStringLiteral("Request")){
                if(json.contains(QStringLiteral("items"))) {
                    HandleRequest(json["items"].toArray());
                }
            }
        }
        else if(isError){
            numOfErrors++;
        }
    }
}

void ClientSocket::SendJSONRequestToSocket(QString request, char destination) {
    if(isConnected == true){
        //qDebug() << "Sending Request: " << request;
        char message[512];
        int len = CreateMessage(message, request.toLatin1().data(), destination, 0);

        QByteArray txMessage;
        txMessage.setRawData(message, len);

        // qDebug() << txMessage << endl;

        send(txMessage);
        //qDebug() << "bytesSent:" << bytesSent;
        //qDebug() << "Flushing Socket";
        socket->flush();
        //qDebug() << "Flushed Socket";
    }
    else{
        qDebug() << "Client is not connected to the server. \n";
    }
}

void ClientSocket::SendJSONResponseToSocket(QString response, char destination) {
    if(isConnected == true){
        //qDebug() << "Sending Request: " << request;
        char message[512];
        int len = CreateMessage(message, response.toLatin1().data(), destination, 0);

        QByteArray txMessage;
        txMessage.setRawData(message, len);

        // qDebug() << txMessage << endl;

        send(txMessage);
        //qDebug() << "bytesSent:" << bytesSent;
        //qDebug() << "Flushing Socket";
        socket->flush();
        //qDebug() << "Flushed Socket";
    }
    else{
        qDebug() << "Client is not connected to the server. \n";
    }
}

void ClientSocket::HandleResponse(QJsonObject obj) {
    QJsonObject response;
    QJsonArray array;
    if(obj.contains(QStringLiteral("R1_Movement"))) {
        array = obj["R1_Movement"].toArray();
        emit sendMovement(SEARCHERMOVER, array[0].toString(), array[1].toString(),
                array[2].toString(), array[3].toString(), array[4].toString());
//        qDebug() << obj["R1_Movement"];
    }
    if(obj.contains(QStringLiteral("LineLocation"))) {
        int location = 0;
        response = obj["LineLocation"].toObject();
        for (int i = 0; i < 8; i++) {
            if (ANALOG) {
                if (v_lineColor == WHITE) {
                    location |= (response[QString::number(i)].toString().toFloat() < v_threshold) << i;
                } else if (v_lineColor == BLACK) {
                    location |= (response[QString::number(i)].toString().toFloat() > v_threshold) << i;
                }
            } else {
                location |= !(response[QString::number(i)].toString().toFloat()) << i;
            }
        }
        emit sendLineLocation(location);
        qDebug() << obj["LineLocation"];
    }
}

void ClientSocket::HandleRequest(QJsonArray array)
{
    if (array.contains(QStringLiteral("InitialData"))) {
        emit initialRequest();
    }
    if (array.contains(QStringLiteral("SensorData"))) {
        QString response_begin = "{\"type\":\"Response\",";
        QString response_end = "}";
        QString data;

        if (sendData) {
            if (isClear) {
                data = "\"0\"";
            } else {
                data = "\"1\"";
            }
            SendJSONResponseToSocket(response_begin + "\"SensorData\":" + data + response_end, SEARCHERMOVER);
        }
    }
}
