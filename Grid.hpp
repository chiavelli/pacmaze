#include <vector>
#include <string>

namespace PacMaze {

    class Cell; // defined in Cell.hpp
    
    class Grid
    {
    public:
        Grid(const std::string& filePath);
        ~Grid();

        void debug_printStatus() const;

        bool isValid() const;
        Cell* findClosestDot();

    private:
        std::vector<Cell*> _grid;   // NOTE: The 2d grid is stored as one-dimensional vector
        bool _isValid;              // Set to true if loaded grid passes all validity checks
        int _width;
        int _height;

        // Pac-man starting coords and cell
        Cell* _startingCell;
        int _pacManX;               
        int _pacManY;

        // Can be set during processing of the input file
        std::string _inputError;

        void readInputFile(const std::string& filePath);
        bool checkInputLineEnd(int lineWidth);
        bool readInputCell(char cellChar, int cellX, int cellY);
        void validateGrid(bool lastLineIsEmpty);
        void buildAdjacencyLists();
        void addAdjacentCell(Cell* baseCell, int x, int y);
        Cell* getCell(int x, int y) const;
    };
}