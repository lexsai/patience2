#include "entity_pool.hpp"

#include <stdexcept>

EntityPool::EntityPool()
{
  m_freeListHead = 1;
  for (int i = 1; i < MAX_ENTITIES - 1; i++)
  {
    m_nextFreeSlot[i] = i + 1;
  }
  // reserve m_entities[0] as nil entity
  m_nextFreeSlot[MAX_ENTITIES - 1] = 0;
}

EntityId EntityPool::createEntity(EntityType type, float x, float y)
{
  int slot = m_freeListHead;
  m_freeListHead = m_nextFreeSlot[slot];
  
  Entity* e = &m_entities.at(slot);
  if (m_allocated[slot])
  {
    throw std::runtime_error("tried creating on allocated slot" + std::to_string(slot));
  }
  // prevent state from prev allocation contaminating
  *e = {};
  m_allocated[slot] = true;

  int generation = m_generation.at(slot)++;
  e->id = { generation, slot };
  e->type = type;
  e->x = x;
  e->y = y;
  e->width = TILE_WIDTH;
  e->height = TILE_WIDTH;

  setupEntity(*e);

  return e->id;
}

Entity* EntityPool::getEntity(EntityId e)
{
  Entity* entity = &m_entities[e.index];
  if (entity->id.generation != e.generation || !m_allocated[e.index])
  {
    return &m_entities[0];
  }

  return entity;
}

void EntityPool::removeEntity(EntityId e)
{
  if (e.index <= 0 || e.index >= MAX_ENTITIES) return;

  Entity *entity = &m_entities[e.index];
  if (entity->id.generation == e.generation)
  {
    if (e.index != entity->id.index)
    {
      throw std::runtime_error("inconsistent entity id state");
    }

    m_allocated[e.index] = false;

    m_nextFreeSlot[e.index] = m_freeListHead;
    m_freeListHead = e.index;
  }
}
