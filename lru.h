#ifndef LRU_H // include guard
#define LRU_H
#include <iostream>
#include <map>
#include "cache.h"

using namespace std;

class Lru{
public:
    Lru(int cap, int bs){
        capacity = cap;
        block_size = bs;
        count = 0;
        head = NULL;
        last = NULL;
        m.clear();
    }

    void write_back(int & btom){
        Node * temp = head;
        while(temp!=NULL){
            if(temp->key->dirty==true)  
                btom+=block_size;
            temp = temp->next;
        }
    }

    void write(int tag, int& hit, int& miss, int& bfromm, int& btom){
        if(!is_empty()){
            map<int, Node*>::iterator iter;
            iter = m.find(tag);
            if(iter!=m.end()){
                hit++;
                move_node(tag);
                head->key->dirty = true;
            }else{
                miss++;
                if((is_full())&&(last->key->dirty == true)){
                    btom += block_size;
                }
                add_node(tag);
                bfromm += block_size;
                head->key->dirty = true;
            }
        }else{
            miss++;
            add_node(tag);
            bfromm += block_size;
            head->key->dirty = true;
        }
    }

    void read(int tag, int& hit, int& miss, int& bfromm, int& btom){
        if(!is_empty()){
            map<int, Node*>::iterator iter;
            iter = m.find(tag);
            if(iter!=m.end()){
                hit++;
                move_node(tag);
            }else{
                miss++;
               if((is_full())&&(last->key->dirty == true)){
                    btom += block_size;
                }
                add_node(tag);
                bfromm += block_size;
                head->key->dirty = false;
            }
        }else{
            miss++;
            add_node(tag);
            bfromm += block_size;
            head->key->dirty = false;
        }
    }


private:
    class Node{
    public:
        Cache * key;
        Node * next;
        Node * prev;
    };

    Node * head, * last; 
    int capacity;
    int block_size;
    int count;
    map <int, Node*> m;

    void add_node(int tag){
        if(is_full()){
            Node * temp = last;
            last = last->prev;
            last->next = NULL;
            m.erase(temp->key->tag);
            temp = new Node();
            temp->key = new Cache();
            temp->key->tag = tag;
            head->prev = temp;
            temp->prev = NULL;
            temp->next = head;
            head = temp;
            m[tag] = temp;
        }else{
            if(is_empty()){
                Node * temp = new Node();
                temp->key = new Cache();
                temp->key->tag = tag;
                temp->prev = NULL;
                temp->next = NULL;
                head = temp;
                last = temp;
                m[tag] = temp;
                count++;
            }else{
                Node * temp = new Node();
                temp->key = new Cache();
                temp->key->tag = tag;
                head->prev = temp;
                temp->prev = NULL;
                temp->next = head;
                head = temp;
                m[tag] = temp;
                count++;
            }
        }
    }
    
    void move_node(int tag){
        if(m[tag] != head){
            Node * temp = m[tag];
            if(temp == last){
                temp->prev->next = NULL;
                last = temp->prev;
            }else{
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
            }
            head->prev = temp;
            temp->next = head;
            temp->prev = NULL;
            head = temp;
        }
    }

    bool is_full(){
        if(count==capacity){
            return true;
        }else{
            return false;
        }
    }
    
    bool is_empty(){
        if(count==0){
            return true;
        }else{
            return false;
        }
    }
};

#endif