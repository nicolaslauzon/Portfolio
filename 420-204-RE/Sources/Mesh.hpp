#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include <fstream>
#include <vector>
#include "Vector.hpp"
#include "Resource.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
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

public:

    TexturedMesh() {}
    TexturedMesh(TexturedMesh* texturedMesh) {
      if (texturedMesh) {
        this->farthestPoint = texturedMesh->farthestPoint;
        this->vertexCount = texturedMesh->vertexCount;

        this->vertices = new double[this->vertexCount * 3];
        this->normals = new double[this->vertexCount * 3];
        this->textureCoords = new double[this->vertexCount * 2];

        copy(&texturedMesh->vertices[0], &texturedMesh->vertices[this->vertexCount * 3], this->vertices);
        copy(&texturedMesh->normals[0], &texturedMesh->normals[this->vertexCount * 3], this->normals);
        copy(&texturedMesh->textureCoords[0], &texturedMesh->textureCoords[this->vertexCount * 2], this->textureCoords);
      }
    }

    void* GetData() {
      return this;
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
            vertices = new double[vertexCount * 3];
            normals = new double[vertexCount * 3];
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
        bool transformationCentered = true;
        if (matrix.GetMatrix()[12] != 0.0 || matrix.GetMatrix()[13] != 0.0 ||matrix.GetMatrix()[14] != 0.0) {
            transformationCentered = false;
        }

        Vector3d convertion;
        for (size_t i = 0; i < vertexCount*3; i+=3) {
            //vertices ------------------
            convertion.x = vertices[i];
            convertion.y = vertices[i+1];
            convertion.z = vertices[i+2];

            // offset
            if (transformationCentered == false) {
                convertion.x -= matrix.GetMatrix()[12];
                convertion.y -= matrix.GetMatrix()[13];
                convertion.z -= matrix.GetMatrix()[14];
            }

            // rotation
            convertion = matrix * convertion;

            // remise en place
            if (transformationCentered == false) {
                convertion.x += matrix.GetMatrix()[12];
                convertion.y += matrix.GetMatrix()[13];
                convertion.z += matrix.GetMatrix()[14];
            }

            // translation
            convertion.x += matrix.GetMatrix()[3];
            convertion.y += matrix.GetMatrix()[7];
            convertion.z += matrix.GetMatrix()[11];
            
            // stockage
            vertices[i] = convertion.x;
            vertices[i+1] = convertion.y;
            vertices[i+2] = convertion.z;

            //normals -------------------
            convertion.x = normals[i];
            convertion.y = normals[i+1];
            convertion.z = normals[i+2];

            // rotation
            convertion = matrix * convertion;

            normals[i] = convertion.x;
            normals[i+1] = convertion.y;
            normals[i+2] = convertion.z;
        }
    }

    void Scale(const Vector3d& scaleValue) {
        Matrix44d matrix;
        matrix.LoadIdentity();
        matrix.LoadScale(scaleValue);

        Vector3d convertion;
        for (size_t i = 0; i < vertexCount*3; i+=3) {
            convertion.x = vertices[i];
            convertion.y = vertices[i+1];
            convertion.z = vertices[i+2];

            convertion = matrix * convertion;

            vertices[i] = convertion.x;
            vertices[i+1] = convertion.y;
            vertices[i+2] = convertion.z;
        }
    }

    inline double GetFarthestPoint() {
        return farthestPoint;
    }
};

#endif
