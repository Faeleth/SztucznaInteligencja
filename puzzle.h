//
// Created by Kacper on 24.10.2022.
//

#ifndef SZTUCZNAINTEL_PUZZLE_H
#define SZTUCZNAINTEL_PUZZLE_H

#include <cstdint>
#include "Header Files/graph_state.hpp"
#include <array>
#include <bits/stdc++.h>
#include <sstream>

template<int M,int N, template<int, int> class Heuristic>
class puzzle: public f_state<uint8_t> {
public:
    puzzle() {
        //puzzle initialization
        uint8_t counter = 0;
        for(int i=0;i<M;i++) {
            for( int j=0;j<N;j++) {
                board[i][j] = counter;
                counter++;
            }
        }
    }

    puzzle(const std::string &s) {
        std::istringstream in(s);
        int w;
        for(int i=0; i<M ;i++) {
            for(int j=0; j<N; j++) {
                if (!(in>>w) || w<0 || w>M*N-1) {
                    throw std::domain_error("Empty Stream");
                }
                this->board[i][j] = w;
            }
        }
    }

    explicit puzzle(const std::array<std::array<uint8_t, N>, M> board) : board(board) {}

    void shuffle(int n) {
        int x =0;
        int y =0;
        for(int i=0;i<n;i++) {
            int dir = rand() % 4;
            switch (dir) {
                case 0:
                    if(x>0) {
                        std::swap(board[x][y], board[x-1][y]);
                        x-=1;
                    }
                    break;
                case 1:
                    if(x<M-1) {
                        std::swap(board[x][y], board[x+1][y]);
                        x+=1;
                    }
                    break;
                case 2:
                    if(y>0) {
                        std::swap(board[x][y], board[x][y-1]);
                        y-=1;
                    }
                    break;
                case 3:
                    if(y<N-1) {
                        std::swap(board[x][y], board[x][y+1]);
                        y+=1;
                    }
                    break;
            }
        }
    }

    std::unique_ptr<graph_state> clone() const override {
        return std::unique_ptr<graph_state>(new puzzle(*this));
    }

    size_t hash_code() const override {
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
        for(int i=0; i<M; i++) {
            for(int j=0; j<N; j++) {
                if(board[i][j] == 0) {
                    for(auto [di,dj]: possibilities(i,j)) {
                        auto c = clone();
                        std::swap(((puzzle &) *c).board[i][j], ((puzzle &) *c).board[i + di][j + dj]);
                        c->update_score(get_g() + 1);
                        successors.push_back(std::move(c));
                    }
                    return successors;
                }
            }
        }
        return {};
    }

    bool is_solution() const override {
        int counter = 0;
        for(const auto& value: board) {
            for(auto s: value) {
                if(s != counter) {
                    return false;
                }
                counter++;
            }
        }
        return true;
    }

    std::string to_string() const override {
        std::ostringstream  out;
        for(const auto& r: board) {
            for(auto s: r) {
                out << (int)s << ' ';
            }
            out << std::endl;
        }
        return out.str();
    }

    const std::array<std::array<uint8_t, N>, M> getBoard() const {
        return board;
    }

protected:
    std::vector<std::pair<int,int>> possibilities(int r, int c) const {
        std::vector<std::pair<int,int>> p;
        if(board[r][c] != 0) {
            return {};
        }
        if(r>0) {
            p.push_back({-1,0});
        }
        if(r<M-1) {
            p.push_back({1,0});
        }
        if(c>0) {
            p.push_back({0,-1});
        }
        if(c<N-1) {
            p.push_back({0,1});
        }
        return p;
    }

    uint8_t get_heuristic_grade() const override {
        return heuristic(board);
    }

    bool is_equal(const graph_state<uint8_t> &s) const override {
        const auto st = dynamic_cast<const puzzle*>(&s);
        return st != nullptr && st->board == this->board;
    }

    std::array<std::array<uint8_t, N>, M> board;
    static constexpr Heuristic<M, N> heuristic {};
};




#endif //SZTUCZNAINTEL_PUZZLE_H

