#include "innpch.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_transformComponentWidget.h"
#include "ui_meshComponentWidget.h"
#include "ui_EntityWidget.h"
#include <QSurfaceFormat>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QAction>
#include "renderwindow.h"
#include "visualobject.h"
#include <QLabel>
#include <QWidget>
#include "meshcomponent.h"
#include "transformcomponent.h"
#include "Entity.h"
#include "entitywidget.h"
#include "systemmanager.h"
#include "componentsystem.h"
#include "input.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    init();
    setPreMadeEntitiesComboBox();

    transUi = new Ui::TransForm;



}

MainWindow::~MainWindow()
{
    delete mRenderWindow;
    delete worldObjects;
    worldObjects = nullptr;


    //delete widget;
    delete transWidget;
    transWidget = nullptr;
    delete meshWidget;
    meshWidget = nullptr;
    delete matWidget;
    matWidget = nullptr;
    delete collWidget;
    collWidget = nullptr;

    delete entityWidget;
    entityWidget = nullptr;

    delete transUi;
    delete ui;
}

void MainWindow::init()
{
    //This will contain the setup of the OpenGL surface we will render into
    QSurfaceFormat format;

    //OpenGL v 4.1 - (Ole Flatens Mac does not support higher than this...)
    //you can try other versions, but then have to update RenderWindow and Shader
    //to inherit from other than QOpenGLFunctions_4_1_Core
    format.setVersion(4, 1);
    //Using the main profile for OpenGL - no legacy code permitted
    format.setProfile(QSurfaceFormat::CoreProfile);
    //A QSurface can be other types that OpenGL
    format.setRenderableType(QSurfaceFormat::OpenGL);

    //This should activate OpenGL debug Context used in RenderWindow::startOpenGLDebugger().
    //This line (and the startOpenGLDebugger() and checkForGLerrors() in RenderWindow class)
    //can be deleted, but it is nice to have OpenGL debug info!
    format.setOption(QSurfaceFormat::DebugContext);

    // The renderer will need a depth buffer - (not requiered to set in glfw-tutorials)
    format.setDepthBufferSize(24);

    //Set the number of samples used for multisampling
    format.setSamples(8);

    //Turn off VSync. If this is set to 1, VSync is on - default behaviour
    format.setSwapInterval(0);

    //Just prints out what OpenGL format we will get
    // - this can be deleted
    qDebug() << "Requesting surface format: " << format;

    //We have a format for the OpenGL window, so let's make it:
    mRenderWindow = new RenderWindow(format, this);

    //Check if renderwindow did initialize, else prints error and quit
    if (!mRenderWindow->context()) {
        qDebug() << "Failed to create context. Can not continue. Quits application!";
        delete mRenderWindow;
        return;
    }

    //The OpenGL RenderWindow got made, so continuing the setup:
    //We put the RenderWindow inside a QWidget so we can put in into a
    //layout that is made in the .ui-file
    mRenderWindowContainer = QWidget::createWindowContainer(mRenderWindow);
    //OpenGLLayout is made in the .ui-file!
    mRenderWindowContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->OpenGLLayout->addWidget(mRenderWindowContainer);


    //sets the keyboard input focus to the RenderWindow when program starts
    // - can be deleted, but then you have to click inside the renderwindow to get the focus
    mRenderWindowContainer->setFocus();

    //Set size of program in % of available screen
    resize(QDesktopWidget().availableGeometry(this).size() * 0.7);

}

void MainWindow::setPreMadeEntitiesComboBox()
{
    ui->PreMadeEntitiesComboBox->setDuplicatesEnabled(false);
    ui->PreMadeEntitiesComboBox->addItem("Empty");
    ui->PreMadeEntitiesComboBox->addItem("Skybox");
    ui->PreMadeEntitiesComboBox->addItem("Ball");
    ui->PreMadeEntitiesComboBox->addItem("Cube");

    ui->CollisionTypeComboBox->addItem("No Collision");
    ui->CollisionTypeComboBox->addItem("BS");
    ui->CollisionTypeComboBox->addItem("AABB");
    ui->CollisionTypeComboBox->addItem("OBB");

    ui->MaterialTypeComboBox->addItem("Default");


}

