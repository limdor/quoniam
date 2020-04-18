//Definition include
#include "DutagaciDialog.h"

//Qt includes
#include <QtCore/QTextStream>

//Dependency includes
#include "glm/trigonometric.hpp"

DutagaciDialog::DutagaciDialog(QWidget *parent): QDialog(parent),
    mUi(new Ui::DutagaciView)
{
    mUi->setupUi(this);
    mUi->filesListWidget->setDragDropMode(QAbstractItemView::InternalMove);
    mUi->filesListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

    connect(mUi->closeButton, SIGNAL(clicked()), this, SLOT(close()));

    mUi->cameraDistanceSpinBox->setValue(6.0);
    on_computeCameraDistanceButton_clicked();
    mUi->addButton->setFocus();
}

DutagaciDialog::~DutagaciDialog()
{
    delete mUi;
}

std::vector<QString> DutagaciDialog::GetFilesToLoad() const
{
    return mFilesToLoad;
}

float DutagaciDialog::GetDistance() const
{
    return mDistance;
}

int DutagaciDialog::GetAngle() const
{
    return mAngle;
}

std::vector<QString> DutagaciDialog::GetFileListFromTXT(const QString& pFileName) const
{
    std::vector<QString> objFiles;
    QFile file(pFileName);
    if( file.open(QIODevice::ReadOnly) )
    {
        QFileInfo fileInfo(pFileName);
        QString folder = fileInfo.absolutePath();
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            QString line = stream.readLine();
            QStringList splittedLine = line.split(" ", QString::SkipEmptyParts);
            if(splittedLine.size() == 1)
            {
                objFiles.push_back(folder + "\\" + line + ".obj");
            }
        }
    }
    return objFiles;
}

void DutagaciDialog::on_computeCameraDistanceButton_clicked()
{
    mUi->cameraAngleSpinBox->setValue( 2.0f * glm::degrees( glm::asin( 1.0f / (float)mUi->cameraDistanceSpinBox->value() ) ) );
}

void DutagaciDialog::on_computeCameraAngleButton_clicked()
{
    mUi->cameraDistanceSpinBox->setValue( 1.0f / glm::sin( glm::radians( mUi->cameraAngleSpinBox->value() / 2.0f ) ) );
}

void DutagaciDialog::on_addButton_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Choose a model"), "./", tr("Supported models (*.obj *.3ds *.dae *.ply *.x *.txt);;Wavefront Object (*.obj);;3ds Max 3DS (*.3ds);;Collada (*.dae);;Stanford Polygon Library (*.ply);;DirectX X (*.x);;List of models (*.txt);;All files (*.*)"));
    for( int i = 0; i < fileNames.size(); i++ )
    {
        QString fileName = fileNames.at(i);
        if( !fileName.isEmpty() )
        {
            if(fileName.right(3) == "txt")
            {
                std::vector<QString> objFiles = GetFileListFromTXT(fileName);
                for( int j = 0; j < objFiles.size(); j++ )
                {
                    mUi->filesListWidget->addItem(objFiles.at(j));
                }
            }
            else
            {
                mUi->filesListWidget->addItem(fileName);
            }
        }
    }
    mUi->computeMatricesButton->setFocus();
}

void DutagaciDialog::on_deletteButton_clicked()
{
    QList< QListWidgetItem* > selectedList = mUi->filesListWidget->selectedItems();
    for( int i = 0; i < selectedList.size(); i++ )
    {
        mUi->filesListWidget->removeItemWidget(selectedList.at(i));
        delete selectedList.at(i);
    }
}

void DutagaciDialog::on_computeMatricesButton_clicked()
{
    int numberOfFiles = mUi->filesListWidget->count();
    for( int i = 0; i < numberOfFiles; i++ )
    {
        mFilesToLoad.push_back(mUi->filesListWidget->item(i)->text());
    }
    mDistance = mUi->cameraDistanceSpinBox->value();
    mAngle = mUi->cameraAngleSpinBox->value();

    accept();
}
