//
// Created by bfloch on 3/20/21.
//

#ifndef PETIT_PROGRAM_GAME_H
#define PETIT_PROGRAM_GAME_H

#include <memory>
#include <string>
#include <unordered_map>

#include "component_sprite.h"
#include "constants.h"
#include "entity.h"
#include "entity_counter.h"
#include "entity_enemy.h"
#include "entity_player.h"
#include "entity_point.h"
#include "levels.h"
#include "sprite_resource.h"
#include <SDL.h>


class Game
{
public:
    enum GameState
    {
        GAMESTATE_INTRO,
        GAMESTATE_PLAYING,
        GAMESTATE_BITTEN,
        GAMESTATE_ENEMY_WON,
        GAMESTATE_PLAYER_WON,
        GAMESTATE_GAME_OVER,
    };

    explicit Game(SDL_Renderer* renderer)
        : m_renderer(renderer)
    {
        m_spriteResources[SPRITE_PLAYER_1] = std::make_shared<SpriteResource>(renderer, PLAYER_DATA, PLAYER_WIDTH, PLAYER_HEIGHT, palette1, PLAYER_FRAMES);
        m_spriteResources[SPRITE_PLAYER_2] = std::make_shared<SpriteResource>(renderer, PLAYER_DATA, PLAYER_WIDTH, PLAYER_HEIGHT, palette2, PLAYER_FRAMES);
        m_spriteResources[SPRITE_POINT] = std::make_shared<SpriteResource>(renderer, POINT_DATA, POINT_WIDTH, POINT_HEIGHT, palette2, POINT_FRAMES);
        m_spriteResources[SPRITE_NUMBERS] = std::make_shared<SpriteResource>(renderer, NUMBERS_DATA, NUMBERS_WIDTH, NUMBERS_HEIGHT, palette1, NUMBERS_FRAMES);

        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_1_IDLE] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_1], std::initializer_list<size_t>{0, 1});
        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_1_EAT] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_1], std::initializer_list<size_t>{0, 1});
        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_1_BITTEN] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_1], std::initializer_list<size_t>{2, 2, 3, 4, 5, 6, 7, 7, 7, 7});

        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_2_IDLE] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_2], std::initializer_list<size_t>{0});
        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_2_EAT] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_2], std::initializer_list<size_t>{0, 1});
        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_2_BITTEN] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_2], std::initializer_list<size_t>{2, 2, 3, 4, 5, 6, 7, 7, 7, 7});

        m_spriteAnimations[SPRITE_ANIMATION_POINT_SMALL] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_POINT], std::initializer_list<size_t>{0});
        m_spriteAnimations[SPRITE_ANIMATION_POINT_BIG] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_POINT], std::initializer_list<size_t>{1});
        m_spriteAnimations[SPRITE_ANIMATION_POINT_BLINK] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_POINT], std::initializer_list<size_t>{0, 1});

        m_spriteAnimations[SPRITE_ANIMATION_NUMBERS] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_NUMBERS], std::initializer_list<size_t>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

        // Center the gridd

        resetLevel();
    }
    void resetLevel()
    {
        m_entities.clear();
        m_numPoints = 0;
        m_gameState = GAMESTATE_INTRO;
        m_time = 0;
        m_numPointsPlayer = 0;
        m_numPointsEnemy = 0;

        constexpr int gridStep = 32;
        constexpr int gridOffsetX = (BACKBUFFER_WIDTH - ((BACKBUFFER_WIDTH / gridStep) - 1) * gridStep) / 2 - POINT_WIDTH / 2;
        constexpr int gridOffsetY = (BACKBUFFER_HEIGHT - ((BACKBUFFER_HEIGHT / gridStep) - 1) * gridStep) / 2 - POINT_HEIGHT / 2;

        const unsigned char* level = &LEVELS[32 * (m_level - 1)];
        int i = 0;
        for (int y = gridOffsetY; y < BACKBUFFER_HEIGHT; y += gridStep)
        {
            for (int x = gridOffsetX; x < BACKBUFFER_WIDTH; x += gridStep)
            {
                if (level[i++] != ' ')
                {
                    auto point = std::make_unique<Point>(m_spriteAnimations);
                    point->setPosition(x, y);
                    m_entities.emplace_back(std::move(point));
                    ++m_numPoints;
                }
            }
        }

        auto p = std::make_unique<Player>(m_spriteAnimations, true);
        p->setPosition(BACKBUFFER_WIDTH - PLAYER_WIDTH - 20, BACKBUFFER_HEIGHT / 2 - PLAYER_HEIGHT / 2);
        m_entities.emplace_back(std::move(p));
        m_playerIndex = m_entities.size() - 1;

        auto e = std::make_unique<Enemy>(m_spriteAnimations, false, m_entities, m_playerIndex);
        e->setPosition(20, BACKBUFFER_HEIGHT / 2 - PLAYER_HEIGHT / 2);
        m_entities.emplace_back(std::move(e));
        m_enemyIndex = m_entities.size() - 1;

        auto ce = std::make_unique<Counter>(m_spriteAnimations, false);
        ce->setPosition(10, 2);
        m_entities.emplace_back(std::move(ce));
        m_counterEnemyIndex = m_entities.size() - 1;

        auto cp = std::make_unique<Counter>(m_spriteAnimations, true);
        cp->setPosition(BACKBUFFER_WIDTH - NUMBERS_WIDTH - 10, 2);
        m_entities.emplace_back(std::move(cp));
        m_counterPlayerIndex = m_entities.size() - 1;
    }

    void update(Uint32 deltaTime)
    {
        if (m_gameState == GAMESTATE_INTRO)
        {
            m_time += deltaTime;
            if (m_time >= 3 * 1000)
            {
                m_gameState = GAMESTATE_PLAYING;
            }
            return;
        }
        else if (m_gameState == GAMESTATE_BITTEN)
        {
            Player* player = dynamic_cast<Player*>(m_entities[m_playerIndex].get());
            player->update(deltaTime);

            Enemy* enemy = dynamic_cast<Enemy*>(m_entities[m_enemyIndex].get());
            enemy->update(deltaTime);

            m_time += deltaTime;
            if (m_time >= 1700)
            {
                if (m_level > LEVELS_COUNT)
                {
                    m_gameState = GAMESTATE_GAME_OVER;
                }
                else
                {
                    resetLevel();
                }
            }
        }
        else if (m_gameState == GAMESTATE_PLAYING)
        {

            // Update the player first and then do the collision check
            Player* player = dynamic_cast<Player*>(m_entities[m_playerIndex].get());
            SDL_Rect playerRect = player->collisionRect();

            Enemy* enemy = dynamic_cast<Enemy*>(m_entities[m_enemyIndex].get());
            SDL_Rect enemyRect = enemy->collisionRect();

            player->update(deltaTime);

            Counter* counterPlayer = dynamic_cast<Counter*>(m_entities[m_counterPlayerIndex].get());
            Counter* counterEnemy = dynamic_cast<Counter*>(m_entities[m_counterEnemyIndex].get());

            // Update and collison check
            for (int i = 0; i < m_entities.size(); ++i)
            {
                auto& entity = m_entities[i];

                if (i == m_playerIndex || entity == nullptr) continue;

                Point* point = dynamic_cast<Point*>(entity.get());
                if (point != nullptr)
                {
                    if (m_numPointsPlayer + m_numPointsEnemy >= m_numPoints - 20) point->setSpeed(FAST_BLINK_SPEED);


                    SDL_Rect pointRect = point->collisionRect();
                    if (SDL_HasIntersection(&playerRect, &pointRect))
                    {
                        m_entities[i] = nullptr;
                        ++m_numPointsPlayer;
                    }
                    else if (SDL_HasIntersection(&enemyRect, &pointRect))
                    {
                        m_entities[i] = nullptr;
                        ++m_numPointsEnemy;
                    }
                    else
                    {
                        entity->update(deltaTime);
                    }

                    enemy->setIsWinning(m_numPointsEnemy >= m_numPointsPlayer);
                }
                else
                {
                    entity->update(deltaTime);
                }
            }

            if (SDL_HasIntersection(&enemyRect, &playerRect))
            {
                m_gameState = GAMESTATE_BITTEN;

                if (m_numPointsEnemy + m_numPointsPlayer == m_numPoints)
                {
                    if (m_numPointsPlayer > m_numPointsEnemy)
                    {
                        enemy->setAnimation(
                            m_spriteAnimations[SPRITE_ANIMATION_PLAYER_2_BITTEN].get());
                        enemy->setAnimationFrame(0);
                        enemy->setAnimationSpeed(5);
                        player->freeze();
                        enemy->freeze();
                        m_time = 0;
                        m_level++;
                        return;
                    }
                }

                player->setAnimation(
                    m_spriteAnimations[SPRITE_ANIMATION_PLAYER_1_BITTEN].get());
                player->setAnimationFrame(0);
                player->setAnimationSpeed(5);
                player->freeze();
                enemy->freeze();
                enemy->setAnimationSpeed(0);
                m_time = 0;
            }

            counterPlayer->setNumber(m_numPointsPlayer);
            counterEnemy->setNumber(m_numPointsEnemy);
        }
    }

    void draw()
    {
        for (auto& entity : m_entities)
        {
            if (entity == nullptr) continue;
            entity->render(m_renderer);

            // Debug information. Show collision Rect
            if (DEBUG_COLLISION_RECT)
            {
                SpriteEntity* spriteEntity = dynamic_cast<SpriteEntity*>(entity.get());
                if (spriteEntity == nullptr) continue;

                SDL_Rect rect = spriteEntity->collisionRect();
                SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
                SDL_RenderDrawRect(m_renderer, &rect);
                SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0);
            }
        }
    }

private:
    GameState m_gameState{GameState::GAMESTATE_INTRO};
    SDL_Renderer* m_renderer = nullptr;
    SpriteAnimationType m_spriteAnimations;
    std::array<std::shared_ptr<SpriteResource>, SPRITE_NUM> m_spriteResources;
    std::vector<std::unique_ptr<Entity>> m_entities;
    size_t m_playerIndex;
    size_t m_enemyIndex;
    size_t m_counterPlayerIndex;
    size_t m_counterEnemyIndex;
    int m_numPoints{0};
    int m_numPointsPlayer{0};
    int m_numPointsEnemy{0};
    int m_time{0};
    int m_level{1};
};

#endif//PETIT_PROGRAM_GAME_H
