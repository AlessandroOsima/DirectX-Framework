#pragma once 
#include <string.h>
#include <math.h>


namespace Math
{
    const float PI = 3.14159265358979323846f;

    inline float Rad2Deg(float radAngle)
    {
        return radAngle * (180/PI);
    }

    inline float Deg2Rad(float degAngle)
    {
        return degAngle * (PI/180);
    }

	struct Vector2
	{
		float x;
		float y;
	};

    template<class T> 
    class Vector3
    {
    public:

        Vector3(T x, T y, T z) : x(x), y(y),z(z)
        {
        }

        Vector3()
        {
            zero();
        }

        inline void zero()
        {
            memset(this,0,sizeof(x) * 3);
        }

		inline Vector3 operator-(const Vector3 & vector)
		{
			return Vector3(x - vector.x, y - vector.y, z - vector.z);
		}

		inline Vector3 operator-()
		{
			return Vector3(-x , -y , -z);
		}

		inline Vector3 operator+(const Vector3 & vector)
		{
			return Vector4(x + vector.x, y + vector.y, z + vector.z);
		}

		inline float lenght()
		{
			return sqrt(x * x + y * y + z * z);
		}

		inline void normalize()
		{
			float vLenght = lenght();

			x = x / vLenght;
			y = y / vLenght;
			z = z / vLenght;

		}

		inline Vector3 normal()
		{
			float vLenght = lenght();

			return Vector3(x / vLenght, y / vLenght, z / vLenght);
		}

		inline Vector3 cross(const Vector3 & vector)
		{
			return Vector3(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
		}

		inline float dot(const Vector3 & vector)
		{
			return this->x * vector.x + this->y * vector.y + this->z * vector.z;
		}

        T x;
        T y;
        T z;
    };

    
    typedef Vector3<int> Vector3i;
    typedef Vector3<float> Vector3f;
    typedef Vector3<double> Vector3d;


    template<class T> 
    class Vector4
    {
    public:

        Vector4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
        {
        }

        Vector4()
        {
           zero();
        }

        inline void zero()
        {
            memset(this,0,sizeof(x) * 4);
        }

		inline Vector4 operator-(const Vector4 & vector)
		{
			return Vector4(x - vector.x, y - vector.y, z - vector.z,w - vector.w);
		}

		inline Vector4 operator+(const Vector4 & vector)
		{
			return Vector4(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
		}

		inline float lenght()
		{
			return sqrt(x * x + y * y + z * z);
		}

		inline void normalize()
		{
			float vLenght = lenght();
			
			x = x / vLenght;
			y = y / vLenght;
			z = z / vLenght;

		}

		inline Vector4 normal()
		{
			float vLenght = lenght();

			return Vector4(x / vLenght, y / vLenght, z / vLenght, 0);
		}

		inline Vector4 cross(const Vector4 & vector)
		{
			return Vector4(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x, 0);
		}

		inline float dot(const Vector4 & vector)
		{
			return this->x * vector.x + this->y * vector.y + this->z * vector.z;
		}

        T x;
        T y;
        T z;
        T w;
    };

    typedef Vector4<int> Vector4i;
    typedef Vector4<float> Vector4f;
    typedef Vector4<double> Vector4d;


	struct Color
	{
        Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a)
        {
        }

		float r;
		float g;
		float b;
		float a;
	};

	struct Vertex
	{
        Vertex(Vector3f position, Color color, Vector3f normal) : position(position), color(color), normal(normal)
        {
        }

        Vertex(Vector3f position, Color color) : position(position), color(color), normal(Vector3f(0,0,0))
        {
        }

		Vector3f position;
		Color color;
        Vector3f normal;
	};

}