#include <iostream>
#include <string>
#include "Grid.hpp"
#include "Cell.hpp"

using PacMaze::Cell;
using PacMaze::Grid;
using std::cout;
using std::endl;

void run_test(const std::string &filePath)
{
    cout << endl
         << "---------- Test maze: " << filePath << " ----------" << endl;
    Grid grid(filePath);
    grid.debug_printStatus();

    if (!grid.isValid())
    {
        cout << endl;
        return;
    }

    Cell *closest = grid.findClosestDot();
    if (closest != NULL)
    {
        cout << "Found closest dot: " << closest->debug_printCoords() << endl;
    }
    else
    {
        cout << "No dots found." << endl;
    }
}

int main()
{
    run_test("levels/obviously_not_a_real_file_come_on_now");
    run_test("levels/badInput/invalidChar.txt");
    run_test("levels/badInput/needsPacMan.txt");
    run_test("levels/badInput/twoPacMen.txt");
    run_test("levels/badInput/empty.txt");
    run_test("levels/badInput/rowTooShort.txt");
    run_test("levels/badInput/1x1empty.txt");
    run_test("levels/badInput/needsNewLine.txt");
    run_test("levels/badInput/needsNewLine1x1.txt");

    run_test("levels/cannotFindDots/pacman1x1.txt");
    run_test("levels/cannotFindDots/3x3allWalls.txt");
    run_test("levels/cannotFindDots/3x3empty.txt");
    run_test("levels/cannotFindDots/dotInaccessible.txt");

    run_test("levels/goodInput/5x5.txt");
    run_test("levels/goodInput/noWalls.txt");
    run_test("levels/goodInput/horizontal.txt");
    run_test("levels/goodInput/vertical.txt");
    run_test("levels/goodInput/normalLevel.txt");
    run_test("levels/goodInput/bigEmpty.txt");
    run_test("levels/goodInput/big.txt");

    return 0;
}