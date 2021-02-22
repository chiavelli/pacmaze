#include "Grid.hpp"
#include "Cell.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <queue>
#include <set>

using std::to_string;
using std::cout;
using std::endl;

namespace PacMaze {

    Grid::Grid(const std::string& filePath)
    {
        _isValid = false;
        _width = 0;
        _height = 0;
        _pacManX = -1;
        _pacManY = -1;
        _inputError = "";

        readInputFile(filePath);
        if (_isValid) {
            buildAdjacencyLists();
            _startingCell = getCell(_pacManX, _pacManY);
        }
    }

    Grid::~Grid()
    {
        for (Cell* cell : _grid) {
            if (cell != NULL) {
                delete cell;
            }
        }
    }

    void Grid::debug_printStatus() const {
        if (!_isValid) {
            cout << "Grid input was invalid: " + _inputError << endl;
        }
        else {
            for (int i=0; i < _grid.size(); ++i) {
                if (i % _width == 0) {
                    cout << endl;
                }

                Cell* c = _grid[i];
                if (c == NULL) {
                    cout << "#";
                }
                else {
                    cout << c->debug_cellChar();
                }
            }

            cout << endl << "Grid setup complete. ";
            cout << "Dims: (" << _width << "," << _height << ") ";
            cout << "Start: (" << _pacManX << "," << _pacManY << ")" << endl;
        }
    }

    bool Grid::isValid() const {
        return _isValid;
    }


    // Implementation of the algorithm for finding the closest dot in the maze.
    // Cells farther down the queue are farther in distance from the starting point
    // The set of visited Cells prevents us from adding the same cell to the queue twice
    Cell* Grid::findClosestDot()
    {
        std::set<Cell*> visited;
        std::queue<Cell*> cells;
        cells.push(_startingCell);

        while (cells.size() > 0) {
            Cell* c = cells.front();
            cells.pop();

            if (c->hasDot()) {
                return c;
            }

            for (Cell* adjacent : c->getAdjacentCells()) {
                // Attempt to insert the cell into our visited set. Only add to queue on success
                auto insertResult = visited.insert(adjacent);
                if (insertResult.second == true) {
                    cells.push(adjacent);
                }
            }
        }

        return NULL;
    }


    // Parse input file for specifying the pacmaze grid
    void Grid::readInputFile(const std::string& filePath)
    {
        std::ifstream fin;
        fin.open(filePath, std::ios::in);

        int currentLineWidth = 0;
        char cellChar;
        while (!fin.eof()) {
            fin.get(cellChar);
            if (fin.gcount() == 0) {
                // character read was unsuccessful
                break;
            }

            if (cellChar == '\n') {
                if (!checkInputLineEnd(currentLineWidth)) {
                    // Stop reading input file if problems detected
                    break;
                }

                // Line ended successfully. Reset the width for tyhe next row
                currentLineWidth = 0;
            }
            else {
                if (!readInputCell(cellChar, currentLineWidth, _height)) {
                    // Stop reading input file if problems detected
                    break;
                }

                // Cell read successfully. Increase the width for this row
                currentLineWidth++;
            }
        }

        validateGrid(currentLineWidth == 0);
        fin.close();
    }


    // Perform correct actions when a line end is detected in the input file
    bool Grid::checkInputLineEnd(int lineWidth)
    {
        if (_width == 0) {
            _width = lineWidth;
            _height++;
        }
        else if (_width == lineWidth) {
            _height++;
        }
        else {
            // Current row ended at a width which does not match previous rows.
            _inputError = "Row " + to_string(_height);
            _inputError += " width " + to_string(lineWidth); 
            _inputError += ". Expected " + to_string(_width);
            _inputError += ". All rows of maze must be equal width";
            return false;
        }
        
        return true;
    }


    // Perform the correct Cell creation action based on the input charcter for the given cell location
    bool Grid::readInputCell(char cellChar, int cellX, int cellY)
    {
        bool createCell = false;
        bool hasDot = false;

        switch (cellChar) {
            case 'd': 
                // it's a dot!
                createCell = true;
                hasDot = true;
            break;

            case ' ': 
                // navigable space
                createCell = true;
            break;
            
            case 'p': 
                // it's pacman!
                if (_pacManX >= 0) {
                    _inputError = "Only one Pac Man allowed.";
                    return false;
                }

                createCell = true;
                _pacManX = cellX;
                _pacManY = cellY;
            break;

            case '#': 
                // Push a null ptr for maze walls
                _grid.push_back(NULL);
            break;

            default:
                _inputError = "Invalid input char at pos (" + to_string(cellX) + "," + to_string(cellY) + ") - ";
                _inputError += cellChar;
                return false;
            break;
        };

        if (createCell) {
            Cell* c = new Cell(hasDot);
            _grid.push_back(c);
            
            c->debug_setCoordinates(cellX, cellY);
            c->debug_setCellCharacter(cellChar);
        }

        return true;
    }


    // Perform validity checks once the grid is done being input
    void Grid::validateGrid(bool lastLineIsEmpty)
    {
        _isValid = false;

        if (_inputError.length() > 0) {
            return;
        }

        if (!lastLineIsEmpty) {
            _inputError = "The input file must end in a new line.";
            return;
        }

        if (_width < 1 || _height < 1) {
            _inputError = "Input file is empty or does not exist.";
            return;
        }

        if (_pacManX < 0 || _pacManY < 0) {
            _inputError = "Where is pac man??";
            return;
        }

        _isValid = true;
    }


    // Make sure each cell in the grid is directly linked to its neighbors
    void Grid::buildAdjacencyLists()
    {
        for (int i = 0; i < _grid.size(); ++i) {
            Cell* cell = _grid[i];
            if (cell != NULL) {
                int x = i % _width;
                int y = i / _width;

                // Get adjacent coordinates. Allow for wrapping
                int leftX = x == 0 ? _width-1 : x-1;
                int rightX = x == _width-1 ? 0 : x+1;
                int upY = y == 0 ? _height-1 : y-1;
                int downY = y == _height-1 ? 0 : y+1;

                addAdjacentCell(cell, leftX, y);
                addAdjacentCell(cell, rightX, y);
                addAdjacentCell(cell, x, upY);
                addAdjacentCell(cell, x, downY);
            }
        }
    }

    
    // Attemot to add a cell at the given coords to the base cell's adjacency list
    void Grid::addAdjacentCell(Cell* baseCell, int x, int y)
    {
        Cell* adjacentCell = getCell(x, y);
        if (adjacentCell != NULL) {
            // Protect against wrapping coords pointing to the same cell
            if (adjacentCell != baseCell) {
                baseCell->addAdjacent(adjacentCell);
            }
        }
    }


    // Convert the given x,y coords into a valid grid index and grab the cell
    Cell* Grid::getCell(int x, int y) const
    {
        int index = (y * _width) + x;
        if (index < 0 || index >= _grid.size()) {
            return NULL;
        }
        return _grid[index];
    }
}