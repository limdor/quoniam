//Definition include
#include "ViewpointMeasureSlider.h"

ViewpointMeasureSlider::ViewpointMeasureSlider(int pMeasure, Qt::Orientation pOrientation, QWidget *pParent): QSlider(pOrientation, pParent),
    mMeasure(pMeasure)
{
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(emitValueChangedWithMeasure(int)));
}

void ViewpointMeasureSlider::emitValueChangedWithMeasure(int pValue)
{
    emit valueChanged(mMeasure, pValue);
}
