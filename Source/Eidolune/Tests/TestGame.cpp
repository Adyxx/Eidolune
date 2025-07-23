#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

struct Card {
    std::string name;
    int cost;
    int power;
    int health;
    bool tapped = false;
    bool summoningSickness = true;

    Card(std::string n, int c, int p, int h) : name(n), cost(c), power(p), health(h) {}
};

struct Player {
    std::string name;
    int health = 20;
    int energy = 0;
    int maxEnergy = 0;

    std::vector<Card> deck;
    std::vector<Card> hand;
    std::vector<Card> board;

    Player(std::string n) : name(n) {}

    void startTurn() {
        maxEnergy = std::min(maxEnergy + 1, 10);
        energy = maxEnergy;
        std::cout << name << "'s turn starts. Energy: " << energy << "/" << maxEnergy << "\n";

        for (auto& c : board) {
            c.tapped = false;
            c.summoningSickness = false;
        }

        drawCard();
    }

    void drawCard() {
        if (deck.empty()) {
            std::cout << name << " has no cards to draw!\n";
            return;
        }
        hand.push_back(deck.back());
        deck.pop_back();
        std::cout << name << " draws a card: " << hand.back().name << "\n";
    }

    void playCard(int idx) {
        if (idx < 0 || idx >= (int)hand.size()) {
            std::cout << "Invalid card index\n";
            return;
        }
        Card& card = hand[idx];
        if (energy < card.cost) {
            std::cout << "Not enough energy to play " << card.name << "\n";
            return;
        }
        energy -= card.cost;
        board.push_back(card);
        hand.erase(hand.begin() + idx);
        std::cout << name << " plays " << card.name << " to the board.\n";
    }
};

int main() {
    Player player1("Alice");
    Player player2("Bob");

    player1.deck.push_back(Card("Soldier", 1, 2, 2));
    player1.deck.push_back(Card("Archer", 2, 3, 1));
    player2.deck.push_back(Card("Knight", 2, 3, 3));
    player2.deck.push_back(Card("Mage", 3, 4, 2));

    for (int i = 0; i < 3; ++i) {
        player1.drawCard();
        player2.drawCard();
    }

    Player* currentPlayer = &player1;
    Player* opponent = &player2;

    while (player1.health > 0 && player2.health > 0) {
        currentPlayer->startTurn();

        std::string command;
        while (true) {
            std::cout << currentPlayer->name << "'s hand:\n";
            for (size_t i = 0; i < currentPlayer->hand.size(); ++i) {
                const auto& card = currentPlayer->hand[i];
                std::cout << "  " << i << ": " << card.name << " (Cost " << card.cost << ")\n";
            }

            std::cout << "Command (play <i> / end): ";
            std::getline(std::cin, command);

            if (command == "end") {
                break;
            } else if (command.rfind("play ", 0) == 0) {
                int idx = std::stoi(command.substr(5));
                currentPlayer->playCard(idx);
            } else {
                std::cout << "Unknown command\n";
            }
        }

        std::swap(currentPlayer, opponent);
    }

    std::cout << "Game Over!\n";
    return 0;
}
