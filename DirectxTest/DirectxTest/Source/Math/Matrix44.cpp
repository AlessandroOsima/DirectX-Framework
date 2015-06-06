#include "Matrix44.h"
#include <math.h>
#include <assert.h>

namespace Math
{
	Matrix44::Matrix44()
	{
		zero();
	}

	void Matrix44::identity()
	{
		axisX.zero();
		axisY.zero();
		axisZ.zero();
		axisW.zero();

		axisX.x = 1;
		axisY.y = 1;
		axisZ.z = 1;
		axisW.w = 1;
	}

	void Matrix44::zero()
	{
		axisX.zero();
		axisY.zero();
		axisZ.zero();
		axisW.zero();
	}

	void Matrix44::scale(const Vector3f & scale)
	{
		identity();

		axisX.x = scale.x;
		axisY.y = scale.y;
		axisZ.z = scale.z;
	}


	void Matrix44::translate(const Vector3f & translate)
	{
		identity();

		axisW.x = translate.x;
		axisW.y = translate.y;
		axisW.z = translate.z;
	}

	void Matrix44::rotateX(float angle)
	{
		float sinAn = sin(angle);
		float cosAn = cos(angle);

		identity();

		axisY.y = cosAn;
		axisY.z = sinAn;

		axisZ.y = -sinAn;
		axisZ.z = cosAn;
	}

	void Matrix44::rotateY(float angle)
	{
		float sinAn = sin(angle);
		float cosAn = cos(angle);

		identity();

		axisX.x = cosAn;
		axisX.z = -sinAn;

		axisZ.x = sinAn;
		axisZ.z = cosAn;

	}

	void Matrix44::rotateZ(float angle)
	{
		float sinAn = sin(angle);
		float cosAn = cos(angle);

		identity();

		axisX.x = cosAn;
		axisX.y = sinAn;

		axisY.x = -sinAn;
		axisY.y = cosAn;

	}

	void Matrix44::lookAt(Vector3f eye, Vector3f at, Vector3f up)
	{
		Vector3f zAxis = (at - eye).normal();
		Vector3f xAxis = (up.cross(zAxis)).normal();
		Vector3f yAxis = zAxis.cross(xAxis);

		this->axisX = Vector4f(xAxis.x, yAxis.x, zAxis.x, 0);
		this->axisY = Vector4f(xAxis.y, yAxis.y, zAxis.y, 0);
		this->axisZ = Vector4f(xAxis.z, yAxis.z, zAxis.z, 0);
		this->axisW = Vector4f(-(xAxis.dot(eye)), -(yAxis.dot(eye)), -(zAxis.dot(eye)), 1);
	}

    void Matrix44::ortho(float width, float height, float zNear, float zFar)
    {
        zero();

        assert(width);
        assert(height);
        assert(zFar -zNear);
        assert(zNear - zFar);

        this->axisX = Vector4f(2/width,        0,                    0,      0);
        this->axisY = Vector4f(      0, 2/height,                    0,      0);
        this->axisZ = Vector4f(      0,        0,      1/(zFar -zNear),      0);
        this->axisW = Vector4f(      0,        0, zNear/(zNear - zFar),      1);
    }

	void Matrix44::perspective(float fovHeight, float aspect, float near, float far)
	{
		float yScale = 1 / tan(fovHeight / 2);
		float xScale = yScale / aspect;

		this->axisX = Vector4f(xScale, 0, 0, 0);
		this->axisY = Vector4f(0, yScale, 0, 0);
		this->axisZ = Vector4f(0, 0, far / (far - near), 1);
		this->axisW = Vector4f(0, 0, (-near * far) / (far - near), 0);
	}

    Vector4f Matrix44::operator*(const Vector4f & vector) const
    {
        Vector4f result;

        result.x = (vector.x * axisX.x) + (vector.y * axisY.x) + (vector.z * axisZ.x) + (vector.w * axisW.x);
        result.y = (vector.x * axisX.y) + (vector.y * axisY.y) + (vector.z * axisZ.y) + (vector.w * axisW.y);
        result.z = (vector.x * axisX.z) + (vector.y * axisY.z) + (vector.z * axisZ.z) + (vector.w * axisW.z);
        result.w = (vector.x * axisX.w) + (vector.y * axisY.w) + (vector.z * axisZ.w) + (vector.w * axisW.w);

        return result;
    }

