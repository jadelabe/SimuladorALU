#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "floatingpoint.h"
#include "alu.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnExit_clicked();
    void confirmExit();

    void on_btnMult_clicked();

    void on_btnSum_clicked();

    void on_btnDiv_clicked();

private:
    Ui::MainWindow *ui;
    Alu *alu;
};

#endif // MAINWINDOW_H
