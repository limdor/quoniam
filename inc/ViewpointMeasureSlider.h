/// \file ViewpointMeasureSlider.h
/// \class ViewpointMeasureSlider
/// \author Xavier Bonaventura
/// \author Copyright: (c) Universitat de Girona

#ifndef VIEWPOINT_MEASURE_SLIDER_H
#define VIEWPOINT_MEASURE_SLIDER_H

//Qt includes
#include <QSlider>

class ViewpointMeasureSlider : public QSlider
{
    Q_OBJECT
public:
    explicit ViewpointMeasureSlider(int pMeasure, Qt::Orientation pOrientation, QWidget *pParent = 0);
private:
    int mMeasure;
signals:
    void valueChanged(int pMeasure, int pValue);
private slots:
    void emitValueChangedWithMeasure(int pValue);
};

#endif
