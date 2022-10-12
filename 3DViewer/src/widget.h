#ifndef WIDGET_H
#define WIDGET_H

#include <math.h>

#include <QDebug>
#include <QFile>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QTimer>

extern "C" {
#include "parse.h"
}

class Widget : public QOpenGLWidget {
  Q_OBJECT

 public:
  Widget(QWidget *parent = nullptr);
  ~Widget();
  struct data objectValue;
  float mashtab, x_coor, y_coor, z_coor;
  int vert_x_color = 0, vert_y_color = 255, vert_z_color = 0;
  int line_x_color = 255, line_y_color = 0, line_z_color = 0;
  QVector4D bkgrColor = QVector4D(0.0, 0.0, 0.0, 1.0);
  int pointSize = 2;
  int flag_start = 0;
  int flag_line_type = 0;
  int flag_point_type = 0;
  int flag_projection_type = 0;
  float lineSize = 1.1;
  float fov = 60.0 * M_PI / 180;
  float heapHeight = 3 / (2 * tan(fov / 2));

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

  void mousePressEvent(QMouseEvent *mouse) override;
  void mouseMoveEvent(QMouseEvent *mouse) override;

  void settingProjection();
  void drawPoint_cycle();
  void drawPoints();
  void drawLines();

 private:
  float xRot, yRot, zRot;
  QPoint mPos;
  QTimer timer;
};
#endif  // WIDGET_H
