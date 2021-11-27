#include "timewidget.h"

timewidget::timewidget(QWidget* parent)
  : QWidget(parent)
{
  QVBoxLayout* box = new QVBoxLayout();
  box->setMargin(0);
  box->setSpacing(0);
  QDateTime ntime = QDateTime::currentDateTime();

  time->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  time->setStyleSheet("Text-align:left");
  time->setFont(QFont("", fsize));
  time->setFlat(true);
  date->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  date->setStyleSheet("Text-align:left");
  date->setFont(QFont("", fsize));
  date->setFlat(true);

  time->setText(ntime.toString("  HH:mm:ss"));
  date->setText(ntime.toString("  yyyy/MM/dd"));

  box->addWidget(time);
  box->addWidget(date);
  setLayout(box);

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &timewidget::showTime);
  timer->start(1000);
  QScreen* screen = QGuiApplication::primaryScreen(); //画面の大きさを取得
  QRect screenRect = screen->availableVirtualGeometry();
  setFixedSize(QSize(300, 100));
}

void
timewidget::showTime()
{
  QDateTime ntime = QDateTime::currentDateTime();
  time->setText(ntime.toString("  HH:mm:ss"));
  date->setText(ntime.toString("  yyyy/MM/dd"));
}
