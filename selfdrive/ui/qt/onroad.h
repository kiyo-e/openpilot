#pragma once

#include <QStackedLayout>
#include <QWidget>
#include <QPushButton>

#include "common/util.h"
#include "selfdrive/ui/qt/widgets/cameraview.h"
#include "selfdrive/ui/ui.h"


// ***** onroad widgets *****
class OnroadAlerts : public QWidget {
  Q_OBJECT

public:
  OnroadAlerts(QWidget *parent = 0) : QWidget(parent) {};
  void updateAlert(const Alert &a, const QColor &color);

protected:
  void paintEvent(QPaintEvent*) override;

private:
  QColor bg;
  Alert alert = {};
};

class ButtonsWindow : public QWidget {
  Q_OBJECT

public:
  ButtonsWindow(QWidget* parent = 0);

private:
  QPushButton *lockOnButton;
  QPushButton *accelCtrlButton;
  QPushButton *decelCtrlButton;
  QPushButton *accelEngagedButton;
  QPushButton *handleCtrlButton;
  QPushButton *startAccelPowerUpButton;

  // int dfStatus = -1;  // always initialize style sheet and send msg
  // const QStringList dfButtonColors = {"#044389", "#24a8bc", "#fcff4b", "#37b868"};

  // int lsStatus = -1;  // always initialize style sheet and send msg
  // const QStringList lsButtonColors = {"#ff3737", "#37b868", "#044389"};

  const QStringList mButtonColors = {"#909090", "#37b868"};

  // model long button
  bool mLockOnButton = true;  // triggers initialization
  bool mAccelCtrlButton = true;  // triggers initialization
  bool mDecelCtrlButton = true;  // triggers initialization
  int mAccelEngagedButton = 0;  // triggers initialization
  bool mHandleCtrlButton = true;  // triggers initialization
  bool mStartAccelPowerUpButton = false;  // triggers initialization

public slots:
  void updateState(const UIState &s);
};

// container window for the NVG UI
class NvgWindow : public CameraViewWidget {
  Q_OBJECT
  Q_PROPERTY(QString speed MEMBER speed);
  Q_PROPERTY(QString speedUnit MEMBER speedUnit);
  Q_PROPERTY(QString maxSpeed MEMBER maxSpeed);
  Q_PROPERTY(bool is_cruise_set MEMBER is_cruise_set);
  Q_PROPERTY(bool engageable MEMBER engageable);
  Q_PROPERTY(bool dmActive MEMBER dmActive);
  Q_PROPERTY(bool hideDM MEMBER hideDM);
  Q_PROPERTY(int status MEMBER status);

public:
  explicit NvgWindow(VisionStreamType type, QWidget* parent = 0);
  void updateState(const UIState &s);

private:
  void drawIcon(QPainter &p, int x, int y, QPixmap &img, QBrush bg, float opacity , float ang);
  void drawText(QPainter &p, int x, int y, const QString &text, int alpha = 255);
  void drawText(QPainter &p, int x, int y, const QString &text, const QColor &col);
  void paintEvent(QPaintEvent *event) override;

  QPixmap engage_img;
  QPixmap dm_img;
  const int radius = 192;
  const int img_size = (radius / 2) * 1.5;
  QString speed;
  QString speedUnit;
  QString maxSpeed;
  bool is_cruise_set = false;
  bool engageable = false;
  bool dmActive = false;
  bool hideDM = false;
  int status = STATUS_DISENGAGED;

//signals:
//  void valueChanged();

  int prev_width = -1;  // initializes ButtonsWindow width and holds prev width to update it

protected:
  void paintGL() override;
  void initializeGL() override;
  void showEvent(QShowEvent *event) override;
  void updateFrameMat(int w, int h) override;
  void drawLaneLines(QPainter &painter, const UIState *s);
  void drawLead(QPainter &painter, const cereal::ModelDataV2::LeadDataV3::Reader &lead_data, const QPointF &vd , int num , size_t leads_num);
  void drawHud(QPainter &p);
  void drawLockon(QPainter &painter, const cereal::ModelDataV2::LeadDataV3::Reader &lead_data, const QPointF &vd , int num , size_t leads_num , const cereal::ModelDataV2::LeadDataV3::Reader &lead0 , const cereal::ModelDataV2::LeadDataV3::Reader &lead1);
  inline QColor redColor(int alpha = 255) { return QColor(201, 34, 49, alpha); }
  inline QColor whiteColor(int alpha = 255) { return QColor(255, 255, 255, alpha); }

  double prev_draw_t = 0;
  FirstOrderFilter fps_filter;
  void knightScanner(QPainter &p);

signals:
  void resizeSignal(int w);
};

// container for all onroad widgets
class OnroadWindow : public QWidget {
  Q_OBJECT

public:
  OnroadWindow(QWidget* parent = 0);
  bool isMapVisible() const { return map && map->isVisible(); }

private:
  void paintEvent(QPaintEvent *event);
  void mousePressEvent(QMouseEvent* e) override;
  OnroadAlerts *alerts;
  NvgWindow *nvg;
  ButtonsWindow *buttons;
  QColor bg = bg_colors[STATUS_DISENGAGED];
  QWidget *map = nullptr;
  QHBoxLayout* split;

private slots:
  void offroadTransition(bool offroad);
  void updateState(const UIState &s);
};
