#ifndef PATIENCE_ENTITY_POOL_HPP
#define PATIENCE_ENTITY_POOL_HPP

#include "entities.hpp"

#include <array>
#include <iterator>
#include <cstddef>

constexpr int MAX_ENTITIES = 1024;

class EntityPool
{
  std::array<Entity, MAX_ENTITIES> m_entities{};

  // keep these separate so we can reset entity state with copy assignment
  // of default entity without messing up array state
  std::array<bool, MAX_ENTITIES> m_allocated{};
  std::array<int, MAX_ENTITIES> m_generation{};
  
  std::array<int, MAX_ENTITIES> m_nextFreeSlot{};
  int m_freeListHead{};
public:
  EntityPool();

  EntityId createEntity(EntityType type, float x, float y);
  Entity* getEntity(EntityId e);
  void removeEntity(EntityId e);

  struct EntityIterator
  {
    std::array<Entity, MAX_ENTITIES>& m_iterEntities;
    std::array<bool, MAX_ENTITIES>& m_iterAllocated;
    int index;

    EntityIterator(std::array<Entity, MAX_ENTITIES>& iterEntities,
                   std::array<bool, MAX_ENTITIES>& iterAllocated,
                   int index)
      : m_iterEntities{ iterEntities }, 
        m_iterAllocated{ iterAllocated },
        index{ index } {
          skipGaps();
        }
  
    void skipGaps()
    {
      while (index < MAX_ENTITIES && !m_iterAllocated[index])
      {
        index++;
      }
    }

    Entity& operator*() const { return m_iterEntities[index]; }
    Entity* operator->() const { return &m_iterEntities[index]; }

    EntityIterator& operator++()
    {
      index++;
      skipGaps();
      return *this;
    }

    EntityIterator& operator++(int)
    {
      EntityIterator tmp = *this;
      ++(*this);
      return tmp;
    }
    
    friend bool operator== (const EntityIterator& a, const EntityIterator& b)
    {
      return a.index == b.index;
    }

    friend bool operator!= (const EntityIterator& a, const EntityIterator& b)
    {
      return a.index != b.index;
    }
  };

  EntityIterator begin() { return EntityIterator(m_entities, m_allocated, 1); }
  EntityIterator end() { return EntityIterator(m_entities, m_allocated, MAX_ENTITIES); }
};

#endif
