// Project Identifier: 950181F63D0A883F183EC0A5CC67B19928FE896A
#include <iostream>
#include <map>
#include <string>
#include <set>
#include <sstream>
#include <deque>
#include <list>
#include <queue>
#include <vector>
#include <cmath>
#include <getopt.h>
#include <algorithm>
using namespace std;

struct Locate {
    uint32_t level;
    uint32_t row;
    uint32_t col;
};

struct mapCreation{
    char nesw;
    char symbol = '.';
    bool discovered = false;
};

class mainClass {
    public:

        mainClass() 
            : isQueue(false), lMode(false), hangar(false), noHangar(false), 
            currLoc{0, 0, 0}, hangarLevel(0), hangarRow(0), hangarCol(0),
            backtraceLevel(0), backtraceRow(0), backtraceCol(0),
            elevatorFloor(0) 
            {
            }

        void getMode(int argc, char* argv[]) {
            opterr = false;
            int choice;
            uint32_t sqcount = 0;
            option long_options[] = {
                {"stack" , no_argument , 0, 's'},
                {"queue", no_argument, 0, 'q'},
                {"output", required_argument, 0, 'o'},
                {"help", no_argument, 0, 'h'},
                { 0, 0, 0, 0},
            };

            while ((choice = getopt_long(argc, argv, "sqo:h", long_options, NULL)) != -1) {
                switch (choice) {
                    case 'h':
                    cout << "This is a space-station program where you must input --s or --stack, --q or --queue, and M or L for map or list respectively. If M or L is not inputed, M for map will be selected by default.";
                    exit(0);

                    case 's':
                    ++sqcount;
                    isQueue = false;
                    break;

                    case 'q':
                    ++sqcount;
                    isQueue = true;
                    break;

                    case 'o':
                    if (string(optarg) == "L") {
                        lMode = true;
                    }
                    else if (string(optarg) == "M") {
                        lMode = false;
                    }
                    break;
                }
            }
            if (sqcount == 0) {
                cerr << "No routing mode specified";
                exit(1);
            }

            else if (sqcount > 1) {
                cerr << "Multiple routing modes specified";
                exit(1);
            }
        }
        
        void insSearchCont (char path, uint32_t level, uint32_t row, uint32_t col, deque<Locate> &insDeque, vector<vector<vector<mapCreation>>> &coordVec) {
            Locate tempCoord;
            coordVec[level][row][col].discovered = true;
            coordVec[level][row][col].nesw = path;
            tempCoord.level = level;
            tempCoord.row = row;
            tempCoord.col = col;
            insDeque.push_back(tempCoord);
        }

        bool checkHangar (char path, uint32_t level, uint32_t row, uint32_t col, vector<vector<vector<mapCreation>>> &coordVec, deque<Locate> &insDeque) {
            if (coordVec[level][row][col].symbol == 'H') {
                hangarLevel = level;
                hangarRow = row;
                hangarCol = col;
                coordVec[level][row][col].discovered = true;
                coordVec[level][row][col].nesw = path;
                Locate tempCoord;
                tempCoord.level = level;
                tempCoord.row = row;
                tempCoord.col = col;
                insDeque.push_back(tempCoord);
                return true;
            }
            else {
                return false;
            }
        }

