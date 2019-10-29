#ifndef ENTITYWIDGET_H
#define ENTITYWIDGET_H
#include <QWidget>
class MainWindow;
namespace Ui{
class entityForm;
}
class EntityWidget : public QWidget
{
    Q_OBJECT
public:
    EntityWidget(MainWindow *mainWindow, QWidget *parent, int EID);
    ~EntityWidget();
    void addCurrentChild(QString name);
    void setLabel(QString name);

    void setComponentBox();


private slots:
    void on_removeButton_clicked();


private:
    MainWindow * mMainWindow;
    Ui::entityForm* ui;
    int eID{-1};
 std::vector <std::string> componentBoxContent;

};

#endif // ENTITYWIDGET_H
