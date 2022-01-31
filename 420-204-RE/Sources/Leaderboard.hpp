///\file Leaderboard.hpp
///\brief Leaderboard pour les meilleurs joueurs avec leur temps
#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

#include <fstream>

#include <string>
#include <iostream>
#include <stdio.h>
#include <list>
#include <chrono>
#include "LabelList.hpp"

using namespace std;


///\class Leaderboard
///\brief Affichage de texte
class Leaderboard {
protected:
    string filename_; ///\ nom du fichier texte
    list<pair<string, double>> listplayers_; ///\ liste des noms des joueurs avec leur temps
    fstream varfichier_; ///\ fichier texte
    bool needupdate_; ///\ Si le fichier a besoin d'etre reecrit

public:

    ///\param filename nom du fichier texte
    Leaderboard(string filename) {
        needupdate_ = false;
        filename_ = filename;
        varfichier_.open(filename, ios::in);
        if (varfichier_) {
            string line_;
            string playername_;
            string playertime_;
            while (getline(varfichier_, line_)) {
                if (((line_[0] >= 'a') && (line_[0] <= 'z')) || (line_[0] >= 'A') && (line_[0] <= 'Z')) {
                    playername_ = line_;
                }
                else {
                    playertime_ = line_;
                    listplayers_.push_back(make_pair(playername_, (stod(playertime_))));
                }
            }
            varfichier_.close();
        }
    }

    ~Leaderboard() {


    }

    ///\brief Ajoute un score
    ///\param playername_ nom du joueur a ajouter
    ///\param time_ temps a ajoute
    void AddScore(string playername, double time) {
        listplayers_.push_back(make_pair(playername, time));
        needupdate_ = true;
    }

    ///\brief Met en ordre la liste
    void Rank() {
        listplayers_.sort([](const pair<string, double>& a, const pair<string, double>& b)
            { return a.second < b.second; });
    }

    ///\brief Prend la liste la met en ordre puis l'ecrit dans le fichier texte
    void Reset() {
   // C:\Users\eliet\Desktop\ToutProg\Programmation\ProgramSession4\PI2021\x64\Debug\Resources\Leaderboard.txt
        if (needupdate_) {
            varfichier_.open(filename_, ios::out);
            needupdate_ = false;
            Rank();
            while (listplayers_.size()) {
                varfichier_ << listplayers_.front().first << "\n";
                varfichier_ << to_string(listplayers_.front().second) << "\n";
                listplayers_.pop_front();
            }
        }
        varfichier_.close();
    }
    

    /*inline list<pair<string, double>> GetList() {
        return listplayers_;
    }

    inline list<string> GetListTime() {
        list<string>tmplist;
        for (auto it : listplayers_) {
            tmplist.push_back(to_string(it.second));
        }
        return tmplist;
    }

    inline list<string> GetListName() {
        list<string>tmplist;
        for (auto it : listplayers_) {
            tmplist.push_back(it.first);
        }
        return tmplist;
    }*/


    ///\brief retourne le nom a la position choisie
    ///\param position position choisie
    ///\return le nom a la position choisie
    string GetName(unsigned int position) {
        list<pair<string, double>>::iterator it = listplayers_.begin();
        advance(it, position - 1);
        return (it->first);
    }


    ///\brief retourne le temps a la position choisie
    ///\param position position choisie
    ///\return le temps a la position choisie
    double GetTime(unsigned int position) {
        list<pair<string, double>>::iterator it = listplayers_.begin();
        advance(it, position - 1);
        return (it->second);
    }

    ///\brief retourne le nombre d'elements sur la liste
    ///\return le nombre d'elements sur la liste
    inline unsigned int GetNumberofPlayers() {
        return listplayers_.size();
    }
};


#endif