void MainWindow::addObjectToPreMadeEntitiesComboBox(QString name)
{

    ui->PreMadeEntitiesComboBox->addItem(name);
}

int MainWindow::getSelectedEntity() const
{
    return selectedEntity;
}

//Example of a slot called from the button on the top of the program.
void MainWindow::on_pushButton_clicked()
{
    mRenderWindow->toggleWireframe();
}

//void MainWindow::on_actionCube_triggered()
//{
////    ui->worldContentList->addItem("Cube" + QString::number(numCube));
////    isAddingCube = true;
////    objectsInWindow.push_back("Cube" + std::to_string(numCube));
//    objectToBeAdded = "Cube";
//    isAddingObject = true;
//    numCube++;
//    //Må putte inn funksjon som gjør at man kan legge til objekt i rendertime

//    addNewItemInWorldContentList();
//}

//void MainWindow::on_actionPlane_triggered()
//{
////    ui->worldContentList->addItem("Plane"+ QString::number(numPlane));
////    objectsInWindow.push_back("Plane" + std::to_string(numPlane));
////    isAddingPlane = true;
//    objectToBeAdded = "Plane";
//    isAddingObject = true;
//    numPlane++;

//    addNewItemInWorldContentList();
//}

//void MainWindow::on_actionBall_triggered()
//{
////    ui->worldContentList->addItem("Ball" + QString::number(numBall));
////    objectsInWindow.push_back("Ball" + std::to_string(numBall));
//    // isAddingBall = true;

//    objectToBeAdded = "Ball";
//    isAddingObject = true;
//    numBall++;

//    addNewItemInWorldContentList();
//}
//Denne er load file


void MainWindow::on_actionAdd_Object_From_File_triggered()
{
    //Dette fungerer bare fra Meshes/ Kan funke på txt også
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open files"), "",
                                                    tr("Finde a file (*.obj);;All Files (*)"));
    std::string temp = fileName.toStdString();
    std::string shortenFileName;
    const int idx =  temp.std::string::find_last_of("/");
    if(std::string::npos !=idx)
    {
        shortenFileName = temp.substr(idx+1);
    }
    //    loadedObjects.push_back(shortenFileName);
    //    ui->worldContentList->addItem(QString::fromStdString(shortenFileName));
    //    objectsInWindow.push_back(shortenFileName);

    objectToBeAdded = shortenFileName;
    isAddingObject = true;


    addNewItemInWorldContentList();
    addObjectToPreMadeEntitiesComboBox(QString::fromStdString(shortenFileName));
    //Må adde objektet på et vis her
}

void MainWindow::loadStartContent()
{
    //    objectsInWindow = mRenderWindow->returnCurrentObjectNames();
    //    for(auto obj : objectsInWindow)
    //        ui->worldContentList->addItem(QString::fromStdString(obj));


    // ui->treeWidgetWorldContent->addTopLevelItem(objects);
}

