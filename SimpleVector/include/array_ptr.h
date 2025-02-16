#pragma once  
 
#include <algorithm>  
#include <cassert>    
#include <cstdlib>    
 
template <typename Type> 
class ArrayPtr { 
public: 
    ArrayPtr() = default; // Конструктор по умолчанию 
 
    explicit ArrayPtr(size_t size) { // Конструктор, принимающий размер массива 
        raw_ptr_ = size == 0 ? nullptr : new Type[size]; // Выделение памяти для массива или установка в nullptr, если размер 0 
    } 
 
    explicit ArrayPtr(Type* raw_ptr) noexcept { // Конструктор, принимающий сырой указатель 
        raw_ptr_ = raw_ptr; 
    } 
 
    ArrayPtr(const ArrayPtr&) = delete; // Запрет копирования объекта 
 
    ArrayPtr(ArrayPtr&& other) noexcept : raw_ptr_(other.raw_ptr_) { // Конструктор перемещения 
        other.raw_ptr_ = nullptr; // Назначение nullptr для сырого указателя другого объекта 
    } 
 
    ArrayPtr& operator=(ArrayPtr&& other) noexcept { // Оператор перемещения 
        if (this != &other) { // Проверка на самоприсваивание 
            delete[] raw_ptr_; // Освобождение памяти текущего объекта 
            raw_ptr_ = other.raw_ptr_; // Перенос сырого указателя другого объекта 
            other.raw_ptr_ = nullptr; // Назначение nullptr для сырого указателя другого объекта 
        } 
        return *this; 
    } 
 
    ~ArrayPtr() { // Деструктор 
        delete[] raw_ptr_; // Освобождение памяти 
    } 
 
    ArrayPtr& operator=(const ArrayPtr&) = delete; // Запрет копирования объекта 
 
    [[nodiscard]] Type* Release() noexcept { // Метод для освобождения сырого указателя и возврата его 
        Type* temp = raw_ptr_; 
        raw_ptr_ = nullptr; 
        return temp; 
    } 
 
    Type& operator[](size_t index) noexcept { // Перегрузка оператора [] для доступа к элементам массива 
        assert(raw_ptr_ != nullptr);  
        return raw_ptr_[index]; 
    } 
 
    const Type& operator[](size_t index) const noexcept { // Перегрузка оператора [] для доступа к элементам массива (константная версия) 
        assert(raw_ptr_ != nullptr);  
        return raw_ptr_[index]; 
    } 
 
    explicit operator bool() const { // Оператор приведения к типу bool, проверяющий, указывает ли сырой указатель на какой-либо объект 
        return raw_ptr_ != nullptr; 
    } 
 
    Type* Get() const noexcept { // Метод для получения сырого указателя 
        return raw_ptr_; 
    } 
 
    void swap(ArrayPtr& other) noexcept { // Метод для обмена содержимым с другим объектом 
        std::swap(raw_ptr_, other.raw_ptr_); 
    } 
 
private: 
    Type* raw_ptr_ = nullptr; // Сырой указатель на массив элементов 
}; 