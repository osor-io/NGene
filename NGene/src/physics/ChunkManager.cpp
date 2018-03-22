#include "ChunkManager.h"
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

std::unordered_set<Chunk, PairIntIntHash> ChunkManager::get_chunks_of(EntityId id) const {
    return m_entity_map.at(id);
}

ChunkSet ChunkManager::calculate_chunks(Entity * entity) const {

    auto chunk_set = ChunkSet{};

    auto id = entity->get_id();
    auto transform = entity->get_component<TransformComponent>();
    if (!transform) return chunk_set;

    auto position = transform->get_position();

    auto extent = entity->get_component<ExtentComponent>();

    /*
    Add here other checks with other components if we need it to.
    */

    auto min_chunk = Chunk{};
    auto max_chunk = Chunk{};

    if (extent) {
        auto center = position + extent->m_offset;
        auto extra_chunk_threshold = extent->m_extra_chunk_threshold * m_chunk_size;
        auto min_position = center - extent->m_extent - sf::Vector2f(extra_chunk_threshold, extra_chunk_threshold);
        auto max_position = center + extent->m_extent + sf::Vector2f(extra_chunk_threshold, extra_chunk_threshold);

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
        auto chunk = std::make_pair(
            gsl::narrow_cast<int>(std::floor(position.x / m_chunk_size)),
            gsl::narrow_cast<int>(std::floor(position.y / m_chunk_size)));

        min_chunk = max_chunk = chunk;
    }
    auto collision_component = entity->get_component<CollisionComponent>();

    /*
    We default to adding the threshold unless we know the entity
    is not dynamic.
    */
    if (!collision_component || collision_component->m_dynamic) {
        min_chunk.first -= m_safe_threshold;
        min_chunk.second -= m_safe_threshold;

        max_chunk.first += m_safe_threshold;
        max_chunk.second += m_safe_threshold;
    }


    /*
    Now we instert the entity on the required maps and check if any of the
    chunks it is in is relevant.
    */

    for (auto x = min_chunk.first; x <= max_chunk.first; ++x) {
        for (auto y = min_chunk.second; y <= max_chunk.second; ++y) {

            auto chunk = std::make_pair(x, y);

            chunk_set.insert(chunk);
        }
    }

    return chunk_set;

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

    auto pos = RenderManager::get().get_main_render_target()->mapPixelToCoords(sf::Vector2i(x, y));

    return std::make_pair(
        gsl::narrow_cast<int>(std::floor((pos.x) / m_chunk_size)),
        gsl::narrow_cast<int>(std::floor((pos.y) / m_chunk_size))
    );
}

inline bool is_chunk_in_range(const Chunk& chunk, const Chunk& min_chunk, const Chunk& max_chunk) {

    return  (chunk.first >= min_chunk.first && chunk.first <= max_chunk.first) &&
        (chunk.second >= min_chunk.second && chunk.second <= max_chunk.second);
}

const EntityIdSet& ChunkManager::get_entities_of_chunk(Chunk chunk) {
    return m_chunk_map[chunk];
}

const std::set<Entity*>& ChunkManager::get_colliding_entities_of_chunk(Chunk chunk) {
    return m_collision_components_cache[chunk];
}

void ChunkManager::update_entity_chunks() {

    auto& entities = EntityManager::get().get_entities();

    const auto& render_target = RenderManager::get().get_main_render_target();
    const auto zoom = RenderManager::get().get_current_zoom();
    const auto& view = render_target->getView();
    const auto& center = view.getCenter();
    const auto& size = view.getSize();

    m_camera_changed = ((camera_previous_center != center) || (camera_previous_size != size));
    camera_previous_center = center;
    camera_previous_size = size;

    m_min_relevant_chunk = std::make_pair(
        gsl::narrow_cast<int>(std::floor((center.x - (size.x * zoom) / 2.f) / m_chunk_size) - m_chunk_threshold),
        gsl::narrow_cast<int>(std::floor((center.y - (size.y * zoom) / 2.f) / m_chunk_size) - m_chunk_threshold)
    );

    m_max_relevant_chunk = std::make_pair(
        gsl::narrow_cast<int>(std::floor((center.x + (size.x * zoom) / 2.f) / m_chunk_size) + m_chunk_threshold),
        gsl::narrow_cast<int>(std::floor((center.y + (size.y * zoom) / 2.f) / m_chunk_size) + m_chunk_threshold)
    );

    for (auto& e : entities) {
        update_single_entity(e.second.get());
    }
}