void MainWindow::setScrollArea(unsigned int EID)
{
    //først sjekke om det er noe der fra før, så slette og erstatte.
    //For at objektet skal være valgt i lista også, hvis man velger objektet i viewport
    if(oldItem != nullptr)
    {
        oldItem->setSelected(false);
        oldItem = nullptr;
    }
    QTreeWidgetItem *item = ui->treeWidgetWorldContent->topLevelItem(EID);
    item->setSelected(true);
    oldItem  = item;

    //Cleaning up from before
    delete entityWidget;
    entityWidget = nullptr;
    delete transWidget;
    transWidget = nullptr;
    delete meshWidget;
    meshWidget = nullptr;
    delete matWidget;
    matWidget = nullptr;
    delete collWidget;
    collWidget = nullptr;


    //std::cout  << " Eid: " << EID <<std::endl;

    //OLD
    //    transTemp = mRenderWindow->getTransComponent(EID);
    //   meshTemp = mRenderWindow->getMeshComponent(EID);

    transTemp = mRenderWindow->getSystemManager()->componentSystem()->getTransCompWithEId(EID);
    meshTemp = mRenderWindow->getSystemManager()->componentSystem()->getMeshCompWithEId(EID);

    //Remove entity vil ødelegge EID greiene. Med mindre man lagrer dem i en array og oppdaterer den når de fjernes.

    //adde en widget som kun sier hvilke Entity det er
    int num = static_cast<int> (EID);
    std::string entityText = "Entity " + std::to_string(num);
    if(transTemp != nullptr)
    {
        //        std::cout << "Trans: " << transTemp->position() << std::endl;
        //        entityWidget = new QWidget();
        //        Ui::entityForm entityForm;
        //        entityForm.setupUi(entityWidget);
        //        entityForm.label->clear();
        //        entityForm.label->setText(QString::fromStdString(entityText)); //Fungerer ikke fra unsigned int?

        selectedEntity = static_cast<int>(EID);

        //  std::cout << "Trans: " << transTemp->position() << std::endl;
        entityWidget = new EntityWidget(this, nullptr,EID);
        entityWidget->setLabel(QString::fromStdString(entityText));

        //old =   mRenderWindow->getEntity(EID);
        Entity* tempEI = mRenderWindow->getSystemManager()->componentSystem()->getEntityWithEId(EID);
        std::vector<Entity*> children = tempEI->getChildren();

        for(unsigned int i = 0; i < children.size(); i++)
        {
            entityText = "Entity " + std::to_string(children.at(i)->eID);
            entityWidget->addCurrentChild(QString::fromStdString(entityText));
        }
        ui->scrollAreaWidgetContents->layout()->addWidget(entityWidget);


    }
    else
        std::cout << "EID is nullptr" << std::endl;

    //oppdateres ikke hvis objektet flytter seg imens
    if(transTemp != nullptr)
    {
        transWidget = new QWidget();

        transUi->setupUi(transWidget);
        transUi->xTransSpinBox->setValue(static_cast<double>(transTemp->position().x));
        transUi->yTransSpinBox->setValue(static_cast<double>(transTemp->position().y));
        transUi->zTransSpinBox->setValue(static_cast<double>(transTemp->position().z));
        ui->scrollAreaWidgetContents->layout()->addWidget(transWidget);

    }
    else
        std::cout << "Trans is nullptr" << std::endl;

    if(meshTemp != nullptr)
    {
        //  std::cout << "Mesh: " << meshTemp->name << std::endl;
        meshWidget = new QWidget();
        Ui::MeshForm meshUi;
        meshUi.setupUi(meshWidget);
        meshUi.meshCompoBox->addItem(QString::fromStdString(meshTemp->name));
        ui->scrollAreaWidgetContents->layout()->addWidget(meshWidget);
    }

    else
        std::cout << "Mesh is nullptr" << std::endl;






    //    delete widget; //dette som ødela... Ikke bruk det
    //    widget = nullptr;
}

void MainWindow::removeEntity(int EID)
{
    //oppdater eIds så rekkefølgen stemmer med world list
    std::vector <int> temp = indexWorldContent;
    for(int i = 0; i < temp.size(); i++)
    {
        if(temp.at(i) == EID)
        {
            QTreeWidgetItem *temp = ui->treeWidgetWorldContent->topLevelItem(EID);
            delete temp;
            temp = nullptr;
            indexWorldContent.erase(indexWorldContent.begin()+i);
            for(int j = 0; j<entitiesInWindow.size(); i++)
            {
                if(entitiesInWindow.at(j) == EID)
                {
                    entitiesInWindow.erase(entitiesInWindow.begin()+j);
                    //Cleaning up from before
                    delete entityWidget;
                    entityWidget = nullptr;
                    delete transWidget;
                    transWidget = nullptr;
                    delete meshWidget;
                    meshWidget = nullptr;
                    delete matWidget;
                    matWidget = nullptr;
                    delete collWidget;
                    collWidget = nullptr;
                    break;
                }
            }
            selectedEntity = -1;
        }
    }

}

