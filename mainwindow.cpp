
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget_tileset->setHeaderHidden(true);
    ui->treeWidget_layer->setHeaderHidden(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}
