//
// Created by Kacper on 03.10.2022.
//

#pragma once
#include <cstdint>
#include "Header Files/graph_state.hpp"
#include <array>
#include <bits/stdc++.h>
#include <sstream>

template<int M, int N, template<int, int> class Heuristic>
class generic_sudoku : public h_state<uint8_t>{
public:
    generic_sudoku(const std::string &s) {
        std::istringstream in(s);
        int w;
        for(int i{}; i < M*N; ++i) {
            for(int j{}; j < M*N; ++j) {
                if (!(in >> w) || w < 0 || w > M*N) {
                    throw std::domain_error("Invalid input");
                }
                board[i][j] = w;
            }
        }
    }

    std::unique_ptr<graph_state> clone() const override{
        return std::unique_ptr<graph_state>(new generic_sudoku(*this));
    }

    size_t hash_code() const override{
        size_t h=1;
        for(const auto& r: board) {
            for(auto s: r) {
                h = h*31 + s;
            }
        }
        return h;
    }

    std::vector<std::unique_ptr<graph_state>> get_successors() const override {
        std::vector<std::unique_ptr<graph_state>> successors;
        for(int i{}; i < M*N; i++) {
            for(int j{}; j < M*N; j++) {
                if(board[i][j] == 0) {
                    for(int8_t p: possibilities(i ,j)) {
                        auto c = clone();
                        dynamic_cast<generic_sudoku&>(*c).board[i][j] = p;
                        c->update_score(get_g() + 1);
                        successors.push_back(std::move(c));
                    }
                    return successors;
                }
            }
        }
        return {};
    }

    std::string to_string() const override{
        std::ostringstream out;
        for(const auto& r: board) {
            for (auto s: r) {
                out << static_cast<int>(s) << ' ';
            }
            out << '\n';
        }
        return out.str();
    }

    bool is_solution() const override {
        for(const auto& r: board) {
            for(auto s: r) {
                if (s == 0) {
                    return false;
                }
            }
        }
        return true;
    }


protected:
    std::unordered_set<int8_t> possibilities(int r, int c) const {
        std::unordered_set<int8_t> possibilities;
        for(int i{1}; i<=M*N; i++) { //init possibilities
            possibilities.insert(i);
        }
        for(int i{} ; i<M*N; i++) {
            possibilities.erase(board[r][i]);
            possibilities.erase(board[i][c]);
        }

        int x = abs(r/M)*M; //wiersz
        int y = abs(c/N)*N; //kolumna

        for(int i{}; i<M; i++) {
            for(int j{}; j<N; j++) {
                possibilities.erase(board[x+i][y+j]);
            }
        }
        return possibilities;
    }

    uint8_t get_heuristic_grade() const override {
        return heuristic(board);
    }

    bool is_equal(const graph_state &s) const override {
        const auto st = dynamic_cast<const generic_sudoku*>(&s);
        return st != nullptr && st->board == this->board;
    }

    std::array<std::array<uint8_t, M*N>, M*N> board;
    static constexpr Heuristic<M, N> heuristic {};
};