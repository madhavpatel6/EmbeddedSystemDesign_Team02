// clientsocket.cpp

#include "clientsocket.h"
#include "pfwindow.h"

ClientSocket::ClientSocket(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket();

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
    isConnected = false;
}

ClientSocket::~ClientSocket()
{
    socket->disconnectFromHost();
    qDebug() << "Disconnected from host" << endl;
    delete socket;
}

void ClientSocket::connectToHost(QString ip,int port)
{
    socket->connectToHost(ip,port);
}

QTcpSocket* ClientSocket::getClient()
{
    return socket;
}

void ClientSocket::sendProximity(int proxVal){
    QString response_begin = "{\"type\":\"Response\",\"targetProximity\":\"";
    QString response_end = "\"}";
    QString curResp = response_begin + QString::number(proxVal) + response_end;
    emit sendCurRespSignal(curResp);
    SendJSONRequestToSocket(response_begin + QString::number(proxVal) + response_end, TARGETGRABBER);
}

void ClientSocket::sendMovement(bool movementBool){
    QString response_begin = "{\"type\":\"Response\",\"movementStopped\":\"";
    QString response_end = "\"}";
    QString curResp = response_begin + QString::number(movementBool) + response_end;
    emit sendCurRespSignal(curResp);
    SendJSONRequestToSocket(response_begin + QString::number(movementBool) + response_end, TARGETGRABBER);
}

void ClientSocket::sendOutsideArena(bool arenaBool){
    QString response_begin = "{\"type\":\"Response\",\"outsideArena\":\"";
    QString response_end = "\"}";
    QString curResp = response_begin + QString::number(arenaBool) + response_end;
    emit sendCurRespSignal(curResp);
    SendJSONRequestToSocket(response_begin + QString::number(arenaBool) + response_end, TARGETGRABBER);
}

void ClientSocket::alignmentRequestedSlot(){
    QString request_begin = "{\"type\":\"Request\",\"items\":[\"";
    QString request_end = "\"]}";
    QString curReq = request_begin + "targetAlignment" + request_end;
    emit sendCurReqSignal(curReq);
    SendJSONRequestToSocket(request_begin + "targetAlignment" + request_end, TARGETGRABBER);
}

void ClientSocket::targetAcquiredRequested(bool targetAcquiredReqBool){
    if(targetAcquiredReqBool == true){
        QString request_begin = "{\"type\":\"Request\",\"items\":[\"";
        QString request_end = "\"]}";
        QString curReq = request_begin + "targetAcquired" + request_end;
        emit sendCurReqSignal(curReq);
        SendJSONRequestToSocket(request_begin + "targetAcquired" + request_end, TARGETGRABBER);
    }
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
            switch(source){
                case TARGETGRABBER:{
                    if(type == QStringLiteral("Response")) {
                        if(json.contains(QStringLiteral("targetAlignment"))){
                            HandleAlignmentResponse(json, TARGETGRABBER);
                        }
                        else if(json.contains(QStringLiteral("targetAcquired"))){
                            HandleAcquiredResponse(json,TARGETGRABBER);
                        }
                    }
                    else if(type == QStringLiteral("Request")){
                        if(json.contains(QStringLiteral("items"))){
                            HandleTGRequest(json,TARGETGRABBER);
                        }
                    }
                    break;
                }
                default:{
                            qDebug() << "BROKEN" << endl;
                            break;
                }
            }
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
        int bytesSent = send(txMessage);
        socket->flush();
    }
    else{
        qDebug() << "Client is not connected to the server. \n";
    }
}

void ClientSocket::HandleAlignmentResponse(QJsonObject obj, char source) {
    //qDebug() << "JSON : " << obj << endl;
    QJsonObject targetAlignmentObj = obj.value("targetAlignment").toObject();
    QJsonDocument doc(targetAlignmentObj);
    QString targetAlignmentJsonStr((doc.toJson(QJsonDocument::Compact)));
    targetAlignmentJsonStr.replace("\"","");
    targetAlignmentJsonStr.replace(",",", ");
    if(obj.contains(QStringLiteral("targetAlignment"))) {
        targetAlignmentObj = obj["targetAlignment"].toObject();
        switch(source){
            case TARGETGRABBER:{
                QString response_begin = "{\"type\":\"Response\",\"targetAlignment\":[\"";
                QString response_end = "\"]}";
                QString tgcurResp = response_begin + targetAlignmentJsonStr + response_end;
                emit sendtgCurRespSignal(tgcurResp);
                emit sendAlignmentSignal(TARGETGRABBER, targetAlignmentObj["IR_0"].toString(), targetAlignmentObj["IR_1"].toString(), targetAlignmentObj["IR_2"].toString(), targetAlignmentObj["IR_3"].toString(), targetAlignmentObj["IR_4"].toString(), targetAlignmentObj["Target_Distance"].toString(), targetAlignmentObj["Target_Angle"].toString(), targetAlignmentObj["IR_0_bool"].toString(), targetAlignmentObj["IR_1_bool"].toString(), targetAlignmentObj["IR_2_bool"].toString(), targetAlignmentObj["IR_3_bool"].toString(), targetAlignmentObj["IR_4_bool"].toString(), targetAlignmentObj["Grabber_Aligned"].toString(), targetAlignmentObj["Alignment_Info"].toString());
                break;
            }
            default:{
                    qDebug() << "BROKEN" << endl;
                    break;
            }
        }
    }
}

void ClientSocket::HandleAcquiredResponse(QJsonObject obj, char source){
    if(obj.contains(QStringLiteral("targetAcquired"))) {
        switch(source){
            case TARGETGRABBER:{
                QString response_begin = "{\"type\":\"Response\",\"targetAcquired\":\"";
                QString response_end = "\"}";
                QString tgcurResp = response_begin + obj["targetAcquired"].toString() + response_end;
                emit sendtgCurRespSignal(tgcurResp);
                emit sendAcquiredSignal(obj["targetAcquired"].toString());
                break;
            }
            default:{
                    qDebug() << "BROKEN" << endl;
                    break;
            }
        }
    }
}

void ClientSocket::HandleTGRequest(QJsonObject obj, char source){
    QJsonValue value = obj.value("items");
    QJsonArray array = value.toArray();
    QString tgcurReq;

    // This is for debugging my incoming requests : items[] array
    //foreach (const QJsonValue & v, array)
    //    qDebug() << "IN LOOP : " << v.toString();

    if(obj.contains(QStringLiteral("items"))) {
        switch(source){
            case TARGETGRABBER:{
                QString request_begin = "{\"type\":\"Request\",\"items\":[\"";
                QString request_end = "\"]}";
                if(array.contains(QJsonValue(QString("movementStopped"))) && array.contains(QJsonValue(QString("targetProximity"))) && array.contains(QJsonValue(QString("outsideArena")))){
                    tgcurReq = request_begin + "outsideArena\",\"targetProximity\",\"movementStopped" + request_end;
                    emit sendtgCurReqSignal(tgcurReq);
                }
                else{
                    tgcurReq = request_begin + "INCORRECT_REQUEST" + request_end;
                    emit sendtgCurReqSignal(tgcurReq);
                }
                break;
            }
            default:{
                    qDebug() << "BROKEN" << endl;
                    break;
            }
        }
    }
}