        void routingScheme(vector<vector<vector<mapCreation> > > &coordVec, Locate startPos, uint32_t dimensions, uint32_t levels) {
            locDeque.push_back(startPos);
            coordVec[startPos.level][startPos.row][startPos.col].discovered = true;
            while(hangar == false) {
                if (isQueue == false) {
                    currLoc = locDeque.back();
                    locDeque.pop_back();
                }
                if (isQueue == true) {
                    currLoc = locDeque.front();
                    locDeque.pop_front();
                }


                if (currLoc.row > 0) {
                    if (coordVec[currLoc.level][currLoc.row - 1][currLoc.col].symbol != '#' && 
                    coordVec[currLoc.level][currLoc.row - 1][currLoc.col].discovered == false) {
                        insSearchCont('n', currLoc.level, currLoc.row - 1, currLoc.col, locDeque, coordVec);
                        if (checkHangar('n', currLoc.level, currLoc.row - 1, currLoc.col, coordVec, locDeque)) {
                            hangar = true;
                            break;
                        }
                    }
                }

                if (currLoc.col + 1 < dimensions) {
                    if (coordVec[currLoc.level][currLoc.row][currLoc.col + 1].symbol != '#' && 
                    coordVec[currLoc.level][currLoc.row][currLoc.col + 1].discovered == false) {
                        insSearchCont('e', currLoc.level, currLoc.row, currLoc.col + 1, locDeque, coordVec);
                        if (checkHangar('e', currLoc.level, currLoc.row, currLoc.col + 1, coordVec, locDeque)) {
                            hangar = true;
                            break;
                        }
                    }
                }

                if (currLoc.row + 1 < dimensions) {
                    if (coordVec[currLoc.level][currLoc.row + 1][currLoc.col].symbol != '#' && 
                    coordVec[currLoc.level][currLoc.row + 1][currLoc.col].discovered == false) {
                        insSearchCont('s', currLoc.level, currLoc.row + 1, currLoc.col, locDeque, coordVec);
                        if (checkHangar('s', currLoc.level, currLoc.row + 1, currLoc.col, coordVec, locDeque)) {
                            hangar = true;
                            break;
                        }
                    }
                }

                if (currLoc.col > 0) {
                    if (coordVec[currLoc.level][currLoc.row][currLoc.col - 1].symbol != '#' && 
                    coordVec[currLoc.level][currLoc.row][currLoc.col - 1].discovered == false) {
                        insSearchCont('w', currLoc.level, currLoc.row, currLoc.col - 1, locDeque, coordVec);
                        if (checkHangar('w', currLoc.level, currLoc.row, currLoc.col - 1, coordVec, locDeque)) {
                            hangar = true;
                            break;
                        }
                    }
                }

                if (coordVec[currLoc.level][currLoc.row][currLoc.col].symbol == 'E') {
                    elevatorFloor = currLoc.level;
                    for (uint32_t i = 0 ; i < levels ; ++i) {
                        if (coordVec[i][currLoc.row][currLoc.col].symbol == 'E' && coordVec[i][currLoc.row][currLoc.col].discovered == false) {
                            insSearchCont(static_cast<char>(currLoc.level + '0'), i, currLoc.row, currLoc.col, locDeque, coordVec);
                        }
                    }
                }

                if (locDeque.empty() == true && hangar == false) {
                    noHangar = true;
                    break;
                }
            }
        }

        void backtrace(vector<vector<vector<mapCreation>>> &coordVec, deque<Locate> &backtraceCoord, deque<char> &backtracePath, Locate startPos) {
            backtraceLevel = hangarLevel;
            backtraceRow = hangarRow;
            backtraceCol = hangarCol;
            
            while (backtraceLevel != startPos.level || backtraceRow != startPos.row || backtraceCol != startPos.col) {
                backtracePath.push_back(coordVec[backtraceLevel][backtraceRow][backtraceCol].nesw);
                uint32_t tempLevel = backtraceLevel;
                

                if (backtracePath.back() == 'n') {
                    coordVec[backtraceLevel][backtraceRow + 1][backtraceCol].symbol = 'n';
                    ++backtraceRow;
                }

                else if (backtracePath.back() == 'e') {
                    coordVec[backtraceLevel][backtraceRow][backtraceCol - 1].symbol = 'e';
                    --backtraceCol;
                }

                else if (backtracePath.back() == 's') {
                    coordVec[backtraceLevel][backtraceRow - 1][backtraceCol].symbol = 's';
                    --backtraceRow;
                }

                else if (backtracePath.back() == 'w') {
                    coordVec[backtraceLevel][backtraceRow][backtraceCol + 1].symbol = 'w';
                    ++backtraceCol;
                }

                else {
                    uint32_t previousLevel = static_cast<uint32_t>(backtracePath.back() - '0');  // This is where you're coming from
                    coordVec[previousLevel][backtraceRow][backtraceCol].symbol = static_cast<char>('0' + tempLevel); // Set the symbol for the previous level
                    backtraceCoord.push_back({previousLevel, backtraceRow, backtraceCol});
                    backtracePath.back() = static_cast<char>(tempLevel + '0');
                    backtraceLevel = previousLevel;// Update to reflect the destination level
                    continue;
                }
                backtraceCoord.push_back({backtraceLevel, backtraceRow, backtraceCol});
            }
        }

