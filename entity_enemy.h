//
// Created by bfloch on 3/27/21.
//

#include "entity_player.h"
#include "entity_point.h"

#ifndef PETIT_PROGRAM_ENTITY_ENEMY_H
#define PETIT_PROGRAM_ENTITY_ENEMY_H

class Enemy : public Player
{
public:
    Enemy(const SpriteAnimationType& spriteAnimations, bool isFlipped, const std::vector<std::unique_ptr<Entity>>& entities, int playerIndex)
    : Player(spriteAnimations, isFlipped)
    , m_entities(entities)
    , m_playerIndex(playerIndex)
    {
        Sprite* sprite = dynamic_cast<Sprite*>(getComponent(SPRITE_COMPONENT_INDEX));
        sprite->setAnimation(spriteAnimations[SPRITE_ANIMATION_PLAYER_2_EAT].get());
        sprite->setSpeed(5.0);
    }

    static int distanceSquared(int x1, int y1, int x2, int y2)
    {
        return abs((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    }

    void getInput(int& dx, int& dy) const override
    {
        if(m_targetIndex == -1) return;

        SpriteEntity* point = dynamic_cast<SpriteEntity*>(m_entities[m_targetIndex].get());
        if (point == nullptr) return;

        if(point->x() > x() + (PLAYER_WIDTH / 2) - POINT_WIDTH / 2)
            dx += m_speed;
        if(point->x() < x() + (PLAYER_WIDTH / 2) - POINT_WIDTH / 2)
            dx -= m_speed;
        if(point->y() > y() + (PLAYER_HEIGHT / 2) - POINT_HEIGHT / 2)
            dy += m_speed;
        if(point->y() < y() + (PLAYER_HEIGHT / 2) - POINT_HEIGHT / 2)
            dy -= m_speed;
    }
    void update(Uint32 deltaTime) override
    {
        bool findNewTarget = true;
        if(m_targetIndex > -1)
        {
            auto& entity = m_entities[m_targetIndex];
            if(entity != nullptr)
                findNewTarget = false;
            else
                m_targetIndex = -1;
        }

        if(findNewTarget)
        {
            int closestDistanceSq = -1;
            for (int i = 0; i < m_entities.size(); ++i)
            {
                auto& entity = m_entities[i];
                if (entity == nullptr) continue;

                Point* point = dynamic_cast<Point*>(entity.get());
                if (point == nullptr) continue;

                int newClosestDistance = distanceSquared(x(), y(), point->x(), point->y());
                if (m_targetIndex == -1 || newClosestDistance < closestDistanceSq)
                {
                    m_targetIndex = i;
                    closestDistanceSq = newClosestDistance;
                }
            }
        }
        if(m_targetIndex == -1 && m_isWinning) {
            m_targetIndex = m_playerIndex;
        }

        Player::update(deltaTime);

    }

    void setIsWinning(bool isWinning)
    {
        m_isWinning = isWinning;
    }

private:
    const std::vector<std::unique_ptr<Entity>>& m_entities;
    int m_targetIndex = -1;
    int m_playerIndex = -1;
    bool m_isWinning = true;
};

#endif//PETIT_PROGRAM_ENTITY_ENEMY_H
