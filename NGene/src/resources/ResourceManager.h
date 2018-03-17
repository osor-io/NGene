#pragma once

#include "../utils/Manager.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <cassert>

#include "File.h"

template<typename T, typename R>
class ResourceManager : public Manager<T>
{
    friend class CRSP<T>;

    using ResourceTuple = std::tuple<std::unique_ptr<R>, unsigned int>;
    using ResourceMap = std::unordered_map<std::string, ResourceTuple>;

public:

    virtual ~ResourceManager() {
        /*
        This should have been cleaned up with shutdown
        */
        assert(m_resources.size() == 0);
    }

    void start_up() {

    }

    void shut_down() {
        m_resources.clear();
    }

    bool exists_resource(const std::string& filename) {
        auto it = m_resources.find(filename);
        if (it == m_resources.end()) {
            return file_exists(filename.c_str());
        }
        else {
            return true;
        }
    }

    R* get_resource(const std::string& filename) {
        auto it = m_resources.find(filename);
        if (it == m_resources.end()) {
            return load_and_store_resource(filename);
        }
        else {
            return std::get<0>(it->second).get();
        }
    }

    R* get_required_resource(const std::string& filename) {
        auto it = m_resources.find(filename);
        if (it == m_resources.end()) {
            return load_and_store_resource(filename, true);
        }
        else {
            ++std::get<1>(it->second);
            return std::get<0>(it->second).get();
        }
    }

    bool release_required_resource(const std::string& filename) {
        auto it = m_resources.find(filename);
        if (it == m_resources.end()) {
            return false;
        }
        else {
            auto count = --std::get<1>(it->second);
            if (count == 0) {
                m_resources.erase(it);
            }
            return true;
        }
    }

private:

    virtual R* load_resource(const std::string & filename) = 0;

    R * load_and_store_resource(const std::string & filename, bool required = false) {

        auto res = load_resource(filename);
        auto uses = 0u;
        if (required) uses = 1;
        m_resources[filename] = std::make_tuple(std::unique_ptr<R>(res), uses);
        return res;
    }

    ResourceMap m_resources;

};

