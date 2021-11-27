#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <KGlobalAccel>
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QGraphicsOpacityEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>
#include <QMainWindow>
#include <QObject>
#include <QProcess>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QScroller>
#include <QStackedWidget>
#include <QStandardPaths>
#include <QStyle>
#include <QTabWidget>
#include <QTimer>
#include <QWidget>
#include <QtCore/QDebug>
#include <QtPlatformHeaders/QXcbWindowFunctions>
#include <iostream>

class launcher : public QMainWindow
{
  Q_OBJECT

public:
  void setpanelwidth(int input);
  void anishow(int input);
  void anihide(int input);
  void switchtab();
  QStackedWidget* sw =
    new QStackedWidget(); //アプリケーションが一覧表示されるウィジェット
  std::tuple<QString,
             QString,
             QStringList,
             QString,
             QIcon,
             QStringList,
             QStringList,
             QStringList,
             QStringList,
             bool>
  fileread(QString path); // Name, Comment, Keywords, Exec, Icon, Categories,
                          // ActionLocalizedNames, ActionNames, ActionExecs
  void launchapp();
  int t = 80;
  int columns = 6;
  int iconsize = 64;
  int tabheight = 50;
  int tabwidth = 150;
  int sidewidth = 200;
  float opacity = 0.95;
  bool sidetab = true;
  bool widget = true;
  int widgetwidth = 200;
  int pwidgetwidth = 200;
  int pluswidth = 200;
  bool isShown = false;

  explicit launcher(QWidget* parent = nullptr);

signals:
};

#endif // LAUNCHER_H
