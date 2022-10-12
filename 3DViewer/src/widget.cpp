#include "widget.h"

Widget::Widget(QWidget *parent) : QOpenGLWidget(parent) {
  this->mashtab = 0;
  this->xRot = 0;
  this->yRot = 0;
  this->zRot = 0;
  this->x_coor = 0;
  this->y_coor = 0;
  this->z_coor = 0;
  this->objectValue.count_of_facets = 0;
}

Widget::~Widget() {}

void Widget::initializeGL() {
  glEnable(GL_DEPTH_TEST);
}

void Widget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void Widget::paintGL() {
  glClearColor(bkgrColor.x(), bkgrColor.y(), bkgrColor.z(), bkgrColor.w());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (!flag_start) return;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  settingProjection();

  glTranslatef(0, 0, -heapHeight * 3);
  glRotatef(xRot, 1, 0, 0);
  glRotatef(yRot, 0, 1, 0);

  drawPoints();
  drawLines();
}

void Widget::mousePressEvent(QMouseEvent *mo) { mPos = mo->pos(); }

void Widget::mouseMoveEvent(QMouseEvent *mo) {
  xRot = 1 / M_PI * (mo->pos().y() - mPos.y());
  yRot = 1 / M_PI * (mo->pos().x() - mPos.x());
  update();
}

void Widget::settingProjection() {
  if (flag_projection_type == 0) {
    glFrustum(-3, 3, -3, 3, heapHeight, 100);
  } else if (flag_projection_type == 1) {
    glOrtho(-3, 3, -3, 3, -10, 100);
  }
}

void Widget::drawPoint_cycle() {
  glPointSize(pointSize);
  for (unsigned int i = 1; i < objectValue.count_of_vertexes; i++) {
    glBegin(GL_POINTS);
    glColor3ub(vert_x_color, vert_y_color, vert_z_color);
    glVertex3dv(objectValue.matrix_3d.matrix[i]);
    glEnd();
  }
}

void Widget::drawPoints() {
  if (flag_point_type == 0) {
    glDisable(GL_POINT_SMOOTH);
  } else if (flag_point_type == 1) {
    glEnable(GL_POINT_SMOOTH);
    drawPoint_cycle();
  } else if (flag_point_type == 2) {
    glDisable(GL_POINT_SMOOTH);
    drawPoint_cycle();
  }
}

void Widget::drawLines() {
  glLineWidth(lineSize);

  if (flag_line_type) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(5, 0x3333);
  } else {
    glDisable(GL_LINE_STIPPLE);
  }

  for (unsigned int i = 0; i < objectValue.count_of_facets; ++i) {
    for (int j = 0; j < objectValue.polygons[i].numbers_of_vertexes_in_facets;
         ++j) {
      for (int k = j; k < objectValue.polygons[i].numbers_of_vertexes_in_facets;
           ++k) {
        if (j != k) {
          glBegin(GL_LINES);
          glColor3ub(line_x_color, line_y_color, line_z_color);
          glVertex3dv(objectValue.matrix_3d
                          .matrix[objectValue.polygons[i].vertexes[j]]);
          glVertex3dv(objectValue.matrix_3d
                          .matrix[objectValue.polygons[i].vertexes[k]]);
          glEnd();
        }
      }
    }
  }
}