void ChunkManager::update_single_entity(Entity * entity){

    auto id = entity->get_id();
    auto transform = entity->get_component<TransformComponent>();
    if (!transform) return;

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
            auto extra_chunk_threshold = extent->m_extra_chunk_threshold * m_chunk_size;
            auto min_position = center - extent->m_extent - sf::Vector2f(extra_chunk_threshold, extra_chunk_threshold);
            auto max_position = center + extent->m_extent + sf::Vector2f(extra_chunk_threshold, extra_chunk_threshold);


            /*
            Here we stop checking if the entity hasn't changed
            */
            {
                if (!m_camera_changed && m_location_cache[id].first == min_position && m_location_cache[id].second == max_position) {
                    return;
                }
                m_location_cache[id].first = min_position;
                m_location_cache[id].second = max_position;

                for (auto& c : m_entity_map[id]) {
                    m_chunk_map[c].erase(id);
                    m_collision_components_cache[c].erase(entity);
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
                if (m_location_cache[id].first == position) {
                    return;
                }
                m_location_cache[id].first = position;

                for (auto& c : m_entity_map[id]) {
                    m_chunk_map[c].erase(id);
                    m_collision_components_cache[c].erase(entity);
                }
                m_entity_map[id].clear();
            }

            auto chunk = std::make_pair(
                gsl::narrow_cast<int>(std::floor(position.x / m_chunk_size)),
                gsl::narrow_cast<int>(std::floor(position.y / m_chunk_size)));

            min_chunk = max_chunk = chunk;
        }


        auto collision_component = entity->get_component<CollisionComponent>();

        /*
        We default to adding the threshold unless we know the entity
        is not dynamic.
        */
        if (!collision_component || collision_component->m_dynamic) {
            min_chunk.first -= m_safe_threshold;
            min_chunk.second -= m_safe_threshold;

            max_chunk.first += m_safe_threshold;
            max_chunk.second += m_safe_threshold;
        }


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

                if (collision_component) {
                    m_collision_components_cache[chunk].insert(entity);
                }

            }
        }

        entity->set_in_relevant_chunk(relevant);

    }

}

void ChunkManager::clear() {

    m_chunk_map.clear();
    m_entity_map.clear();
    m_location_cache.clear();
    m_collision_components_cache.clear();

}

void ChunkManager::draw_debug_chunks() {

    auto current_chunk = m_min_relevant_chunk;

    const auto converted_chunk_size = m_chunk_size * RenderManager::get().get_current_zoom();

    const auto beg = RenderManager::get().map_coords_to_pixel(sf::Vector2f(
        m_min_relevant_chunk.first * m_chunk_size,
        m_min_relevant_chunk.second * m_chunk_size));

    const auto end = RenderManager::get().map_coords_to_pixel(sf::Vector2f(
        m_max_relevant_chunk.first * m_chunk_size,
        m_max_relevant_chunk.second * m_chunk_size));

    auto draw_list = ImGui::GetWindowDrawList();
    draw_list->PushClipRectFullScreen();

    auto i = m_min_relevant_chunk.first;

    for (auto x = beg.x;
        x <= end.x;
        x += converted_chunk_size, ++i) {

        auto j = m_min_relevant_chunk.second;

        ImGui::BeginChild("##avoiding_limit");

        for (auto y = beg.y;
            y <= end.y;
            y += converted_chunk_size, ++j) {


            auto count_all = get_entities_of_chunk(std::make_pair(i, j)).size();
            auto count_colliding = get_colliding_entities_of_chunk(std::make_pair(i, j)).size();

            if (count_colliding > 0) {
                draw_list->AddQuadFilled(
                    ImVec2(x, y),
                    ImVec2(x + converted_chunk_size, y),
                    ImVec2(x + converted_chunk_size, y + converted_chunk_size),
                    ImVec2(x, y + converted_chunk_size),
                    ImGui::GetColorU32((ImVec4)ImColor(
                    (count_colliding == 0 ? 0 : 100 * count_colliding),
                        0,
                        (count_colliding == 0 ? 255 : 0),
                        (count_colliding == 0 ? 20 : 150)))
                );
            }
            else {
                draw_list->AddQuadFilled(
                    ImVec2(x, y),
                    ImVec2(x + converted_chunk_size, y),
                    ImVec2(x + converted_chunk_size, y + converted_chunk_size),
                    ImVec2(x, y + converted_chunk_size),
                    ImGui::GetColorU32((ImVec4)ImColor(
                    (count_all == 0 ? 0 : 255 * count_all),
                        (count_all == 0 ? 0 : 255 * count_all),
                        0,
                        (count_all == 0 ? 20 : 150)))
                );
            }

            draw_list->AddQuad(
                ImVec2(x, y),
                ImVec2(x + converted_chunk_size, y),
                ImVec2(x + converted_chunk_size, y + converted_chunk_size),
                ImVec2(x, y + converted_chunk_size),
                ImGui::GetColorU32((ImVec4)ImColor(50 * count_colliding, 0, 255, 200))
            );


        }

        ImGui::EndChild();

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


