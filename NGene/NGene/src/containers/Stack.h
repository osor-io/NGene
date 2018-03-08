#pragma once

namespace config {
    constexpr size_t default_stack_size = 64;
}  // namespace config

template <typename T, size_t startingSize = config::default_stack_size>
class Stack {
public:
    Stack() {
        m_data = static_cast<T*>(malloc(startingSize * sizeof(T)));
        m_maxSize = startingSize;
        m_used = 0;
    }
    ~Stack() { free(m_data); };

    bool hasElement(T elem) const {
        for (size_t i = 0; i < m_used; ++i) {
            if (elem == m_data[i]) return true;
        }
        return false;
    }

    void printTop(size_t elems) const {
        for (size_t i = 0; i < elems; ++i) {
            std::cout << &m_data[m_used - i - 1] << ": " << m_data[m_used - i - 1]
                << std::endl;
        }
    }

    void push(T elem) {
        if (m_used == m_maxSize) {
            m_maxSize <<= 1;
            m_data = static_cast<T*>(realloc(m_data, m_maxSize * sizeof(T)));
        }
        m_data[m_used++] = elem;
    }

    T pop() {
        return m_data[--m_used];
    }

    void clear() { m_used = 0; }

    size_t size() const { return m_used; }

    size_t currentMaxSize() const { return m_maxSize; }

private:
    size_t m_maxSize;
    size_t m_used;
    T* m_data;
};
