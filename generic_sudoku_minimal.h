//
// Created by Kacper on 23.10.2022.
//

#pragma once
#include "generic_sudoku.h"

template <int M, int N, template<int, int> class Heuristic>
class generic_sudoku_minimal : public generic_sudoku<M, N, Heuristic> {
    using Base = generic_sudoku<M, N, Heuristic>;
    using StatePtr = std::unique_ptr<graph_state<uint8_t>>;

    public:
        explicit generic_sudoku_minimal(const std::string &s) : Base(s) {}
        
        StatePtr clone() const {
            return StatePtr(new generic_sudoku_minimal(*this));
        }

        std::vector<StatePtr> get_successors() const override {
            std::vector<StatePtr> successors;
            uint8_t min_pos_number = M * N;
            std::array<uint8_t, 2> min_pos_indexes{0, 0};

            for (int i{}; i < M * N; ++i) {
                for (int j{}; j < M * N; ++j) {
                    if (this->board[i][j] == 0) {
                        auto pos = this->possibilities(i, j);
                        if (min_pos_number > pos.size()) {
                            min_pos_number = pos.size();
                            min_pos_indexes[0] = i;
                            min_pos_indexes[1] = j;
                        }
                    }
                }
            }

            for (int8_t p : this->possibilities(min_pos_indexes[0], min_pos_indexes[1])) {
                auto c = clone();
                dynamic_cast<generic_sudoku_minimal&>(*c).board[min_pos_indexes[0]][min_pos_indexes[1]] = p;
                c->update_score(this->get_g() + 1);
                successors.push_back(std::move(c));
            }

            return successors;
        }
};

