#include <stdio.h>
#include <iostream>
#include <fstream>
#include<stdlib.h>
using namespace std;
int mem[128][32];
int disk[128][32];
int PTDR =  0x6c;
char outfile[]="answer.txt"; //结果输出文件
  ofstream out(outfile);
int test(int addr)
{
         printf("%x  \n",addr);
        int pde_off = (addr>>10) & 31;
        cout<<pde_off<<endl;
        int pte_off = (addr>>5) & 31;
        cout<<pte_off<<endl;
        int phy_off = addr & 31;
        cout<<phy_off<<endl;
    
}
int find(int addr){
  
    int pde_off = (addr>>10) & 31;
    int pte_off = (addr>>5) & 31;
    int phy_off = addr & 31;
    int pde_con = mem[PTDR][pde_off];
    bool pde_valid = pde_con >> 7;
    char str[4][30];
    itoa(pde_off,str[0],16);
    itoa(pde_con,str[1],16);
    itoa((int)pde_con&127,str[2],16);
    out<<"pde index:0x"<<str[0]<<"  pde contents:(0x"<<str[1]<<",  valid "<<pde_valid<<", pfn 0x"<<str[2]<<")"<<endl;

    if (pde_valid){
        pde_con &= 127;
        int pte_addr = pde_con;
        int pte_con = mem[pte_addr][pte_off];
        bool pte_valid = pte_con >> 7;
         itoa(pte_off,str[0],16);
          itoa(pte_con,str[1],16);
          itoa((int)pte_con&127,str[2],16);
    out<<"pte index:0x"<<str[0]<<"  pte contents:(0x"<<str[1]<<",  valid "<<pde_valid<<", pfn 0x"<<str[2]<<")"<<endl;
        if (pte_valid){

            pte_con &= 127;
            int phy_addr = pte_con;
            int phy_con = mem[phy_addr][phy_off];
            itoa((32*pte_con+phy_off),str[0],16);
            itoa(phy_con,str[1],16);
            out<<" To Physical Address 0x"<<str[0]<< "--> value : 0x"<<str[1]<<endl;
            return phy_con;
        } else {
            pte_con &= 127;
            int disk_addr = pte_con;
            //disk
            int disk_con = disk[disk_addr][phy_off];
             itoa((32*pte_con+phy_off),str[0],16);
             itoa(disk_con,str[1],16);
              out<<" To Physical Address 0x"<<str[0]<< "--> value : 0x"<<str[1]<<endl;
            return disk_con;
        }
    } else {
           out<<"Fault (page directory entry not valid)"<<endl;
        return -1;
    }
}
int ctod(char a) {
    int b;
    if ((int)a < 58) {
        b = (int)(a-'0');
    }
    else {
        b = (int)(a-'a')+10;
    }
    return b;
}
int stonum(char* b,int col)
{
    
    return ctod(b[col*3+9])*16+ctod(b[col*3+10]);
}
int main(){
    
    ifstream in("mem.txt");  
    char buffer[256];
    if (! in.is_open()) {
      //  cout << "opening file failed!"<<endl; 
    }  
    else {      
        int l = 128;
        int j = 0;
        while (l--) { 
            in.getline (buffer,256); 
            for (int i = 0; i < 32; i++) {

                    mem[j][i] = stonum(buffer,i);
            }
            j++;
           
        } 
    } 
    in.close();
    in.open("disk.txt");
     if (! in.is_open()) {
        //cout << "opening file failed!"<<endl; 
    }  
    else {      
        int l = 128;
        int j = 0;
        while (l--) { 
            in.getline (buffer,256); 
            for (int i = 0; i < 32; i++) {

                    disk[j][i] = stonum(buffer,i);
            }
            j++;
           
        } 
    } 
    in.close();
   // find(0x0bd6);
    //test(0x0bd6);
    find(0x6653);
    find(0x1c13);
    find(0x6890);
    find(0xaf6);
    find(0x1e6f);
    out.close();
    system("pause");
    return 0;
}
