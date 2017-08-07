#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnExit_clicked()
{
    confirmExit();
}

void MainWindow::confirmExit()
{
    QMessageBox msgBox(
                QMessageBox::Question,
                "Salir", "¿Cerrar la aplicación?",
                QMessageBox::Yes | QMessageBox::No);

    msgBox.setButtonText(QMessageBox::Yes, trUtf8("Si"));
    msgBox.setButtonText(QMessageBox::No, trUtf8("Cancelar"));

    if (msgBox.exec() == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void MainWindow::on_btnMult_clicked()
{

    float num1 = ui->lnOper_1->text().toFloat();
    float num2 = ui->lnOper_2->text().toFloat();

    FloatingPoint oper1(num1);
    FloatingPoint oper2(num2);

    ui->lblHexOp_1->setText(QString::fromStdString(oper1.getHexValue()));
    ui->lblHexOp_2->setText(QString::fromStdString(oper2.getHexValue()));

    FloatingPoint res = alu->productFP(oper1, oper2);

    ui->lblHexRes->setText((QString::fromStdString((res.getHexValue()))));

    ui->lnResult->setText((QString::number((res.getDecValue()))));

    ui->lblOperation->setText("Producto");
}

void MainWindow::on_btnSum_clicked()
{
    float num1 = ui->lnOper_1->text().toFloat();
    float num2 = ui->lnOper_2->text().toFloat();

    FloatingPoint oper1(num1);
    FloatingPoint oper2(num2);

    ui->lblHexOp_1->setText(QString::fromStdString(oper1.getHexValue()));
    ui->lblHexOp_2->setText(QString::fromStdString(oper2.getHexValue()));

    FloatingPoint res = alu->additionFP(oper1, oper2);

    ui->lblHexRes->setText((QString::fromStdString((res.getHexValue()))));

    ui->lnResult->setText((QString::number((res.getDecValue()))));

    ui->lblOperation->setText("Suma");
}

void MainWindow::on_btnDiv_clicked()
{
    float num1 = ui->lnOper_1->text().toFloat();
    float num2 = ui->lnOper_2->text().toFloat();

    FloatingPoint oper1(num1);
    FloatingPoint oper2(num2);

    ui->lblHexOp_1->setText(QString::fromStdString(oper1.getHexValue()));
    ui->lblHexOp_2->setText(QString::fromStdString(oper2.getHexValue()));

    FloatingPoint res = alu->divisionFP(oper1, oper2);

    ui->lblHexRes->setText((QString::fromStdString((res.getHexValue()))));

    ui->lnResult->setText((QString::number((res.getDecValue()))));

    ui->lblOperation->setText("División");
}
