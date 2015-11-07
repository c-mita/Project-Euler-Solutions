#include <iostream>
#include <chrono>
#include <cstring>
#include <sstream>
#include <fstream>
#include <vector>

#define LENGTH 9
#define SIZE 81
#define PEERS 20
#define INPUT_FILE "096.txt"

/*
 * Basic backtracking Su Doku solver
 * Grid is an array of cells, with each cell knowing what numbers it could possibly take
 *
 * Select a cell and a choose one of the possible values for that cell,
 * eliminate that value from its peers (cells in the same row, column, or box)
 * and recurse, stepping back if no solution can be found.
 *
 * When eliminating options from peers, check for cells that only have a single possible value
 * and allow this set/check process to recurse.
 *
 * Project Euler grids can be solved in ~20ms (3.4Ghz i5)
 */


static int example[SIZE] = {
        0, 0, 3, 0, 2, 0, 6, 0, 0,
        9, 0, 0, 3, 0, 5, 0, 0, 1,
        0, 0, 1, 8, 0, 6, 4, 0, 0,
        0, 0, 8, 1, 0, 2, 9, 0, 0,
        7, 0, 0, 0, 0, 0, 0, 0, 8,
        0, 0, 6, 7, 0, 8, 2, 0, 0,
        0, 0, 2, 6, 0, 9, 5, 0, 0,
        8, 0, 0, 2, 0, 3, 0, 0, 9,
        0, 0, 5, 0, 1, 0, 3, 0, 0 };


struct Cell {
    int value;
    int possibilities[10];
    int num_possibilities;
};

class Grid {
public:
    Grid() {}
    Grid( int arr[] ) {
        for ( int i = 0; i < SIZE; i++ ) {
            cells[i].value = arr[i];
            memset( cells[i].possibilities, arr[i] ? 0 : 1, sizeof(cells[i].possibilities) );
            cells[i].possibilities[0] = 0;
            cells[i].num_possibilities = 9;
        }
        for ( int i = 0; i < LENGTH; i++ ) {
            for ( int j = 0; j < LENGTH; j++ ) {
                int value = cells[i + j * LENGTH].value;
                if ( value ) {
                    setCellValue( i, j, value );
                }
            }
        }
    }

    //If SIZE were huge, we'd allocate dynamically and create an explicit copy-ctor
    //*or* we'd preallocate a load of Grid objects and reference those in our recursion (and maybe memset over them...)
    Cell cells[SIZE];

    void setCellValue( int x, int y, int value ) {
        if ( value == 0 ) {
            return;
        }
        int idx = x + y * LENGTH;
        cells[idx].value = value;
        memset( cells[idx].possibilities, 0, sizeof(cells[idx].possibilities) );
        cells[idx].num_possibilities = 0;
        Cell* peers[PEERS];
        getPeers( x, y, peers );
        for ( int k = 0; k < PEERS; k++ ) {
            if ( peers[k]->possibilities[value] ) {
                peers[k]->possibilities[value] = 0;
                peers[k]->num_possibilities--;
            }
        }
        clean();
    }

    void clean() {
        for ( int i = 0; i < SIZE; i++ ) {
            if ( cells[i].num_possibilities == 1 ) {
                int value = 0;
                for ( int k = 1; k < 10; k++ ) {
                    if ( cells[i].possibilities[k] ) {
                        value = k;
                        break;
                    }
                }
                setCellValue( i % LENGTH, i / LENGTH, value );
            }
        }
    }

    //get all peers of specified cell - of which there are 20
    //use zero-based indexing
    void getPeers( int x, int y, Cell* out[] ) {
        //Row
        for ( int i = 0; i < x; i++ ) {
            *(out++) = &(cells[i + y * LENGTH]);
        }
        for ( int i = x + 1; i < LENGTH; i++ ) {
            *(out++) = &(cells[i + y * LENGTH]);
        }

        //Column
        for ( int i = 0; i < y; i++ ) {
            *(out++) = &(cells[x + i * LENGTH]);
        }
        for ( int i = y + 1; i < LENGTH; i++ ) {
            *(out++) = &(cells[x + i * LENGTH]);
        }

        //Mark the cells in a box we "don't" want to add - avoids ugly switch statement
        bool done_box[9] = {false};
        for ( int i = 0; i < 3; i++ ) {
            done_box[x % 3 + i * 3] = true;
            done_box[i + (y % 3) * 3] = true;
        }

        //Box
       int box_x = (x / 3) * 3;
       int box_y = (y / 3) * 3;
       for ( int i = box_x, k = 0; k < 3; i++, k++ ) {
           for ( int j = box_y, l = 0; l < 3; j++, l++ ) {
               if ( !done_box[k + l * 3] ) {
                   *(out++) = &(cells[i + j * LENGTH]);
               }
           }
       }
    }

