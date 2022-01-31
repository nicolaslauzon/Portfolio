///\file FileSystem.hpp
///\brief Système de fichiers 

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <string>

using namespace std;

class FileSystem {
public:
  static string fontsPath;  ///< Chemin des polices de caractères
  static string imagesPath; ///< Chemin des images
  static string modelsPath; ///< Chemin des modèles
  static string resourcePath; ///< Chemin de la resourcelist et autre resources
  static string spritePath; ///< Chemin des sprites

  static void setPaths(char* path[]) {
  string execPath(path[0]);
  
#if _WIN32
  while (execPath.back() != '\\')
      execPath.pop_back();

  FileSystem::resourcePath = execPath + "Resources\\";
  FileSystem::fontsPath = execPath + "Resources\\Fonts\\";
  FileSystem::imagesPath = execPath + "Resources\\Images\\";
  FileSystem::modelsPath = execPath + "Resources\\Obj\\";
  FileSystem::spritePath = execPath + "Resources\\Obj\\Sprites\\";
#else
  while (execPath.back() != '/')
      execPath.pop_back();
  FileSystem::resourcePath = execPath + "Resources/";
  FileSystem::fontsPath = execPath + "Resources/Fonts/";
  FileSystem::imagesPath = execPath + "Resources/Images/";
  FileSystem::modelsPath = execPath + "Resources/Obj/";
  FileSystem::spritePath = execPath + "Resources/Obj/Sprites/";
#endif
    
  }
};


string FileSystem::resourcePath = "";
string FileSystem::fontsPath = "";
string FileSystem::imagesPath = "";
string FileSystem::modelsPath = "";
string FileSystem::spritePath = "";

#endif
