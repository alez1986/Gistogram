#include "SearchController.h"
#include <QFile>
#include <QFuture>
#include <QTextStream>
#include <QtConcurrent>


SearchController::SearchController(QObject* parent)
  : QObject(parent)
{
  m_watcher.reset(new QFutureWatcher<void>());
}


QStringList SearchController::category() const
{
  return m_dict.keys();
}


QVariantList SearchController::value() const
{
  QVariantList data;
  for (int i : m_dict.values())
    data.append(i);

  return data;
}


int SearchController::maximum() const
{
  return m_maximum;
}


void SearchController::readFile(const QString& fileUrl)
{
  qDebug() << "Read file slot";

  m_dict.clear();

  QUrl url(fileUrl);
  QFile file(url.toLocalFile());
  if (!file.open(QFile::ReadOnly | QFile::Text))
    return;

  qDebug() << "File opened";
  emit readProgress(0);

  QTextStream in(&file);
  QString data = in.readAll().simplified();
  auto words = data.split(" ");
  int wordCount = words.size();

  connect(m_watcher.get(), SIGNAL(finished()), this, SIGNAL(readFinished()));
  QFuture<void> future = QtConcurrent::run([=]() {
    for (int i = 0; i < wordCount; ++i) {
      auto word = words[i].toLower();
      if (m_dict.contains(word))
        m_dict[word] = m_dict.value(word) + 1;
      else
        m_dict[word] = 1;

      emit readProgress(i * 100 / wordCount);
    }

    top15(m_dict);
    auto valuesList = m_dict.values();
    m_maximum = *std::max_element(valuesList.constBegin(), valuesList.constEnd());
    qDebug() << "Max value:" << m_maximum;

    emit readProgress(100);
    emit categoryChanged();
    emit valueChanged();
    emit maximumChanged();
  });

  m_watcher->setFuture(future);
}


void SearchController::top15(QMap<QString,int>& map) {
  if (map.size() < 16)
    return;

  auto values = map.values();
  std::sort(values.begin(), values.end(), std::greater<int>());
  int minValue = values[15];

  for (auto& key : map.keys()) {
    if (map.value(key) < minValue)
      map.remove(key);
  }

  if (map.size() > 15) {
    for (auto& key : map.keys()) {
      if (map.value(key) == minValue)
        map.remove(key);

      if (map.size() == 15)
        break;
    }
  }
}