void MainWindow::updateValues()
{
    gsl::Vector3D changePos;

    if(meshTemp != nullptr)
    {
        //update
    }
    if(transTemp != nullptr)
    {
        if(isPlaying == true)
        {
            //Trengs hvis objektet beveger seg av seg selv
            transUi->xTransSpinBox->setValue(static_cast<double>(transTemp->position().x));
            transUi->yTransSpinBox->setValue(static_cast<double>(transTemp->position().y));
            transUi->zTransSpinBox->setValue(static_cast<double>(transTemp->position().z));
        }

        else
        {
        //Piltastene kan nå endre på possisjon i xz
            double speed{0.01};
            if(mRenderWindow->getInput().UP == true)
            {
                  changePos = gsl::Vector3D{static_cast<float>(transUi->xTransSpinBox->value()),
                                            static_cast<float>(transUi->yTransSpinBox->value()),
                                            static_cast<float>(transUi->zTransSpinBox->value()-speed)};

                  transUi->zTransSpinBox->setValue(static_cast<double>(transTemp->position().z));
            }
            else if(mRenderWindow->getInput().DOWN == true)

            {
                changePos = gsl::Vector3D{static_cast<float>(transUi->xTransSpinBox->value()),
                                          static_cast<float>(transUi->yTransSpinBox->value()),
                                          static_cast<float>(transUi->zTransSpinBox->value()+speed)};

                transUi->zTransSpinBox->setValue(static_cast<double>(transTemp->position().z));
            }
            else if(mRenderWindow->getInput().LEFT == true)
            {
                changePos = gsl::Vector3D{static_cast<float>(transUi->xTransSpinBox->value()-speed),
                                          static_cast<float>(transUi->yTransSpinBox->value()),
                                          static_cast<float>(transUi->zTransSpinBox->value())};
                    transUi->xTransSpinBox->setValue(static_cast<double>(transTemp->position().x));
            }
            else if(mRenderWindow->getInput().RIGHT == true)
            {
                changePos = gsl::Vector3D{static_cast<float>(transUi->xTransSpinBox->value()+speed),
                                          static_cast<float>(transUi->yTransSpinBox->value()),
                                          static_cast<float>(transUi->zTransSpinBox->value())};
                transUi->xTransSpinBox->setValue(static_cast<double>(transTemp->position().x));

            }
            else
            {//Putte inn her om man bruker pilen for å flytte objektet
                changePos = gsl::Vector3D{static_cast<float>(transUi->xTransSpinBox->value()),static_cast<float>(transUi->yTransSpinBox->value()),static_cast<float>(transUi->zTransSpinBox->value())};

        }
            //må fikse så child flytter seg også
            transTemp->matrix().setPosition(changePos.x,changePos.y,changePos.z);
        }

    }
}

void MainWindow::addEntityInWorldContentList(int EID)
{
    bool isThere = false;
    for(auto temp : entitiesInWindow)
    {
        if(temp == EID)
        {
            std::cout << "Item already in the list" << std::endl;
            isThere = true;
        }
    }
    if(isThere == false)
    {
        worldObjects = new QTreeWidgetItem(ui->treeWidgetWorldContent);
        worldObjects->setText(0,"Entity"+QString::fromStdString(std::to_string(EID)));
        worldObjects->setCheckState(0,Qt::CheckState::Checked);
        worldObjects->setFlags(worldObjects->flags() |Qt::ItemIsUserCheckable /*| Qt::ItemIsEditable*/);
        entitiesInWindow.push_back(EID);
        indexWorldContent.push_back(EID);
        numEntities ++;
    }

}

