#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QWidget;
class RenderWindow;
class QListWidgetItem;
class QTreeWidgetItem;
class MeshComponent;
class EntityWidget;
class TransformComponent;
namespace Ui {
class MainWindow;
class TransForm;

//class TransformComponentWidget;
}
//class TransformComponentWidget : QWidget
//{
//public:
//    explicit TransformComponentWidget(QWidget *parent = nullptr); //: QWidget(parent), transUi(new Ui::TransformComponentWidget)
//    ~TransformComponentWidget();

//private:
//    Ui::TransformComponentWidget *transUi;
//};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
//    void on_actionCube_triggered();

//    void on_actionPlane_triggered();

//    void on_actionBall_triggered();

    void on_actionAdd_file_triggered();


    void on_treeWidgetWorldContent_itemClicked(QTreeWidgetItem *item, int column);

    void on_PlayButton_clicked();

    void on_StopButton_clicked();

    void on_addEntityButton_clicked();

    void on_actionAdd_From_File_triggered();

    void on_actionSave_File_triggered();

public slots:
   void on_actionAdd_Object_From_File_triggered();

public:
  std::vector<std::string> loadedObjects;
  bool isAddingCube = false;
  bool isAddingPlane = false;
  bool isAddingBall = false;

  bool isAddingObject = false;
  std::string objectToBeAdded;

  void loadStartContent();

  void setScrollArea(unsigned int EID);

  float mX = 0; //så man kan sette et koordinat til det man legger ut.
  float mY = 0;
  float mZ = 0;

  float scale = 0;

  int numCube = 0;
  int numBall = 0;
  int numPlane = 0;

  int numEntities = 0;

  void removeEntity(int EID);
  void updateValues();

  void addEntityInWorldContentList(int EID);
    void addNewItemInWorldContentList();
    int getSelectedEntity() const;

    bool isPlaying {false};
    bool isReseting{false};

private:
    void init();
    Ui::MainWindow *ui;
    QWidget *widget;

    std::vector<int> eIDs;
    std::vector<int> indexWorldContent;
    std::vector <int> entitiesInWindow;
    QWidget *mRenderWindowContainer;
    RenderWindow *mRenderWindow;

    int selectedEntity{-1};


   QTreeWidgetItem *worldObjects;



   //Component widgets:
   QWidget *transWidget = nullptr;
   QWidget *meshWidget = nullptr;
   QWidget *matWidget = nullptr;
   QWidget *collWidget = nullptr;
   EntityWidget *entityWidget = nullptr;

   TransformComponent *transTemp = nullptr;
   MeshComponent *meshTemp = nullptr;

   Ui::TransForm *transUi;

   QTreeWidgetItem *oldItem {nullptr};

   QString entityObjectName{"Empty"};

   void setPreMadeEntitiesComboBox();
   void addObjectToPreMadeEntitiesComboBox(QString name);


};

#endif // MAINWINDOW_H
