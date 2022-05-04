#include "game.h"
#include <algorithm>

Game::Game(SDL_Renderer *sdl_renderer, std::unique_ptr<Renderer> renderer, std::unique_ptr<Player> player):
_sdl_renderer(sdl_renderer), _player(std::move(player)),_renderer(std::move(renderer)){

	_ammo[WPN_PISTOL] = 12;
	_ammo[WPN_UZI] = 100;
	_enemy_spawn_timer = 0;
	_player_die = false;
	_timeout = FPS * 4;
	_title = std::make_unique<Title>(sdl_renderer);
	_new_highscore = nullptr;

	for(int i=0;i<(NUM_HIGHSCORES + 1);i++)
		_high_scores.emplace_back(std::move(Highscore()));
}

Game::~Game() {

	std::cout << "Game Destructor called" << std::endl;
	//clear all internal vectors
	_bullets.clear();
	_effects.clear();
	_enemies.clear();
	_uzi_powerups.clear();
	_health_powerups.clear();
	_high_scores.clear();
}

/*****************************************************************
 * 
 * Implementation of function templates
 * 
 * ***************************************************************/
template<typename T>
void Game::updatePowerups(T &powerups) {

	for(auto &p : powerups) {

		auto hp = p.get();
		hp->tick();
		hp->x += hp->dx;
		hp->y += hp->dy;
		touchOthers(hp);
		hp->reload = MAX(hp->reload - 1, 0);
		hp->health--;
	}
	
	powerups.erase(std::remove_if(powerups.begin(), powerups.end(), [](auto &pu){ 
        return ((--pu->health) <= 0);}), powerups.end());
}

template <typename T> 
void Game::touchOthers(T *source) {

	int distance = Util::getDistance(source->x, source->y, _player->x, _player->y);
	if (distance < (_player->radius + source->radius)){
		source->touch(_player.get());
	}
}

template<typename T> 
void Game::drawItems(T &list) {

	for(auto &entity : list) {
		// SDL_SetTextureColorMod(entity->texture, entity->color.r, entity->color.g, entity->color.b);
        Renderer::blitRotated(_sdl_renderer, entity->texture, entity->x, entity->y, entity->angle);
    }
}

/**************************************************************
 * 
 * Implementation of the game logic loop
 * 
 * ************************************************************/
void Game::begin() {

	float remainder = 0;
	long then = SDL_GetTicks();
	resetGameInternals();
	while(true) {

		_renderer->prepareScene();
		_title->tuneTitle();
		_title->displayTitle();
		_player->handleInput();
		_renderer->presentScene();
		capFrameRate(then, remainder);

		if(_player->getMouse().button[SDL_BUTTON_LEFT])
			break;
	}
}

void Game::run(){

    float remainder = 0;
	/* Get the number of milliseconds since SDL library initialization. */
    long then = SDL_GetTicks();

    while(true){
	
		if(!_player_die)
			gameFlowWithPlayer(then, remainder);
		else{
			gameFlowWithoutPlayer(then, remainder);
			break;
		}
	}
	addHighscore(_player->score);
}

bool Game::endWithRecord() {

	bool new_record = false;
	float remainder = 0;
	/* Get the number of milliseconds since SDL library initialization. */
    long then = SDL_GetTicks();

	while(true) {

		//update rendering
		_renderer->prepareScene();
		if(_new_highscore){
			_new_highscore->drawHighScoreName(_new_highscore->name,_sdl_renderer,_cursorBlink);
			new_record = true;
		}
		else{
			drawHighScores();
			if (_timeout % 40 < 20){
				Renderer::drawText(_sdl_renderer,SCREEN_WIDTH / 2, 600, 255, 255, 255, TEXT_CENTER, (char *)"PRESS FIRE TO PLAY!");
			}
		}
		_renderer->presentScene();
		//adjust the frame rate
		capFrameRate(then, remainder);

		//update logic 
		_player->resetInputText();
		_player->handleInput();
		if(_new_highscore){
			if(!_new_highscore->updateHighScoreName(_new_highscore->name ,_player->getUserInput(), _player->getKeyboard()))
				_new_highscore = nullptr;
		}
		else{
			_timeout--;
			if(_timeout <= 0)
				break;
		}

		if (++_cursorBlink >= FPS){
			_cursorBlink = 0;
		}
	}
	resetGameInternals();
	return new_record;
}

/**************************************************************
 * 
 * Implementation of private member functions
 * 
 * ************************************************************/
