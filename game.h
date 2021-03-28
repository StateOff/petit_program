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
#include "entity_text.h"
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
        GAMESTATE_GAME_OVER,
    };

    explicit Game(SDL_Renderer* renderer)
        : m_renderer(renderer)
    {
        m_spriteResources[SPRITE_PLAYER_1] = std::make_shared<SpriteResource>(renderer, PLAYER_DATA, PLAYER_WIDTH, PLAYER_HEIGHT, palette1, PLAYER_FRAMES);
        m_spriteResources[SPRITE_PLAYER_2] = std::make_shared<SpriteResource>(renderer, PLAYER_DATA, PLAYER_WIDTH, PLAYER_HEIGHT, palette2, PLAYER_FRAMES);
        m_spriteResources[SPRITE_POINT] = std::make_shared<SpriteResource>(renderer, POINT_DATA, POINT_WIDTH, POINT_HEIGHT, palette2, POINT_FRAMES);
        m_spriteResources[SPRITE_NUMBERS] = std::make_shared<SpriteResource>(renderer, NUMBERS_DATA, NUMBERS_WIDTH, NUMBERS_HEIGHT, palette1, NUMBERS_FRAMES);
        m_spriteResources[SPRITE_TEXT] = std::make_shared<SpriteResource>(renderer, TEXT_DATA, TEXT_WIDTH, TEXT_HEIGHT, palette1, TEXT_FRAMES);

        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_1_IDLE] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_1], std::initializer_list<size_t>{0, 1});
        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_1_EAT] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_1], std::initializer_list<size_t>{0, 1});
        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_1_FEAR] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_1], std::initializer_list<size_t>{2});
        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_1_BITTEN] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_1], std::initializer_list<size_t>{3, 3, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8});

        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_2_IDLE] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_2], std::initializer_list<size_t>{0});
        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_2_EAT] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_2], std::initializer_list<size_t>{0, 1});
        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_2_FEAR] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_2], std::initializer_list<size_t>{2});
        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_2_BITTEN] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_PLAYER_2], std::initializer_list<size_t>{3, 3, 4, 5, 6, 7, 8, 8, 8, 8, 8, 8});

        m_spriteAnimations[SPRITE_ANIMATION_POINT_SMALL] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_POINT], std::initializer_list<size_t>{0});
        m_spriteAnimations[SPRITE_ANIMATION_POINT_BIG] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_POINT], std::initializer_list<size_t>{1});
        m_spriteAnimations[SPRITE_ANIMATION_POINT_BLINK] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_POINT], std::initializer_list<size_t>{0, 1});

        m_spriteAnimations[SPRITE_ANIMATION_NUMBERS] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_NUMBERS], std::initializer_list<size_t>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

        m_spriteAnimations[SPRITE_ANIMATION_TEXT_LEVEL] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_TEXT], std::initializer_list<size_t>{1, 2, 3, 2, 1});
        m_spriteAnimations[SPRITE_ANIMATION_TEXT_READY] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_TEXT], std::initializer_list<size_t>{8, 2, 5, 9, 10, 11});
        m_spriteAnimations[SPRITE_ANIMATION_TEXT_GAME_OVER] = std::make_unique<SpriteAnimation>(m_spriteResources[SPRITE_TEXT], std::initializer_list<size_t>{4, 5, 6, 2, 0, 7, 3, 2, 8});

        resetLevel();
    }


    void update(Uint32 deltaTime)
    {
        switch (m_gameState)
        {
            case GAMESTATE_INTRO:
                stateIntro(deltaTime);
                break;
            case GAMESTATE_BITTEN:
                stateBitten(deltaTime);
                break;
            case GAMESTATE_PLAYING:
                statePlaying(deltaTime);
                break;
        }
    }

    void render()
    {
        for (auto& entity : m_entities)
        {
            if (entity == nullptr) continue;
            entity->render(m_renderer);

// Don't bother including this in release builds.
#ifndef NDEBUG
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
#endif
        }
    }

