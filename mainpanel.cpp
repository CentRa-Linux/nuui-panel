#include "mainpanel.h"
#include "launcher.h"
#include "taskmanager.h"

#include <kwindowinfo.h>
#include <kwindowsystem.h>

#include "ui_mainpanel.h"

mainpanel::mainpanel(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::mainpanel)
{
  QScreen* screen = QGuiApplication::primaryScreen(); //画面の大きさを取得
  QRect screenRect = screen->availableVirtualGeometry();

  ui->setupUi(this);
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint |
                 Qt::WindowDoesNotAcceptFocus);
  setAttribute(Qt::WA_X11NetWmWindowTypeDock);
  setAttribute(Qt::WA_X11DoNotAcceptFocus);

  setWindowOpacity(0.95);

  mainpanel::resize(screenRect.width(), width); //取得した情報に合わせリサイズ
  move(0, screenRect.height() - width); //位置を変更

  setFixedHeight(width);
  KWindowInfo info(winId(), 0, NET::WM2WindowClass);

  KWindowSystem::setStrut(effectiveWinId(), 0, 0, 0, width);
  KWindowSystem::setType(effectiveWinId(), NET::Dock);

  QWidget* central = new QWidget(this);
  QHBoxLayout* contents = new QHBoxLayout(central);

  QTimer* timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &mainpanel::showTime);
  timer->start(1000);

  home->setIcon(QIcon::fromTheme("go-home-symbolic"));
  home->setIconSize(QSize(biconsize, biconsize));
  home->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  home->setFixedWidth(width * 3);
  home->setFlat(true);
  connect(home, &QPushButton::clicked, this, &mainpanel::onlauncher);

  nowapps->setIcon(QIcon::fromTheme("window-restore-symbolic"));
  nowapps->setIconSize(QSize(biconsize, biconsize));
  nowapps->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  nowapps->setFixedWidth(width * 3);
  nowapps->setFlat(true);
  connect(nowapps, &QPushButton::clicked, this, &mainpanel::onmanager);

  notification->setIcon(QIcon::fromTheme("notifications"));
  notification->setIconSize(QSize(biconsize, biconsize));
  notification->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
  notification->setFlat(true);
  notification->setFixedWidth(width * 3);

  time->setText(dateTime.time().toString("HH:mm:ss"));
  time->setFont(QFont("", width / 3));
  time->setFlat(true);
  time->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

  contents->addWidget(home);
  contents->addWidget(nowapps);
  contents->addStretch();
  contents->addWidget(time);
  contents->addWidget(notification);
  contents->setMargin(margin);
  setCentralWidget(central);

  lnch.setpanelwidth(width);
  task.setpanelwidth(width);
}

void
mainpanel::onlauncher()
{
  if (lnch.isShown == true) {
    lnch.anihide(width);
  } else {
    lnch.anishow(width);
    if (task.isShown == true) {
      task.anihide();
    }
  }
}

void
mainpanel::onmanager()
{
  if (task.isShown == true) {
    task.anihide();
  } else {
    task.anishow();
    if (lnch.isShown == true) {
      lnch.anihide(width);
    }
  }
}

void
mainpanel::showTime()
{
  QDateTime ntime = QDateTime::currentDateTime();
  time->setText(ntime.time().toString("HH:mm:ss"));
}

mainpanel::~mainpanel()
{
  delete ui;
}
