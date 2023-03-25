#pragma once

#include <cassert>
#include <cstddef>
#include <iterator>
#include <string>
#include <utility>
#include <algorithm>

template <typename Type>
class SingleLinkedList
{
    // Узел списка
    struct Node
    {
        Node() = default;
        Node(const Type &val, Node *next)
            : value(val), next_node(next)
        {
        }
        Type value;
        Node *next_node = nullptr;
    };

    // Шаблон класса Базовый Итератор.
    // Определяет поведение итератора на элементы односвязного списка
    // ValueType - совпадает с Type (для Iterator) либо с const Type (для ConstIterator)
    template <typename ValueType>
    class BasicIterator
    {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node *node)
        {
            node_ = node;
        }

    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

        // Категория итератора - forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType *;
        // Тип ссылки на итерируемое значение
        using reference = ValueType &;

        BasicIterator() = default;

        /// @brief Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type> &other) noexcept
        {
            node_ = other.node_;
        }

        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас.
        BasicIterator &operator=(const BasicIterator &rhs) = default;

        /// @brief Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка, либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type> &rhs) const noexcept
        {
            return this->node_ == rhs.node_;
        }

        /// @brief Оператор проверки итераторов на неравенство
        [[nodiscard]] bool operator!=(const BasicIterator<const Type> &rhs) const noexcept
        {
            return !(this->node_ == rhs.node_);
        }

        /// @brief Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка, либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type> &rhs) const noexcept
        {
            return this->node_ == rhs.node_;
        }

        /// @brief Оператор проверки итераторов на неравенство
        [[nodiscard]] bool operator!=(const BasicIterator<Type> &rhs) const noexcept
        {
            return !(this->node_ == rhs.node_);
        }

        /// @brief Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator &operator++() noexcept
        {
            assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        /// @brief Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка.
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept
        {
            auto old_value(*this);
            node_ = node_->next_node;
            return old_value;
        }

        /// @brief Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept
        {
            assert(node_ != nullptr);
            return node_->value;
        }

        /// @brief Операция доступа к члену класса. Возвращает указатель на текущий элемент списка.
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept
        {
            assert(node_ != nullptr);
            return &node_->value;
        }

    private:
        Node *node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type &;
    using const_reference = const value_type &;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;

    SingleLinkedList()
    {
    }

    /// @brief Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept
    {
        // Заглушка. Реализуйте метод самостоятельно
        return size_;
    }

    /// @brief  Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept
    {
        // Заглушка. Реализуйте метод самостоятельно
        return size_ == 0;
    }

    /// @brief  Вставляет элемент value в начало списка за время O(1)
    void PushFront(const Type &value)
    {
        // Реализуйте метод самостоятельно
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }

    /// @brief  Очищает список за время O(N)
    void Clear() noexcept
    {
        // Реализуйте метод самостоятельно
        while (head_.next_node != nullptr)
        {
            Node *new_head = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = new_head;
        }
        size_ = 0;
    }

    /// @brief Удаляет первый элемента непустого списка за время O(1). Не выбрасывает исключений
    void PopFront() noexcept
    {
        assert(!IsEmpty());

        Node *new_head = head_.next_node->next_node;
        delete head_.next_node;
        head_.next_node = new_head;
        --size_;
    }

    /// @brief вставляет в список новое значение следом за элементом, на который ссылается переданный в InsertAfter итератор
    Iterator InsertAfter(ConstIterator pos, const Type &value)
    {
        assert(pos.node_ != nullptr);

        pos.node_->next_node = new Node(value, pos.node_->next_node);
        ++size_;
        return Iterator{pos.node_->next_node};
    }

    /// @brief удаляет из списка элемент, следующий за элементом, на который ссылается переданный в InsertAfter итератор
    Iterator EraseAfter(ConstIterator pos) noexcept
    {
        assert(pos.node_ != nullptr &&
               pos.node_->next_node != nullptr);

        Node *temp = pos.node_->next_node->next_node;
        delete pos.node_->next_node;
        pos.node_->next_node = temp;
        --size_;

        return Iterator{pos.node_->next_node};
    }

    ~SingleLinkedList()
    {
        Clear();
    }

    [[nodiscard]] Iterator begin() noexcept
    {
        return Iterator{head_.next_node};
    }
    [[nodiscard]] Iterator end() noexcept
    {
        return Iterator{nullptr};
    }

    [[nodiscard]] ConstIterator begin() const noexcept
    {
        return ConstIterator{head_.next_node};
    }
    [[nodiscard]] ConstIterator end() const noexcept
    {
        return ConstIterator{nullptr};
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept
    {
        return ConstIterator{head_.next_node};
    }
    [[nodiscard]] ConstIterator cend() const noexcept
    {
        return ConstIterator{nullptr};
    }

    [[nodiscard]] Iterator before_begin() noexcept
    {
        return Iterator{&head_};
    }
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept
    {
        return ConstIterator{const_cast<Node *>(&head_)};
    }
    [[nodiscard]] ConstIterator before_begin() const noexcept
    {
        return cbefore_begin();
    }

    SingleLinkedList(std::initializer_list<Type> values)
    {
        Assign(values.begin(), values.end());
    }

    SingleLinkedList(const SingleLinkedList &other)
    {
        Assign(other.begin(), other.end());
    }

    SingleLinkedList &operator=(const SingleLinkedList &rhs)
    {
        if (this != &rhs)
        {
            SingleLinkedList tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    void swap(SingleLinkedList &other) noexcept
    {
        // Реализуйте обмен содержимого списков самостоятельно
        std::swap(other.head_.next_node, head_.next_node);
        std::swap(other.size_, size_);
    }

private:
    template <typename InputIterator>
    void Assign(InputIterator from, InputIterator to)
    {
        SingleLinkedList<Type> tmp;
        Node **node_ptr = &tmp.head_.next_node;
        while (from != to)
        {
            assert(*node_ptr == nullptr);
            *node_ptr = new Node(*from, nullptr);
            ++tmp.size_;
            node_ptr = &((*node_ptr)->next_node);
            ++from;
        }
        swap(tmp);
    }

    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type> &lhs, SingleLinkedList<Type> &rhs) noexcept
{
    lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    std::equal(lhs.begin(), lhs.end(), rhs.begin(), lhs.end());
    return true;
}

template <typename Type>
bool operator!=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    return true;
}

template <typename Type>
bool operator<=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return rhs < lhs;
}

template <typename Type>
bool operator>(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs)
{
    return rhs < lhs;
}
