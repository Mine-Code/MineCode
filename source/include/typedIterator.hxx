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

    T peek(){
        return array[index];
    };
    T peek(size_t offset){
        if(index <= 0 || array.size() <= index){
            std::cerr << "Out of size" << std::endl;;
            return EOF;
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