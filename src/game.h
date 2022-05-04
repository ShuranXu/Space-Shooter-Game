
#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "defs.h"
#include "renderer.h"
#include "player.h"
#include "bullet.h"
#include "powerup.h"
#include "enemy.h"
#include "effect.h"
#include "title.h"
#include "highscore.h"
#include <memory>

class Game {

    public:
        Game(SDL_Renderer *sdl_renderer, std::unique_ptr<Renderer> renderer, std::unique_ptr<Player> player);
        ~Game();
        void run();
        void begin();
        bool endWithRecord();
        int _ammo[WPN_MAX];

    private:
        std::vector<std::unique_ptr<Bullet>> _bullets;
        std::vector<std::unique_ptr<UziPowerup>> _uzi_powerups;
        std::vector<std::unique_ptr<HealthPowerup>> _health_powerups;
        std::vector<std::unique_ptr<Enemy>> _enemies;
        std::vector<std::unique_ptr<Effect>> _effects;
        std::vector<Highscore> _high_scores;
        std::unique_ptr<Renderer> _renderer;
        int _enemy_spawn_timer;
        SDL_Renderer *_sdl_renderer;
        std::unique_ptr<Player> _player; 
        std::unique_ptr<Title> _title;
        bool _player_die;
        int _cursorBlink;
        int _timeout;
        Highscore *_new_highscore;

        void gameFlowWithPlayer(long &then, float &remainder); 
        void gameFlowWithoutPlayer(long &then, float &remainder);
        void resetGameInternals();
        void capFrameRate(long &then, float &remainder);
        void addBullets(std::unique_ptr<Bullet> bullet);
        void updatePlayer();
        void updateBullets();
        void updateEntities();
        void updateEnemies();
        void updateEffects();
        void playerBulletHitEnemies(Bullet *b);
        void enemyBulletHitPlayer(Bullet *b);
        void powerupHitPlayer();
        void addBulletHitEffect(Bullet *bullet);
        void addEnemyDeathEffect(Enemy *enemy);
        void addDonkDeathEffect();
        void drawEffects();
        void addHighscore(int score);
        void drawHighScores();
        void spawnEnemy();
        void addRandomPowerup(int x, int y);
        void blitRotated(SDL_Texture *texture, int x, int y, float angle);

        //define a function template for touching others entities
        template <typename T> void touchOthers(T *source);
        //define a function template for updating powerup objects
        template<typename T> void updatePowerups(T &powerups);
        //define a function template for drawing items
        template<typename T> void drawItems(T &list);
};


#endif 