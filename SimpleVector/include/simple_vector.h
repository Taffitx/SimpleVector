#pragma once 
 
#include <cassert> 
#include <initializer_list> 
#include <stdexcept> 
#include <algorithm> 
#include <memory> 
#include <utility> 
 
#include "array_ptr.h"  
 
// Класс, представляющий объект резервирования 
class ReserveProxyObj { 
public: 
    explicit ReserveProxyObj(size_t capacity) 
        : capacity_(capacity) {} 
 
    size_t GetCapacity() const { 
        return capacity_; 
    } 
 
private: 
    size_t capacity_; 
}; 
 
// Функция для создания объекта ReserveProxyObj 
ReserveProxyObj Reserve(size_t capacity_to_reserve) { 
    return ReserveProxyObj(capacity_to_reserve); 
} 
 
// Шаблонный класс SimpleVector 
template <typename Type> 
class SimpleVector { 
public: 
    using Iterator = Type*;           // Определение типа итератора 
    using ConstIterator = const Type*; // Определение типа константного итератора 
 
    SimpleVector() noexcept = default; // Конструктор по умолчанию 
 
    // Конструктор с резервированием 
    SimpleVector(ReserveProxyObj reserve_obj) { 
        Reserve(reserve_obj.GetCapacity()); 
    } 
 
    // Конструктор с заданным размером 
    explicit SimpleVector(size_t size) 
        : size_(size), capacity_(size), data_(size ? new Type[size] : nullptr) { 
        std::fill(data_.Get(), data_.Get() + size_, Type()); 
    } 
 
    // Конструктор с заданным размером и значением 
    SimpleVector(size_t size, const Type& value) 
        : size_(size), capacity_(size), data_(size ? new Type[size] : nullptr) { 
        std::fill(data_.Get(), data_.Get() + size_, value); 
    } 
 
    // Конструктор с инициализатором списка 
    SimpleVector(std::initializer_list<Type> init) 
        : size_(init.size()), capacity_(init.size()), data_(init.size() ? new Type[init.size()] : nullptr) { 
        std::copy(init.begin(), init.end(), data_.Get()); 
    } 
 
    // Копирующий конструктор 
    SimpleVector(const SimpleVector& other) 
        : size_(other.size_), capacity_(other.capacity_), data_(other.capacity_ ? new Type[other.capacity_] : nullptr) { 
        std::copy(other.begin(), other.end(), data_.Get()); 
    } 
 
    // Перемещающий конструктор 
    SimpleVector(SimpleVector&& other) noexcept 
        : size_(other.size_), capacity_(other.capacity_), data_(std::move(other.data_)) { 
        other.size_ = 0; 
        other.capacity_ = 0; 
    } 
 
    // Копирующее присваивание 
    SimpleVector& operator=(const SimpleVector& rhs) { 
        if (this != &rhs) { 
            SimpleVector tmp(rhs); 
            swap(tmp); 
        } 
        return *this; 
    } 
 
    // Перемещающее присваивание  
    SimpleVector& operator=(SimpleVector&& other) noexcept { 
        if (this != &other) { 
            size_ = std::exchange(other.size_, 0); 
            capacity_ = std::exchange(other.capacity_, 0); 
            data_ = std::move(other.data_); 
        } 
        return *this; 
    } 
 
    ~SimpleVector() { 
        Clear(); 
    } 
 
    // Получение текущего размера вектора 
    size_t GetSize() const noexcept { 
        return size_; 
    } 
 
    // Получение текущей емкости вектора 
    size_t GetCapacity() const noexcept { 
        return capacity_; 
    } 
 
    // Проверка на пустоту 
    bool IsEmpty() const noexcept { 
        return size_ == 0; 
    } 
 
    // Оператор доступа к элементу по индексу 
    Type& operator[](size_t index) noexcept { 
        assert(index < size_); 
        return data_[index]; 
    } 
 
    const Type& operator[](size_t index) const noexcept { 
        assert(index < size_); 
        return data_[index]; 
    } 
 
    // Метод доступа к элементу с проверкой границ 
    Type& At(size_t index) { 
        if (index >= size_) { 
            throw std::out_of_range("Index out of range"); 
        } 
        return data_[index]; 
    } 
 
    const Type& At(size_t index) const { 
        if (index >= size_) { 
            throw std::out_of_range("Index out of range"); 
        } 
        return data_[index]; 
    } 
 
    // Очистка вектора 
    void Clear() noexcept { 
        for (size_t i = 0; i < size_; ++i) { 
            data_[i].~Type(); // Явный вызов деструктора 
        } 
        size_ = 0; 
    } 
 
    // Изменение размера вектора 
    void Resize(size_t new_size) { 
        if (new_size <= size_) { 
            for (size_t i = new_size; i < size_; ++i) { 
                data_[i].~Type(); 
            } 
        } 
        else { 
            Reserve(new_size); // Увеличение емкости при необходимости 
            for (size_t i = size_; i < new_size; ++i) { 
                new (data_.Get() + i) Type(); // Размещение новых объектов 
            } 
        } 
        size_ = new_size; 
    } 
 
