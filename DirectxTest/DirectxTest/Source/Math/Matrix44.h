#pragma once
#include "Math.h"

namespace Math
{
    class Matrix44
    {
    public:

        Vector4f axisX;
        Vector4f axisY;
        Vector4f axisZ;
        Vector4f axisW;

        Matrix44();

        void identity();
        void zero();

        
        void scale(const Vector3f & scale);
        void translate(const Vector3f & translate);
        void rotateX(float angle);
        void rotateY(float angle);
        void rotateZ(float angle);
		void lookAt(Vector3f eye, Vector3f at, Vector3f up);
        void ortho(float width, float height, float zNear, float zFar);
		void perspective(float fovHeight, float aspect, float near, float far);


        //void  lookAtLH();
        
        Vector4f operator*(const Vector4f & vector) const;
        Vector3f operator*(const Vector3f & vector) const;

        Matrix44 operator*(const Matrix44 & matrix) const;

        Matrix44 mult(const Matrix44 & matrix) const;

    };
}
