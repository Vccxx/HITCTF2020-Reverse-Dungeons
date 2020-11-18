#ifndef MAZE_H
#define MAZE_H
#include <list>
#include <vector>
#include <unordered_map>
using namespace std;
namespace MazeNamespace
{
    class Cell
    {
    public:
        int pos_x;
        int pos_y;
        bool visited = false;
        bool valid = true;
        bool isExit = false;
        Cell();
        Cell(int x, int y, bool valid);
        unordered_map<char,Cell*> connected_cell;
        Cell* visit();
        bool Connect(Cell* c);
        bool isValid();
    };

    class Maze_DFS
    {
    public:
        int width;
        int height;
        Cell *map;
        Maze_DFS(int width, int height);
        ~Maze_DFS();
        Cell* get_cell(int x, int y, bool check);
        bool boss_dfs(char* pwd,int keylen);
        int get_module(char* big_number_str,int mod,int keylen);
    };

    class RandomGenerator
    {
    public:
        char *uname;
        int range;
        int seed;
        RandomGenerator(char *uanme, int range);
        int get_next();
        int seed_generator();
    };
    list<Cell*> GetAvailableneighbors(Maze_DFS* m, Cell* c);
    void DFS(int width, int height, Cell* startCell,Maze_DFS* maze);

    class Maze_Prims{
        public:
            int width;
            int height;
            vector<Cell*> connected_cell;
            Cell* map;
            Maze_Prims(int width, int height);
            ~Maze_Prims();
            Cell* get_cell(int x, int y, bool check);
            bool boss_prims(char* pwd);
            void Prim(Cell* startCell);
            void connect(Cell * nc);
            
    };
    list<Cell*> GetVisitedneighbors(Maze_Prims* m, Cell* c);
    
} 

#endif