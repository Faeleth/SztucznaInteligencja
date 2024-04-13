#include <iostream>
#include "Header Files/graph_search.hpp"
#include "Header Files/graph_state.hpp"
//#include "generic_sudoku.h"
#include "generic_sudoku_minimal.h"
#include "puzzle.h"


//SUDOKU
template<int Rows, int Cols>
struct H_remaining {
    uint8_t operator()(const auto &board) const {
        int counter = 0;
        for (const auto& i:board){
            for (auto j:i) {
                if(j==0) counter++;
            }
        }
        return counter;
    }
};

// ----------------------------------- //
int factorial(int n){
    int ret = 1;
    for(int i=n;i>1;i--) {
        ret *= i;
    }
    return ret;
}

std::string printSolutions(informative_searcher<uint8_t>& informativeSearcher) {
    std::string ret = "";
    for (int i = 0; i < informativeSearcher.get_number_of_solutions(); i++) {
        ret += "Solution number: " + std::to_string(i+1) + "\n";
        ret += informativeSearcher.get_solution(i)->to_string() + "\n";
    }
    return ret;
}

std::string spaced(std::string s){
    std::string spacedString;
    for (auto itr : s){
        spacedString += itr;
        spacedString += ' ';
    }
    return spacedString;
}

std::string parseSudokuTable(std::string s) {
    std::string ret = spaced(s);
    std::replace(ret.begin(), ret.end(), '.', '0');
    return ret;
}

void zad_1() {
    std::cout<< "Zad. 2.19\n";
    std::string s = parseSudokuTable(".4.....4..2323.."); //2 rozwiazania
    generic_sudoku<2, 2, H_remaining> sudoku(s);
    std::cout << sudoku.to_string() <<std::endl;
    informative_searcher<uint8_t> informativeSearcher(sudoku, default_h_compare<uint8_t>, std::numeric_limits<size_t>::max());
    std::cout << informativeSearcher.get_stats() << std::endl;
    std::cout << printSolutions(informativeSearcher);
}

void zad_2() {
    std::cout<< "Zad. 2.20\n";
    std::string s = parseSudokuTable("123456....................................");
    generic_sudoku<2,3,H_remaining> sudoku(s);
    informative_searcher<uint8_t> informativeSearcher(sudoku, default_h_compare<uint8_t>, std::numeric_limits<size_t>::max());
    std::cout << informativeSearcher.get_stats();
    std::cout << "Number of all solutions for 6x6 board (M=2, N=3): " << informativeSearcher.get_number_of_solutions()* factorial(6);

    std::cout<<"\n\n";
    generic_sudoku_minimal<2,3,H_remaining> sudokuMinimal(s);
    informative_searcher<uint8_t> informativeSearcherMinimal(sudoku, default_h_compare<uint8_t>, std::numeric_limits<size_t>::max());
    std::cout << informativeSearcherMinimal.get_stats();
}

void zad_3() {
    std::cout<< "Zad. 2.21\n";
    std::string s = parseSudokuTable("123456....................................");

    generic_sudoku<2,3,H_remaining> sudoku(s);
    informative_searcher<uint8_t> informativeSearcher(sudoku, default_h_compare<uint8_t>, std::numeric_limits<size_t>::max());
    std::cout << informativeSearcher.get_stats();

    std::cout<<"\n\n";

    generic_sudoku_minimal<2,3,H_remaining> sudokuMinimal(s);
    informative_searcher<uint8_t> informativeSearcherMinimal(sudokuMinimal, default_h_compare<uint8_t>, std::numeric_limits<size_t>::max());
    std::cout << informativeSearcherMinimal.get_stats();
}
//SUDOKU
// ----------------------------------- //

//PUZZLE
template<int Rows, int Cols>
struct H_misplaced_tiles {
    uint8_t operator()(const auto &board) const {
        int counter = 0;
        for (int i = 0; i < Rows; i++) {
            for (int j = 0; j < Cols; j++) {
                if (board[i][j] != i * Cols + j + 1)
                    counter++;
            }
        }
        return counter;
    }
};

template<int Rows, int Cols>
struct H_manhattan {
    uint8_t operator()(const auto &board) const {
        int counter = 0;
        for (int i = 0; i < Rows; i++) {
            for (int j = 0; j < Cols; j++) {
                int value = board[i][j];
                if (value != 0) {
                    int correctRow = (value - 1) / Cols;
                    int correctCol = (value - 1) % Cols;
                    counter += abs(correctRow - i) + abs(correctCol - j);
                }
            }
        }
        return counter;
    }
};