    // Добавление элемента в конец (перемещающий вариант) 
    void PushBack(Type&& value) { 
        if (size_ == capacity_) { 
            Reserve(std::max(capacity_ * 2, static_cast<size_t>(1))); 
        } 
        new (data_.Get() + size_) Type(std::move(value)); // Размещение нового элемента 
        ++size_; 
    } 
 
    // Добавление элемента в конец (копирующий вариант) 
    void PushBack(const Type& value) { 
        if (size_ == capacity_) { 
            Reserve(std::max(capacity_ * 2, static_cast<size_t>(1))); 
        } 
        new (data_.Get() + size_) Type(value); // Размещение нового элемента 
        ++size_; 
    } 
 
    // Удаление последнего элемента  
    void PopBack() noexcept { 
        assert(!IsEmpty()); 
        data_[size_ - 1].~Type(); // Явный вызов деструктора  
        --size_; 
    } 
 
    // Вставка элемента в произвольную позицию (перемещающий вариант)  
    Iterator Insert(ConstIterator pos, Type&& value) { 
        assert(pos >= cbegin() && pos <= cend()); 
        size_t index = pos - cbegin(); 
        if (size_ == capacity_) { 
            Reserve(std::max(2 * capacity_, static_cast<size_t>(1))); 
        } 
        Iterator insert_pos = data_.Get() + index; 
        if (insert_pos != data_.Get() + size_) { 
            std::move_backward(insert_pos, data_.Get() + size_, data_.Get() + size_ + 1); 
        } 
        new (insert_pos) Type(std::move(value)); // Размещение нового элемента  
        ++size_; 
        return insert_pos; 
    } 
 
    // Вставка элемента в произвольную позицию (копирующий вариант)  
    Iterator Insert(ConstIterator pos, const Type& value) { 
        assert(pos >= cbegin() && pos <= cend()); 
        size_t index = pos - cbegin(); 
        if (size_ == capacity_) { 
            Reserve(std::max(2 * capacity_, static_cast<size_t>(1))); 
        } 
        Iterator insert_pos = data_.Get() + index; 
        if (insert_pos != data_.Get() + size_) { 
            std::move_backward(insert_pos, data_.Get() + size_, data_.Get() + size_ + 1); 
        } 
        new (insert_pos) Type(value); // Размещение нового элемента  
        ++size_; 
        return insert_pos; 
    } 
 
    // Удаление элемента в произвольной позиции  
    Iterator Erase(ConstIterator pos) { 
        assert(pos >= cbegin() && pos < cend()); 
        size_t index = pos - cbegin(); 
        Iterator erase_pos = data_.Get() + index; 
        erase_pos->~Type(); // Явный вызов деструктора  
        std::move(erase_pos + 1, data_.Get() + size_, erase_pos); // Перемещение элементов  
        --size_; 
        return erase_pos; 
    } 
 
    // Резервирование памяти 
    void Reserve(size_t new_capacity) { 
        if (new_capacity > capacity_) { 
            ArrayPtr<Type> new_data(new_capacity); // Создание нового массива 
            std::move(data_.Get(), data_.Get() + size_, new_data.Get()); // Перемещение элементов 
            for (size_t i = 0; i < size_; ++i) { 
                data_[i].~Type(); // Явный вызов деструктора 
            } 
            data_ = std::move(new_data); // Перемещение данных 
            capacity_ = new_capacity; 
        } 
    } 
 
    // Обмен данными с другим вектором 
    void swap(SimpleVector& other) noexcept { 
        std::swap(size_, other.size_); 
        std::swap(capacity_, other.capacity_); 
        data_.swap(other.data_); 
    } 
 
    // Итераторы 
    Iterator begin() noexcept { 
        return data_.Get(); 
    } 
 
    Iterator end() noexcept { 
        return data_.Get() + size_; 
    } 
 
    ConstIterator begin() const noexcept { 
        return data_.Get(); 
    } 
 
    ConstIterator end() const noexcept { 
        return data_.Get() + size_; 
    } 
 
    ConstIterator cbegin() const noexcept { 
        return data_.Get(); 
    } 
 
    ConstIterator cend() const noexcept { 
        return data_.Get() + size_; 
    } 
 
private: 
    size_t size_ = 0;           // Текущий размер вектора 
    size_t capacity_ = 0;       // Текущая емкость вектора 
    ArrayPtr<Type> data_;       // Указатель на массив элементов 
}; 
 
// Операторы сравнения 
template <typename Type> 
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) { 
    return lhs.GetSize() == rhs.GetSize() && std::equal(lhs.begin(), lhs.end(), rhs.begin()); 
} 
 
template <typename Type> 
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) { 
    return !(lhs == rhs); 
} 
 
template <typename Type> 
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) { 
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); 
} 
 
template <typename Type> 
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) { 
    return !(rhs < lhs); 
} 
 
template <typename Type> 
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) { 
    return rhs < lhs; 
} 
 
template <typename Type> 
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) { 
    return !(lhs < rhs); 
} 