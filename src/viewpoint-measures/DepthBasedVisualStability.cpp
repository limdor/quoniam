//Definition include
#include "DepthBasedVisualStability.h"

//Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>

//Dependency includes
#include "opencv2/highgui/highgui.hpp"

//Project includes
#include "Debug.h"
#include "Tools.h"

DepthBasedVisualStability::DepthBasedVisualStability(const QString &pName): Measure(pName, true)
{

}

void DepthBasedVisualStability::Compute(const SceneInformationBuilder *pSceneInformationBuilder)
{
    const float threshold = 0.87f;
    const size_t numberOfViewpoints = pSceneInformationBuilder->GetProjectedAreasMatrix()->GetNumberOfViewpoints();
    mValues.resize( numberOfViewpoints );
    std::fill(mValues.begin(), mValues.end(), 0.0f);
    QString pathImage(".\\images\\DepthImage_%1.pgm");
    QString pathZippedImage(".\\images\\DepthImage_%1.bzip2");
    QProcess zipImage;
    QString program("c:\\program files\\7-zip\\7z.exe");
    QStringList arguments;
    arguments << "a" << "-tbzip2" << "-mx9";
    for( size_t currentViewpointI = 0; currentViewpointI < numberOfViewpoints; currentViewpointI++ )
    {
        cv::Mat depthImageI = pSceneInformationBuilder->GetDepthImage(currentViewpointI);
        cv::Size depthImageSize = depthImageI.size();
        QString pathZippedImageI = pathZippedImage.arg(currentViewpointI);
        QFileInfo fileZippedImageI(pathZippedImageI);
        if(!fileZippedImageI.exists())
        {
            QString pathImageI = pathImage.arg(currentViewpointI);
            cv::imwrite(pathImageI.toStdString(), depthImageI);
            QStringList specificArguments(arguments);
            specificArguments << pathZippedImageI << pathImageI;
            zipImage.execute(program, specificArguments);
            fileZippedImageI.refresh();
            QFile::remove(pathImageI);
        }
        for( size_t currentViewpointJ = currentViewpointI + 1; currentViewpointJ < numberOfViewpoints; currentViewpointJ++ )
        {
            cv::Mat depthImageJ = pSceneInformationBuilder->GetDepthImage(currentViewpointJ);
            QString pathZippedImageJ = pathZippedImage.arg(currentViewpointJ);
            QFileInfo fileZippedImageJ(pathZippedImageJ);
            if(!fileZippedImageJ.exists())
            {
                QString pathImageJ = pathImage.arg(currentViewpointJ);
                cv::imwrite(pathImageJ.toStdString(), depthImageJ);
                QStringList specificArguments(arguments);
                specificArguments << pathZippedImageJ << pathImageJ;
                zipImage.execute(program, specificArguments);
                fileZippedImageJ.refresh();
                QFile::remove(pathImageJ);
            }
            QString pathJoinImage(".\\images\\DepthImage_%1_%2.pgm");
            QString pathZippedJoinImage(".\\images\\DepthImage_%1_%2.bzip2");
            pathJoinImage = pathJoinImage.arg(currentViewpointI).arg(currentViewpointJ);
            pathZippedJoinImage = pathZippedJoinImage.arg(currentViewpointI).arg(currentViewpointJ);
            cv::Mat joinImage(depthImageSize.height, depthImageSize.width * 2, CV_8UC1);
            joinImage.adjustROI(0, 0, 0, -depthImageSize.width);
            depthImageI.copyTo(joinImage);
            joinImage.adjustROI(0, 0, -depthImageSize.width, depthImageSize.width);
            depthImageJ.copyTo(joinImage);
            joinImage.adjustROI(0, 0, depthImageSize.width, 0);
            cv::imwrite(pathJoinImage.toStdString(), joinImage);
            QStringList specificArguments(arguments);
            specificArguments << pathZippedJoinImage << pathJoinImage;
            zipImage.execute(program, specificArguments);
            QFile::remove(pathJoinImage);

            int sizeI = fileZippedImageI.size();
            int sizeJ = fileZippedImageJ.size();
            QFileInfo fileZippedJoinImage(pathZippedJoinImage);
            int sizeJoin = fileZippedJoinImage.size();
            int min = qMin(sizeI, sizeJ);
            int max = qMax(sizeI, sizeJ);
            float ncd = (sizeJoin - min) / (float)max;
            if(ncd < threshold)
            {
                mValues[currentViewpointI]++;
                mValues[currentViewpointJ]++;
            }
            QFile::remove(pathZippedJoinImage);
        }
        QFile::remove(pathZippedImageI);
    }
    mSort = Tools::GetOrderedIndexes(mValues);
    mPositions = Tools::GetOrderedIndexes(mSort);
    mComputed = true;
}
