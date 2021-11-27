#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <KGlobalAccel>
#include <QApplication>
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QLocale>
#include <QMainWindow>
#include <QObject>
#include <QProcess>
#include <QPushButton>
#include <QScreen>
#include <QScrollArea>
#include <QStandardPaths>
#include <QStyle>
#include <QTimer>
#include <QVariant>
#include <QWidget>
#include <QtCore/QDebug>
#include <QtPlatformHeaders/QXcbWindowFunctions>
#include <iostream>

class taskmanager : public QMainWindow
{
  Q_OBJECT
public:
  explicit taskmanager(QWidget* parent = nullptr);
  void setpanelwidth(int input);
  void activate();
  void forceclose();
  void anishow();
  void anihide();
  int iconsize = 40;
  int cancelsize = 32;
  int fontsize = 12;
  bool isShown = false;

private slots:
  void windowchanged();

signals:
};

#endif // TASKMANAGER_H