void MainWindow::addNewItemInWorldContentList()
{
    worldObjects = new QTreeWidgetItem(ui->treeWidgetWorldContent);
    worldObjects->setText(0,"Entity"+QString::fromStdString(std::to_string(numEntities)));
    worldObjects->setCheckState(0,Qt::CheckState::Checked);
    worldObjects->setFlags(worldObjects->flags() |Qt::ItemIsUserCheckable /*| Qt::ItemIsEditable*/);
    entitiesInWindow.push_back(numEntities);
    indexWorldContent.push_back(numEntities);
    numEntities ++;
}



//void MainWindow::on_worldContentList_itemPressed(QListWidgetItem *item)
//{
//    ui->WorldContentInfo->clear();
//    int selectedRow = ui->worldContentList->row(item);
//    VisualObject *mVisualObj;
//    if(mRenderWindow->ReturnObj(selectedRow) != nullptr)
//    {
//        mVisualObj= mRenderWindow->ReturnObj(selectedRow);

//        std::string objName = mVisualObj->mName;
//        gsl::Vector3D  objPos = mVisualObj->mMatrix.getPosition();

//        QString infoText = QString::fromStdString("Object Name: " + objName /*+ "/n"*/ +
//                                                  " Coordinates: x: ") + QString::number(*objPos.xP())+
//                " y: " + QString::number(*objPos.yP())+
//                " z: " +QString::number(*objPos.zP());

//        //Etter hvert skaffe component info

//        ui->WorldContentInfo->insertPlainText(infoText);

//    }
//    //Putte inn informasjon om objektet i en textboks


//}

void MainWindow::on_treeWidgetWorldContent_itemClicked(QTreeWidgetItem *item, int column)
{


    setScrollArea(ui->treeWidgetWorldContent->indexOfTopLevelItem(item));
    //Må adde så det collision box er synlig når objektene velges her også

}

void MainWindow::on_PlayButton_clicked()
{
    isPlaying = true;
    isReseting = false;

}

void MainWindow::on_StopButton_clicked()
{
    isPlaying = false;
    isReseting = true;
}

void MainWindow::on_addEntityButton_clicked()
{

    //I PreMadeEntitiesComboBox addes alle objekter man har, og de man legger inn i add object
    //samme med texturer i en annen drop down meny
    entityObjectName = ui->PreMadeEntitiesComboBox->currentText();
    QString entityMaterialName = ui->MaterialTypeComboBox->currentText();
    QString entityCollisionName = ui->CollisionTypeComboBox->currentText();

    mRenderWindow->getSystemManager()->componentSystem()->addEntity(entityObjectName.toStdString(), entityMaterialName.toStdString(),entityCollisionName.toStdString());


    //    if(entityObjectName == "Empty")
    //    {
    //        //Add empty entity to world
    //    }
    //    else
    //    {
    //        //add object by name eks. "Cube"...
    //    }

}

void MainWindow::on_actionAdd_From_File_triggered()
{

    //Dette fungerer bare fra Meshes/ Kan funke på txt også
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open files"), "",
                                                    tr("Finde a file (*.json);;All Files (*)"));
    std::string temp = fileName.toStdString();
    std::string shortenFileName;
    const int idx =  temp.std::string::find_last_of("/");
    if(std::string::npos !=idx)
    {
        shortenFileName = temp.substr(idx+1);
    }
    //    loadedObjects.push_back(shortenFileName);
    //    ui->worldContentList->addItem(QString::fromStdString(shortenFileName));
    //    objectsInWindow.push_back(shortenFileName);

    objectToBeAdded = shortenFileName;
    isAddingObject = true;


    addNewItemInWorldContentList();
    addObjectToPreMadeEntitiesComboBox(QString::fromStdString(shortenFileName));
    //Må adde objektet på et vis her
}
void MainWindow::on_actionAdd_file_triggered()
{
    //Denne gjør ingen ting for nå
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open files"), "",
                                                    tr("Finde a file (*.json);;All Files (*)"));
    //Load scene from file



    //Not in use
}

void MainWindow::on_actionSave_File_triggered()
{
    //mRenderWindow->getSystemManager()->componentSystem()->saveScene();
}
