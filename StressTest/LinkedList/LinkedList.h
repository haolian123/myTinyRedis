#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include<iostream>
#include<vector>
#include<memory>
#include<random>
#include<cstring>
#include<string>
#include<fstream>
#include<mutex>

template<typename T,typename U>
struct LinkedNode{
    T key;
    U value;
    std::shared_ptr<LinkedNode<T,U>>next;
    LinkedNode(T key,U value):key(key),value(value),next(nullptr){}
};

template<typename Key,typename Value>
class LinkedList{
    std::shared_ptr<LinkedNode<Key,Value>>head;
    std::mutex mutex;
public:
    LinkedList(){
        Key key;
        Value value;
        head = std::make_shared<LinkedNode<Key,Value>>(key,value);
    }
    ~LinkedList(){

    }
    bool addItem(const Key& key, const Value& value){
        mutex.lock();
        auto curNode = head;
        auto newNode = std::make_shared<LinkedNode<Key,Value>>(key,value);
        while(curNode->next&&curNode->next->key<key){
            curNode = curNode->next;
        }
        if(curNode->next == nullptr){
            curNode->next = newNode;
        }else{
            newNode->next = curNode->next;
            curNode->next = newNode;
        }
        mutex.unlock();
        return true;
    }
    std::shared_ptr<LinkedNode<Key,Value>> searchItem(const Key& key){
        mutex.lock();
        auto curNode = head;
        while(curNode->next&&curNode->next->key<key){
            curNode = curNode->next;
        }
        mutex.unlock();
        if(curNode->next && curNode->next->key==key){
            return std::shared_ptr<LinkedNode<Key,Value>>(curNode->next);
        }
        return nullptr;
    }
    bool deleteItem(const Key& key){
        mutex.lock();
        auto curNode = head;
        while(curNode->next&&curNode->next->key<key){
            curNode = curNode->next;
        }
        mutex.unlock();
        if(curNode->next && curNode->next->key==key){
            auto tmpNode = curNode->next;
            curNode->next = tmpNode->next;
            return true;
        }
        return false;
    }
};

#endif 