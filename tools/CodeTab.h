#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/* Lucily I find a website restoring codes What Deng Junhun write in his class 
 * The website is https://dsa.cs.tsinghua.edu.cn/~deng/ds/src_link/
 */


bool CodeTab(const char* fileOut, const char* fileIn, int tabSize){
    fstream CodeSrc(fileOut);
    fopen(fileIn,"w");
    if(!CodeSrc){
        cout << "CodeSrc.txt open failed."<<endl;
        return false;
    }

    ofstream CodeMod("CodeModelize.txt");
    fopen("CodeModelize.txt","w");
    if(!CodeMod){
        cout << "CodeModelize.txt open failed." <<endl;
        return false; 
    }

    string tmp;
    while (getline(CodeSrc,tmp)){
        tmp = tmp.substr(5,tmp.size());
        CodeMod << tmp<< endl;
    }

    CodeMod.close();
    CodeSrc.close();
    return true;
}