        void finalPrint(vector<vector<vector<mapCreation>>> &coordVec, deque<Locate> &backtraceCoord, deque<char> &backtracePath, Locate startPos, uint32_t levels, uint32_t dimensions) {
            if (lMode == false) {
                if (noHangar == false) {
                    cout << "Start in level " << startPos.level << ", row " << startPos.row << ", column " << startPos.col << '\n';
                }
                for (uint32_t i = 0 ; i < levels ; ++i) {
                    cout << "//level " << i << '\n';
                    for (uint32_t j = 0 ; j < dimensions ; ++j) {
                        for (uint32_t k = 0 ; k < dimensions ; ++k) {
                            cout << coordVec[i][j][k].symbol;
                        }
                        cout << '\n';
                    }
                }
            }
            if (lMode == true) {
                if (noHangar == true) {
                    cout << "//path taken";
                }
                else {
                cout << "//path taken" << '\n';
                while (backtraceCoord.empty() != true) {
                    cout << "(" << backtraceCoord.back().level << "," << backtraceCoord.back().row << "," << backtraceCoord.back().col << "," << backtracePath.back() << ")" << '\n';
                    backtraceCoord.pop_back();
                    backtracePath.pop_back();
                }
                }
            }
        }

    private:
        bool isQueue;
        bool lMode;
        bool hangar = false;
        bool noHangar = false;
        deque<Locate> locDeque;
        Locate currLoc;
        uint32_t hangarLevel = 0;
        uint32_t hangarRow = 0;
        uint32_t hangarCol = 0;
        uint32_t backtraceLevel;
        uint32_t backtraceRow;
        uint32_t backtraceCol;
        uint32_t elevatorFloor;
};

int main(int argc, char* argv[]) {
    mainClass object;
    char listOrMapInput;
    uint32_t levels;
    uint32_t dimensions;
    char indivChar;
    string comment;
    Locate startPos;
    vector<vector<vector<mapCreation> > > coordVec;
    deque<Locate> backtraceCoord;
    deque<char> backtracePath;

    ios::sync_with_stdio(false);
    object.getMode(argc, argv);
    cin >> listOrMapInput;
    cin >> levels;
    cin >> dimensions;

    coordVec.resize(levels);
    for (uint32_t i = 0 ; i < levels; ++i) {
        coordVec[i].resize(dimensions);
        for (uint32_t j = 0 ; j < dimensions; ++j) {
            coordVec[i][j].resize(dimensions);
        }
    }

    if (listOrMapInput == 'M') {
        uint32_t level = 0;
        uint32_t row = 0;
        uint32_t col = 0;
        while (cin >> indivChar) {
            if (indivChar == '/') {
                getline(cin, comment);
                continue;
            }
            else if (indivChar != 'S' && indivChar != '#'  && indivChar != 'E' && indivChar != '.' && indivChar != 'H') {
                cerr << "Invalid map character";
                exit(1);
            }
            else {
                if (indivChar == 'S') {
                    startPos.level = level;
                    startPos.row = row;
                    startPos.col = col;
                }
                coordVec[level][row][col].symbol = indivChar;
                ++col;
                if (col == dimensions) {
                    col = 0;
                    ++row;
                }
                
                if (row == dimensions) {
                    col = 0;
                    row = 0;
                    ++level;
                }
                if (level == levels) {
                    break;
                }
            }
        }
    }

    if (listOrMapInput == 'L') {
        uint32_t inputLevel;
        uint32_t inputRow;
        uint32_t inputCol;
        char closeParenth;
        char sym;
        char comma;
        while (cin >> indivChar) {
            if (indivChar == '/') {
                getline(cin, comment);
                continue;
            }
            else if (indivChar == '(') {
                cin >> inputLevel;
                if (inputLevel >= levels) {
                    cerr << "Invalid map level";
                    exit(1);
                }
                cin >> comma;
                cin >> inputRow;
                if (inputRow >= dimensions) {
                    cerr << "Invalid map row";
                    exit(1);
                }
                cin >> comma;
                cin >> inputCol;
                if (inputCol >= dimensions) {
                    cerr << "Invalid map column";
                    exit(1);
                }
                cin >> comma;
                cin >> sym;
                if (sym != 'S' && sym != '#'  && sym != 'E' && sym != '.' && sym != 'H') {
                    cerr << "Invalid map character";
                    exit(1);
                }
                cin >> closeParenth;
                if (sym == 'S') {
                        //cout << "S detected" << endl; 
                    startPos.level = inputLevel;
                    startPos.row = inputRow;
                    startPos.col = inputCol;
                }
                coordVec[inputLevel][inputRow][inputCol].symbol = sym;
            }
        }
    }

    // After reading the input

    object.routingScheme(coordVec, startPos, dimensions, levels);
    object.backtrace(coordVec, backtraceCoord, backtracePath, startPos);
    object.finalPrint(coordVec, backtraceCoord, backtracePath, startPos, levels, dimensions);
};