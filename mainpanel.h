#ifndef MAINPANEL_H
#define MAINPANEL_H
#include "launcher.h"
#include "taskmanager.h"

#include <KGlobalAccel>
#include <QCoreApplication>
#include <QDateTime>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QPushButton>
#include <QScreen>
#include <QStyle>
#include <QTimer>
#include <QWidget>
#include <QtCore/QDebug>
#include <QtPlatformHeaders/QXcbWindowFunctions>

QT_BEGIN_NAMESPACE
namespace Ui {
class mainpanel;
}
QT_END_NAMESPACE

class mainpanel : public QMainWindow
{
  Q_OBJECT

public:
  QPushButton* home = new QPushButton();
  QPushButton* nowapps = new QPushButton();
  QPushButton* notification = new QPushButton();
  QPushButton* time = new QPushButton();
  QDateTime dateTime = QDateTime::currentDateTime();
  launcher lnch;
  taskmanager task;

  int width = 40; //パネルの太さを指定
  int biconsize = 28;
  int margin = 0;
  int islaunchershowed = 0;

  mainpanel(QWidget* parent = nullptr);
  void onlauncher();
  void onmanager();
  void showTime();
  ~mainpanel();

private:
  Ui::mainpanel* ui;
};
#endif // MAINPANEL_H
