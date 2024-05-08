#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "calc.h"
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for(int i = 0; i < 4; ++i){
        for(int j = 0; j < 5; ++j){
            auto button = (QPushButton*)ui->buttons->itemAtPosition(i, j)->widget();
            connect(button, &QPushButton::released, this, &MainWindow::handleButton);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton(){
    QPushButton* button = (QPushButton*)sender();
    QString s = button->text();
    QString t = ui->input_line->text();
    if(s == "⌫"){
        t.chop(1);
        ui->input_line->setText(t);
        ui->output_line->setText("");
    }else if(s == "C"){
        ui->input_line->setText("");
        ui->output_line->setText("");
    }else if(s == "="){
        auto [flag, res] = calc(t.toStdString());
        if(flag){
            ui->input_line->setText("");
        }
        ui->output_line->setText(QString::fromStdString(res));
    }else{
        if(s == "×"){
            t+="*";
        }else if(s=="÷"){
            t+="/";
        }else{
            t+=s;
        }
        ui->input_line->setText(t);
        ui->output_line->setText("");
    }
}
