#include "taskmanager.h"
#include "launcher.h"

#include <KWindowSystem>
#include <QPropertyAnimation>
#include <QX11Info>
#include <kwindowinfo.h>
#include <kwindowsystem.h>
#include <netwm.h>
#include <netwm_def.h>

std::vector<std::tuple<int, QString, bool, QIcon>> apps;

taskmanager::taskmanager(QWidget* parent)
  : QMainWindow(parent)
{
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);
  setAttribute(Qt::WA_X11NetWmWindowTypeDock);
  setAttribute(Qt::WA_X11DoNotAcceptFocus);

  QScreen* screen = QGuiApplication::primaryScreen(); //画面の大きさを取得
  QRect screenRect = screen->availableVirtualGeometry();
  move(0, 0);

  setWindowOpacity(0.95);

  // init
  for (auto wid : KWindowSystem::windows()) {
    KWindowInfo info(wid,
                     NET::WMState | NET::WMWindowType | NET::WMAllProperties);
    if (info.valid()) {
      auto type = info.windowType(NET::AllTypesMask);
      auto view_type = type == NET::Normal || type == NET::Unknown;
      bool visible = !info.hasState(NET::SkipTaskbar) && view_type;
      apps.push_back(
        std::make_tuple(wid,
                        info.visibleName(),
                        visible,
                        QIcon(KWindowSystem::icon(wid).scaled(80, 80))));
    }
  }

  QVBoxLayout* main = new QVBoxLayout();
  for (auto app : apps) {
    if (std::get<2>(app) == true) {
      QHBoxLayout* entry = new QHBoxLayout();

      QPushButton* button = new QPushButton();
      button->setText("  " + std::get<1>(app));
      button->setIcon(std::get<3>(app));
      button->setIconSize(QSize(iconsize, iconsize));
      button->setFont(QFont("", fontsize));
      button->setStyleSheet("Text-align:left");
      button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      button->setObjectName(QString::number(std::get<0>(app)));

      connect(button, &QPushButton::clicked, this, &taskmanager::activate);

      QPushButton* closebutton = new QPushButton();
      closebutton->setIcon(QIcon::fromTheme("window-close"));
      closebutton->setIconSize(QSize(cancelsize, cancelsize));
      closebutton->setSizePolicy(QSizePolicy::Preferred,
                                 QSizePolicy::Expanding);
      closebutton->setObjectName(QString::number(std::get<0>(app)));

      connect(
        closebutton, &QPushButton::clicked, this, &taskmanager::forceclose);

      entry->addWidget(button);
      entry->addWidget(closebutton);

      main->addLayout(entry);
    }
  }

  auto system = KWindowSystem::self();
  connect(system, SIGNAL(windowAdded(WId)), this, SLOT(windowchanged()));
  connect(system, SIGNAL(windowRemoved(WId)), this, SLOT(windowchanged()));
  connect(
    system, SIGNAL(currentDesktopChanged(int)), this, SLOT(windowchanged()));

  QWidget* cwidget = new QWidget();
  cwidget->setFixedWidth(screenRect.width() / 2.5 - 20);
  cwidget->setLayout(main);
  QScrollArea* central = new QScrollArea();
  central->setAlignment(Qt::AlignCenter);
  central->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  central->setWidget(cwidget);
  QScroller::grabGesture(central, QScroller::LeftMouseButtonGesture);
  setCentralWidget(central);
}

