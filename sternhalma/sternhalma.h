#pragma once

#include <array>
#include <algorithm>
#include <string>

namespace sternhalma {


struct position_t {
    int i;
    int j;
};

enum class slot_type_t {
    FORBIDDEN = -1,
    FREE      = 0,
    C1        = 1,
    C2        = 2,
    C3        = 3,
    C4        = 4,
    C5        = 5,
    C6        = 6,
};

struct player_t {
    std::string name;
    bool        active;
    slot_type_t home;
};

constexpr bool is_home(slot_type_t home, slot_type_t pos) {
    return pos == home;
}

constexpr bool is_dest(slot_type_t home, slot_type_t pos) {
    switch (home) {
        case slot_type_t::C1: return pos == slot_type_t::C4;
        case slot_type_t::C2: return pos == slot_type_t::C5;
        case slot_type_t::C3: return pos == slot_type_t::C6;
        case slot_type_t::C4: return pos == slot_type_t::C1;
        case slot_type_t::C5: return pos == slot_type_t::C2;
        case slot_type_t::C6: return pos == slot_type_t::C3;
        default: return false;
    }
}

constexpr bool is_enemy(slot_type_t home, slot_type_t pos) {
    return pos != slot_type_t::FREE && !is_home(home, pos) && !is_dest(home, pos);
}

enum class line_rule_t {
    ANY              = 0,
    LINE_OR_DIAGONAL = 1,
    LINE             = 2,
};

struct config_t {
    line_rule_t line_rule;
    bool require_single_leap;
    bool require_symmetry;
    bool require_leap;
    bool disallow_temporary_enemy_landing;
};

constexpr int width(int N) {
    return N * 4 + 1;
}

constexpr int triangular_number(int N) {
    return N * (N + 1) / 2;
}

template<size_t N>
class Board {
public:
    Board(config_t config) : m_config(config) {
        for (int i = 0; i < width(N); i++) {
            for (int j = 0; j < width(N); j++) {
                if ((i < 2 * N + 1 && j < N) || (j < 2 * N + 1 && i < N)) {
                    m_board_base[i][j] = slot_type_t::FORBIDDEN;
                    continue;
                } else if ((i >= 3 * N + 1 && j < N) || (j >= 3 * N + 1 && i < N)) {
                    m_board_base[i][j] = slot_type_t::FORBIDDEN;
                    continue;
                } else if ((i >= 2 * N && j >= 3 * N + 1) || (j >= 2 * N && i >= 3 * N + 1)) {
                    m_board_base[i][j] = slot_type_t::FORBIDDEN;
                    continue;
                } else if ((i >= N && i < 2 * N + 1 && j >= 2 * N && j < 3 * N + 1)) {
                    m_board_base[i][j] = slot_type_t::FREE;
                    continue;
                } else if (i >= 2 * N && i < 3 * N + 1 && j >= N && j < 2 * N + 1) {
                    m_board_base[i][j] = slot_type_t::FREE;
                    continue;
                } else if (i >= N && i < 2 * N && j >= N && j < 2 * N) {
                    int x = i - N;
                    int y = j - N;
                    m_board_base[i][j] = x + y < N ? slot_type_t::C1 : slot_type_t::FREE;
                    continue;
                } else if (i >= 2 * N + 1 && i < 3 * N + 1 && j < N) {
                    int w = 3 * N - i;
                    int z = N - j - 1;
                    m_board_base[i][j] = w + z < N ? slot_type_t::C6 : slot_type_t::FORBIDDEN;
                    continue;
                } else if (i >= 3 * N + 1 && j >= N && j < 2 * N) {
                    int x = i - (3 * N + 1);
                    int y = j - N;
                    m_board_base[i][j] = x + y < N ? slot_type_t::C5 : slot_type_t::FORBIDDEN;
                    continue;
                } else if (i >= 2 * N + 1 && i < 3 * N + 1 && j >= 2 * N + 1 && j < 3 * N + 1) {
                    int w = 3 * N - i;
                    int z = 3 * N - j;
                    m_board_base[i][j] = w + z < N ? slot_type_t::C4 : slot_type_t::FREE;
                    continue;
                } else if (i >= N && i < 2 * N && j >= 3 * N + 1) {
                    int x = i - N;
                    int y = j - (3 * N + 1);
                    m_board_base[i][j] = x + y < N ? slot_type_t::C3 : slot_type_t::FORBIDDEN;
                    continue;
                } else if (i < N && j >= 2 * N + 1 && j < 3 * N + 1) {
                    int w = N - i - 1;
                    int z = 3 * N - j;
                    m_board_base[i][j] = w + z < N ? slot_type_t::C2 : slot_type_t::FORBIDDEN;
                    continue;
                }

                throw std::exception("Failed to consider values");
            }
        }
    }

