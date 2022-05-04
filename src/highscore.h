#ifndef _HIGHSCORE_H_
#define _HIGHSCORE_H_

#include <string>
#include "defs.h"
#include "SDL.h"

class Highscore {

    public:
        Highscore();
        ~Highscore();
        Highscore(Highscore const &source);
        Highscore(Highscore &&source);
        Highscore &operator=(Highscore const &source);
        Highscore &operator=(Highscore &&source);
        int updateHighScoreName(std::string &current_score_name,std::string input_text, int *keyboard);
        void drawHighScoreName(std::string name, SDL_Renderer *sdl_renderer, int cursorBlink);
        std::string name;
        int recent;
        int score;
};

#endif
