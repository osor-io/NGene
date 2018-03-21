#pragma once
#include <Manager.h>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../types/def.h"
#include <gsl\gsl>
#include "SFML/Window.hpp"
#include <utility>
#include <../_entity/Entity.h>
#include <../_entity/EntityManager.h>

#include "../_component/components/TransformComponent.h"
#include "../_component/components/CollisionComponent.h"


class ChunkManager : public Manager<ChunkManager> {
    friend class CRSP<ChunkManager>;
    using ChunkMap = std::unordered_map<Chunk, EntityIdSet, PairIntIntHash>;
    using EntityMap = std::unordered_map<EntityId, ChunkSet>;
    using PositionCache = std::unordered_map<EntityId, std::pair<sf::Vector2f, sf::Vector2f>>;
    using ChunkLengthType = float;
    using GroupedEntities = std::unordered_map<Chunk, std::set<Entity*>, PairIntIntHash>;
private:
    ChunkManager();
    ~ChunkManager();

public:

    void start_up() override;
    void shut_down() override;

    std::vector<Chunk> get_relevant_chunks() const;

    ChunkSet get_chunks_of(EntityId id) const;

    Chunk get_chunk_from_position(float x, float y) const;

    bool share_chunks(EntityId a, EntityId b) const;

    void update_entity_chunks();

    void clear();

    /*
    There are not const because it might require to generate vectors for new chunks without entities
    */
    void draw_debug_chunks();

    void draw_debug_chunk_configuration();

    const EntityIdSet& get_entities_of_chunk(Chunk chunk);

    const std::set<Entity*>& get_colliding_entities_of_chunk(Chunk chunk);

    /*
    @@TODO @@OPTIMIZATION
    We can make a specific version of this function for the elements with a collision component
    so we can return an internal member that we calculate on our "update_entity_chunks" instead
    of calculating it on request like we are doing here
    */

    /**
    Returns a vector of vectors with the entities grouped by chunks, that is, in each vector
    we have all the entities that are in a particular chunk, entities can be repeated in more
    than one vector.
    */
    template<typename... T>
    GroupedEntities get_grouped_entities_with_components() {

        auto general = GroupedEntities{};

        for (auto & c : m_chunk_map) {

            if (c.second.size() == 0) continue;

            auto& entity_set = c.second;

            auto entities_in_chunk = std::vector<Entity*>{};

            for (auto& e : entity_set) {
                auto entity = EntityManager::get().get_entity(e);

                if (!entity->is_in_relevant_chunk()) continue;

                auto checks = { entity->has_component<T>()... };

                if (std::all_of(checks.begin(), checks.end(), [](bool i) {return i; })) {
                    entities_in_chunk.push_back(entity);
                }
            }

            general[c.first] = (std::move(entities_in_chunk));

        }
        return general;
    }

    template<>
    GroupedEntities get_grouped_entities_with_components<TransformComponent, CollisionComponent>() {
        return m_collision_components_cache;
    }

private:

    /**
    The length of the side of each square chunk
    */
    ChunkLengthType m_chunk_size{ 64 };

    /**
    Number of chunks outside the visible screen that we still consider relevant
    */
    unsigned int m_chunk_threshold{ 1 };

    /**
    This is used in case an entity enters a chunk exactly in the frame that we have to check
    something in that new chunk, we register the entity as being in the chunks that it
    really is plus "m_safe_threshold" more in each direction.
    */
    unsigned int m_safe_threshold{ 1 };


    ChunkMap m_chunk_map{};
    EntityMap m_entity_map{};
    PositionCache m_location_cache{};


    Chunk m_min_relevant_chunk{};
    Chunk m_max_relevant_chunk{};

    GroupedEntities m_collision_components_cache{};


};