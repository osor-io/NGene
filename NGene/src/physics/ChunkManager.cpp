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

    for (auto x = m_min_relevant_chunk.first; x <= m_max_relevant_chunk.first; ++x) {
        for (auto y = m_min_relevant_chunk.second; y <= m_max_relevant_chunk.second; ++y) {
            vec.push_back(std::make_pair(x, y));
        }
    }

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

const EntitySet& ChunkManager::get_entities_of_chunk(Chunk chunk) {
    return m_chunk_map[chunk];
}

void ChunkManager::update_entity_chunks() {

    auto& entities = EntityManager::get().get_entities();

    const auto& render_target = RenderManager::get().get_main_render_target();

    const auto& view = render_target->getView();
    const auto& center = view.getCenter();
    const auto& size = view.getSize();

    m_min_relevant_chunk = std::make_pair(
        gsl::narrow_cast<int>(std::floor((center.x - size.x / 2.f) / m_chunk_size) - m_chunk_threshold),
        gsl::narrow_cast<int>(std::floor((center.y - size.y / 2.f) / m_chunk_size) - m_chunk_threshold)
    );

    m_max_relevant_chunk = std::make_pair(
        gsl::narrow_cast<int>(std::floor((center.x + size.x / 2.f) / m_chunk_size) + m_chunk_threshold),
        gsl::narrow_cast<int>(std::floor((center.y + size.y / 2.f) / m_chunk_size) + m_chunk_threshold)
    );


    for (auto& e : entities) {


        auto entity = e.second.get();
        auto id = entity->get_id();
        auto transform = entity->get_component<TransformComponent>();
        if (!transform) continue;

        auto position = transform->get_position();


        auto extent = entity->get_component<ExtentComponent>();

        /*
        Add here other checks with other components if we need it to.
        */
        if (transform) {

            auto min_chunk = Chunk{};
            auto max_chunk = Chunk{};

            if (extent) {
                auto center = position + extent->m_offset;
                auto min_position = center - extent->m_extent;
                auto max_position = center + extent->m_extent;


                /*
                Here we stop checking if the entity hasn't changed
                */
                {
                    /*
                    @@HACK
                    This only fails if we move the offset and the extent in the
                    exact same (float) amount in the same frame. Which we are probably ok
                    with since I don't plan on ever changing offset during runtime, only when
                    debugging and setting up the scenes.
                    */
                    if (m_location_cache[id] == min_position) {
                        continue;
                    }
                    m_location_cache[id] = min_position;

                    for (auto& c : m_entity_map[id]) {
                        m_chunk_map[c].erase(id);
                    }
                    m_entity_map[id].clear();
                }

                /*
                We calculate the minimum and maximum chunks the entity can be in
                by calculating the chunk of the minimum and maximum points.
                */

                min_chunk.first = gsl::narrow_cast<int>(std::floor(min_position.x / m_chunk_size));
                min_chunk.second = gsl::narrow_cast<int>(std::floor(min_position.y / m_chunk_size));

                max_chunk.first = gsl::narrow_cast<int>(std::floor(max_position.x / m_chunk_size));
                max_chunk.second = gsl::narrow_cast<int>(std::floor(max_position.y / m_chunk_size));
            }
            else {
                /*
                Here we stop checking if the entity hasn't changed
                */
                {
                    if (m_location_cache[id] == position) {
                        continue;
                    }
                    m_location_cache[id] = position;

                    for (auto& c : m_entity_map[id]) {
                        m_chunk_map[c].erase(id);
                    }
                    m_entity_map[id].clear();
                }

                auto chunk = std::make_pair(
                    gsl::narrow_cast<int>(std::floor(position.x / m_chunk_size)),
                    gsl::narrow_cast<int>(std::floor(position.y / m_chunk_size)));

                min_chunk = max_chunk = chunk;
            }

            /*
            @@TODO: Only do this if the entity is dynamic, that is, it
            can move.
            */
            min_chunk.first -= m_safe_threshold;
            min_chunk.second -= m_safe_threshold;

            max_chunk.first += m_safe_threshold;
            max_chunk.second += m_safe_threshold;


            auto relevant = false;
            /*
            Now we instert the entity on the required maps and check if any of the
            chunks it is in is relevant.
            */

            for (auto x = min_chunk.first; x <= max_chunk.first; ++x) {
                for (auto y = min_chunk.second; y <= max_chunk.second; ++y) {

                    auto chunk = std::make_pair(x, y);

                    m_chunk_map[chunk].insert(id);
                    m_entity_map[id].insert(chunk);

                    if (!relevant) {
                        relevant = is_chunk_in_range(chunk, m_min_relevant_chunk, m_max_relevant_chunk);
                    }
                }
            }

            entity->set_in_relevant_chunk(relevant);
        }

    }

}

