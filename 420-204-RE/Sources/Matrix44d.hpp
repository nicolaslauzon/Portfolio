///\file Matrix44d.hpp

#ifndef MATRIX44D_HPP
#define MATRIX44D_HPP

#include <algorithm>
#include <cmath>

#include "Vector.hpp"

using namespace std;

#define m11 matrix_[0] 
#define m12 matrix_[1] 
#define m13 matrix_[2] 
#define m14 matrix_[3]
#define m21 matrix_[4] 
#define m22 matrix_[5] 
#define m23 matrix_[6] 
#define m24 matrix_[7]
#define m31 matrix_[8] 
#define m32 matrix_[9] 
#define m33 matrix_[10] 
#define m34 matrix_[11]
#define m41 matrix_[12] 
#define m42 matrix_[13] 
#define m43 matrix_[14] 
#define m44 matrix_[15]

///\class Matrix44d
///\brief Matrice 4 par 4 de doubles
class Matrix44d {

private:
    double matrix_[16]; ///< Matrice

public:
    Matrix44d()=default;

    Matrix44d(const double* matrix) {
        for (int i = 0; i < 16; ++i) {
            matrix_[i] = matrix[i];
        }
    }

    ///\brief Charge une matrice identite dans le tableau
    void LoadIdentity() {
        m11 = 1.0;
        m22 = 1.0;
        m33 = 1.0;
        m44 = 1.0;

        m12 = 0.0;
        m13 = 0.0;
        m14 = 0.0;

        m21 = 0.0;
        m23 = 0.0;
        m24 = 0.0;

        m31 = 0.0;
        m32 = 0.0;
        m34 = 0.0;

        m41 = 0.0;
        m42 = 0.0;
        m43 = 0.0;
    }

    ///\brief Charge une rotation sur l'axe des z
    ///\param angle angle en radian
    void LoadZRotation(const double& angle) {
        double s = std::sin(angle);
        double c = std::cos(angle);

        m11 = c;
        m12 = -s;
        m13 = 0.0;

        m21 = s;
        m22 = c;
        m23 = 0.0;

        m31 = 0.0;
        m32 = 0.0;
        m33 = 1.0;

        m14 = 0.0;
        m24 = 0.0;
        m34 = 0.0;
        m44 = 1.0;
        m41 = 0.0;
        m42 = 0.0;
        m43 = 0.0;
    }

    ///\brief Charge une matrice de rotation
    ///\param axis Axe de rotation
    ///\param angle Angle de rotation en radian
    void LoadRotation(Vector3d axis, double angle) {
        /*
        axis.Normalize();
        const double c = cos(angle);
        const double s = sin(angle);
        m11 = axis.x * axis.x * (1 - c) + c;          m12 = axis.x * axis.y * (1 - c) - axis.z * s; m13 = axis.x * axis.z * (1 - c) + axis.y * s; m14 = 0.0;
        m21 = axis.x * axis.y * (1 - c) + axis.z * s; m22 = axis.y * axis.y * (1 - c) + c;          m23 = axis.z * axis.y * (1 - c) - axis.x * s; m24 = 0.0;
        m31 = axis.x * axis.z * (1 - c) - axis.y * s; m32 = axis.z * axis.y * (1 - c) - axis.x * s; m33 = axis.z * axis.z * (1 - c) + c;          m34 = 0.0;
        m41 = 0.0;                                    m42 = 0.0;                                    m43 = 0.0;                                    m44 = 1.0;
        */

        axis.Normalize();

        double u2 = axis.x*axis.x;
        double v2 = axis.y*axis.y;
        double w2 = axis.z*axis.z;
        double c = std::cos(angle);
        double _1_c = 1 - c;
        double s = std::sin(angle);


        m11 = c + u2 * _1_c;
        m12 = axis.x*axis.y * _1_c - axis.z*s;
        m13 = axis.x*axis.z * _1_c + axis.y*s;
        m14 = 0.;

        m21 = axis.x*axis.y * _1_c + axis.z*s;
        m22 = c + v2 * _1_c;
        m23 = axis.y*axis.z * _1_c - axis.x*s;
        m24 = 0.;

        m31 = axis.x*axis.z * _1_c - axis.y*s;
        m32 = axis.y*axis.z * _1_c + axis.x*s;
        m33 = c + w2 * _1_c;
        m34 = 0.;

        //m41 = m42 = m43 = 0.;
        m44 = 1.;       
    }

    ///\brief Charge une matrice de translation
    ///\param translation translation souhaitée
    void Translate(const Vector3d& translation) {
        m14 += translation.x;
        m24 += translation.y;
        m34 += translation.z;
    }

    ///\brief Charge une matrice de scaling
    ///\param scaleValue valeur de scaling
    void LoadScale(const Vector3d& scaleValue) {
        m11 = scaleValue.x;
        m22 = scaleValue.y;
        m33 = scaleValue.z;
    }

    ///\brief Charge l'origine de la transformation dans la matrice
    ///\param origin Origine de la transformation
    void LoadTransformationOrigin(const Vector3d& origin) {
        m41 += origin.x;
        m42 += origin.y;
        m43 += origin.z;
    }

    ///\brief Charge une matrice de vue
    ///\param front Vecteur du devant
    ///\param side Vecteur du côté
    ///\param up Vecteur du dessus
    void LoadView(const Vector3d& front, const Vector3d& side, const Vector3d& up) {
        m11 = side.x;   m12 = up.x;    m13 = -front.x;    m14 = 0.0;
        m21 = side.y;   m22 = up.y;    m23 = -front.y;    m24 = 0.0;
        m31 = side.z;   m32 = up.z;    m33 = -front.z;    m34 = 0.0;
        m41 = 0.0;      m42 = 0.0;     m43 = 0.0;         m44 = 1.0;
    }

    ///\brief Transpose la matrice
    void Transpose() {
        swap(m12, m21);
        swap(m13, m31);
        swap(m14, m41);

        swap(m23, m32);
        swap(m24, m42);

        swap(m34, m43);

    }

    ///\brief Retourne la matrice
    double* GetMatrix() {
        return matrix_;
    }

    ///\brief Retourne la ligne de translation
    inline Vector3d GetTranslation() {
        return { m14,m24,m34 };
    }

    ///\brief Retourne la ligne de l'origine
    inline Vector3d GetOrigin() {
        return { m41,m42,m43 };
    }

    ///\brief Multiplication de la matrice par un vecteur
    ///\param v Vecteur3d
    ///\return Vecteur qui a subit la transformation
    Vector3d operator*(const Vector3d& v) {
		return Vector3d((v.x * m11 + v.y * m21 + v.z * m31),
		                (v.x * m12 + v.y * m22 + v.z * m32),
		                (v.x * m13 + v.y * m23 + v.z * m33));
    }
};
#endif // MATRIX44D_HPP
