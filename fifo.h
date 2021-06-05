#ifndef FIFO_H // include guard
#define FIFO_H
#include <iostream>
#include "cache.h"

class Fifo{
public:
    Fifo(int cap, int bs){
        capacity = cap;
        block_size = bs;
        count = 0;
        top = NULL;
        end = NULL;
    }
    
    void write_back(int & btom){
        Node * temp = top;
        while(temp!=NULL){
            if(temp->key->dirty==true)  
                btom+=block_size;
            temp = temp->next;
        }
    }

    void write(int tag, int& hit, int& miss, int& bfromm, int& btom){
        
        if(!is_empty()){
            Node * temp = top;
            while(temp!=NULL){
                if(temp->key->tag==tag){
                    hit++;
                    temp->key->dirty = true;
                    break;
                }
                temp = temp->next;
            }
            if(temp==NULL){
                miss++;
                if((is_full())&&(top->key->dirty == true)){
                    btom += block_size;
                }
                add_node(tag);
                bfromm += block_size;
                end->key->dirty = true;
            }
        }else{
            miss++;
            add_node(tag);
            bfromm += block_size;
            end->key->dirty = true;
        }
    }
    
    void read(int tag, int& hit, int& miss, int& bfromm, int& btom){
        
        if(!is_empty()){
            Node * temp = top;
            while(temp!=NULL){
                if((temp->key->tag)==tag){
                    hit++;
                    break;
                }
                temp = temp->next;
            }
            if(temp==NULL){
                if((is_full())&&(top->key->dirty == true)){
                    btom += block_size;
                }
                miss++;
                add_node(tag);
                end->key->dirty = false;
                bfromm += block_size;
            }
        }else{
            miss++;
            add_node(tag);
            end->key->dirty = false;
            bfromm += block_size;
        }
    }
    
private:
    class Node{
    public:
        Cache * key;
        Node * next;
    };
    Node * top;
    Node * end;
    int capacity;
    int block_size;
    int count;
    void add_node(int tag){
        if(is_full()){
            Node * temp = top;
            top = top->next;
            free(temp);
            end->next = new Node();
            end = end->next;
            end->key = new Cache();
            end->key->tag = tag;
            end->next = NULL;
        }else{
            if(is_empty()){
                top = new Node();
                top->key = new Cache();
                top->key->tag = tag;
                end = top;
                top->next = NULL;
                count++;
            }else{
                end->next = new Node();
                end = end->next;
                end->key = new Cache();
                end->key->tag = tag;
                end->next = NULL;
                count++;
            }
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