///\file Vector.hpp
///\brief Vecteurs mathématiques

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>

///\class Vector2i
///\brief Vecteur de 2 composantes entières
class Vector2i {
  public:
    int x; ///< Composante x
    int y; ///< Composante y

  ///\param x Composante x
  ///\param y Composante y
  Vector2i(const int& x = 0.0, const int& y = 0.0) {
    this->x = x;
    this->y = y;
  }
};

///\class Vector2d
///\brief Vecteur de 2 composantes réelles
class Vector2d {
  public:
    double x; ///< Composante x
    double y; ///< Composante y

  ///\param x Composante x
  ///\param y Composante y
  Vector2d(const double& x = 0.0, const double& y = 0.0) {
    this->x = x;
    this->y = y;
  }
};

///\class Vector3d
///\brief Vecteur de 3 composantes réelles
class Vector3d {
public:
  double x; ///< Composantes x
  double y; ///< Composantes y
  double z; ///< Composantes z

  ///\param x Composante x
  ///\param y Composante y
  ///\param z Composante z
  Vector3d(const double& x = 0.0, const double& y = 0.0, const double& z = 0.0) {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  ///\brief copy-constructor
  Vector3d(const Vector3d& vector) {
    x = vector.x;
    y = vector.y;
    z = vector.z;
  }

  ///\brief construit un vecteur à partir d'une norme et d'une orientation
  Vector3d(const Vector3d& orientation, double norm) {
    x = orientation.x;
    y = orientation.y;
    z = orientation.z;
    Normalize();
    Scale(norm);
  }

  ///\brief Obtention de l'angle
  ///\param v Vecteur
  ///\return Angle en radian
  double GetAngle(const Vector3d& v) const {
    const double v1Norm = GetNorm(), v2Norm = v.GetNorm();
    return (v1Norm && v2Norm) ? acos((*this * v) / (v1Norm * v2Norm)) : 0.0;
  }

  ///\brief Obtention de la norme
  ///\return Norme
  inline double GetNorm() const {
    return std::sqrt(x * x + y * y + z * z);
  }

  ///\brief Normalisation
  void Normalize() {
    const double norm = GetNorm();
    if (norm) {
      x /= norm;
      y /= norm;
      z /= norm;
    }
  }

  ///\brief Obtient le vecteur normalisé
  ///\return Le vecteur normalisé
  const Vector3d Normalized() const {
    double norm = GetNorm();
    return Vector3d(x / norm, y / norm, z / norm);
  }

  ///\brief Multiplie le vecteur par un scalaire
  ///\param scale scalaire
  void Scale(double scale) {
    *this = *this * scale;
  }
  
  ///\brief Obtient le vecteur multiplié par un scalaire
  ///\param scale scalaire
  ///\return Le vecteur multiplié par un scalaire
  const Vector3d Scaled(double scale) {
    return *this * scale;
  }

  ///\brief obtient la projection du vecteur "this" sur le vecteur "vector"
  ///\param vector target de la projection
  ///\return retourne le résultat de la projection
  const Vector3d Projected(const Vector3d& vector) const {
    Vector3d vector2(*this);
    vector2.Project(vector);
    return vector2;
  }
  
  ///\brief projecte le vecteur "this" sur le vecteur "vector"
  ///\param vector target de la projection
  void Project(const Vector3d& vector) {
    *this = vector * ((*this * vector) / (pow(vector.GetNorm(), 2)));
  }

  ///\brief Somme
  ///\param v Vecteur
  inline Vector3d operator+(const Vector3d& v) const {
    return Vector3d(x + v.x, y + v.y, z + v.z);
  }

  ///\brief Différence
  ///\param v Vecteur
  inline Vector3d operator-(const Vector3d& v) const {
    return Vector3d(x - v.x, y - v.y, z - v.z);
  }

  ///\brief Produit vectoriel
  ///\param v Vecteur
  inline Vector3d operator%(const Vector3d& v) const {
    return Vector3d(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
  }

  ///\brief Opérateur =
  ///\param v Vecteur
  void operator=(const Vector3d& v) {
    x = v.x;
    y = v.y;
    z = v.z;
  }

  ///\brief Produit scalaire
  ///\param v Vecteur
  inline double operator*(const Vector3d& v) const {
    return x * v.x + y * v.y + z * v.z;
  }

  ///\brief Produit par un scalaire
  ///\param s Scalaire
  inline Vector3d operator*(const double& s) const {
    return Vector3d(x * s, y * s, z * s);
  }

  ///\brief !=
  ///\param v Vecteur
  inline bool operator!=(const Vector3d& v) const {
    return x != v.x || y != v.y || z != v.z;
  }
};
#endif // VECTOR_HPP
