#include <vector>
#include <string>

namespace PacMaze {

    class Cell {
    public:
        Cell(bool hasDot);
        void addAdjacent(Cell* cell);

        // Required interface for the "find closest dot" algorithm
        bool hasDot() const;
        const std::vector<Cell*> getAdjacentCells() const;


        // Testing output functionality for this demo
        void debug_setCoordinates(int x, int y);
        void debug_setCellCharacter(char ch);
        std::string debug_printCoords() const;
        char debug_cellChar() const;

    private:
        bool _hasDot;
        std::vector<Cell*> _adjacentCells;

        // Used for test output functionality
        int _xCoord;
        int _yCoord;
        char _cellChar;
    };
}