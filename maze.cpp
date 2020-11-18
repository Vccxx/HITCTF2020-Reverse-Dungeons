#include "maze.h"
#include <malloc.h>
#include <random>
#include <stack>
#include <cstdlib>
#include <string.h>
#include <iostream>
#include <vector>
#include <set>
#include <bits/stdc++.h>
#define SUDOKU_SIZE 9
using namespace MazeNamespace;
using namespace std;
Maze_DFS::Maze_DFS(int width, int height)
{
    this->width = width;
    this->height = height;
    this->map = new Cell[width * height];
    //this->map = (Cell *)malloc(sizeof(Cell) * width * height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            this->map[i * width + j] = Cell(i, j, true);
        }
    }
}
Maze_DFS::~Maze_DFS()
{
    free(this->map);
}

int Maze_DFS::get_module(char *big_number_str, int mod, int keylen)
{
    int sum = big_number_str[0] - '0';
    for (int i = 1; i < keylen; i++)
    {
        sum = (sum * 10 + big_number_str[i] - '0') % mod;
    }
    return sum;
}

bool Maze_DFS::boss_dfs(char *pwd, int keylen)
{
    //http://blog.terrynini.tw/en/2019-PragyanCTF/#Easy-RSA
    if (get_module(pwd, 27644437, keylen) == 213)
    {
        if (get_module(pwd, 10459, keylen) == 229)
        {
            if (get_module(pwd, 1489, keylen) == 25)
            {
                if (get_module(pwd, 1046527, keylen) == 83)
                {
                    if (get_module(pwd, 16127, keylen) == 135)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}
Cell *Maze_DFS::get_cell(int x, int y, bool check)
{
    if (check)
    {
        if (x < 0 || x >= this->width)
        {
            return nullptr;
        }
        if (y < 0 || y >= this->height)
        {
            return nullptr;
        }
    }
    Cell *c = &(this->map[x * this->width + y]);
    return c;
}

Maze_Prims::Maze_Prims(int width, int height)
{
    this->width = width;
    this->height = height;
    this->map = new Cell[width * height];
    //this->map = (Cell *)malloc(sizeof(Cell) * width * height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            this->map[i * width + j] = Cell(i, j, true);
        }
    }
}
Maze_Prims::~Maze_Prims()
{
    free(this->map);
}
void Maze_Prims::connect(Cell *nc)
{
    for (auto k = this->connected_cell.begin(); k != connected_cell.end(); k++)
    {
        if ((*k)->Connect(nc))
        {
            //successfully connected
            this->connected_cell.push_back(nc);
            break;
        }
    }
}
void Maze_Prims::Prim(Cell *startCell)
{
    set<Cell *> pathSet;
    pathSet.insert(startCell);
    bool exit_flag = false;
    this->connected_cell.push_back(startCell);
    // While the set of cells is not empty
    while (!pathSet.empty())
    {
        // Select randomly a cell to extend the path and remove it from the set
        // Mark the cell as visited
        set<Cell *>::iterator it = pathSet.begin();
        int test_size = pathSet.size();
        int n = rand() % pathSet.size();
        advance(it, n);
        Cell *cell = *it;
        pathSet.erase(it);
        cell->visit();
        this->connect(cell);
        // Get available neighbors from bottom, left, right, top and visited
        // Randomly connect to one
        list<Cell *> neighbors = GetVisitedneighbors(this, cell);
        if (!neighbors.empty())
        {
            auto randIdx = rand() % neighbors.size();
            vector<Cell *> vec_neighbors(neighbors.begin(), neighbors.end());
            for (int i = 0; i < neighbors.size(); i++)
            {
                if (randIdx != i)
                {
                    // Add all unvisited neighbors to the set
                    Cell *test_nei = vec_neighbors[i];
                    pathSet.insert(test_nei);
                }
                else
                {
                    // Randomly connect to an available cell
                    Cell *nc = vec_neighbors[randIdx];
                    nc->visit();
                    this->connect(nc);
                    list<Cell *> neighbors_c = GetVisitedneighbors(this, nc);
                    for (auto ii = neighbors_c.begin(); ii != neighbors_c.end(); ii++)
                    {
                        pathSet.insert(*ii);
                    }
                }
            }
        }
        else
        {
            if (!exit_flag)
            {
                cell->isExit = true;
                exit_flag = true;
            }
        }
    }
}
bool check_dup(int *na, int na_len)
{
    sort(na, na + na_len);
    for (int i = 0; i < na_len - 1; i++)
    {
        if (na[i] == na[i + 1])
        {
            return false;
        }
    }
    return true;
}
bool Maze_Prims::boss_prims(char *pwd)
{
    //Sudoku 9*9
    //solution : 
    /*
    [5, 3, 4, 6, 7, 8, 9, 1, 2],
    [6, 7, 2, 1, 9, 5, 3, 4, 8],
    [1, 9, 8, 3, 4, 2, 5, 6, 7],
    [8, 5, 9, 7, 6, 1, 4, 2, 3],
    [4, 2, 6, 8, 5, 3, 7, 9, 1],
    [7, 1, 3, 9, 2, 4, 8, 5, 6],
    [9, 6, 1, 5, 3, 7, 2, 8, 4],
    [2, 8, 7, 4, 1, 9, 6, 3, 5],
    [3, 4, 5, 2, 8, 6, 1, 7, 9]

    4,6,8,9,1,2,
    7,2,3,4,8,
    1,3,4,2,5,7,
    5,9,7,1,4,2,
    2,6,5,7,9,
    1,3,9,4,8,5,
    9,1,5,3,7,4,
    2,8,7,6,3,
    3,4,5,2,6,1,

    468912723481342575971422657913948591537428763345261
    */
    int sudoku[SUDOKU_SIZE][SUDOKU_SIZE] = {{5, 3, 0, 0, 7, 0, 0, 0, 0},
                                            {6, 0, 0, 1, 9, 5, 0, 0, 0},
                                            {0, 9, 8, 0, 0, 0, 0, 6, 0},
                                            {8, 0, 0, 0, 6, 0, 0, 0, 3},
                                            {4, 0, 0, 8, 0, 3, 0, 0, 1},
                                            {7, 0, 0, 0, 2, 0, 0, 0, 6},
                                            {0, 6, 0, 0, 0, 0, 2, 8, 0},
                                            {0, 0, 0, 4, 1, 9, 0, 0, 5},
                                            {0, 0, 0, 0, 8, 0, 0, 7, 9}};
    int pwd_len = 51;
    int *sudo_ptr = &sudoku[0][0];
    int position_record = 0;
    for (int j = 0; j < pwd_len; ++j)
    {
        for (int k = position_record; k < SUDOKU_SIZE * SUDOKU_SIZE; k++)
        {
            if (*(sudo_ptr + k) != 0)
            {
                continue;
            }
            *(sudo_ptr + k) = int(pwd[j]) - int('0');
            position_record = k;
            break;
        }
    }
    // check column
    for (int i = 0; i < SUDOKU_SIZE; i++)
    {
        int check[SUDOKU_SIZE] = {0};
        for (int j = 0; j < SUDOKU_SIZE; j++)
        {
            if (sudoku[i][j] >= 10 || sudoku[i][j] <= 0)
            {
                return false;
            }
            check[j] = sudoku[j][i];
        }
        if (!check_dup(check, SUDOKU_SIZE))
            return false;
    }
    //check row
    for (int i = 0; i < SUDOKU_SIZE; i++)
    {
        int check[SUDOKU_SIZE] = {0};
        for (int j = 0; j < SUDOKU_SIZE; j++)
        {
            check[j] = sudoku[i][j];
        }
        if (!check_dup(check, SUDOKU_SIZE))
            return false;
    }
    int box_size = int(sqrt(SUDOKU_SIZE));
    for (int i0 = 0; i0 < box_size; i0++)
    {
        for (int j0 = 0; j0 < box_size; j0++)
        {
            int check[SUDOKU_SIZE] = {0};
            int check_point = 0;
            for (int i = 0; i < box_size; i++)
            {
                for (int j = 0; j < box_size; j++)
                {
                    check[check_point++] = sudoku[3*i0 + i][3*j0 + j];
                }
            }
            if (!check_dup(check, SUDOKU_SIZE))
                return false;
        }
    }
    return true;
}
Cell *Maze_Prims::get_cell(int x, int y, bool check)
{
    if (check)
    {
        if (x < 0 || x >= this->width)
        {
            return nullptr;
        }
        if (y < 0 || y >= this->height)
        {
            return nullptr;
        }
    }
    Cell *c = &(this->map[x * this->width + y]);
    return c;
}
list<Cell *> MazeNamespace::GetVisitedneighbors(Maze_Prims *m, Cell *c)
{
    list<Cell *> neighbors;
    int x = c->pos_x;
    int y = c->pos_y;
    Cell *up_cell = m->get_cell(x - 1, y, true);
    if (up_cell != nullptr && !up_cell->visited)
    {
        neighbors.push_back(up_cell);
    }
    Cell *left_cell = m->get_cell(x, y - 1, true);
    if (left_cell != nullptr && !left_cell->visited)
    {
        neighbors.push_back(left_cell);
    }
    Cell *right_cell = m->get_cell(x, y + 1, true);
    if (right_cell != nullptr && !right_cell->visited)
    {
        neighbors.push_back(right_cell);
    }
    Cell *down_cell = m->get_cell(x + 1, y, true);
    if (down_cell != nullptr && !down_cell->visited)
    {
        neighbors.push_back(down_cell);
    }

    return neighbors;
}

Cell::Cell()
{
    this->valid = true;
    this->visited = false;
}
Cell::Cell(int x, int y, bool valid)
{
    this->pos_x = x;
    this->pos_y = y;
    this->valid = valid;
}

bool Cell::Connect(Cell *c)
{
    int x = this->pos_x;
    int y = this->pos_y;
    int cx = c->pos_x;
    int cy = c->pos_y;
    if (x == cx)
    {
        //same column:up or down
        if (y + 1 == cy)
        {
            //up
            this->connected_cell.insert({'s', c});
        }
        else if (y == cy + 1)
        {
            //down
            this->connected_cell.insert({'w', c});
        }
        else
        {
            //not connected
            return false;
        }
    }
    else if (y == cy)
    {
        //same row : left or right
        if (x + 1 == cx)
        {
            //right
            this->connected_cell.insert({'d', c});
        }
        else if (x == cx + 1)
        {
            //left
            this->connected_cell.insert({'a', c});
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

Cell *Cell::visit()
{
    this->visited = true;
    return this;
}
bool Cell::isValid()
{
    return this->valid;
}
list<Cell *> MazeNamespace::GetAvailableneighbors(Maze_DFS *m, Cell *c)
{
    list<Cell *> neighbors;
    int x = c->pos_x;
    int y = c->pos_y;
    Cell *up_cell = m->get_cell(x - 1, y, true);
    if (up_cell != nullptr && !up_cell->visited)
    {
        neighbors.push_back(up_cell);
    }
    Cell *left_cell = m->get_cell(x, y - 1, true);
    if (left_cell != nullptr && !left_cell->visited)
    {
        neighbors.push_back(left_cell);
    }
    Cell *right_cell = m->get_cell(x, y + 1, true);
    if (right_cell != nullptr && !right_cell->visited)
    {
        neighbors.push_back(right_cell);
    }
    Cell *down_cell = m->get_cell(x + 1, y, true);
    if (down_cell != nullptr && !down_cell->visited)
    {
        neighbors.push_back(down_cell);
    }
    return neighbors;
}
void PrintStack(stack<Cell *> s)
{
    // If stack is empty then return
    if (s.empty())
        return;

    Cell *x = s.top();

    // Pop the top element of the stack
    s.pop();

    // Recursively call the function PrintStack
    PrintStack(s);

    // Print the stack element starting
    // from the bottom
    cout << x->pos_x << ',' << x->pos_y << " ";

    // Push the same element onto the stack
    // to preserve the order
    s.push(x);
}
void MazeNamespace::DFS(int width, int height, Cell *startCell, Maze_DFS *maze)
{
    //Maze maze(width, height);
    stack<Cell *> pathStack;
    stack<Cell *> toprintStack;
    pathStack.push(startCell);
    Cell *exit_cell = nullptr;
    bool exit_flag = false;
    int pre_stack_size = -1;
    // While there is node to be handled in the stack
    while (!pathStack.empty())
    {
        // toprintStack = pathStack;
        // PrintStack(toprintStack);
        // char a;
        // cin>>a;
        // Handle the cell at the top of the stack:
        // Get available neighbors from bottom, left, right, top and unvisited
        Cell *cell = pathStack.top();
        pathStack.pop();
        // int stack_size = pathStack.size();
        // if(stack_size >= pre_stack_size){
        //     pre_stack_size = stack_size;
        // }
        // else{
        //     exit_cell = cell;
        // }
        list<Cell *> neighbors = GetAvailableneighbors(maze, cell);
        vector<Cell *> vec_neighbors(neighbors.begin(), neighbors.end());
        // If there is available node to process (loop to backtrack - 'pop' otherwise)
        if (!neighbors.empty())
        {
            // Randomly select a node to be processed
            auto randIdx = rand() % neighbors.size();
            Cell *target;
            //cout << neighbors.size() << endl;
            // For each available node: connect to the cell, mark it as visited
            // and push it into the stack.
            for (auto i = 0; i < neighbors.size(); ++i)
            {
                Cell *n = vec_neighbors[i];
                cell->Connect(n->visit());
                // Only the chosen item should be add to the top following a DFS strategy
                if (i != randIdx)
                {
                    pathStack.push(n);
                }
                else
                {
                    target = n;
                }
            }
            //Add the chosen node as the next one to be processed
            pathStack.push(target);
        }
        else
        {
            if (!exit_flag)
            {
                exit_flag = true;
                exit_cell = cell; //get exit cell
                exit_cell->isExit = true;
            }
        }
    }
}
RandomGenerator::RandomGenerator(char *uanme, int range)
{
    this->range = range;
    this->uname = uanme;
    this->seed = this->seed_generator();
    srand(this->seed);
}

int RandomGenerator::seed_generator()
{
    int s = 0;
    char *seed = this->uname;
    // Transform between username and seed
    int uname_len = strlen(seed);
    for (int i = 0; i < uname_len; i++)
    {
        s += int(seed[i]);
    }
    return s;
}

int RandomGenerator::get_next()
{
    return rand() % this->range;
}