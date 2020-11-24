#ifndef SEARCHCONTROLLER_H
#define SEARCHCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariantList>
#include <QFutureWatcher>


class SearchController : public QObject
{
  Q_OBJECT

  Q_PROPERTY(QStringList category READ category NOTIFY categoryChanged)
  // QList<int> не захотел Charts никак отображать
  Q_PROPERTY(QVariantList value READ value NOTIFY valueChanged)
  Q_PROPERTY(int maximum READ maximum NOTIFY maximumChanged)

  public:
    explicit SearchController(QObject* parent = nullptr);

    QStringList category() const;
    QVariantList value() const;
    int maximum() const;

  signals:
    void readProgress(int value);
    void readFinished();

    void categoryChanged();
    void valueChanged();
    void maximumChanged();

  public slots:
    void readFile(const QString& fileUrl);

  private:
    void top15(QMap<QString, int> &map);

  private:
    QMap<QString,int> m_dict;
    int m_maximum = 10;

    std::unique_ptr<QFutureWatcher<void>> m_watcher;
};

#endif // SEARCHCONTROLLER_H
