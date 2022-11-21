//
// Created by Kacper on 23.10.2022.
//

#ifndef SZTUCZNAINTEL_GENERIC_SUDOKU_MINIMAL_H
#define SZTUCZNAINTEL_GENERIC_SUDOKU_MINIMAL_H
#include "generic_sudoku.h"

template <int M, int N, template<int, int> class Heuristic>
class generic_sudoku_minimal : public generic_sudoku<M, N, Heuristic>
{
public:
    explicit generic_sudoku_minimal(const std::string &s) : generic_sudoku<M, N, Heuristic>(s) {}

    std::unique_ptr<graph_state<uint8_t>> clone() const override{
        return std::unique_ptr<graph_state<uint8_t>>(new generic_sudoku_minimal(*this));
    }


    std::vector<std::unique_ptr<graph_state<uint8_t>>> get_successors() const override{
        std::vector<std::unique_ptr<graph_state<uint8_t>>> successors;
        uint8_t min_pos_number = M*N;
        uint8_t min_pos_indexes[2]{ 0, 0 };

        for (int i = 0; i < M*N; ++i){
            for (int j = 0; j < M*N; ++j){
                if (this->board[i][j] == 0){
                    auto pos = this->possibilities(i, j);
                    if (min_pos_number > pos.size()){
                        min_pos_number = pos.size();
                        min_pos_indexes[0] = i;
                        min_pos_indexes[1] = j;
                    }
                }
            }
        }
        for (int8_t p : generic_sudoku_minimal<M, N, Heuristic>::possibilities(min_pos_indexes[0],min_pos_indexes[1])){
            auto c = clone();
            ((generic_sudoku_minimal&)*c).board[min_pos_indexes[0]][min_pos_indexes[1]] = p;
            c->update_score(this->get_g() + 1);
            successors.push_back(std::move(c));
        }
        return successors;
    }
};
#endif //SZTUCZNAINTEL_GENERIC_SUDOKU_MINIMAL_H
