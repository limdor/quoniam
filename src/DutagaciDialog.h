/// \file DutagaciDialog.h
/// \class DutagaciDialog
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona
///
#ifndef DUTAGACI_DIALOG_H
#define DUTAGACI_DIALOG_H

//UI include
#include "ui_DutagaciView.h"

//Qt includes
#include <QDialog>
#include <QFileDialog>

namespace Ui {
    class DutagaciView;
}

/// Dialog to run the Dutagaci benchmark
class DutagaciDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DutagaciDialog(QWidget *parent = 0);
    ~DutagaciDialog();

    /// Return the list of files loaded in the dialog
    QVector<QString> GetFilesToLoad() const;
    /// Get the distance as times the radius of the scene
    float GetDistance() const;
    /// Get the field of view of the cameras
    int GetAngle() const;

private:
    /// User interface
    Ui::DutagaciView* mUi;
    QVector<QString> mFilesToLoad;
    float mDistance;
    int mAngle;

    QVector<QString> GetFileListFromTXT(const QString &pFileName) const;

private slots:
    /// Compute camera distance button method
    void on_computeCameraDistanceButton_clicked();
    /// Compute camera angle button method
    void on_computeCameraAngleButton_clicked();
    /// Add button clicked method
    void on_addButton_clicked();
    /// Delete button clicked method
    void on_deletteButton_clicked();
    /// Compute matrices button clicked method
    void on_computeMatricesButton_clicked();
};

#endif
