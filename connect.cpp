#include <QSslSocket>
#include <QDebug>
#include <QMap>

#include "connect.h"


connectYt::connectYt(QObject *parent) : QObject(parent), flagError(false)
{
    mpNetManager.reset(new QNetworkAccessManager(parent));
    QObject::connect(mpNetManager.get(), SIGNAL(finished(QNetworkReply*)), this, SLOT(fetchResult(QNetworkReply*)));
}

QVariantMap connectYt::getStatisticMap(const QVariantMap &aMap)
{
    QMap<QString, QVariant> error = {{"null", 0}};
    QList<QVariant> list = aMap["items"].toList();
    int size = list.count();

    if(!size) {
        flagError = true;
        return error;
    }

    else {
        auto item = list.at(0);
        flagError = false;
        return item.toMap()["statistics"].toMap();
    }
}

void connectYt::fillYtStatistic(ytStatistic &aObj, const QVariantMap &aMap)
{
    aObj.viewCount      = aMap["viewCount"].toInt();
    aObj.likeCount      = aMap["likeCount"].toInt();
    aObj.dislikeCount   = aMap["dislikeCount"].toInt();
    aObj.favoriteCount  = aMap["favoriteCount"].toInt();
    aObj.commentCount   = aMap["commentCount"].toInt();
}

void connectYt::fetchData(const QString &id)
{
    QUrl url(URL_Yt);

    QUrlQuery query;
    query.addQueryItem("part", STAT_Yt);
    query.addQueryItem("id"  , id);
    query.addQueryItem("key" , KEY_Yt);

    url.setQuery(query);
    mpNetManager.get()->get(QNetworkRequest(url));
}

void connectYt::fetchResult(QNetworkReply * apReply)
{
    if(apReply->error()) {
        emit sendData(false, 0);
        return;
    }

    QString data =  static_cast<QString>(apReply->readAll());
    QJsonDocument jsonDoc    = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject   jsonObject = jsonDoc.object();
    QVariantMap   vmap       = jsonObject.toVariantMap();

    if(!vmap.isEmpty()) {

        QVariantMap tmp = getStatisticMap(vmap);
        if(!flagError) {
            fillYtStatistic(mStatistic, tmp);
            emit sendData(true, mStatistic.likeCount);
        }
        else {
            emit sendData(false, 0);
        }
    }

    else
        emit sendData(false, 0);    
}