    Vector3f Matrix44::operator*(const Vector3f & vector) const
    {
        Vector3f result;

        result.x = (vector.x * axisX.x) + (vector.y * axisY.x) + (vector.z * axisZ.x);
        result.y = (vector.x * axisX.y) + (vector.y * axisY.y) + (vector.z * axisZ.y);
        result.z = (vector.x * axisX.z) + (vector.y * axisY.z) + (vector.z * axisZ.z);

        return result;
    }

    Math::Matrix44 Matrix44::operator*(const Matrix44 & matrix) const
    {
        return mult(matrix);
    }

    Math::Matrix44 Matrix44::mult(const Matrix44 & matrix) const
    {
        Matrix44 mtx;

        mtx.axisX.x = (this->axisX.x * matrix.axisX.x) + (this->axisX.y * matrix.axisY.x) + (this->axisX.z * matrix.axisZ.x) + (this->axisX.w * matrix.axisW.x); 
        mtx.axisX.y = (this->axisX.x * matrix.axisX.y) + (this->axisX.y * matrix.axisY.y) + (this->axisX.z * matrix.axisZ.y) + (this->axisX.w * matrix.axisW.y);
        mtx.axisX.z = (this->axisX.x * matrix.axisX.z) + (this->axisX.y * matrix.axisY.z) + (this->axisX.z * matrix.axisZ.z) + (this->axisX.w * matrix.axisW.z);
        mtx.axisX.w = (this->axisX.x * matrix.axisX.w) + (this->axisX.y * matrix.axisY.w) + (this->axisX.z * matrix.axisZ.w) + (this->axisX.w * matrix.axisW.w);


        mtx.axisY.x = (this->axisY.x * matrix.axisX.x) + (this->axisY.y * matrix.axisY.x) + (this->axisY.z * matrix.axisZ.x) + (this->axisY.w * matrix.axisW.x); 
        mtx.axisY.y = (this->axisY.x * matrix.axisX.y) + (this->axisY.y * matrix.axisY.y) + (this->axisY.z * matrix.axisZ.y) + (this->axisY.w * matrix.axisW.y);
        mtx.axisY.z = (this->axisY.x * matrix.axisX.z) + (this->axisY.y * matrix.axisY.z) + (this->axisY.z * matrix.axisZ.z) + (this->axisY.w * matrix.axisW.z);
        mtx.axisY.w = (this->axisY.x * matrix.axisX.w) + (this->axisY.y * matrix.axisY.w) + (this->axisY.z * matrix.axisZ.w) + (this->axisY.w * matrix.axisW.w);

        mtx.axisZ.x = (this->axisZ.x * matrix.axisX.x) + (this->axisZ.y * matrix.axisY.x) + (this->axisZ.z * matrix.axisZ.x) + (this->axisZ.w * matrix.axisW.x); 
        mtx.axisZ.y = (this->axisZ.x * matrix.axisX.y) + (this->axisZ.y * matrix.axisY.y) + (this->axisZ.z * matrix.axisZ.y) + (this->axisZ.w * matrix.axisW.y);
        mtx.axisZ.z = (this->axisZ.x * matrix.axisX.z) + (this->axisZ.y * matrix.axisY.z) + (this->axisZ.z * matrix.axisZ.z) + (this->axisZ.w * matrix.axisW.z);
        mtx.axisZ.w = (this->axisZ.x * matrix.axisX.w) + (this->axisZ.y * matrix.axisY.w) + (this->axisZ.z * matrix.axisZ.w) + (this->axisZ.w * matrix.axisW.w);

        mtx.axisW.x = (this->axisW.x * matrix.axisX.x) + (this->axisW.y * matrix.axisY.x) + (this->axisW.z * matrix.axisZ.x) + (this->axisW.w * matrix.axisW.x); 
        mtx.axisW.y = (this->axisW.x * matrix.axisX.y) + (this->axisW.y * matrix.axisY.y) + (this->axisW.z * matrix.axisZ.y) + (this->axisW.w * matrix.axisW.y);
        mtx.axisW.z = (this->axisW.x * matrix.axisX.z) + (this->axisW.y * matrix.axisY.z) + (this->axisW.z * matrix.axisZ.z) + (this->axisW.w * matrix.axisW.z);
        mtx.axisW.w = (this->axisW.x * matrix.axisX.w) + (this->axisW.y * matrix.axisY.w) + (this->axisW.z * matrix.axisZ.w) + (this->axisW.w * matrix.axisW.w);

        return mtx;
    }



}

