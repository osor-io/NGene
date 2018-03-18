#pragma once
#include <Manager.h>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "../types/def.h"
#include <gsl\gsl>
#include "SFML/Window.hpp"


class ChunkManager : public Manager<ChunkManager> {
    friend class CRSP<ChunkManager>;
    using ChunkMap = std::unordered_map<Chunk, EntitySet, ChunkHash>;
    using EntityMap = std::unordered_map<EntityId, ChunkSet>;
    using PositionCache = std::unordered_map<EntityId, sf::Vector2f>;
    using ChunkLengthType = float;
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

    /*
    Not const because it might require to generate vectors for new chunks without entities
    */
    void draw_debug_chunks() ;

    /*
    It is not const vecause it might create an empty vector for the required chunk
    */
    const EntitySet& get_entities_of_chunk(Chunk chunk);

private:
    ChunkLengthType m_chunk_size{ 128 };
    unsigned int m_chunk_threshold{ 1 };

    ChunkMap m_chunk_map{};
    EntityMap m_entity_map{};
    PositionCache m_location_cache{};


    Chunk m_min_relevant_chunk{};
    Chunk m_max_relevant_chunk{};


};