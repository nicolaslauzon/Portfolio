#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <fstream>
#include <vector>
#include "Vector.hpp"
#include "Resource.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "Physic.hpp"
#include "Matrix44d.hpp"
//#include <iostream>

using namespace std;

///\class Mesh
///\brief Modèle en 3 dimensions
class Mesh : public Resource {
protected:
    unsigned int vertexCount; ///< Nombre de sommets à afficher
    double* vertices; ///< Tableau de composantes de sommets
    double* normals; ///< Tableau de composantes de normales
    double farthestPoint = 0.0;
    double variablePoint;

 
public:
    ///\brief Chargement du modèle
    ///\param fileName Nom du fichier de modèle à charger
    virtual void load(const char* fileName) = 0;

    virtual void* GetData() = 0;
};

///\class TexturedMesh
///\brief Modèle texturé en 3 dimensions
class TexturedMesh : public Mesh {
private:
    unsigned int textureId; ///< Identifiant de texture
    double* textureCoords; ///< Tableau de coordonnées de texture
    size_t i1, i2;
    unsigned int vertexCount3;
public:

    TexturedMesh() {}
    TexturedMesh(TexturedMesh* texturedMesh) {
      if (texturedMesh) {
        this->farthestPoint = texturedMesh->farthestPoint;
        this->vertexCount = texturedMesh->vertexCount;
        vertexCount3 = vertexCount * 3;
        this->vertices = new double[this->vertexCount3];
        this->normals = new double[this->vertexCount3];
        this->textureCoords = new double[this->vertexCount * 2];

        copy(&texturedMesh->vertices[0], &texturedMesh->vertices[this->vertexCount3], this->vertices);
        copy(&texturedMesh->normals[0], &texturedMesh->normals[this->vertexCount3], this->normals);
        copy(&texturedMesh->textureCoords[0], &texturedMesh->textureCoords[this->vertexCount * 2], this->textureCoords);
      }
    }

    void* GetData() {
      return this;
    }

    const double* GetVertices() const {
        return vertices;
    }

    const double* GetNormals() const {
        return normals;
    }

    unsigned int GetVertexCount() const {
        return vertexCount;
    }

    ///\brief Chargement du modèle
    ///\param fileName Nom du fichier de modèle à charger
    void load(const char* fileName) {
    //C:\Users\eliet\Desktop\ToutProg\Programmation\ProgramSession4\INTEGRATION\x64\Debug\Resources\Obj\Sprites\cubeanim_000001.obj
        ifstream ifs(fileName);
        if (ifs.is_open()) {
            string str = " ";
            vector<double> tmpTexCoords;
            vector<double> tmpVertices, tmpNormals;
            vector<unsigned int> tmpVertexIndex, tmpNormalIndex, tmpTexCoordIndex;

            while (!str.empty()) {
                str.clear();
                ifs >> str;
                double component;
                unsigned int index;
                switch (str[0]) {
                case 'v':
                    if (str.size() == 1) {
                        for (unsigned char c = 0; c < 3; c++) {
                            ifs >> component;
                            tmpVertices.push_back(component);
                        }
                    }
                    else
                        switch (str[1]) {
                        case 't':
                            for (unsigned char c = 0; c < 2; c++) {
                                ifs >> component;
                                tmpTexCoords.push_back(component);
                            }
                            break;

                        case 'n':
                            for (unsigned char c = 0; c < 3; c++) {
                                ifs >> component;
                                tmpNormals.push_back(component);
                            }
                            break;
                        }
                    break;

                case 'f':
                    for (int i = 0; i < 3; i++) {
                        ifs >> index;
                        tmpVertexIndex.push_back(index);
                        ifs.ignore();

                        ifs >> index;
                        tmpTexCoordIndex.push_back(index);
                        ifs.ignore();

                        ifs >> index;
                        tmpNormalIndex.push_back(index);
                        ifs.ignore();
                    }
                    break;

                default:
                    ifs.ignore(30, 10); // Ignore les lignes qui ne nous sont pas nécessaires.
                    break;
                }
            }
            ifs.close();

            vertexCount = tmpVertexIndex.size();
            vertexCount3 = vertexCount * 3;
            vertices = new double[vertexCount3];
            normals = new double[vertexCount3];
            textureCoords = new double[vertexCount * 2];

            for (unsigned int i = 0; i < vertexCount; i++) {
                const unsigned int c = i * 3;
                const unsigned int x = (tmpVertexIndex[i] - 1) * 3;
                vertices[c] = tmpVertices[x];
                vertices[c + 1] = tmpVertices[x + 1];
                vertices[c + 2] = tmpVertices[x + 2];
                
                variablePoint = sqrt((vertices[c] * vertices[c]) + (vertices[c + 1] * vertices[c + 1]) + (vertices[c + 2] + vertices[c + 2]));
                if (variablePoint > farthestPoint)
                    farthestPoint = variablePoint;

            }

            for (unsigned int i = 0; i < vertexCount; i++) {
                const unsigned int c = i * 3;
                const unsigned int x = (tmpNormalIndex[i] - 1) * 3;

                normals[c] = tmpNormals[x];
                normals[c + 1] = tmpNormals[x + 1];
                normals[c + 2] = tmpNormals[x + 2];

            }

            for (unsigned int i = 0; i < vertexCount; i++) {
                const unsigned int c = i * 2;
                const unsigned int x = (tmpTexCoordIndex[i] - 1) * 2;

                textureCoords[c] = tmpTexCoords[x];
                textureCoords[c + 1] = 1.0 - tmpTexCoords[x + 1];
            }

        }
    }

