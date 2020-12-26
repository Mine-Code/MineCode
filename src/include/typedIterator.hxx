#pragma once
#ifndef CHITER_H
#define CHITER_H

#include <vector>
#include <iostream>

template <typename T>
class iterator
{
private:
    std::vector<T> array;
public:
    size_t index;
    iterator()
        :array(), index(0)
    {
        array=std::vector<T>();
    }
    iterator(std::vector<T> src)
        :array(src), index(0)
    {}
    T next(){
        if(array.size() <= index){
            std::cerr << "Out of size" << std::endl;;
            throw L"outofsize";
        }
        return array[index++];
    }
    T prev(){
        if(index <= 0){
            std::cerr << "Out of size" << std::endl;;
            throw L"outofsize";
        }
        return array[index--];
    }
    T peekSafe(){
        if(index <= 0 || array.size() <= index){
            return T();
        }
        return array[index];
    }
    T peek(){
        if(index <= 0 || array.size() <= index){
            return T();
        }
        return array[index];
    };
    T peek(size_t offset){
        if(index <= 0 || array.size() <= index){
            return T();
        }
        return array[index+offset];
    }
    bool hasData(){
        if(array.size() <= index){
            return false;
        }
        return true;
    }
};

#endif