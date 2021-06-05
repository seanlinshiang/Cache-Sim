#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <vector>
#include "cache.h"
#include "fifo.h"
#include "lru.h"

using namespace std;

void direct(char **, int&, int&, int&, int&, int&, int&);
void tfefway(char **, int&, int&, int&, int&, int&, int&);
void hex2bin(string, string&);

//cache <cache size> <block size> <associativity> <replace policy> <file name>
int main (int argc, char *argv[]){
    //input arguments
    char asc = argv[3][0];
 
    //output arguments
    int cache_hit = 0, cache_miss= 0, l0_count= 0, l1_count= 0, b_from_m= 0, b_to_m= 0;
    double miss_rate;
    
    if(asc == '1')
        direct(argv, cache_hit, cache_miss, l0_count, l1_count, b_from_m, b_to_m);
    else if(asc == '2' || asc == '4' || asc == '8' || asc == 'f')
        tfefway(argv, cache_hit, cache_miss, l0_count, l1_count, b_from_m, b_to_m);
    else 
        cout<<"wrong associativity";

    double a = cache_miss, b = cache_hit+cache_miss;
    miss_rate = ((double)cache_miss/(double)(cache_hit+cache_miss));
    //output
    cout<<"Input file: "<<argv[5]<<endl;
    cout<<"Demand fetch: "<<(long)(cache_hit+cache_miss)<<endl;
    cout<<"Cache hit: "<<cache_hit<<endl;
    cout<<"Cache miss: "<<cache_miss<<endl;
    cout<<"Miss rate: "; printf("%.4lf\n",miss_rate);
    cout<<"Read data: "<<l0_count<<endl;
    cout<<"Write data: "<<l1_count<<endl;
    cout<<"Bytes from Memory: "<<b_from_m<<endl;
    cout<<"Bytes to Memory: "<<b_to_m<<endl;
}

void hex2bin(string input, string& output){
    int i;
    int a = 0;
    int b = 1;
    int c = 2;
    int d = 3;
    int x = 4;
    int size;
    size = input.size();
    for(i = 0; i < 32 - 4*size; i++){
        output+='0';
    }
    for (i = 0; i < size; i++){
        if (input[i] =='0'){
            output+="0000";
        }
        else if (input[i] =='1'){
            output+="0001";
        }    
        else if (input[i] =='2'){
            output+="0010";
        }    
        else if (input[i] =='3'){
            output+="0011";
        }    
        else if (input[i] =='4'){
            output+="0100";
        }    
        else if (input[i] =='5'){
            output+="0101";
        }    
        else if (input[i] =='6'){
            output+="0110";
        }    
        else if (input[i] =='7'){
            output+="0111";
        }    
        else if (input[i] =='8'){
            output+="1000";
        }
        else if (input[i] =='9'){
            output+="1001";
        }
        else if (input[i] =='a'){    
            output+="1010";
        }
        else if (input[i] =='b'){
            output+="1011";
        }
        else if (input[i] =='c'){
            output+="1100";
        }
        else if (input[i] =='d'){    
            output+="1101";
        }
        else if (input[i] =='e'){    
            output+="1110";
        }
        else if (input[i] =='f'){
            output+="1111";
        }
    }

    output+='\0';
}

void direct(char ** argv, int& cache_hit, int& cache_miss, int& l0_count, int& l1_count, int& b_from_m, int& b_to_m){
    //input arguments
    string replace, filename;
    int cache_size, block_size;
    fstream file;
    
    string address;
    int label, block_num, i_bits, o_bits, tag_bits;
    
    //main arguments
    cache_size = atoi(argv[1]);
    block_size = atoi(argv[2]);
    replace = argv[4];
    filename = argv[5];
    
    //calculate the bits
    block_num = cache_size*1024/block_size;
    i_bits = round(log2(block_num));
    o_bits = log2(block_size);
    tag_bits = 32 - i_bits - o_bits;
    
    //open file
    file.open(filename, ios::in);
    if(!file)
        cout<<"error";
    
    //Cache array initialize
    Cache * cache = (Cache*)malloc(block_num * sizeof(Cache)); 
    

    while(file>>label){
        file>>address;
        string baddress, tag, index;
        int tag_i, index_i;

        hex2bin(address, baddress);
        tag.append(baddress,0,tag_bits);
        index.append(baddress,tag_bits,i_bits);
        tag_i = stoi(tag,nullptr,2);
        index_i = stoi(index,nullptr,2);
        
        //read case
        if((label == 0) || (label == 2)){
            if(label==0) 
                l0_count++;
            if(cache[index_i].valid == false){
                cache[index_i].valid = true;
                cache[index_i].tag = tag_i;
                cache[index_i].dirty = false;
                b_from_m += block_size;
                cache_miss++;
            }else{
                if(cache[index_i].tag == tag_i){
                   cache_hit++;
                }else{
                    cache[index_i].tag = tag_i;
                    if(cache[index_i].dirty == true){
                        b_to_m += block_size;
                    }
                    b_from_m += block_size;
                    cache[index_i].dirty = false;
                    cache_miss++;
                }
            }
        //write case
        }else if(label == 1){
            l1_count++;
            if(cache[index_i].valid == false){
                cache[index_i].valid = true;
                cache[index_i].tag = tag_i;
                b_from_m += block_size;
                cache_miss++;
            }else{
                if(cache[index_i].tag == tag_i){
                    cache_hit++;
                }else{
                    cache[index_i].tag = tag_i;
                    if(cache[index_i].dirty == true){
                        b_to_m += block_size;
                    }
                    b_from_m += block_size;
                    cache_miss++;
                }
            }
            cache[index_i].dirty = true;
        }
    }
    for(int i = 0; i < block_num; i++){
        if(cache[i].dirty==true){
            b_to_m += block_size;
        }
    }
    file.close();
}