void Game::drawHighScores() {
	
	int i, y, r, g, b;
	i = 0;
	y = 150;
	
	Renderer::drawText(_sdl_renderer,SCREEN_WIDTH / 2, 70, 255, 255, 255, TEXT_CENTER, (char *)"HIGHSCORES");

	for(auto &curr_score : _high_scores){
		r = 255;
		g = 255;
		b = curr_score.recent == 1 ? 0 : 255;

		Renderer::drawText(_sdl_renderer, SCREEN_WIDTH / 2, y, r, g, b, TEXT_CENTER, (char *)"#%d. %-15s ...... %05d", i++, 
		curr_score.name.c_str(), curr_score.score);

		y += 50;
	}
}

void Game::addHighscore(int score){
	
	//set all previous high scores as old score
	for(auto &score : _high_scores)
		score.recent = 0;
	//set the new high score as the recent high score
	_high_scores[NUM_HIGHSCORES].score = score;
	_high_scores[NUM_HIGHSCORES].recent = 1;
	//sort in descending order
	std::sort(_high_scores.begin(), _high_scores.end(), Util::highscoreComparator);

	//update _new_highscore
	auto new_highscore = std::find_if(_high_scores.begin(), _high_scores.end(), [](auto &score) { return score.recent == 1;});
	if(new_highscore == _high_scores.begin())
		_new_highscore = &(*new_highscore);
	else
		_new_highscore = nullptr;
}

void Game::resetGameInternals() {

	//clear all internal vectors
	_bullets.clear();
	_effects.clear();
	_enemies.clear();
	_uzi_powerups.clear();
	_health_powerups.clear();

	_player_die = false;
	_ammo[WPN_PISTOL] = 12;
	_ammo[WPN_UZI] = 100;
	_cursorBlink = 0;
	_timeout = FPS * 4;
	_player->health = 5;
	_player->score = 0;
	_player->weaponType = WPN_PISTOL;
	_enemy_spawn_timer = 0;
}

void Game::gameFlowWithPlayer(long &then, float &remainder){

	_renderer->prepareScene();
	_player->handleInput();
	_player->updatePlayerPosition(); 
	addBullets(_player->updatePlayerAmmo(_ammo));

	//updating all objects
	updatePlayer();
	updateBullets();
	updatePowerups(_health_powerups);
	updatePowerups(_uzi_powerups);
	updateEnemies();
	updateEffects();
	spawnEnemy();

	//rendering all objects
	_player->draw();
	drawItems(_bullets);
	drawItems(_enemies);
	drawEffects();
	drawItems(_health_powerups);
	drawItems(_uzi_powerups);

	_renderer->drawGameScreen(_player->getMouse(), _ammo, _player->health, _player->score, _player->weaponType);
	_renderer->presentScene();
	//adjust the frame rate
	capFrameRate(then, remainder);
}

void Game::gameFlowWithoutPlayer(long &then, float &remainder){

	_renderer->prepareScene();
	_player->handleInput();
	updateEffects();
	updateEnemies();
	drawItems(_enemies);
	drawEffects();

	_renderer->drawGameScreen(_player->getMouse(), _ammo, _player->health, _player->score, _player->weaponType);
	_renderer->presentScene();
	//adjust the frame rate
	capFrameRate(then, remainder);
}

void Game::capFrameRate(long &then, float &remainder){

    long wait, frameTime;

	wait = 16 + remainder;
	remainder -= (int)remainder;
	frameTime = SDL_GetTicks() - then;
	wait -= frameTime;
	
	if (wait < 1){
		wait = 1;
	}
		
	SDL_Delay(wait);
	
	remainder += 0.667;
	then = SDL_GetTicks();
}

void Game::addBullets(std::unique_ptr<Bullet> bullet) {

    if(bullet == nullptr)
        return;
    
    _bullets.emplace_back(std::move(bullet));
}

void Game::updateBullets() {

    for(auto &bullet : _bullets) {
        bullet->x += bullet->dx;
        bullet->y += bullet->dy;

		if(bullet->side == SIDE_PLAYER)
			playerBulletHitEnemies(bullet.get());
		else
			enemyBulletHitPlayer(bullet.get());
		
		--bullet->health;
    }
    _bullets.erase(std::remove_if(_bullets.begin(), _bullets.end(), [](std::unique_ptr<Bullet> &bullet){ 
        return ((bullet->health) <= 0);}), _bullets.end());
}

void Game::updatePlayer() {

	_player->x += _player->dx;
    _player->y += _player->dy;
    _player->reload = MAX(_player->reload - 1, 0);

	if(_player->health > 0){
		// prevent the player icon disappearing from the screen
		_player->x = MIN(MAX(_player->x, _player->w / 2), SCREEN_WIDTH - _player->w / 2);
		_player->y = MIN(MAX(_player->y, _player->h / 2), SCREEN_HEIGHT - _player->h / 2);
	}
	else{
		_player_die = true;
		addDonkDeathEffect();
	}
}

