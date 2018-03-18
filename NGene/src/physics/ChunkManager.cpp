#include "ChunkManager.h"
#include "../_entity/Entitymanager.h"
#include "../_component/components/TransformComponent.h"
#include "../_component/components/ExtentComponent.h"
#include "../render/RenderManager.h"

ChunkManager::ChunkManager() {

}

ChunkManager::~ChunkManager() {

}

void ChunkManager::start_up() {

}

void ChunkManager::shut_down() {

}

std::vector<Chunk> ChunkManager::get_relevant_chunks() const {
    auto vec = std::vector<Chunk>();

    //@@TODO: get visible chunks using the render manager to calculate it.

    return vec;
}

std::unordered_set<Chunk, ChunkHash> ChunkManager::get_chunks_of(EntityId id) const {
    return m_entity_map.at(id);
}

bool ChunkManager::share_chunks(EntityId a, EntityId b) const {
    const auto& set_a = m_entity_map.at(a);
    const auto& set_b = m_entity_map.at(a);

    /*
    We iterate in the smallest map since lookup is constant and we
    benefit from avoiding iterating through more elements on the
    bigger map
    */
    if (set_a.size() < set_b.size()) {
        for (auto i = set_a.begin(); i != set_a.end(); ++i) {
            if (set_b.find(*i) != set_b.end()) return true;
        }
    }
    else {
        for (auto i = set_b.begin(); i != set_b.end(); ++i) {
            if (set_a.find(*i) != set_a.end()) return true;
        }
    }

    return false;
}

Chunk ChunkManager::get_chunk_from_position(float x, float y) const {
    return std::make_pair(
        gsl::narrow_cast<int>(std::floor((x) / m_chunk_size)),
        gsl::narrow_cast<int>(std::floor((y) / m_chunk_size))
    );
}


inline bool is_chunk_in_range(const Chunk& chunk, const Chunk& min_chunk, const Chunk& max_chunk) {

    return  (chunk.first >= min_chunk.first && chunk.first <= max_chunk.first) &&
        (chunk.second >= min_chunk.second && chunk.second <= max_chunk.second);
}

const std::vector<EntityId>& ChunkManager::get_entities_of_chunk(Chunk chunk) {
    return m_chunk_map[chunk];
}

void ChunkManager::update_entity_chunks() {

    auto& entities = EntityManager::get().get_entities();

    const auto& render_target = RenderManager::get().get_main_render_target();

    const auto& viewport = render_target->getViewport(render_target->getView());

    m_min_relevant_chunk = std::make_pair(
        gsl::narrow_cast<int>(std::floor((viewport.left) / m_chunk_size) - m_chunk_threshold),
        gsl::narrow_cast<int>(std::floor((viewport.top) / m_chunk_size) - m_chunk_threshold)
    );

    m_max_relevant_chunk = std::make_pair(
        gsl::narrow_cast<int>(std::floor((viewport.left + viewport.width) / m_chunk_size) + m_chunk_threshold),
        gsl::narrow_cast<int>(std::floor((viewport.top + viewport.height) / m_chunk_size) + m_chunk_threshold)
    );

    //@@OPTIMIZATION: Keep previous maps and only change the ones that moved.
    m_chunk_map.clear();
    m_entity_map.clear();

    for (auto& e : entities) {
        auto entity = e.second.get();
        auto transform = entity->get_component<TransformComponent>();
        auto extent = entity->get_component<ExtentComponent>();

        //@@TODO add other check with the extent components.
        if (transform) {
            if (extent) {
                auto min_chunk = Chunk{};
                auto max_chunk = Chunk{};
                auto center = transform->get_position() + extent->m_offset;
                auto min_position = center - extent->m_extent;
                auto max_position = center + extent->m_extent;

                min_chunk.first = gsl::narrow_cast<int>(std::floor(min_position.x / m_chunk_size));
                min_chunk.second = gsl::narrow_cast<int>(std::floor(min_position.y / m_chunk_size));

                max_chunk.first = gsl::narrow_cast<int>(std::floor(max_position.x / m_chunk_size));
                max_chunk.second = gsl::narrow_cast<int>(std::floor(max_position.y / m_chunk_size));

                auto relevant = false;

                for (auto x = min_chunk.first; x <= max_chunk.first; ++x) {
                    for (auto y = min_chunk.second; y <= max_chunk.second; ++y) {

                        auto chunk = std::make_pair(x,y);
                        
                        m_chunk_map[chunk].push_back(entity->get_id());
                        m_entity_map[entity->get_id()].insert(chunk);

                        if (!relevant) {
                            relevant = is_chunk_in_range(chunk, m_min_relevant_chunk, m_max_relevant_chunk);
                        }
                    }
                }

                entity->set_in_relevant_chunk(relevant);

            }
            else {
                auto position = transform->get_position();
                auto chunk = Chunk{};
                chunk.first = gsl::narrow_cast<int>(std::floor(position.x / m_chunk_size));
                chunk.second = gsl::narrow_cast<int>(std::floor(position.y / m_chunk_size));
                m_chunk_map[chunk].push_back(entity->get_id());
                m_entity_map[entity->get_id()].insert(chunk);
                entity->set_in_relevant_chunk(is_chunk_in_range(chunk, m_min_relevant_chunk, m_max_relevant_chunk));
            }
        }

    }

}

void ChunkManager::draw_debug_chunks() {

    auto current_chunk = m_min_relevant_chunk;


    auto draw_list = ImGui::GetWindowDrawList();
    draw_list->PushClipRectFullScreen();
    for (auto x = m_min_relevant_chunk.first; x <= m_max_relevant_chunk.first; ++x) {
        for (auto y = m_min_relevant_chunk.second; y <= m_max_relevant_chunk.second; ++y) {

            auto count = get_entities_of_chunk(std::make_pair(x, y)).size();


            draw_list->AddQuadFilled(
                ImVec2((x * m_chunk_size), (y*m_chunk_size)),
                ImVec2((x * m_chunk_size) + m_chunk_size, (y*m_chunk_size)),
                ImVec2((x * m_chunk_size) + m_chunk_size, (y*m_chunk_size) + m_chunk_size),
                ImVec2((x * m_chunk_size), (y*m_chunk_size) + m_chunk_size),
                ImGui::GetColorU32((ImVec4)ImColor(
                (count == 0 ? 0 : 100 * count),
                    0,
                    (count == 0 ? 255 : 0),
                    (count == 0 ? 20 : 70)))
            );

            draw_list->AddQuad(
                ImVec2((x * m_chunk_size), (y*m_chunk_size)),
                ImVec2((x * m_chunk_size) + m_chunk_size, (y*m_chunk_size)),
                ImVec2((x * m_chunk_size) + m_chunk_size, (y*m_chunk_size) + m_chunk_size),
                ImVec2((x * m_chunk_size), (y*m_chunk_size) + m_chunk_size),
                ImGui::GetColorU32((ImVec4)ImColor(50 * count, 0, 255, 200))
            );


        }
    }
    draw_list->PopClipRect();




    //@@TODO: draw from min to max chunk
}


