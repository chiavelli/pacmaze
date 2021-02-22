#include "Cell.hpp"
#include <vector>
#include <string>

namespace PacMaze {
    Cell::Cell(bool hasDot)
    : _hasDot(hasDot)
    {
    }

    bool Cell::hasDot() const
    {
        return _hasDot;
    }

    const std::vector<Cell*> Cell::getAdjacentCells() const
    {
        return _adjacentCells;
    }

    void Cell::addAdjacent(Cell* cell)
    {
        _adjacentCells.push_back(cell);
    }

    void Cell::debug_setCoordinates(int x, int y) {
        _xCoord = x;
        _yCoord = y;
    }

    void Cell::debug_setCellCharacter(char ch) {
        _cellChar = ch;
    }

    std::string Cell::debug_printCoords() const {
        return "[" + std::to_string(_xCoord) + "," + std::to_string(_yCoord) + "]";
    }

    char Cell::debug_cellChar() const {
        return _cellChar;
    }
}