void tfefway(char ** argv, int& cache_hit, int& cache_miss, int& l0_count, int& l1_count, int& b_from_m, int& b_to_m){
    //input arguments
    string replace, filename;
    int asc;
    int cache_size, block_size;
    fstream file;
    
    string address;
    int label, block_num, i_bits, o_bits, tag_bits;
    
    //main arguments
    cache_size = atoi(argv[1]);
    block_size = atoi(argv[2]);
    asc = atoi(argv[3]);
    replace = argv[4];
    filename = argv[5];
    
    //calculate the bits
    if(asc == 2 || asc == 4 || asc == 8)
        block_num = (cache_size*1024/block_size)/asc;
    else    
        block_num = (cache_size*1024/block_size);
    if(asc == 2 || asc == 4 || asc == 8)
        i_bits = round(log2(block_num));
    else
        i_bits = 0;
    o_bits = log2(block_size);
    tag_bits = 32 - i_bits - o_bits;

    //open file
    file.open(filename, ios::in);
    if(!file)
        cout<<"error";

    if(replace == "FIFO"){
        vector<Fifo> f;
        
        for(int i = 0; i < (block_num); i++){
            if(asc == 2 || asc == 4 || asc == 8)
                f.push_back(Fifo(asc, block_size));
            else
                f.push_back(Fifo(block_num, block_size));
        }

        while(file>>label){
            file>>address;
            string baddress, tag, index;
            int tag_i, index_i;
            hex2bin(address, baddress);
            tag.append(baddress,0,tag_bits);
            if(asc == 2 || asc == 4 || asc == 8)  
                index.append(baddress,tag_bits,i_bits);
            tag_i = stoi(tag,nullptr,2);
            if(asc == 2 || asc == 4 || asc == 8)  
                index_i = stoi(index,nullptr,2);
            else
                index_i = 0;            
            //read case
            if((label == 0) || (label == 2)){
                f[index_i].read(tag_i,cache_hit,cache_miss,b_from_m,b_to_m);
                if(label==0) 
                    l0_count++;
            //write case
            }else if(label == 1){
                f[index_i].write(tag_i,cache_hit,cache_miss,b_from_m,b_to_m);
                l1_count++;
            }
            
        }
        for(int i = 0; i < (block_num); i++){
            f[i].write_back(b_to_m);
        }
    }else if(replace == "LRU"){
        vector<Lru> l;
            
        for(int i = 0; i < (block_num); i++){
            if(asc == 2 || asc == 4 || asc == 8)
                l.push_back(Lru(asc, block_size));
            else
                l.push_back(Lru(block_num, block_size));
        }
        
        while(file>>label){
            file>>address;
            string baddress, tag, index;
            int tag_i, index_i;
            hex2bin(address, baddress);
            tag.append(baddress,0,tag_bits);
            if(asc == 2 || asc == 4 || asc == 8)  
                index.append(baddress,tag_bits,i_bits);
            tag_i = stoi(tag,nullptr,2);
            if(asc == 2 || asc == 4 || asc == 8)  
                index_i = stoi(index,nullptr,2);
            else
                index_i = 0;            
            //read case
            if((label == 0) || (label == 2)){
                l[index_i].read(tag_i,cache_hit,cache_miss,b_from_m,b_to_m);
                if(label==0) 
                    l0_count++;
            //write case
            }else if(label == 1){
                l[index_i].write(tag_i,cache_hit,cache_miss,b_from_m,b_to_m);
                l1_count++;
            }
        }
        for(int i = 0; i < (block_num); i++){
            l[i].write_back(b_to_m);
        }
    }
    file.close();
}