


#pragma once

class Player;

struct Position {
    int row;
    int col;

    Player* owner = nullptr;

    Position(int r, int c) : row(r), col(c) {}
    bool IsValid() const { return row >= 0 && col >= 0; }
};


// Assumes 2-row (0 and 1), 7-column (0 to 6) layout
inline Position FromID(int id) {
    if (id < 1 || id > 14) return Position(-1, -1);
    int row = (id - 1) / 7;
    int col = (id - 1) % 7;
    return Position(row, col);
}
