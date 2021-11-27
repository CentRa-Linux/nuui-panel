#include <QWidget>

#include "launcher.h"
#include "timewidget.h"
#include <QPropertyAnimation>

QVector<std::tuple<QString,
                   QString,
                   QStringList,
                   QString,
                   QIcon,
                   QStringList,
                   QStringList,
                   QStringList,
                   QStringList,
                   bool>>
  objects;

launcher::launcher(QWidget* parent)
  : QMainWindow(parent)
{
  std::vector<QString> categories = {
    "AudioVideo", "Development", "Engineering", "Education",
    "Game",       "Graphics",    "Network",     "Office",
    "Science",    "Settings",    "System",      "Utility"
  };
  std::vector<QString> cnames = {
    tr("AudioVideo"), tr("Development"), tr("Engineering"), tr("Education"),
    tr("Game"),       tr("Graphics"),    tr("Network"),     tr("Office"),
    tr("Science"),    tr("Settings"),    tr("System"),      tr("Utility")
  };
  std::vector<QString> inames = {
    "applications-multimedia",  "applications-development",
    "applications-engineering", "applications-education",
    "applications-games",       "applications-graphics",
    "applications-network",     "applications-office",
    "applications-science",     "preferences",
    "applications-system",      "applications-utilities"
  };

  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus);
  setAttribute(Qt::WA_X11NetWmWindowTypeDock);
  setAttribute(Qt::WA_X11DoNotAcceptFocus);

  setWindowOpacity(opacity); //透明度をファイルから読み込めるようにしたい

  QScreen* screen = QGuiApplication::primaryScreen(); //画面の大きさを取得
  QRect screenRect = screen->availableVirtualGeometry();
  launcher::resize(screenRect.width(), screenRect.height());
  move(0, screenRect.height()); //位置を変更

  QDir d1("/usr/share/applications");
  QDir d2(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
          "/.local/share/applications");
  QStringList e1 = d1.entryList(QStringList() << "*.desktop", QDir::Files);
  QStringList e2 = d2.entryList(QStringList() << "*.desktop", QDir::Files);
  foreach (QString name, e1) {
    objects.append(fileread("/usr/share/applications/" + name));
  }
  foreach (QString name, e2) {
    objects.append(
      fileread(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
               "/.local/share/applications/" + name));
  }

  if (sidetab == true) {
    pluswidth = 0;
  }

  std::sort(
    objects.begin(),
    objects.end(),
    [](const std::tuple<QString,
                        QString,
                        QStringList,
                        QString,
                        QIcon,
                        QStringList,
                        QStringList,
                        QStringList,
                        QStringList,
                        bool>& x,
       const std::tuple<QString,
                        QString,
                        QStringList,
                        QString,
                        QIcon,
                        QStringList,
                        QStringList,
                        QStringList,
                        QStringList,
                        bool>& y) { return std::get<0>(x) < std::get<0>(y); });

  QWidget* cwidget =
    new QWidget(); //最終的にランチャー画面に配置するウィジェット
  QVBoxLayout* swbuttons = new QVBoxLayout(); //上を選ぶためのタブ的なやつ
  swbuttons->setMargin(0);
  swbuttons->setSpacing(0);
  QHBoxLayout* swunderbuttons =
    new QHBoxLayout(); //上を選ぶためのタブ的なやつ（下に置くよう
  swunderbuttons->setMargin(0);
  swunderbuttons->setSpacing(0);
  QHBoxLayout* pane = new QHBoxLayout(); //ランチャー側
  pane->setMargin(0);
  pane->setSpacing(0);
  QVBoxLayout* widgetpane = new QVBoxLayout(); //ウィジェット側
  QGridLayout* maingrid = new QGridLayout(); //これにアプリケーション一覧が入る
  if (widget == true) {
    pwidgetwidth = 0;
  }
  int cnt = 0;

  for (int i = 0; i < objects.size(); i++) { //ここで全体の一覧を取得
    if (std::get<9>(objects[i]) == false) {
      QPushButton* iconbutton = new QPushButton();
      QVBoxLayout* vbox = new QVBoxLayout();
      QWidget* awidget = new QWidget();
      iconbutton->setIcon(std::get<4>(objects[i]));
      iconbutton->setIconSize(QSize(iconsize, iconsize));
      iconbutton->setSizePolicy(
        QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
      iconbutton->setFixedHeight(iconsize + 40);
      iconbutton->setObjectName(QString::number(i));
      iconbutton->setFlat(true);

      connect(iconbutton, &QPushButton::clicked, this, &launcher::launchapp);

      QLabel* label = new QLabel(std::get<0>(objects[i]));
      vbox->addWidget(iconbutton);
      vbox->addWidget(label);
      awidget->setLayout(vbox);
      awidget->setFixedWidth(
        (screenRect.width() - 800 + pluswidth + widgetwidth) / columns);
      maingrid->addWidget(awidget,
                          cnt / columns,
                          cnt - ((cnt / columns)) * columns,
                          Qt::AlignCenter);
      cnt++;
    }
  }

  if (widget == true) {
    timewidget* t = new timewidget();
    t->setFixedWidth(widgetwidth);
    widgetpane->addWidget(t);
  }

  cwidget->setLayout(maingrid);
  QScrollArea* central = new QScrollArea();
  central->setAlignment(Qt::AlignCenter);
  central->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  central->setWidget(cwidget);
  QScroller::grabGesture(central, QScroller::TouchGesture);
  sw->addWidget(central);
  QPushButton* tabbutton = new QPushButton(); // swbuttonsに追加するボタンを作る
  tabbutton->setText(tr("All"));
  tabbutton->setIcon(QIcon::fromTheme("applications-all"));
  tabbutton->setObjectName(QString::number(0));
  if (sidetab == false) {
    tabbutton->setFixedWidth(tabwidth);
  } else {
    tabbutton->setFixedWidth(sidewidth - 10);
  }
  tabbutton->setCheckable(true);
  tabbutton->setChecked(true);
  tabbutton->setFlat(true);
  tabbutton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  connect(tabbutton, &QPushButton::clicked, this, &launcher::switchtab);
  swbuttons->addWidget(tabbutton);
  swunderbuttons->addWidget(tabbutton);
  int c2 = 1;

  for (int i = 0; i < categories.size(); i++) {
    cnt = 0;
    QGridLayout* launchgrid = new QGridLayout();
    for (int c = 0; c < objects.size(); c++) {
      if (std::get<9>(objects[c]) == false &&
          std::find(std::get<5>(objects[c]).begin(),
                    std::get<5>(objects[c]).end(),
                    categories[i]) != std::get<5>(objects[c]).end()) {
        QPushButton* iconbutton = new QPushButton();
        QVBoxLayout* vbox = new QVBoxLayout();
        QWidget* awidget = new QWidget();
        iconbutton->setIcon(std::get<4>(objects[c]));
        iconbutton->setIconSize(QSize(iconsize, iconsize));
        iconbutton->setSizePolicy(
          QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
        iconbutton->setFixedHeight(iconsize + 40);
        iconbutton->setObjectName(QString::number(c));
        iconbutton->setFlat(true);

        connect(iconbutton, &QPushButton::clicked, this, &launcher::launchapp);

        QLabel* label = new QLabel(std::get<0>(objects[c]));
        vbox->addWidget(iconbutton);
        vbox->addWidget(label);
        awidget->setLayout(vbox);
        awidget->setFixedWidth(
          (screenRect.width() - 800 + pluswidth + widgetwidth) / columns);
        launchgrid->addWidget(awidget,
                              cnt / columns,
                              cnt - ((cnt / columns)) * columns,
                              Qt::AlignCenter);
        cnt++;
      }
    }
    QWidget* nwidget = new QWidget();
    nwidget->setLayout(launchgrid);
    QScrollArea* scarea = new QScrollArea();
    scarea->setAlignment(Qt::AlignCenter);
    scarea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scarea->setWidget(nwidget);
    QScroller::grabGesture(scarea, QScroller::TouchGesture);
    if (launchgrid->isEmpty() == false) {
      sw->addWidget(scarea);
      QPushButton* tabbutton = new QPushButton();
      tabbutton->setText(cnames[i]);
      tabbutton->setIcon(QIcon::fromTheme(inames[i]));
      tabbutton->setObjectName(QString::number(i + 1));
      if (sidetab == false) {
        tabbutton->setFixedWidth(tabwidth);
      } else {
        tabbutton->setFixedWidth(sidewidth - 10);
      }
      tabbutton->setCheckable(true);
      tabbutton->setChecked(false);
      tabbutton->setFlat(true);
      tabbutton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      connect(tabbutton, &QPushButton::clicked, this, &launcher::switchtab);
      swbuttons->addWidget(tabbutton);
      swunderbuttons->addWidget(tabbutton);
      c2++;
    }
  }

  qDebug() << sw->width();
  pane->addLayout(widgetpane);
  if (sidetab == true) {
    QScrollArea* swbarea = new QScrollArea();
    QWidget* nwidget = new QWidget();
    nwidget->setLayout(swbuttons);
    swbarea->setAlignment(Qt::AlignCenter);
    swbarea->setWidget(nwidget);
    swbarea->setFixedWidth(sidewidth);
    swbarea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    swbarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScroller::grabGesture(swbarea, QScroller::TouchGesture);
    pane->addWidget(swbarea);
    pane->addWidget(sw);
  } else {
    QVBoxLayout* templayout = new QVBoxLayout();
    QScrollArea* swbarea = new QScrollArea();
    QWidget* nwidget = new QWidget();
    nwidget->setLayout(swunderbuttons);
    swbarea->setWidget(nwidget);
    swbarea->setFixedHeight(tabheight + 15);
    swbarea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScroller::grabGesture(swbarea, QScroller::TouchGesture);
    templayout->addWidget(sw);
    templayout->addWidget(swbarea);
    pane->addLayout(templayout);
  }
  QWidget* tempwidget = new QWidget();
  tempwidget->setLayout(pane);

  setCentralWidget(tempwidget);
}

void
launcher::setpanelwidth(int input)
{
  QScreen* screen = QGuiApplication::primaryScreen(); //画面の大きさを取得
  QRect screenRect = screen->availableVirtualGeometry();
  launcher::resize(screenRect.width(), screenRect.height() - input);
}

void
launcher::switchtab()
{
  QString name = sender()->objectName();
  QPushButton* button = launcher::window()->findChild<QPushButton*>(name);
  QPushButton* prevbutton = launcher::window()->findChild<QPushButton*>(
    QString::number(sw->currentIndex()));
  button->setChecked(true);
  prevbutton->setChecked(false);
  sw->setCurrentIndex(name.toInt());
}

void
launcher::anishow(int input)
{
  QScreen* screen = QGuiApplication::primaryScreen(); //画面の大きさを取得
  QRect screenRect = screen->availableVirtualGeometry();
  QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
  show();
  animation->setEasingCurve(QEasingCurve::InOutCubic);
  animation->setDuration(200);
  animation->setStartValue(
    QRect(0, screenRect.height(), screenRect.width(), screenRect.height()));
  animation->setEndValue(QRect(0, 0, screenRect.width(), screenRect.height()));
  animation->start();
  isShown = true;
}

void
launcher::anihide(int input)
{
  QScreen* screen = QGuiApplication::primaryScreen(); //画面の大きさを取得
  QRect screenRect = screen->availableVirtualGeometry();
  QPropertyAnimation* animation = new QPropertyAnimation(this, "geometry");
  animation->setEasingCurve(QEasingCurve::InOutCubic);
  animation->setDuration(200);
  animation->setStartValue(
    QRect(0, 0, screenRect.width(), screenRect.height()));
  animation->setEndValue(
    QRect(0, screenRect.height(), screenRect.width(), screenRect.height()));
  animation->start();
  connect(animation, &QAbstractAnimation::finished, this, &launcher::hide);
  isShown = false;
}

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
launcher::fileread(QString path)
{
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
    result;
  QFile* desktopfile = new QFile(path);
  desktopfile->open(QIODevice::ReadOnly);
  QTextStream* in = new QTextStream(desktopfile);
  QString line = "";
  QString name = "";
  QLocale* locale = new QLocale();
  QString lang = locale->name().split("_")[0];
  bool firstname = true;
  bool firstexec = true;
  bool firsticon = true;
  while (!in->atEnd()) {
    line = in->readLine();
    QStringList keys = line.split("=");
    if (keys[0] == QString("Name[") + lang + QString("]") &&
        firstname == true) {
      firstname = false;
      std::get<0>(result) = keys[1];
      name = keys[1];
    }
    if (keys[0] == "Name" && firstname == true) {
      std::get<0>(result) = keys[1];
      name = keys[1];
    }
    if (keys[0] == "Comment") {
      std::get<1>(result) = keys[1];
    }
    if (keys[0] == QString("Comment[") + lang + QString("]")) {
      std::get<1>(result) = keys[1];
    }
    if (keys[0] == "Keywords") {
      std::get<2>(result) = keys[1].split(";");
    }
    if (keys[0] == QString("Keywords[") + lang + QString("]")) {
      std::get<2>(result) = keys[1].split(";");
    }
    if (keys[0] == "Exec" && firstexec == true) {
      firstexec = false;
      std::get<3>(result) = line.mid(5);
    }
    if (keys[0] == "Icon" && firsticon == true) {
      firsticon = false;
      std::get<4>(result) = QIcon::fromTheme(line.mid(5));
    }
    if (keys[0] == "Categories") {
      std::get<5>(result) = keys[1].split(";");
    }
    if (line.split(" ").length() > 1) {
      if (line.split(" ")[1] == "Action") {
        firstname = false;
      }
    }
    if (keys[0] == QString("Name[") + lang + QString("]") &&
        firstname == false) {
      std::get<6>(result).append(keys[1]);
    }
    if (keys[0] == "Name" && firstname == false && firstexec == false) {
      std::get<7>(result).append(keys[1]);
    }
    if (keys[0] == "Exec" && firstname == false && firstexec == false) {
      std::get<8>(result).append(keys[1]);
    }
    if (keys[0] == "NoDisplay") {
      if (keys[1] == "true") {
        std::get<9>(result) = true;
      } else {
        std::get<9>(result) = false;
      }
    }
  }
  /*qDebug() << QString("Comment[") + lang + QString("]");
  qDebug() << std::get<0>(result);
  qDebug() << std::get<1>(result);
  qDebug() << std::get<2>(result);
  qDebug() << std::get<3>(result);
  qDebug() << std::get<4>(result);
  qDebug() << std::get<5>(result);
  qDebug() << std::get<6>(result);
  qDebug() << std::get<7>(result);
  qDebug() << std::get<8>(result);
  qDebug() << std::get<9>(result);*/
  return result;
}

void
launcher::launchapp()
{
  QDir::setCurrent(QStringLiteral("~/"));
  QProcess* process = new QProcess(this);
  QString name = sender()->objectName();
  int i = name.toInt();
  qDebug() << "Command:　" << std::get<3>(objects[i]);
  QStringList splitted = std::get<3>(objects[i]).split("%")[0].split(" ");
  QString command = splitted[0];
  auto beg = splitted.begin() + 1;
  auto end = splitted.end();
  QStringList options{ beg, end };
  qDebug() << "Command: " << command << options;
  if (options.length() == 1 && options[0] == "") {
    options = QStringList();
  }
  process->startDetached(command, options);
  anihide(64);
}
