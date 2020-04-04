#pragma once

#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <chrono>

using namespace std;

class Log
{
private:
    string dir;

public:
    Log(){}
    Log(const string path){
        SET_LOG_PATH(path);
    }
    ~Log(){}

    void SET_LOG_PATH(const string path){dir = path;}

    string GET_CURR_TIME(){
        auto t = chrono::system_clock::to_time_t(chrono::system_clock::now());
	    stringstream ss;
	    ss << put_time(std::localtime(&t), "%Y-%m-%d-%H-%M-%S");
        return ss.str();
    }

    void REGARD(char op, uint64_t key, string val){
        ofstream fout(dir, ios::binary | ios::app);

        string str = GET_CURR_TIME();

        fout.write((char*)&op, sizeof(char));

        fout.write((char*)&key, sizeof(uint64_t));

        fout.write(val.c_str(), sizeof(char) * (val.size()));

        fout.write(str.c_str(), sizeof(char) * (str.size()));

        fout.close();
    }

    void RESET(){
        remove(dir.c_str());
    }
};

#endif // log_hpp