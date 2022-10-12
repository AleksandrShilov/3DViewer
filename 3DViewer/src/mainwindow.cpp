#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "widget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setFixedHeight(671);
  this->setFixedWidth(1015);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_rendering_clicked() {
  if (ui->openGLWidget->flag_start) {
    char *filepath = filename.toLocal8Bit().data();
    struct data objectValue2;
    setlocale(LC_NUMERIC, "C");
    read_file_for_counting(filepath, &objectValue2);
    read_file_for_writing(filepath, &objectValue2);
    QString scoreEdge = QString::number(objectValue2.count_of_facets);
    QString scoreVertexes = QString::number(objectValue2.count_of_vertexes - 1);

    ui->label_edge->setText("Рёбер: " + scoreEdge);
    ui->label_vertex->setText("Вершин: " + scoreVertexes);
    ui->label_name_file->setText("Файл: " + QFileInfo(filename).fileName());
    ui->openGLWidget->objectValue = objectValue2;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_pushButton_open_f_clicked() {
  filename = QFileDialog::getOpenFileName(this, "Выберите файл",
                                          QDir::homePath(), "*.obj");
  ui->openGLWidget->flag_start = 1;
}

void MainWindow::on_doubleSpinBox_x_valueChanged(double arg1) {
  if (ui->openGLWidget->flag_start) {
    if (arg1 > this->ui->openGLWidget->x_coor) {
      turn_axis_x(this->ui->openGLWidget->objectValue.matrix_3d, 0.1);
    } else {
      turn_axis_x(this->ui->openGLWidget->objectValue.matrix_3d, -0.1);
    }
    this->ui->openGLWidget->x_coor = arg1;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_doubleSpinBox_y_valueChanged(double arg1) {
  if (ui->openGLWidget->flag_start) {
    if (arg1 > this->ui->openGLWidget->y_coor) {
      turn_axis_y(this->ui->openGLWidget->objectValue.matrix_3d, 0.1);
    } else {
      turn_axis_y(this->ui->openGLWidget->objectValue.matrix_3d, -0.1);
    }
    this->ui->openGLWidget->y_coor = arg1;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_doubleSpinBox_z_valueChanged(double arg1) {
  if (ui->openGLWidget->flag_start) {
    if (arg1 > this->ui->openGLWidget->z_coor) {
      turn_axis_z(this->ui->openGLWidget->objectValue.matrix_3d, 0.1);
    } else {
      turn_axis_z(this->ui->openGLWidget->objectValue.matrix_3d, -0.1);
    }
    this->ui->openGLWidget->z_coor = arg1;
    ui->openGLWidget->update();
  }
}

void MainWindow::on_doubleSpinBox_mashtab_valueChanged(double arg1) {
  if (arg1 > this->ui->openGLWidget->mashtab) {
    change_scale(this->ui->openGLWidget->objectValue.matrix_3d, 1.5);
  } else {
    change_scale(this->ui->openGLWidget->objectValue.matrix_3d, 0.75);
  }
  this->ui->openGLWidget->mashtab = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_vertex_size_valueChanged(double arg1) {
  ui->openGLWidget->pointSize = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_edge_size_valueChanged(double arg1) {
  ui->openGLWidget->lineSize = arg1;
  ui->openGLWidget->update();
}

void MainWindow::on_comboBox_edge_type_activated(const QString &arg1) {
  if (arg1 == "Пунктирная") {
    ui->openGLWidget->flag_line_type = 1;
  } else if (arg1 == "Сплошная") {
    ui->openGLWidget->flag_line_type = 0;
  }
  ui->openGLWidget->update();
}

void MainWindow::on_comboBox_vertex_type_activated(const QString &arg1) {
  if (arg1 == "Отсутствует") {
    ui->openGLWidget->flag_point_type = 0;
  } else if (arg1 == "Круг") {
    ui->openGLWidget->flag_point_type = 1;
  } else if (arg1 == "Квадрат") {
    ui->openGLWidget->flag_point_type = 2;
  }
  ui->openGLWidget->update();
}

void MainWindow::on_comboBox_proection_activated(const QString &arg1) {
  if (arg1 == "Параллельная") {
    ui->openGLWidget->flag_projection_type = 0;
  } else if (arg1 == "Центральная") {
    ui->openGLWidget->flag_projection_type = 1;
  }
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_save_clicked() {
  QString format;
  QString path = get_path();
  if (ui->radioButton_jpeg->isChecked()) {
    format = "jpeg";
    this->ui->openGLWidget->grab().save(path + "/" + "screencast" +
                                        QTime::currentTime().toString() + "." +
                                        format);
  } else if (ui->radioButton_bmp->isChecked()) {
    format = "bmp";
    this->ui->openGLWidget->grab().save(path + "/" + "screencast" +
                                        QTime::currentTime().toString() + "." +
                                        format);
  }
}

QString MainWindow::get_path() {
  return QFileDialog::getExistingDirectory(
      this->ui->openGLWidget, tr("Choose files"), QDir::homePath(),
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

void MainWindow::on_pushButton_gif_clicked() {
  path_gif = new QString(get_path());
  gif = new QGifImage();
  timerScore = 0;
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(record_gif()));
  qDebug() << "SSS";
  timer->start(100);
}

void MainWindow::record_gif() {
  ++timerScore;
  qDebug() << timerScore;
  gif->addFrame(ui->openGLWidget->grabFramebuffer(), 100);

  if (timerScore == 50) {
    timer->stop();
    gif->save(*path_gif + "/" + QDate::currentDate().toString() + " " +
              QTime::currentTime().toString() + ".gif");
    delete gif;
  }
}

void MainWindow::on_pushButton_save_settings_clicked() {
  QSettings settings("settings.ini", QSettings::IniFormat);

  settings.beginGroup("projection");
  settings.setValue("projectionType", ui->openGLWidget->flag_projection_type);
  settings.endGroup();

  settings.beginGroup("backgroundColor");
  settings.setValue("backColorR", ui->openGLWidget->bkgrColor.x());
  settings.setValue("backColorG", ui->openGLWidget->bkgrColor.y());
  settings.setValue("backColorB", ui->openGLWidget->bkgrColor.z());
  settings.setValue("backColorA", ui->openGLWidget->bkgrColor.w());
  settings.endGroup();

  settings.beginGroup("vertexesColor");
  settings.setValue("vertexesColorX", ui->openGLWidget->vert_x_color);
  settings.setValue("vertexesColorY", ui->openGLWidget->vert_y_color);
  settings.setValue("vertexesColorZ", ui->openGLWidget->vert_z_color);
  settings.endGroup();

  settings.beginGroup("vertexesType");
  settings.setValue("pointType", ui->openGLWidget->flag_point_type);
  settings.endGroup();

  settings.beginGroup("vertexesSize");
  settings.setValue("pointSize", ui->openGLWidget->pointSize);
  settings.endGroup();

  settings.beginGroup("lineColor");
  settings.setValue("lineColorX", ui->openGLWidget->line_x_color);
  settings.setValue("lineColorY", ui->openGLWidget->line_y_color);
  settings.setValue("lineColorZ", ui->openGLWidget->line_z_color);
  settings.endGroup();

  settings.beginGroup("lineType");
  settings.setValue("lineTypeFlag", ui->openGLWidget->flag_line_type);
  settings.endGroup();

  settings.beginGroup("lineSize");
  settings.setValue("lineSizes", ui->openGLWidget->lineSize);
  settings.endGroup();

  settings.beginGroup("mashtab");
  settings.setValue("valueMashtab", ui->openGLWidget->mashtab);
  settings.endGroup();

  settings.beginGroup("path");
  char *filepath = filename.toLocal8Bit().data();
  settings.setValue("pathFile", filepath);
  settings.endGroup();
}

void MainWindow::on_pushButton_download_settings_clicked() {
  QSettings settings("settings.ini", QSettings::IniFormat);

  settings.beginGroup("projection");
  int typeProjection = settings.value("projectionType").toInt();
  settings.endGroup();
  ui->openGLWidget->flag_projection_type = typeProjection;

  settings.beginGroup("backgroundColor");
  double color_r = settings.value("backColorR").toDouble();
  double color_g = settings.value("backColorG").toDouble();
  double color_b = settings.value("backColorB").toDouble();
  double color_a = settings.value("backColorA").toDouble();
  settings.endGroup();
  QVector4D colorBack = QVector4D(color_r, color_g, color_b, color_a);
  ui->openGLWidget->bkgrColor = colorBack;

  settings.beginGroup("vertexesColor");
  float color_x = settings.value("vertexesColorX").toFloat();
  float color_y = settings.value("vertexesColorY").toFloat();
  float color_z = settings.value("vertexesColorZ").toFloat();
  settings.endGroup();
  ui->openGLWidget->vert_x_color = color_x;
  ui->openGLWidget->vert_y_color = color_y;
  ui->openGLWidget->vert_z_color = color_z;

  settings.beginGroup("vertexesType");
  int type_point = settings.value("pointType").toInt();
  settings.endGroup();
  ui->openGLWidget->flag_point_type = type_point;

  settings.beginGroup("vertexesSize");
  int size_point = settings.value("pointSize").toInt();
  settings.endGroup();
  ui->openGLWidget->pointSize = size_point;

  settings.beginGroup("lineColor");
  float line_colorX = settings.value("lineColorX").toFloat();
  float line_colorY = settings.value("lineColorY").toFloat();
  float line_colorZ = settings.value("lineColorZ").toFloat();
  settings.endGroup();
  ui->openGLWidget->line_x_color = line_colorX;
  ui->openGLWidget->line_y_color = line_colorY;
  ui->openGLWidget->line_z_color = line_colorZ;

  settings.beginGroup("lineType");
  int line_type = settings.value("lineTypeFlag").toInt();
  settings.endGroup();
  ui->openGLWidget->flag_line_type = line_type;

  settings.beginGroup("lineSize");
  float line_size = settings.value("lineSizes").toFloat();
  settings.endGroup();
  ui->openGLWidget->lineSize = line_size;

  settings.beginGroup("mashtab");
  float mashtab_value = settings.value("valueMashtab").toFloat();
  settings.endGroup();
  ui->openGLWidget->mashtab = mashtab_value;

  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_clear_clicked() {
  ui->openGLWidget->flag_start = 0;
  ui->comboBox_proection->setCurrentIndex(0);
  ui->comboBox_edge_type->setCurrentIndex(0);
  ui->comboBox_vertex_type->setCurrentIndex(0);
  ui->doubleSpinBox_edge_size->setValue(0.0);
  ui->doubleSpinBox_vertex_size->setValue(0.0);
  ui->doubleSpinBox_mashtab->setValue(0.0);
  ui->doubleSpinBox_x->setValue(0.0);
  ui->doubleSpinBox_y->setValue(0.0);
  ui->doubleSpinBox_z->setValue(0.0);
  ui->doubleSpinBox_move_x->setValue(0.0);
  ui->doubleSpinBox_move_y->setValue(0.0);
  ui->doubleSpinBox_move_z->setValue(0.0);
  ui->radioButton_jpeg->setChecked(true);
  ui->label_edge->setText("Рёбер:");
  ui->label_vertex->setText("Вершин:");
  ui->label_name_file->setText("Файл:");
  ui->openGLWidget->vert_x_color = 0;
  ui->openGLWidget->vert_y_color = 255;
  ui->openGLWidget->vert_z_color = 0;
  ui->openGLWidget->line_x_color = 255;
  ui->openGLWidget->line_y_color = 0;
  ui->openGLWidget->line_z_color = 0;
  ui->openGLWidget->bkgrColor = QVector4D(0.0, 0.0, 0.0, 0.0);
  ui->openGLWidget->pointSize = 2;
  ui->openGLWidget->lineSize = 1.1;
  ui->openGLWidget->flag_start = 0;
  ui->openGLWidget->flag_line_type = 0;
  ui->openGLWidget->flag_point_type = 0;
  ui->openGLWidget->flag_projection_type = 0;
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_color_edge_clicked() {
  QColor colorLines = QColorDialog::getColor();
  ui->openGLWidget->line_x_color = colorLines.red();
  ui->openGLWidget->line_y_color = colorLines.green();
  ui->openGLWidget->line_z_color = colorLines.blue();
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_background_color_clicked() {
  QColor colorLines = QColorDialog::getColor();
  ui->openGLWidget->bkgrColor =
      QVector4D(colorLines.redF(), colorLines.greenF(), colorLines.blueF(),
                colorLines.alphaF());
  ui->openGLWidget->update();
}

void MainWindow::on_pushButton_vertex_color_clicked() {
  QColor colorLines = QColorDialog::getColor();
  ui->openGLWidget->vert_x_color = colorLines.red();
  ui->openGLWidget->vert_y_color = colorLines.green();
  ui->openGLWidget->vert_z_color = colorLines.blue();
  ui->openGLWidget->update();
}

void MainWindow::on_doubleSpinBox_move_x_valueChanged(double arg1) {
  if (ui->openGLWidget->flag_start) {
    if (arg1 > this->ui->openGLWidget->x_coor) {
      move_object(this->ui->openGLWidget->objectValue.matrix_3d, 0.1, 0);
      this->ui->openGLWidget->x_coor = arg1;
    } else {
      move_object(this->ui->openGLWidget->objectValue.matrix_3d, -0.1, 0);
      this->ui->openGLWidget->x_coor = arg1;
    }
    ui->openGLWidget->update();
  }
}

void MainWindow::on_doubleSpinBox_move_y_valueChanged(double arg1) {
  if (ui->openGLWidget->flag_start) {
    if (arg1 > this->ui->openGLWidget->y_coor) {
      move_object(this->ui->openGLWidget->objectValue.matrix_3d, 0.1, 1);
      this->ui->openGLWidget->y_coor = arg1;
    } else {
      move_object(this->ui->openGLWidget->objectValue.matrix_3d, -0.1, 1);
      this->ui->openGLWidget->y_coor = arg1;
    }
    ui->openGLWidget->update();
  }
}

void MainWindow::on_doubleSpinBox_move_z_valueChanged(double arg1) {
  if (ui->openGLWidget->flag_start) {
    if (arg1 > this->ui->openGLWidget->z_coor) {
      move_object(this->ui->openGLWidget->objectValue.matrix_3d, 0.1, 2);
      this->ui->openGLWidget->z_coor = arg1;
    } else {
      move_object(this->ui->openGLWidget->objectValue.matrix_3d, -0.1, 2);
      this->ui->openGLWidget->z_coor = arg1;
    }
    ui->openGLWidget->update();
  }
}
