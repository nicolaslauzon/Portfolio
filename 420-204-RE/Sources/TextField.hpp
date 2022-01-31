///\file TextField.h

#ifndef TEXTFIELD_HPP
#define TEXTFIELD_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <map>

#include "Label.hpp"
#include "Drawable.hpp"
#include "RectFill.hpp"
#include "Observer.hpp"

using namespace std;

const map<SDL_Keycode,char> keyMap = {
//--------- Letters -------------//
    { SDLK_q,'q' },
    { SDLK_w,'w' },
    { SDLK_e,'e' },
    { SDLK_r,'r' },
    { SDLK_t,'t' },
    { SDLK_y,'y' },
    { SDLK_u,'u' },
    { SDLK_i,'i' },
    { SDLK_o,'o' },
    { SDLK_p,'p' },
    { SDLK_a,'a' },
    { SDLK_s,'s' },
    { SDLK_d,'d' },
    { SDLK_f,'f' },
    { SDLK_g,'g' },
    { SDLK_h,'h' },
    { SDLK_j,'j' },
    { SDLK_k,'k' },
    { SDLK_l,'l' },
    { SDLK_z,'z' },
    { SDLK_x,'x' },
    { SDLK_c,'c' },
    { SDLK_v,'v' },
    { SDLK_b,'b' },
    { SDLK_n,'n' },
    { SDLK_m,'m' },

//--------- Numbers -------------//
    { SDLK_1,'1' },
    { SDLK_2,'2' },
    { SDLK_3,'3' },
    { SDLK_4,'4' },
    { SDLK_5,'5' },
    { SDLK_6,'6' },
    { SDLK_7,'7' },
    { SDLK_8,'8' },
    { SDLK_9,'9' },
    { SDLK_0,'0' },

//--------- Other -------------//
    { SDLK_SPACE,' ' }
    };

///\class TextField
///\brief Zone de saisie de texte 2D openGl
class TextField : public Drawable , public Observer<SDL_Event> {

private:
    bool isFocused_; ///< true si l'utilisateur sélectionne la zone de texte // false quand elle n'est plus sélectionné
    bool canWrite_; ///< true si on peut écrire dans la zone de texte // false si la zone de texte est pleine
    bool uppercaseOn_; ///< true si "shift" est enfoncé // false si "shift" n'est pas enfoncé

    std::string content_; ///< string affiché dans la zone de texte

    bool isHovered_; ///< true -> la souris survole la zone de texte // false -> la souris ne survole pas la zone de texte
    RectFill* hover_; ///< indique que la souris survole le textField

    RectFill* background_; ///< boite de couleur représentant la zone de saisie de texte
    Label* text_; ///< texte affiché dans la boite

    ///\brief enleve des caractères si la string est trop large pour la zone de texte
    ///\return true : Si la méthode a retiré des caractères // false : Si la méthode a rien retiré
    bool RemoveExtra() {
        int wString = 0;
        const char* content = content_.c_str();
        TTF_SizeText(text_->GetFont(), content, &wString, nullptr);

        // Too wide
        if (wString > rect_.w) {
            content_.pop_back();
            // Répète l'opération
            RemoveExtra();
            return true; 
        }
        else {
            return false;
        }
    }

public:

    ///\brief constructeur de base
    ///\param textFieldRect dimensions du rectangle de saisie. 
    ///\param font police de caractère utilisé.
    ///\param textColor couleur du texte.
    ///\param boxColor couleur de la boite de saisie.
    TextField(const SDL_Rect& textFieldRect, TTF_Font* font, const SDL_Color& textColor, const SDL_Color& boxColor) {

        text_ = new Label(textFieldRect, font, textColor, nullptr);
        background_ = new RectFill(textFieldRect, boxColor);

        //ajustement de la hauteur du rectangle de saisie à la hauteur du texte
        int textHeight;
        TTF_SizeText(font, "0", nullptr, &textHeight);
        background_->SetRect({textFieldRect.x, textFieldRect.y, textFieldRect.w, textHeight});
        content_ = "";
        isFocused_ = canWrite_ = uppercaseOn_ = isHovered_ = false;
        rect_ = textFieldRect;
        rect_.h = textHeight;

        hover_ = new RectFill(rect_, {150, 150, 150, 150});
    }

    ~TextField() {
        if (background_) {
            delete background_;
            background_ = nullptr;
        }
        if (text_) {
            delete text_;
            text_ = nullptr;
        }
        if (hover_) {
            delete hover_;
            hover_ = nullptr;
        }
    }

    const string& GetContent() const {
        return content_;
    }

    ///\brief gestion des notifications 
    ///\param sdlEvent évènement sdl2 qui a causé la notification
    void Notification(SDL_Event sdlEvent) {
        bool stringChanged = false;

        SDL_Keycode keyId = sdlEvent.key.keysym.sym;

        // Est-ce que la text box est survolé par le curseur
        SDL_Point click = {sdlEvent.button.x, sdlEvent.button.y};
        isHovered_ = SDL_PointInRect(&click, &rect_);

        // Est-ce que la zone de texte est "focussed" par l'utilisateur
        if (sdlEvent.type == SDL_MOUSEBUTTONUP) {
            if (isHovered_ == true) {
                isFocused_ = true;
            }
            else {
                isFocused_ = false;
            }
        }
        
        // Test des touches shifts
        /*if (keyId == SDLK_RSHIFT || keyId == SDLK_LSHIFT) {
            if (sdlEvent.type == SDL_KEYDOWN) {
                uppercaseOn_ = true;
            }
            else if (sdlEvent.type == SDL_KEYUP) {
                uppercaseOn_ = false;
            }
        }*/

        //Gestion de l'écriture dans la zone de texte
        if (isFocused_ && sdlEvent.type == SDL_KEYUP) {
            auto key = keyMap.find(keyId);
            if (key != keyMap.end()) {
                if (uppercaseOn_) {
                    content_.push_back(toupper(key->second));
                }
                else {
                    content_.push_back(key->second);
                }
                stringChanged = true; 
            }
            // Gestion du backspace 
            else if (sdlEvent.key.keysym.sym == SDLK_BACKSPACE) {
                if (content_ != "") {
                    content_.pop_back();
                    stringChanged = true;
                }   
            }
        }
        if (stringChanged) {
            RemoveExtra();
            if (content_ != "") {
                text_->SetText(content_.c_str());
            }
            else {
                text_->SetText(nullptr);
            }
        }
    }

    ///\brief Dessine le text field
    void Draw() {
        //background_->Draw();
        text_->Draw();
        if (isHovered_) {
            hover_->Draw();
        }
    }
};
#endif // TEXTFIELD_HPP