std::string getMoves(const graph_state<uint8_t>* s) {
    std::string ret = "";
    while (s->get_parent() != nullptr) {
        //remove white spaces from current and parent graph_state
        std::string current = s->to_string();
        std::string parent = s->get_parent()->to_string();
        current.erase(std::remove_if(current.begin(), current.end(), ::isspace), current.cend());
        parent.erase(std::remove_if(parent.begin(), parent.end(), ::isspace), parent.cend());
        //check cords for 0 in current graph_state->to_string() skipping whitespaces
        int x = 0, y = 0;
        for (int i = 0; i < current.size(); i++) {
            if (current[i] == '0') {
                x = i % 3;
                y = i / 3;
                break;
            }
        }
        //check cords for 0 in parent graph_state->to_string() skipping whitespaces
        int xParent = 0, yParent = 0;
        for (int i = 0; i < parent.size(); i++) {
            if (parent[i] == '0') {
                xParent = i % 3;
                yParent = i / 3;
                break;
            }
        }
        //check where 0 went
        if (x == xParent) {
            if (y > yParent) {
                ret += "D";
            } else {
                ret += "U";
            }
        } else {
            if (x > xParent) {
                ret += "R";
            } else {
                ret += "L";
            }
        }
        s = s->get_parent();
    }
    return std::string(ret.rbegin(), ret.rend());
}


void puzzleMain1(){
    std::string s = parseSudokuTable("125348670");
    auto p = puzzle<3,3,H_manhattan>(s);
    p.shuffle(1000);
    std::cout << p.to_string() << std::endl;
    informative_searcher<uint8_t> informativeSearcher(p, default_f_compare<uint8_t>);
    std::cout << informativeSearcher.get_stats();
    std::cout << "Moves: \n";
    std::cout << getMoves(informativeSearcher.get_solution(0));
}

void puzzleMain2(){
    puzzle<3,3,H_manhattan>* puzzle1;
    puzzle<3,3,H_misplaced_tiles>* puzzle2;
    informative_searcher<uint8_t>* informativeSearcher;
    int64_t time1={}, time2={};
    int state1closed={}, state2closed={};
    int state1open={}, state2open={};
    int counter = 0;

    for (int i = 0; i < 100; i++) {
        puzzle1 = new puzzle<3,3,H_manhattan>();
        puzzle1->shuffle(1000);
        puzzle2 = new puzzle<3,3,H_misplaced_tiles>(puzzle1->getBoard());

        informativeSearcher = new informative_searcher<uint8_t>(*puzzle1, default_f_compare<uint8_t>);
        time1 += informativeSearcher->get_elapsed_time();
        state1closed += informativeSearcher->get_closed().size();
        state1open += informativeSearcher->get_open().size();
        delete informativeSearcher;

        informativeSearcher = new informative_searcher<uint8_t>(*puzzle2, default_f_compare<uint8_t>);
        time2 += informativeSearcher->get_elapsed_time();
        state2closed += informativeSearcher->get_closed().size();
        state2open += informativeSearcher->get_open().size();

        delete informativeSearcher;
        delete puzzle1;
        delete puzzle2;
        std::cout << counter++ << std::endl;
    }
    std::cout << "Manhattan: avgTime: " << parse_time(time1/100) << "\tavgStatesClosed: " << state1closed/100. << "\tavgStatesOpen: " << state1open/100. << std::endl;
    std::cout << "MisplacedTiles: avgTime: " << parse_time(time2/100) << "\tavgStatesClosed: " << state2closed/100. << "\tavgStatesOpen: " << state2open/100. << std::endl;
    std::cout << parse_time(time1) << std::endl;
    std::cout << parse_time(time2) << std::endl;
}

