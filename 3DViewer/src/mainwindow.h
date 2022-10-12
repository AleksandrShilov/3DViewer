#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QSettings>
#include <QToolTip>

#include "QtCore/qdatetime.h"
#include "qgifimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

extern "C" {
#include "parse.h"
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QString filename;

 protected:
  QString get_path();

 private slots:
  void on_pushButton_rendering_clicked();

  void on_pushButton_open_f_clicked();

  void on_doubleSpinBox_x_valueChanged(double arg1);

  void on_doubleSpinBox_y_valueChanged(double arg1);

  void on_doubleSpinBox_z_valueChanged(double arg1);

  void on_doubleSpinBox_mashtab_valueChanged(double arg1);

  void on_doubleSpinBox_vertex_size_valueChanged(double arg1);

  void on_doubleSpinBox_edge_size_valueChanged(double arg1);

  void on_comboBox_edge_type_activated(const QString &arg1);

  void on_comboBox_vertex_type_activated(const QString &arg1);

  void on_comboBox_proection_activated(const QString &arg1);

  void on_pushButton_save_clicked();

  void on_pushButton_gif_clicked();

  void record_gif();

  void on_pushButton_save_settings_clicked();

  void on_pushButton_download_settings_clicked();

  void on_pushButton_clear_clicked();

  void on_pushButton_color_edge_clicked();

  void on_pushButton_background_color_clicked();

  void on_pushButton_vertex_color_clicked();

  void on_doubleSpinBox_move_x_valueChanged(double arg1);

  void on_doubleSpinBox_move_y_valueChanged(double arg1);

  void on_doubleSpinBox_move_z_valueChanged(double arg1);

 private:
  Ui::MainWindow *ui;

  int timerScore = 0;
  QGifImage *gif;
  QTimer *timer;
  QString *path_gif;
};

#endif  // MAINWINDOW_H
