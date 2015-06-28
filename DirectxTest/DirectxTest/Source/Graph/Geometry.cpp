#include "Geometry.h"

namespace Graph
{
    Geometry::Geometry() : primitiveTopology(PrimitiveTopology::TriangleList)
    {
        scaleMatrix.identity();
        translateMatrix.identity();
        rotateMatrix.identity();
    }

    void Geometry::rotate(const Math::Vector3f & rotate)
    {
        Math::Matrix44 rotationX;
		rotationX.identity();
        rotationX.rotateX(Math::Deg2Rad(rotate.x));

        Math::Matrix44 rotationY;
		rotationY.identity();
        rotationY.rotateY(Math::Deg2Rad(rotate.y));

        Math::Matrix44 rotationZ;
		rotationZ.identity();
        rotationZ.rotateZ(Math::Deg2Rad(rotate.z));

        rotateMatrix = rotationX * rotationY * rotationZ;

    }

    void Geometry::translate(const Math::Vector3f & translate)
    {
        Math::Matrix44 translation;
        translation.translate(translate);

        translateMatrix =  translation;
    }

    void Geometry::scale(const Math::Vector3f & scaleAmount)
    {
        Math::Matrix44 scaling;
        scaling.scale(scaleAmount);

        scaleMatrix = scaling;
    }
}
