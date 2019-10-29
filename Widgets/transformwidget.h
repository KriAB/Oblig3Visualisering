#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H
#include <QWidget>
class MainWindow;
namespace Ui{
    class TransForm;
}
class TransformWidget : public QWidget
{
    Q_OBJECT
public:

    TransformWidget(MainWindow *mainWindow, QWidget *parent);
   ~TransformWidget();
    void updateData();
    void remove();

signals:
    void widgetRemoved(TransformWidget *widget);

private:
    MainWindow * mMainWindow;
    Ui::TransForm* ui;
};

#endif // TRANSFORMWIDGET_H