void Game::updateEffects(){

	for(auto &e : _effects){

		auto effect = e.get();
		effect->x += effect->dx;
		effect->y += effect->dy;
		effect->color.a = MAX(effect->color.a - 1, 0);
		--effect->life;
	}

	_effects.erase(std::remove_if(_effects.begin(), _effects.end(), [](auto &effect){ 
        return ((effect->life) <= 0);}), _effects.end());
}

void Game::updateEnemies() {

	for(auto &e : _enemies) {

        auto enemy = e.get();
		auto eb = enemy->tick(_player->x, _player->y);
		if(eb)
			_bullets.emplace_back(std::move(eb));
        enemy->x += enemy->dx;
        enemy->y += enemy->dy;

        enemy->reload = MAX(enemy->reload - 1, 0);
		if(enemy->health <= 0){
			_player->score += 1;
			addEnemyDeathEffect(enemy);
			if (rand() % 2 == 0) 
				addRandomPowerup(enemy->x, enemy->y); //add powerup as reward for killing enemies
		}
    }
	 _enemies.erase(std::remove_if(_enemies.begin(), _enemies.end(), [](auto &e){ 
    	return ((e->health) <= 0);}), _enemies.end());
}

void Game::enemyBulletHitPlayer(Bullet *b){

	if(Util::getDistance(_player->x, _player->y, b->x, b->y) < (_player->radius + b->radius)){
		b->health = 0;
		_player->health--;
		addBulletHitEffect(b);
		return;
	}
}

void Game::playerBulletHitEnemies(Bullet *b){

	int distance;
	for(auto &e : _enemies) {
		distance = Util::getDistance(e->x, e->y, b->x, b->y);
		if (distance < (e->radius + b->radius)){
			b->health = 0;
			e->health--;
			addBulletHitEffect(b);
			return;
		}
	}
}

void Game::spawnEnemy(){

	int x, y;
	
	if (--_enemy_spawn_timer <= 0)
	{
		switch (rand() % 4)
		{
			case 0:
				x = -100;
				y = rand() % SCREEN_HEIGHT;
				break;
				
			case 1:
				x = SCREEN_WIDTH + 100;
				y = rand() % SCREEN_HEIGHT;
				break;
				
			case 2:
				x = rand() % SCREEN_WIDTH;
				y = -100;
				break;
				
			case 3:
				x = rand() % SCREEN_WIDTH;
				y = SCREEN_HEIGHT + 100;
				break;
		}
		
		//add enemy
		_enemies.emplace_back(std::move(std::make_unique<Enemy>(x, y, _sdl_renderer)));
		//update spanw timer
		_enemy_spawn_timer = FPS + (rand() % FPS);
	}
}

void Game::addRandomPowerup(int x, int y) {
	
	if(rand() % 5 == 0){
		// update healthpowerup
		_health_powerups.emplace_back(std::move(std::make_unique<HealthPowerup>(x,y,_sdl_renderer)));
	}
	else{ 
		//update uzipowerup
		_uzi_powerups.emplace_back(std::move(std::make_unique<UziPowerup>(x,y,_sdl_renderer, _ammo)));
	}
}

void Game::addEnemyDeathEffect(Enemy *enemy){

	for(int i = 0; i < DEATH_EFFECT_RANGE; i++) {
		_effects.emplace_back(std::move(std::make_unique<Effect>(_sdl_renderer, enemy->x, enemy->y, SIDE_ENEMY)));
	}
}

void Game::addDonkDeathEffect() {

	for(int i = 0; i < DEATH_EFFECT_RANGE; i++) {

		auto effect = std::make_unique<Effect>(_sdl_renderer, _player->x, _player->y, SIDE_PLAYER);
		effect->color.r = rand() % 128;
		effect->color.g = 128 + rand() % 128;
		effect->color.b = 255;
		effect->color.a = rand() % 255;
		_effects.emplace_back(std::move(effect));
	}
}

void Game::drawEffects(){
	
	SDL_SetRenderDrawBlendMode(_sdl_renderer, SDL_BLENDMODE_ADD);
	for(auto &e : _effects) {

		SDL_SetTextureBlendMode(e->texture, SDL_BLENDMODE_ADD);
		SDL_SetTextureColorMod(e->texture, e->color.r, e->color.g, e->color.b);
		SDL_SetTextureAlphaMod(e->texture, e->color.a);
		
		Renderer::blit(_sdl_renderer,e->texture, e->x, e->y, 1);
	}
	SDL_SetRenderDrawBlendMode(_sdl_renderer, SDL_BLENDMODE_NONE);
}

void Game::addBulletHitEffect(Bullet *bullet){

	for(int i = 0; i < BULLET_HIT_RANGE; i++) {

		auto effect = std::make_unique<Effect>(_sdl_renderer, bullet->x, bullet->y, SIDE_PLAYER);
		effect->color = bullet->color;
		effect->color.a = rand() % 255;
		_effects.emplace_back(std::move(effect));
	}
}

