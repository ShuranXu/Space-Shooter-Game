#include "highscore.h"
#include "renderer.h"


Highscore::Highscore() {

    name = std::string();
    score = 0;
    recent = 0;
}

Highscore::~Highscore() {
    
}

Highscore::Highscore(Highscore const &source) {

    name = source.name;
    score = source.score;
    recent = source.recent;
}

Highscore::Highscore(Highscore &&source) {

    name = source.name;
    score = source.score;
    recent = source.recent;

    source.name = {0};
    source.score = 0;
    source.recent = 0;
}

Highscore& Highscore::operator=(const Highscore &source){

    if(this == &source)
        return *this;

    name = std::string(source.name);
    score = source.score;
    recent = source.recent;
    return *this;
}

Highscore& Highscore::operator=(Highscore &&source) {

    if(this == &source)
        return *this;

    name = std::string(source.name);
    score = source.score;
    recent = source.recent;

    source.name = {0};
    source.score = 0;
    source.recent = 0;
    return *this;
}

int Highscore::updateHighScoreName(std::string &current_score_name,std::string input_text, int *keyboard) {

	int i, n;
	char c;

    for(i=0;i<input_text.length();i++){
        c = toupper(input_text[i]);
        current_score_name += c;
    }

    n = current_score_name.length();

	if (n > 0 && keyboard[SDL_SCANCODE_BACKSPACE]){

        current_score_name.resize(n-1);
		keyboard[SDL_SCANCODE_BACKSPACE] = 0;
	}
	
	else if (keyboard[SDL_SCANCODE_RETURN]){

        if(current_score_name.length() == 0)
            current_score_name = std::string("ANONYMOUS");
        return 0;
	}
    
    return 1;
} 

void Highscore::drawHighScoreName(std::string name, SDL_Renderer *sdl_renderer, int cursorBlink) {

	SDL_Rect r;
	
	Renderer::drawText(sdl_renderer, SCREEN_WIDTH / 2, 70, 255, 255, 255, TEXT_CENTER, (char *)"CONGRATULATIONS, YOU'VE GAINED A HIGHSCORE!");
	Renderer::drawText(sdl_renderer, SCREEN_WIDTH / 2, 120, 255, 255, 255, TEXT_CENTER, (char *)"ENTER YOUR NAME BELOW:");
	Renderer::drawText(sdl_renderer, SCREEN_WIDTH / 2, 250, 128, 255, 128, TEXT_CENTER, (char *)name.c_str());
	
	if (cursorBlink < (FPS >> 1)){

		r.x = ((SCREEN_WIDTH >> 1) + ((name.length() * GLYPH_WIDTH) >> 1)) + 5;
		r.y = 250;
		r.w = GLYPH_WIDTH;
		r.h = GLYPH_HEIGHT;
		
		SDL_SetRenderDrawColor(sdl_renderer, 0, 255, 0, 255);
		SDL_RenderFillRect(sdl_renderer, &r);
	}

	Renderer::drawText(sdl_renderer, SCREEN_WIDTH / 2, 625, 255, 255, 255, TEXT_CENTER, (char *)"PRESS RETURN WHEN FINISHED");
}
