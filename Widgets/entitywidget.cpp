#include "entitywidget.h"
#include "ui_EntityWidget.h"
#include "mainwindow.h"
#include <iostream>

EntityWidget::EntityWidget(MainWindow* mainWindow, QWidget *parent, int EID) : QWidget(parent),mMainWindow(mainWindow), ui(new Ui::entityForm)
{

    ui->setupUi(this);
    eID = EID;
    setComponentBox();
}

EntityWidget::~EntityWidget()
{

}
void EntityWidget::on_removeButton_clicked()
{
    int temp =  mMainWindow->getSelectedEntity();
    if(temp != -1)
    {
        mMainWindow->removeEntity(temp);

        std::cout <<" Remove Entity: "<< temp << std::endl;
    }
}

void EntityWidget::addCurrentChild(QString name)
{
    ui->ChildBox->addItem(name);
}

void EntityWidget::setLabel(QString name)
{
    ui->label->clear();
    ui->label->setText(name); //Fungerer ikke fra unsigned int?
}

void EntityWidget::setComponentBox()
{

    ui->ComponentBox->addItem("Sound Component");
    ui->ComponentBox->addItem("Material Component");
    ui->ComponentBox->addItem("Mesh Component");
    ui->ComponentBox->addItem("Collision Component");
    ui->ComponentBox->addItem("Transform Component");

}
