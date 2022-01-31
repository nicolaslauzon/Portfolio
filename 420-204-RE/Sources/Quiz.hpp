///\file Quiz.hpp

#ifndef QUIZ_HPP
#define QUIZ_HPP

#include "QuestionMath.hpp"
#include "Label.hpp"
#include "ResourceManager.hpp"
#include "Application.hpp"
#include "Character.hpp"
#include <random>
#include <algorithm>
#include<fstream>

///\class QuestionQuiz
///\brief Une question de quiz
class QuestionQuiz {
private:
    Label* options_ = nullptr;
    Label* question_ = nullptr;
    int answer_;
    int sizeY_;

    Label* GenerateLabel(const string& answers, int y) {
        TTF_Font* font = ResourceManager::GetResource<TTF_Font*>("font40");
        const char* text = answers.c_str();
        int w, h;
        TTF_SizeText(font, text, &w, &h);
        if (w <= Application::GetInstance()->GetWindow()->GetSize().x) {
            sizeY_ += h;
            return new Label({0, y, w, h}, font, {255, 255, 255, 255}, text);
        }
        font = ResourceManager::GetResource<TTF_Font*>("font30");
        TTF_SizeText(font, text, &w, &h);
        if (w <= Application::GetInstance()->GetWindow()->GetSize().x) {
            sizeY_ += h;
            return new Label({0, y, w, h}, font, {255, 255, 255, 255}, text);
        }
        font = ResourceManager::GetResource<TTF_Font*>("font20");
        TTF_SizeText(font, text, &w, &h);
        if (w <= Application::GetInstance()->GetWindow()->GetSize().x) {
            sizeY_ += h;
            return new Label({0, y, w, h}, font, {255, 255, 255, 255}, text);
        }
        font = ResourceManager::GetResource<TTF_Font*>("font10");
        TTF_SizeText(font, text, &w, &h);
        if (w <= Application::GetInstance()->GetWindow()->GetSize().x) {
            sizeY_ += h;
            return new Label({0, y, w, h}, font, {255, 255, 255, 255}, text);
        }
        font = ResourceManager::GetResource<TTF_Font*>("font5");
        TTF_SizeText(font, text, &w, &h);
        if (w <= Application::GetInstance()->GetWindow()->GetSize().x) {
            sizeY_ += h;
            return new Label({0, y, w, h}, font, {255, 255, 255, 255}, text);
        }
        font = ResourceManager::GetResource<TTF_Font*>("font3");
        TTF_SizeText(font, text, &w, &h);
        if (w <= Application::GetInstance()->GetWindow()->GetSize().x) {
            sizeY_ += h;
            return new Label({0, y, w, h}, font, {255, 255, 255, 255}, text);
        }
        font = ResourceManager::GetResource<TTF_Font*>("font1");
        TTF_SizeText(font, text, &w, &h);
        if (w <= Application::GetInstance()->GetWindow()->GetSize().x) {
            sizeY_ += h;
            return new Label({0, y, w, h}, font, {255, 255, 255, 255}, text);
        }
        return nullptr;
    }

    void GenerateAnswer(int answer) {
        answer_ = answer + 48;
    }

public:
    QuestionQuiz(unsigned int complexityMath) {
        sizeY_ = 0;
        Question answer = QuestionMath::GenerateQuestion(complexityMath);
        question_ =  GenerateLabel(std::get<0>(answer), sizeY_);
        // 5 options de réponses
        vector<int> options;
        int reponse = std::get<1>(answer);
        options.push_back(reponse);
        if (reponse == 0)
            reponse = QuestionMath::GenerateRandomInt(50, 1);
        for (int i = 0; i < 4; ++i) {
            options.push_back(QuestionMath::GenerateRandomInt(reponse, 0));
        }
        random_device rd;
        mt19937 g(rd());
        shuffle(options.begin(), options.end(), g);

        int i = 1;
        string answers;

        for (int it : options) {
            answers += to_string(i);
            answers += ") ";
            answers += to_string(it);
            answers += "   ";
            if (it == std::get<1>(answer)) {
                GenerateAnswer(i);
            }
            ++i;
        }
        options_ = GenerateLabel(answers, sizeY_);
    }

    QuestionQuiz(const char* fileName) {
        sizeY_ = 0;
        ifstream ifs(fileName);
        if (ifs.is_open()) {
            string question;
            string options;
            int answer;
            int nombreDeQuestions = 0;
            string line;
            while (!ifs.eof()) {
                getline(ifs, line);
                ++nombreDeQuestions;
            }
            nombreDeQuestions /= 3;
            const int questionChoisie = QuestionMath::GenerateRandomInt(nombreDeQuestions, 0)*3;
            ifs.seekg(0, ifs.beg);
            for (int i = 0; i < questionChoisie; ++i) {
                getline(ifs, line);
            }
            getline(ifs, question);
            getline(ifs, options);
            ifs >> answer;
            GenerateAnswer(answer);
            question_ = GenerateLabel(question, sizeY_);
            options_ = GenerateLabel(options, sizeY_);
            ifs.close();
        }
    }

    ~QuestionQuiz() {
        if (options_) {
            delete options_;
            options_ = nullptr;
        }
        if (question_) {
            delete question_;
            question_ = nullptr;
        }
    }

    void Draw() {
        question_->Draw();
        options_->Draw();
    }

    int GetAnswer() {
        return answer_;
    }

    int GetSizeY() {
        return sizeY_;
    }

};