void ChunkManager::draw_debug_chunks() {

    auto current_chunk = m_min_relevant_chunk;

    /*
    const auto origin_center = RenderManager::get().get_main_render_target()->getView().getCenter();
    const auto origin_size = RenderManager::get().get_main_render_target()->getView().getSize();
    const auto origin = origin_center - origin_size / 2.0f;
    */

    const auto origin = RenderManager::get().get_main_render_target()->mapCoordsToPixel(sf::Vector2f(
        m_min_relevant_chunk.first,
        m_min_relevant_chunk.second));

    auto draw_list = ImGui::GetWindowDrawList();
    draw_list->PushClipRectFullScreen();
    for (auto x = m_min_relevant_chunk.first; x <= m_max_relevant_chunk.first; ++x) {
        for (auto y = m_min_relevant_chunk.second; y <= m_max_relevant_chunk.second; ++y) {

            auto count = get_entities_of_chunk(std::make_pair(x, y)).size();

            draw_list->AddQuadFilled(
                ImVec2(origin.x + (x * m_chunk_size), origin.y + (y*m_chunk_size)),
                ImVec2(origin.x + (x * m_chunk_size) + m_chunk_size, origin.y + (y*m_chunk_size)),
                ImVec2(origin.x + (x * m_chunk_size) + m_chunk_size, origin.y + (y*m_chunk_size) + m_chunk_size),
                ImVec2(origin.x + (x * m_chunk_size), origin.y + (y*m_chunk_size) + m_chunk_size),
                ImGui::GetColorU32((ImVec4)ImColor(
                (count == 0 ? 0 : 100 * count),
                    0,
                    (count == 0 ? 255 : 0),
                    (count == 0 ? 20 : 70)))
            );

            draw_list->AddQuad(
                ImVec2(origin.x + (x * m_chunk_size), origin.y + (y*m_chunk_size)),
                ImVec2(origin.x + (x * m_chunk_size) + m_chunk_size, origin.y + (y*m_chunk_size)),
                ImVec2(origin.x + (x * m_chunk_size) + m_chunk_size, origin.y + (y*m_chunk_size) + m_chunk_size),
                ImVec2(origin.x + (x * m_chunk_size), origin.y + (y*m_chunk_size) + m_chunk_size),
                ImGui::GetColorU32((ImVec4)ImColor(50 * count, 0, 255, 200))
            );


        }
    }
    draw_list->PopClipRect();
}

void ChunkManager::draw_debug_chunk_configuration() {

    ImGui::SetNextWindowSize(ImVec2(430, 200), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Chunk Configuration"))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("Chunk Size"); ImGui::SameLine(150); ImGui::DragFloat("##Chunk Size", &m_chunk_size, 1.0f, 32.f, 512.f);
    ImGui::Text("Map Threshold"); ImGui::SameLine(150); ImGui::DragInt("##Map Threshold", reinterpret_cast<int*>(&m_chunk_threshold), 1.0f, 0, 10);
    ImGui::Text("Entity Threshold"); ImGui::SameLine(150); ImGui::DragInt("##Entity Threshold", reinterpret_cast<int*>(&m_safe_threshold), 1.0f, 0, 10);

    ImGui::End();

}


