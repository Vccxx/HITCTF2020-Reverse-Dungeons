#include <iostream>
#include <string.h>
#include "maze.h"
#define UNAME_LEN 11
#define PWD_LEN 98
#define MAP_SIZE 5
#define KEY_LEN 25
using namespace std;
using namespace MazeNamespace;
char flag_char_table[16] = {'L', 'l', 'i', 'I', 'p', '!', 'a', 'A', 'M', 'm', 'T', 't', 'e', '2', 'z', 'v'};
char *get_flag(char *pwd)
{
    int idx = 0;
    char *flag = (char *)calloc(sizeof(char), 32);
    for (int i = 0; i < strlen(pwd); i++)
    {
        idx += int(pwd[i]) * rand();
    }
    int i = 0;
    while (idx != 0)
    {
        flag[i++] = flag_char_table[idx & 0x0f];
        idx = idx >> 4;
    }
    return flag;
}
bool check_pwd(char *pwd, Maze_DFS m, Cell *startCell)
{
    Cell *c = startCell;
    for (int i = 0; i < strlen(pwd); i++)
    {
        int s = c->connected_cell.size();
        unordered_map<char, Cell *>::const_iterator got = c->connected_cell.find(pwd[i]);
        if (c->isExit)
        {
            if(m.boss_dfs(pwd + i, KEY_LEN)){
                Maze_Prims m_prims(MAP_SIZE, MAP_SIZE);
                int init_x = rand() % MAP_SIZE;
                int init_y = rand() % MAP_SIZE;
                Cell *startCell = m_prims.get_cell(init_x, init_y, true);
                m_prims.Prim(startCell);
                Cell *c = startCell;
                char * pwd_ptr = pwd + i + KEY_LEN;
                int test_pwd_len = strlen(pwd_ptr);
                for(int j = 0;j <= strlen(pwd_ptr);j++){
                    int s = c->connected_cell.size();
                    char test = pwd_ptr[j];
                    unordered_map<char, Cell *>::const_iterator got = c->connected_cell.find(test);
                    if (c->isExit){
                        return m_prims.boss_prims(pwd_ptr + j);
                    }
                    if (got == c->connected_cell.end())
                    {
                        //can't move to next cell : false password
                        return false;
                    }
                    else
                    {
                        c = got->second; //move to next cell
                    }
                }
            }
        }
        if (got == c->connected_cell.end())
        {
            //can't move to next cell : false password
            return false;
        }
        else
        {
            c = got->second; //move to next cell
        }
    }
    return true;
}
int main(int, char **)
{
    char username[UNAME_LEN] = {0};
    char pwd[PWD_LEN] = {0};
    cout << "Welcome to HITCTF 2020 :) \n  ----Hope you have fun in these GAMEs lol\n"<<endl;
    cout << "First Input your username:" << endl;
    cin.read(username,UNAME_LEN);
    cout << "Now Input Correlated license:" << endl;
    cin.read(pwd,PWD_LEN);
    username[UNAME_LEN - 1] = 0;
    pwd[PWD_LEN - 1] = 0;
    // char username[] = "hitctf2020";
    // char pwd[] = "wwwwddddssaassdd3087629750608333480917556dssdd468912723481342575971422657913948591537428763345261";
    MazeNamespace::Maze_DFS m(MAP_SIZE, MAP_SIZE);
    MazeNamespace::RandomGenerator rg(username, MAP_SIZE); //run srand(username)
    int init_x = rg.get_next();
    int init_y = rg.get_next();
    MazeNamespace::Cell *startCell = m.get_cell(init_x, init_y, true);
    MazeNamespace::DFS(init_x, init_y, startCell, &m);
    if (check_pwd(pwd, m, startCell))
    {
        char *flag = get_flag(pwd);
        cout << "Congratulations , you have got the flag!!! \n Your flag is : HITCTF2020{"
             << flag << "}"<<endl;
    }
    else
    {
        cout << "Try harder :)" << endl;
    }
    system("pause");
    return 0;
}