private:
    void resetLevel()
    {
        m_entities.clear();

        m_numPoints = 0;
        m_gameState = GAMESTATE_INTRO;
        m_time = 0;
        m_numPointsPlayer = 0;
        m_numPointsEnemy = 0;

        createPointsGrid();

        createPlayers();

        createHUD();
    }

    bool allPointsEaten() const
    {
        return m_numPointsPlayer + m_numPointsEnemy == m_numPoints;
    }

    bool isPlayerWinning() const
    {
        return m_numPointsPlayer > m_numPointsEnemy;
    }

    void statePlaying(Uint32 deltaTime)
    {
        // Update the player first and then do the collision check
        Player* player = dynamic_cast<Player*>(m_entities[m_playerIndex].get());
        SDL_Rect playerRect = player->collisionRect();

        Enemy* enemy = dynamic_cast<Enemy*>(m_entities[m_enemyIndex].get());
        SDL_Rect enemyRect = enemy->collisionRect();

        player->update(deltaTime);

        Counter* counterPlayer = dynamic_cast<Counter*>(m_entities[m_counterPlayerIndex].get());
        Counter* counterEnemy = dynamic_cast<Counter*>(m_entities[m_counterEnemyIndex].get());

        // -- Update and collision check with Points

        for (int i = 0; i < m_entities.size(); ++i)
        {
            auto& entity = m_entities[i];

            // -- Skip for players or inactive entities

            if (i == m_playerIndex || entity == nullptr) continue;

            // -- Check if the player / enemy has eaten a point

            Point* point = dynamic_cast<Point*>(entity.get());
            if (point != nullptr)
            {
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
                    // Handles the point's animation
                    entity->update(deltaTime);
                }

                // If the last point has been eaten, this state tells the enemy
                // to pursue the player or wait.
                enemy->setIsWinning(!isPlayerWinning());
            }
            else
            {
                // Update other entity
                entity->update(deltaTime);
            }
        }

        // -- Collision Check between Player and Enemy

        if (SDL_HasIntersection(&enemyRect, &playerRect))
        {
            m_gameState = GAMESTATE_BITTEN;

            // -- If all points are gone, the player might have won the collision based on points

            if (allPointsEaten())
            {
                if (isPlayerWinning())
                {
                    enemy->setAnimation(
                        m_spriteAnimations[SPRITE_ANIMATION_PLAYER_2_BITTEN].get());
                    enemy->setAnimationFrame(0);
                    enemy->setAnimationSpeed(6);
                    player->freeze();
                    enemy->freeze();
                    m_time = 0;
                    m_level++;
                    return;
                }
            }

            // -- Enemy won collision over Player

            player->setAnimation(
                m_spriteAnimations[SPRITE_ANIMATION_PLAYER_1_BITTEN].get());
            player->setAnimationFrame(0);
            player->setAnimationSpeed(6);
            player->freeze();
            enemy->freeze();
            enemy->setAnimationSpeed(0);
            m_time = 0;
        }

        // -- If there are not points left
        if (allPointsEaten() && isPlayerWinning())
        {
            enemy->setAnimation(
                m_spriteAnimations[SPRITE_ANIMATION_PLAYER_2_FEAR].get());
        }

        counterPlayer->setNumber(m_numPointsPlayer);
        counterEnemy->setNumber(m_numPointsEnemy);
    }

    void stateBitten(Uint32 deltaTime)
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

                auto txtGameOver = std::make_unique<Text>(m_spriteAnimations, true);
                txtGameOver->setPosition(BACKBUFFER_WIDTH / 2 + TEXT_WIDTH * 5, BACKBUFFER_HEIGHT / 2 - TEXT_HEIGHT);
                txtGameOver->setText(m_spriteAnimations[SPRITE_ANIMATION_TEXT_GAME_OVER].get());
                m_entities.emplace_back(std::move(txtGameOver));
                m_textGameOverIndex = m_entities.size() - 1;

                m_entities[m_playerIndex] = nullptr;
            }
            else
            {
                resetLevel();
            }
        }
    }

    void stateIntro(Uint32 deltaTime)
    {
        m_time += deltaTime;
        if (m_time >= 3 * 1000)
        {
            m_gameState = GAMESTATE_PLAYING;
            m_entities[m_textReadyIndex] = nullptr;
        }
    }

    void createHUD()
    {
        // -- HUD: EnemyPoints - Level - Player Points

        auto counterEnemy = std::make_unique<Counter>(m_spriteAnimations, false);
        counterEnemy->setPosition(10, 2);
        m_entities.emplace_back(std::move(counterEnemy));
        m_counterEnemyIndex = m_entities.size() - 1;

        auto counterPlayer = std::make_unique<Counter>(m_spriteAnimations, true);
        counterPlayer->setPosition(BACKBUFFER_WIDTH - NUMBERS_WIDTH - 10, 2);
        m_entities.emplace_back(std::move(counterPlayer));
        m_counterPlayerIndex = m_entities.size() - 1;

        auto txtLevel = std::make_unique<Text>(m_spriteAnimations, true);
        txtLevel->setPosition(BACKBUFFER_WIDTH / 2, 2);
        m_entities.emplace_back(std::move(txtLevel));

        auto counterLevel = std::make_unique<Counter>(m_spriteAnimations, false);
        counterLevel->setPosition(BACKBUFFER_WIDTH / 2 + TEXT_WIDTH * 2, 2);
        counterLevel->setNumber(m_level);
        m_entities.emplace_back(std::move(counterLevel));

        // -- Ready text (is being destroyed at the beginning of the level)

        auto txtReady = std::make_unique<Text>(m_spriteAnimations, true);
        txtReady->setPosition(BACKBUFFER_WIDTH / 2 + TEXT_WIDTH * 3, BACKBUFFER_HEIGHT / 2 - TEXT_HEIGHT);
        txtReady->setText(m_spriteAnimations[SPRITE_ANIMATION_TEXT_READY].get());
        m_entities.emplace_back(std::move(txtReady));
        m_textReadyIndex = m_entities.size() - 1;
    }

    void createPlayers()
    {
        // -- Player & Enemy

        auto player = std::make_unique<Player>(m_spriteAnimations, true);
        player->setPosition(BACKBUFFER_WIDTH - PLAYER_WIDTH - 20, BACKBUFFER_HEIGHT / 2 - PLAYER_HEIGHT / 2);
        m_entities.emplace_back(std::move(player));
        m_playerIndex = m_entities.size() - 1;

        auto enemy = std::make_unique<Enemy>(m_spriteAnimations, false, m_entities, m_playerIndex);
        enemy->setPosition(20, BACKBUFFER_HEIGHT / 2 - PLAYER_HEIGHT / 2);
        m_entities.emplace_back(std::move(enemy));
        m_enemyIndex = m_entities.size() - 1;
    }

    void createPointsGrid()
    {
        // -- Center the points grid

        constexpr int gridStep = 32;
        constexpr int gridOffsetX = (BACKBUFFER_WIDTH - ((BACKBUFFER_WIDTH / gridStep) - 1) * gridStep) / 2 - POINT_WIDTH / 2;
        constexpr int gridOffsetY = (BACKBUFFER_HEIGHT - ((BACKBUFFER_HEIGHT / gridStep) - 1) * gridStep) / 2 - POINT_HEIGHT / 2;

        // -- Draw the points grid

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
    size_t m_textReadyIndex;
    size_t m_textGameOverIndex;
    int m_numPoints{0};
    int m_numPointsPlayer{0};
    int m_numPointsEnemy{0};
    int m_time{0};
    int m_level{1};
};

#endif//PETIT_PROGRAM_GAME_H
