#pragma once

namespace config {
    constexpr size_t default_stack_size = 64;
}  // namespace config

template <typename T, size_t startingSize = config::default_stack_size>
class Stack {
public:
    Stack() {
        m_data = static_cast<T*>(malloc(startingSize * sizeof(T)));
        m_max_size = startingSize;
        m_used = 0;
    }
    ~Stack() { free(m_data); };

    bool has_element(T elem) const {
        for (size_t i = 0; i < m_used; ++i) {
            if (elem == m_data[i]) return true;
        }
        return false;
    }

    void print_top(size_t elems) const {
        for (size_t i = 0; i < elems; ++i) {
            std::cout << &m_data[m_used - i - 1] << ": " << m_data[m_used - i - 1]
                << std::endl;
        }
    }

    void push(T elem) {
        if (m_used == m_max_size) {
            m_max_size <<= 1;
            m_data = static_cast<T*>(realloc(m_data, m_max_size * sizeof(T)));
        }
        m_data[m_used++] = elem;
    }

    T pop() {
        return m_data[--m_used];
    }

    void clear() { m_used = 0; }

    size_t size() const { return m_used; }

    size_t current_max_size() const { return m_max_size; }

private:
    size_t m_max_size;
    size_t m_used;
    T* m_data;
};