void
taskmanager::windowchanged()
{
  QScreen* screen = QGuiApplication::primaryScreen(); //画面の大きさを取得
  QRect screenRect = screen->availableVirtualGeometry();
  apps.clear();

  // refresh
  for (auto wid : KWindowSystem::windows()) {
    KWindowInfo info(wid,
                     NET::WMState | NET::WMWindowType | NET::WMAllProperties);
    if (info.valid()) {
      auto type = info.windowType(NET::AllTypesMask);
      auto view_type = type == NET::Normal || type == NET::Unknown;
      bool visible = !info.hasState(NET::SkipTaskbar) && view_type;
      apps.push_back(
        std::make_tuple(wid,
                        info.visibleName(),
                        visible,
                        QIcon(KWindowSystem::icon(wid).scaled(80, 80))));
    }
  }

  QVBoxLayout* main = new QVBoxLayout();
  for (auto app : apps) {
    if (std::get<2>(app) == true) {
      QHBoxLayout* entry = new QHBoxLayout();

      QPushButton* button = new QPushButton();
      button->setText("  " + std::get<1>(app));
      button->setIcon(std::get<3>(app));
      button->setIconSize(QSize(iconsize, iconsize));
      button->setFont(QFont("", fontsize));
      button->setStyleSheet("Text-align:left");
      button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      button->setObjectName(QString::number(std::get<0>(app)));
      button->setFlat(true);

      connect(button, &QPushButton::clicked, this, &taskmanager::activate);

      QPushButton* closebutton = new QPushButton();
      closebutton->setIcon(QIcon::fromTheme("window-close"));
      closebutton->setIconSize(QSize(cancelsize, cancelsize));
      closebutton->setSizePolicy(QSizePolicy::Preferred,
                                 QSizePolicy::Expanding);
      closebutton->setObjectName(QString::number(std::get<0>(app)));
      closebutton->setFlat(true);

      connect(
        closebutton, &QPushButton::clicked, this, &taskmanager::forceclose);

      entry->addWidget(button);
      entry->addWidget(closebutton);

      main->addLayout(entry);
    }
  }

  QWidget* cwidget = new QWidget();
  cwidget->setFixedWidth(screenRect.width() / 2.5 - 40);
  cwidget->setLayout(main);
  QScrollArea* central = new QScrollArea();
  central->setAlignment(Qt::AlignCenter);
  central->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  central->setWidget(cwidget);
  QScroller::grabGesture(central, QScroller::TouchGesture);
  setCentralWidget(central);
}

void
taskmanager::setpanelwidth(int input)
{
  QScreen* screen = QGuiApplication::primaryScreen(); //画面の大きさを取得
  QRect screenRect = screen->availableVirtualGeometry();
  move(0, 0); //位置を変更

  taskmanager::resize(screenRect.width() / 2.5, screenRect.height() - input);
}

void
taskmanager::anishow()
{
  QScreen* screen = QGuiApplication::primaryScreen(); //画面の大きさを取得
  QRect screenRect = screen->availableVirtualGeometry();
  move(screenRect.width() / 2.5 * -1, 0);
  QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
  show();
  animation->setEasingCurve(QEasingCurve::InOutCubic);
  animation->setDuration(200);
  animation->setStartValue(QRect(screenRect.width() / 2.5 * -1,
                                 0,
                                 screenRect.width() / 2.5,
                                 screenRect.height()));
  animation->setEndValue(
    QRect(0, 0, screenRect.width() / 2.5, screenRect.height()));
  animation->start();
  isShown = true;
}

void
taskmanager::anihide()
{
  QScreen* screen = QGuiApplication::primaryScreen(); //画面の大きさを取得
  QRect screenRect = screen->availableVirtualGeometry();
  move(screenRect.width() / 2.5 * -1, 0);
  QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
  animation->setEasingCurve(QEasingCurve::InOutCubic);
  animation->setDuration(200);
  animation->setStartValue(
    QRect(0, 0, screenRect.width() / 2.5, screenRect.height()));
  animation->setEndValue(QRect(screenRect.width() / 2.5 * -1,
                               0,
                               screenRect.width() / 2.5,
                               screenRect.height()));
  animation->start();
  connect(animation, &QAbstractAnimation::finished, this, &taskmanager::hide);
  isShown = false;
}

void
taskmanager::activate()
{
  KWindowSystem::forceActiveWindow(sender()->objectName().toInt());
  anihide();
}

void
taskmanager::forceclose()
{
  NETRootInfo ri(QX11Info::connection(), NET::CloseWindow);
  ri.closeWindowRequest(sender()->objectName().toInt());
  anihide();
}
