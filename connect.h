#ifndef CONNECT_H
#define CONNECT_H

#include <QObject>

#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

#define URL_Yt    "https://youtube.googleapis.com/youtube/v3/videos"
#define KEY_Yt    "AIzaSyDaJhLAprn2M0bHjjGKLRJI8NDVQH08Xuc"
#define STAT_Yt   "statistics"

struct ytStatistic
{
  unsigned long  viewCount;
  unsigned long  likeCount;
  unsigned long  dislikeCount;
  unsigned long  favoriteCount;
  unsigned long  commentCount;
};

class connectYt : public QObject
{
    Q_OBJECT

public:

    explicit connectYt(QObject *parent = nullptr);
    QVariantMap getStatisticMap(const QVariantMap &aMap);
    void fillYtStatistic(ytStatistic &aObj, const QVariantMap &aMap);

public slots:

    void fetchData(const QString &);
    void fetchResult(QNetworkReply*);

private:

    std::unique_ptr<QNetworkAccessManager> mpNetManager{nullptr};
    ytStatistic  mStatistic;
    bool flagError;

signals:

    void sendData(bool, unsigned long);

};

#endif // CONNECT_H