class Door : public Objet3dDrawable {
private:
    bool isLocked_ = true;
    double lock_;
public:
    Door(Vector3d pos) : Objet3dDrawable(ResourceManager::GetResource<TexturedMesh*>("door"), *ResourceManager::GetResource<unsigned int*>("doorT")) {
        Translate(pos);
        lock_ = pos.x; 
    }

    void HandleUpdate(Character* charac) {
        const Vector3d v = charac->GetSpeed()->Normalized();
        bool condition = isLocked_ &&
                         v.x >= 0.0 &&
                         mesh_->Collision(charac->GetPosition().GetOrigin(), v);
        if (condition) {
            Vector3d newPos = charac->GetPosition().GetOrigin();
            newPos.x = lock_;
            Matrix44d m;
            m.LoadIdentity();
            m.Translate(newPos - charac->GetPosition().GetOrigin());
            charac->Transform(m);
            charac->SetPosition(charac->GetPosition().GetOrigin() + m.GetTranslation());
            charac->SetSpeed(Vector3d());
        }
    }

    void UnlockDoor() {
        isLocked_ = false;
    }

};

enum QuizStatus {
    NOTACTIVE = 0,
    Q1 = 1,
    Q2 = 2,
    Q3 = 3,
    Q4 = 4,
    Q5 = 5
};

///\class Quiz
///\brief Quiz de 5 questions
class Quiz : public Observer<SDL_Event> {
private:
    unsigned int status_;
    bool questionAnswered_[5] = {0, 0, 0, 0, 0};
    vector<QuestionQuiz*> questions_;
    list<Door*> doors_;

    bool cooldownOn_ = false;
    double cooldownBuffer_;
    Label* cooldown_;
    string tryAgain_ = "Try again: ";
public:
    Quiz() {
        QuestionQuiz* tmp;
        tmp = new QuestionQuiz(5);
        questions_.push_back(tmp);
        tmp = new QuestionQuiz(10);
        questions_.push_back(tmp);

        const char* filename = "QuestionsProg.txt";
        tmp = new QuestionQuiz(filename);
        questions_.push_back(tmp);
        tmp = new QuestionQuiz(filename);
        questions_.push_back(tmp);
        tmp = new QuestionQuiz(filename);
        questions_.push_back(tmp);

        doors_.push_back(new Door(Vector3d(4.0, 1.0, 0.0)));
        doors_.push_back(new Door(Vector3d(12.0, 1.0, 0.0)));
        doors_.push_back(new Door(Vector3d(32.0, 1.0, 0.0)));
        doors_.push_back(new Door(Vector3d(40.0, 1.0, 0.0)));
        doors_.push_back(new Door(Vector3d(60.0, 1.0, 0.0)));

        random_device rd;
        mt19937 g(rd());
        shuffle(questions_.begin(), questions_.end(), g);
        status_ = Q1;
        cooldownBuffer_ = 0.0;
        TTF_Font* font = ResourceManager::GetResource<TTF_Font*>("font20");
        int w, h;
        const double d = 2.0 - cooldownBuffer_;
        const string txt = (tryAgain_ + to_string(d));
        const char* ctxt = txt.c_str();
        TTF_SizeText(font, ctxt, &w, &h);
        cooldown_ = new Label({0, questions_.at(status_-1)->GetSizeY(), w, h}, font, {255, 255, 255, 255}, ctxt);
    }

    ~Quiz() {
        for (auto it : doors_) {
            delete it;
        }
        for (auto it : questions_) {
            delete it;
        }
        delete cooldown_;
    }

    bool GetQuestionStatus(int i) {
        return questionAnswered_[i];
    }

    void HandleUpdate(double deltaTime, Character* charac) {
        int i = 0;
        for (auto it : doors_) {
            if (questionAnswered_[i] == true) {
                it->UnlockDoor();
            }
            it->HandleUpdate(charac);
            ++i;
        }
        if (cooldownOn_) {
            // compute cooldown
            cooldownBuffer_ += deltaTime;
            if (cooldownBuffer_ <= 2.0) {
                TTF_Font* font = ResourceManager::GetResource<TTF_Font*>("font20");
                int w, h;
                const double d = 2.0 - cooldownBuffer_;
                const string txt = (tryAgain_ + to_string(d));
                const char* ctxt = txt.c_str();
                TTF_SizeText(font, ctxt, &w, &h);
                cooldown_->SetRect({0, questions_.at(status_-1)->GetSizeY(), w, h});
                cooldown_->SetText(ctxt);
            }
            else {
                cooldownBuffer_ = 0.0;
                cooldownOn_ = false;
            }
        }
    }

    void Notification(SDL_Event event) {
        if (status_ != NOTACTIVE) {
            unsigned int key = event.key.keysym.sym;
            if (key >= '0' && key <= '9') {
                if ((cooldownOn_ == false) && key == questions_.at(status_-1)->GetAnswer()) {
                    // Bonne réponse
                    questionAnswered_[status_-1] = true;
                    ++status_;
                    status_ %= 6;
                    cout << status_ << endl;
                }
                else {
                    cooldownOn_ = true;
                }
            }
        }
    }

    void Draw2D() {
        if (status_ != NOTACTIVE) {
            questions_.at(status_-1)->Draw();
        }
        if (cooldownOn_) {
            cooldown_->Draw();
        }
    }

    void Draw3D() {
        for (auto it : doors_) {
            it->Draw();
        }
    }
};
#endif