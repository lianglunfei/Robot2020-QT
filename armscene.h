#ifndef ARMSCENE_H
#define ARMSCENE_H

#include <QtCore/QObject>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QPhongMaterial>

class ArmScene : public QObject
{
    Q_OBJECT

public:
    explicit ArmScene(Qt3DCore::QEntity *rootEntity);
    ~ArmScene();

    void setArm(const int,double, double, double, double, double, double);
    void setArm(const int,double[3],double[3]);
    void setArms(double[3],double[3],double[3],double[3],double[3],double[3]);

public slots:
    void enableCylinder(bool enabled);


private:
    void initArm(const int ID, const QVector3D& start, const QVector3D& end, const QColor& color);

    Qt3DCore::QTransform *armTransform[6];
    Qt3DExtras::QCylinderMesh *arm[6];
    Qt3DCore::QEntity *m_armEntity[6];


    Qt3DCore::QEntity *m_rootEntity;
    Qt3DCore::QEntity *m_planeEntity;


};


#endif // ARMSCENE_H