void ex2_24(){
    puzzle<3,3,H_manhattan>* puzzle1;
    puzzle<3,3,H_manhattan>* puzzle2;
    informative_searcher<uint8_t>* informativeSearcher;
    int64_t time1={}, time2={};

    int state1closed={}, state2closed={};
    int state1open={}, state2open={};
    int solutionPathCounter1 = 0, solutionPathCounter2 = 0;
    int counter = 0;


    for (int i = 0; i < 100; i++) {
        puzzle1 = new puzzle<3,3,H_manhattan>();
        puzzle1->shuffle(1000);
        puzzle2 = new puzzle<3,3,H_manhattan>(puzzle1->getBoard());

        informativeSearcher = new informative_searcher<uint8_t>(*puzzle1, default_f_compare<uint8_t>);
        time1 += informativeSearcher->get_elapsed_time();
        state1closed += informativeSearcher->get_closed().size();
        state1open += informativeSearcher->get_open().size();
        solutionPathCounter1 += informativeSearcher->get_solution_path(0).size();
        delete informativeSearcher;

        informativeSearcher = new informative_searcher<uint8_t>(*puzzle2, default_g_compare<uint8_t>);
        time2 += informativeSearcher->get_elapsed_time();
        state2closed += informativeSearcher->get_closed().size();
        state2open += informativeSearcher->get_open().size();
        solutionPathCounter2 += informativeSearcher->get_solution_path(0).size();
        delete informativeSearcher;
        delete puzzle1;
        delete puzzle2;
        std::cout << counter++ << std::endl;
    }
    std::cout << "A*: avgTime: " << parse_time(time1/100) << "\tavgStatesClosed: " << state1closed/100. << "\tavgStatesOpen: " << state1open/100. << "\tavgSolutionPathSize: " << solutionPathCounter1/100. << std::endl;
    std::cout << "BFS: avgTime: " << parse_time(time2/100) << "\tavgStatesClosed: " << state2closed/100. << "\tavgStatesOpen: " << state2open/100. << "\tavgSolutionPathSize: " << solutionPathCounter2/100. << std::endl;
    std::cout << parse_time(time1) << std::endl;
    std::cout << parse_time(time2) << std::endl;
}

template<typename score_type>
auto comp(const graph_state<score_type> &a, const graph_state<score_type> &b) {
    return a.get_f() < b.get_f() || a.get_f() == b.get_f() && a.get_g() < b.get_g();
};
void ex2_25(){
    puzzle<3,3,H_manhattan>* puzzle1;
    puzzle<3,3,H_manhattan>* puzzle2;
    informative_searcher<uint8_t>* informativeSearcher;
    int64_t time1={}, time2={};

    int state1closed={}, state2closed={};
    int state1open={}, state2open={};
    int solutionPathCounter1 = 0, solutionPathCounter2 = 0;
    int counter = 0;


    for (int i = 0; i < 100; i++) {
        puzzle1 = new puzzle<3,3,H_manhattan>();
        puzzle1->shuffle(1000);
        puzzle2 = new puzzle<3,3,H_manhattan>(puzzle1->getBoard());

        informativeSearcher = new informative_searcher<uint8_t>(*puzzle1, default_f_compare<uint8_t>);
        time1 += informativeSearcher->get_elapsed_time();
        state1closed += informativeSearcher->get_closed().size();
        state1open += informativeSearcher->get_open().size();
        solutionPathCounter1 += informativeSearcher->get_solution_path(0).size();
        delete informativeSearcher;

        informativeSearcher = new informative_searcher<uint8_t>(*puzzle2, comp<uint8_t>);
        time2 += informativeSearcher->get_elapsed_time();
        state2closed += informativeSearcher->get_closed().size();
        state2open += informativeSearcher->get_open().size();
        solutionPathCounter2 += informativeSearcher->get_solution_path(0).size();
        delete informativeSearcher;
        delete puzzle1;
        delete puzzle2;
        std::cout << counter++ << std::endl;
    }
    std::cout << "A*: avgTime: " << parse_time(time1/100) << "\tavgStatesClosed: " << state1closed/100. << "\tavgStatesOpen: " << state1open/100. << "\tavgSolutionPathSize: " << solutionPathCounter1/100. << std::endl;
    std::cout << "BFS: avgTime: " << parse_time(time2/100) << "\tavgStatesClosed: " << state2closed/100. << "\tavgStatesOpen: " << state2open/100. << "\tavgSolutionPathSize: " << solutionPathCounter2/100. << std::endl;
    std::cout << parse_time(time1) << std::endl;
    std::cout << parse_time(time2) << std::endl;
}

int main() {
    srand((unsigned) time(NULL));
    puzzleMain2();
    return 0;
}