    bool validate_move(const player_t &player, const position_t &prev, const position_t &pos) const {
        if (m_did_single_hop_occur_this_round) {
            // Can't move after a single-hop
            return false;
        }

        if (m_config.disallow_temporary_enemy_landing && is_enemy(player.home, board_now(pos))) {
            // Can't stay in enemy territory, even temporarily
            return false;
        }

        if (board_base(pos) != slot_type_t::FREE) {
            // Must not be occupied
            return false;
        }

        bool in_bounds = (
               (pos.i >= 0)
            && (pos.i < width(N))
            && (pos.j >= 0)
            && (pos.j < width(N))
        );
        if (not in_bounds) {
            // Must move within bounds
            return false;
        }

        if (m_config.line_rule != line_rule_t::ANY) {
            // Must follow the line rule
            bool on_line = ((pos.i == prev.i) || (pos.j == prev.j) || (pos.i - prev.i == prev.j - pos.j));
            if (not on_line) {
                if (m_config.line_rule == line_rule_t::LINE) {
                    return false;
                }
                bool on_diagonal = (pos.i - prev.i == pos.j - prev.j);
                if (not on_diagonal) {
                    return false;
                }
            }
        }

        const int i_max = std::max(prev.i, pos.i);
        const int i_min = std::min(prev.i, pos.i);
        const int j_max = std::max(prev.j, pos.j);
        const int j_min = std::min(prev.j, pos.j);
        const int i_diff = i_max - i_min;
        const int j_diff = j_max - j_min;

        if (m_config.require_single_leap) {
            // Must follow multi-leap rule
            int num_occupied = 0;
            if (pos.i == prev.i) {
                for (int s = 1; s < j_diff; s++) {
                    num_occupied += occupied(pos.i, j_min + s);
                }
            } else if (pos.j == prev.j) {
                for (int s = 1; s < i_diff; s++) {
                    num_occupied += occupied(i_min + s, pos.j);
                }
            } else if (pos.i - prev.i == prev.j - pos.j) {
                for (int s = 1; s < i_diff; s++) {
                    num_occupied += occupied(i_min + s, j_max - s);
                }
            } else if (pos.i - prev.i == pos.j - prev.j) {
                for (int s = 1; s < i_diff; s++) {
                    num_occupied += occupied(i_min + s, j_min + s);
                }
            } else {
                return false;
            }
            if (num_occupied > 1) {
                return false;
            }
        }

        if (m_config.require_symmetry) {
            // Must follow mirror symmetry rule
            if (pos.i == prev.i) {
                for (int s = 1; s < j_diff; s++) {
                    if (occupied(pos.i, j_min + s) != occupied(pos.i, j_max - s)) {
                        return false;
                    }
                }
            } else if (pos.j == prev.j) {
                for (int s = 1; s < i_diff; s++) {
                    if (occupied(i_min + s, pos.j) != occupied(i_max - s, pos.j)) {
                        return false;
                    }
                }
            } else if (pos.i - prev.i == prev.j - pos.j) {
                for (int s = 1; s < i_diff; s++) {
                    if (occupied(i_min + s, j_max - s) != occupied(i_max - s, j_min + s)) {
                        return false;
                    }
                }
            } else if (pos.i - prev.i == pos.j - prev.j) {
                for (int s = 1; s < i_diff; s++) {
                    if (occupied(i_min + s, j_min + s) != occupied(i_max - s, j_max - s)) {
                        return false;
                    }
                }
            } else {
                return false;
            }
        }

        if (m_config.require_leap && ((m_hops == 0) && (j_diff <= 1) && (i_diff <= 1))) {
            // Must follow leap rule
            bool leapt = false;
            if (pos.i == prev.i) {
                for (int s = 1; s < j_diff; s++) {
                    if (occupied(pos.i, j_min + s)) {
                        leapt = true;
                        break;
                    }
                }
            } else if (pos.j == prev.j) {
                for (int s = 1; s < i_diff; s++) {
                    if (occupied(i_min + s, pos.j)) {
                        leapt = true;
                        break;
                    }
                }
            } else if (pos.i - prev.i == prev.j - pos.j) {
                for (int s = 1; s < i_diff; s++) {
                    if (occupied(i_min + s, j_max - s)) {
                        leapt = true;
                        break;
                    }
                }
            } else if (pos.i - prev.i == pos.j - prev.j) {
                for (int s = 1; s < i_diff; s++) {
                    if (occupied(i_min + s, j_min + s)) {
                        leapt = true;
                        break;
                    }
                }
            }
            if (!leapt) {
                return false;
            }
        }

        // All rules passed
        return true;
    }

    bool validate_destination(const player_t &player, const position_t &pos) {
        return !is_enemy(player.home, board_now(pos));
    }

    slot_type_t board_base(position_t pos) {
        return m_board_base[pos.i][pos.j];
    }

    slot_type_t board_now(position_t pos) {
        return m_board_now[pos.i][pos.j];
    }

    bool occupied(position_t pos) {
        return board_now(pos) != slot_type_t::FREE;
    }
private:
    slot_type_t m_board_base[width(N)][width(N)];
    slot_type_t m_board_now[width(N)][width(N)];
    bool m_did_single_hop_occur_this_round = false;
    config_t m_config;
    int m_hops = 0;
};


}