    //Find a cell with no given value
    bool findUnknown( int* x, int* y, Cell** cell ) {
        //Ideally should return cells with fewer possibilities first
        //But it doesn't really matter
        for ( int i = 0; i < SIZE; i++ ) {
            if ( cells[i].value == 0 ) {
                *x = i % LENGTH;
                *y = i / LENGTH;
                *cell = &cells[i];
                return true;
            }
        }
        return false;
    }

    std::string str() {
        std::stringstream ss;
        for ( int i = 0; i < LENGTH; i++ ) {
            for ( int j = 0; j < LENGTH; j++ ) {
                ss << cells[ j + i * LENGTH ].value << " ";
            }
            ss << "\n";
        }
        return ss.str();
    }
};

bool solve( Grid grid, Grid* out, int x, int y, int val ) {
    grid.setCellValue(x, y, val);
    Cell* cell;
    int nx = x, ny = y;
    if ( grid.findUnknown(&nx, &ny, &cell) ) {
        for ( int i = 1; i < 10; i++ ) {
            if ( cell->possibilities[i] ) {
                if ( solve( grid, out, nx, ny, i ) ) {
                    return true;
                }
            }
        }
        return false;
    } else {
        //I've written nicer recursive functions...
        memcpy( out->cells, grid.cells, sizeof(grid.cells) );
        return true;
    }
}

bool solve( Grid grid, Grid* out ) {
    return solve( grid, out, 0, 0, grid.cells[0].value );
}

std::vector< std::vector<int> >
parseGrids( const char* path ) {
    std::ifstream file( path );
    std::string line;
    int digitCount = 0;
    std::vector< std::vector<int> > grids ;
    std::vector<int> grid;
    while ( std::getline( file, line ) ) {
        if ( line[0] == 'G' ) continue;
        for ( char c : line ) {
            if ( c == '.' ) c = '0';
            grid.push_back( c - '0' );
            digitCount++;
            if ( digitCount % SIZE == 0 ) {
                grids.push_back( grid );
                grid = std::vector<int>();
            }
        }
    }
    return grids;
}

int main( int argc, char* argv[] ) {
    auto startTime = std::chrono::steady_clock::now();

    const char* filePath;
    if ( argc <= 1 ) {
        filePath = INPUT_FILE;
    } else {
        filePath = argv[1];
    }
    std::vector< std::vector<int> > grids = parseGrids( filePath );

    Grid out;
    int sum = 0;
    for ( auto it = grids.begin(); it != grids.end(); ++it ) {
        auto gridTime = std::chrono::steady_clock::now();
        Grid g( it->data() );
        if ( !solve( g, &out ) ) {
            std::cout << "FAILED" << std::endl;
            std::cout << g.str() << std::endl;
            throw;
        } else {
            auto gridRunTime = std::chrono::steady_clock::now() - gridTime;
            double timeTaken = std::chrono::duration<double, std::milli> (gridRunTime).count();
            if ( timeTaken > 1000. ) {
                std::cout << "This took " << timeTaken << "ms" << std::endl;
                std::cout << out.str() << std::endl;
            }
        }
        int n = out.cells[0].value * 100 + out.cells[1].value * 10 + out.cells[2].value;
        sum += n;
    }

    std::cout << "Answer: " << sum << std::endl;

    auto endTime = std::chrono::steady_clock::now();
    auto runTime = endTime - startTime;
    std::cout << "Time: " << std::chrono::duration<double, std::milli> (runTime).count() << " ms" << std::endl;

    return 0;
}