    ///\brief Détermination de la texture du modèle
    ///\param textureId Identifiant de la texture à utiliser
    inline void setTexture(const unsigned int& textureId) {
        this->textureId = textureId;
    }
    ///\brief Affichage du modèle
    void Draw() {

        glBindTexture(GL_TEXTURE_2D, textureId);

        glVertexPointer(3, GL_DOUBLE, 0, vertices);
        glNormalPointer(GL_DOUBLE, 0, normals);
        glTexCoordPointer(2, GL_DOUBLE, 0, textureCoords);

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

     void Transform(Matrix44d matrix) {
        double* tMatrix = matrix.GetMatrix();
        size_t y, z, n = vertexCount * 3;
        double vX, vY, vZ, nX, nY, nZ;

        for (size_t x = 0; x < n; x += 3) {
            // index
            y = x + 1;
            z = x + 2;

            // vertex
            vX = vertices[x] - tMatrix[12];
            vY = vertices[y] - tMatrix[13];
            vZ = vertices[z] - tMatrix[14];

            // rotation
            nX = vX * tMatrix[0] + vY * tMatrix[4] + vZ * tMatrix[8];
            nY = vX * tMatrix[1] + vY * tMatrix[5] + vZ * tMatrix[9];
            nZ = vX * tMatrix[2] + vY * tMatrix[6] + vZ * tMatrix[10];
            
            // stock
            vertices[x] = nX + tMatrix[12] + tMatrix[3];
            vertices[y] = nY + tMatrix[13] + tMatrix[7];
            vertices[z] = nZ + tMatrix[14] + tMatrix[11];

            // normal
            vX = normals[x];
            vY = normals[y];
            vZ = normals[z];

            // rotation & stock
            normals[x] = (vX * tMatrix[0] + vY * tMatrix[4] + vZ * tMatrix[8]);
            normals[y] = (vX * tMatrix[1] + vY * tMatrix[5] + vZ * tMatrix[9]);
            normals[z] = (vX * tMatrix[2] + vY * tMatrix[6] + vZ * tMatrix[10]);
        }
    }

    void Scale(const Vector3d& scaleValue) {
        Matrix44d matrix;
        matrix.LoadIdentity();
        matrix.LoadScale(scaleValue);
        
        vertexCount3 = vertexCount * 3;

        Vector3d convertion;
        for (size_t i = 0; i < vertexCount3; i+=3) {
            i1 = i + 1;
            i2 = i + 2;

            convertion.x = vertices[i];
            convertion.y = vertices[i1];
            convertion.z = vertices[i2];

            convertion = matrix * convertion;

            vertices[i] = convertion.x;
            vertices[i1] = convertion.y;
            vertices[i2] = convertion.z;
        }
    }

    inline double GetFarthestPoint() {
        return farthestPoint;
    }

    const Vector3d* Collision(Vector3d origin, Vector3d segment) const {
        // tous les 3 indices = 1 vecteur // tous les 9 indices = 1 triangle
        // tous les 3 indices = 1 normale // tous les 9 indices = 3 normales
        Vector3d* result = nullptr;
        for (int i = 0; i < vertexCount*3; i+=9) {
            Vector3d triangle[3];
            triangle[0].x = vertices[i]; triangle[0].y = vertices[i+1]; triangle[0].z = vertices[i+2];
            triangle[1].x = vertices[i+3]; triangle[1].y = vertices[i+4]; triangle[1].z = vertices[i+5];
            triangle[2].x = vertices[i+6]; triangle[2].y = vertices[i+7]; triangle[2].z = vertices[i+8];

            Vector3d normal;
            normal.x = normals[i]; normal.y = normals[i+1]; normal.z = normals[i+2];

            Vector3d* tmp = Physic::RayThroughPlane(segment, origin, triangle, normal);
            if (!result) {
                result = tmp;
            }
            else if (tmp && ((*tmp - origin).GetNorm() < (*result - origin).GetNorm())) {
                delete result;
                result = tmp;
            }
        }
        if (result)
            return result;
        return nullptr;
    }

    const std::tuple<Vector3d*, Vector3d> CollisionReturnNormal(Vector3d origin, Vector3d segment) const {
        // tous les 3 indices = 1 vecteur // tous les 9 indices = 1 triangle
        // tous les 3 indices = 1 normale // tous les 9 indices = 3 normales
        Vector3d* result = nullptr;
        Vector3d normal;
        for (int i = 0; i < vertexCount*3; i+=9) {
            Vector3d triangle[3];
            triangle[0].x = vertices[i]; triangle[0].y = vertices[i+1]; triangle[0].z = vertices[i+2];
            triangle[1].x = vertices[i+3]; triangle[1].y = vertices[i+4]; triangle[1].z = vertices[i+5];
            triangle[2].x = vertices[i+6]; triangle[2].y = vertices[i+7]; triangle[2].z = vertices[i+8];

            normal.x = normals[i]; normal.y = normals[i+1]; normal.z = normals[i+2];

            Vector3d* tmp = Physic::RayThroughPlane(segment, origin, triangle, normal);
            if (!result) {
                result = tmp;
            }
            else if (tmp && ((*tmp - origin).GetNorm() < (*result - origin).GetNorm())) {
                delete result;
                result = tmp;
            }
        }
        return std::tuple<Vector3d*, Vector3d>{result, normal};
    }
};

#endif
