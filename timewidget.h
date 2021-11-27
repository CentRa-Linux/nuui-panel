#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <KGlobalAccel>
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
#include <QWidget>
#include <QtCore/QDebug>
#include <QtPlatformHeaders/QXcbWindowFunctions>
#include <iostream>

class timewidget : public QWidget
{
  Q_OBJECT
public:
  void showTime();
  explicit timewidget(QWidget* parent = nullptr);
  int fsize = 15;

  QPushButton* time = new QPushButton();
  QPushButton* date = new QPushButton();

signals:
};

#endif // TIMEWIDGET_H
