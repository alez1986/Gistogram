#include <QtWidgets/QApplication>
#include <QtQuick/QQuickView>
#include <QtCore/QDir>
#include <QtQml/QQmlEngine>
#include <QQmlContext>

#include "SearchController.h"


int main(int argc, char *argv[])
{
    // Qt Charts uses Qt Graphics View Framework for drawing, therefore QApplication must be used.
  QApplication app(argc, argv);

  QQuickView viewer;
#ifdef Q_OS_WIN
  QString extraImportPath(QStringLiteral("%1/../../../../%2"));
#else
  QString extraImportPath(QStringLiteral("%1/../../../%2"));
#endif
  viewer.engine()->addImportPath(extraImportPath.arg(QGuiApplication::applicationDirPath(),
                                                     QString::fromLatin1("qml")));
  QObject::connect(viewer.engine(), &QQmlEngine::quit, &viewer, &QWindow::close);

  viewer.setTitle(QStringLiteral("Top 15 Word Chart"));

  auto context = viewer.engine()->rootContext();
  SearchController* searchController = new SearchController;
  context->setContextProperty("searchController", searchController);

  viewer.setSource(QUrl("qrc:/main.qml"));
  viewer.setResizeMode(QQuickView::SizeRootObjectToView);
  viewer.show();

  return app.exec();
}
