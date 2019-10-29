#include "transformwidget.h"
#include "ui_transformComponentWidget.h"

TransformWidget::TransformWidget(MainWindow* mainWindow, QWidget *parent)
    : QWidget(parent),mMainWindow(mainWindow), ui(new Ui::TransForm)
{
   ui->setupUi(this);
}

TransformWidget::~TransformWidget()
{

}
