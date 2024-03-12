#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <limits>

using namespace std;

class Player { // class for creating the player 
public:
    Player() {}
    Player(const std::string& n, int g) : name(n), guess(g) {}
    std::string getName() const { return name; }
    void setName(const std::string& n) { name = n; }
    int getGuess() const { return guess; }
    void setGuess(int g) { guess = g; }
private:
    std::string name;
    int guess;
};

class Leaderboard // class for creating the leaderboard
{
public:
    Leaderboard();
    void DisplayLeaderboard() const;
    void InsertPlayer(const Player& player);
private:
    static const int NUM_LEADERS = 5;
    Player leaders[NUM_LEADERS];
};

Leaderboard::Leaderboard() { // initializes the leaderboard with default Player objects
    for (int i = 0; i < NUM_LEADERS; ++i) {
        leaders[i] = Player();  
    }
}

void Leaderboard::DisplayLeaderboard() const { // displays the leaderboard using the player's name and guesses
    std::cout << "Here are the current leaders:\n";
    for (const auto& player : leaders) {
        if (!player.getName().empty() && player.getGuess() != 0) {
            std::cout << player.getName() << " made " << player.getGuess() << " guesses\n";
        }
    }
}

void Leaderboard::InsertPlayer(const Player& player) { // inserts the player into the leaderboard
    for (int i = 0; i < NUM_LEADERS; ++i) {
        if (player.getGuess() < leaders[i].getGuess() || leaders[i].getGuess() == 0) {
            for (int j = NUM_LEADERS - 1; j > i; --j) {
                leaders[j] = leaders[j - 1];
            }
            leaders[i] = player;
            break;
        }
    }
}

void askName(Player& player) { // asks the player for their name
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Please enter your name to start: \n";
    std::string name;  // temporary string to store the name
    std::getline(std::cin, name);  // use getline with a string object
    player.setName(name);  // store the name in the player object
}

void countGuesses(Player& player) { // keeps track of the player's number of guesses
    int count = 0;
    int guess = 0;
    srand(static_cast<unsigned int>(time(nullptr)));
    int answer = rand() % 91 + 10;
    double value = sqrt(answer);
    std::cout << std::fixed << "The square root of what number" << " is " << value << ". Guess a value between 10 and 100: \n";
    do {
        count++;
        std::cin >> guess;
        if (guess > answer) {
            std::cout << "Too high, guess again: Guess a value between 10 and 100: \n";
        }
        else if (guess < answer) {
            std::cout << "Too low, guess again: Guess a value between 10 and 100: \n";
        }
        else if (guess == answer) {
            std::cout << "You got it baby! \n";
            std::cout << "You made " << count << " guesses. \n";
            player.setGuess(count);
        }
    } while (guess != answer);
}

void writeToFile(const Player& person) { // writes to the leaderboard text file
    const char* fileName = "leaderboard.txt";
    std::ofstream file(fileName, std::ios_base::app);
    if (!file.is_open()) {
        std::cerr << "Cannot open " << fileName << ". \n";
        exit(1);
    }
    file << person.getName() << " made " << person.getGuess() << " guesses. \n";
}

void PlayGuessingGame(Leaderboard& leaderboard) { // plays the game
    Player player;
    askName(player);
    countGuesses(player);
    writeToFile(player);
    leaderboard.InsertPlayer(player);
    leaderboard.DisplayLeaderboard();
}

int main() {
    Leaderboard leaderboard;
    char c;
    std::cout << "Welcome! Press 'q' to quit or any other key to continue: \n";
    std::cin >> c;
    while (c != 'q') {
        PlayGuessingGame(leaderboard);
        std::cout << "Press 'q' to quit or any other key to continue: \n";
        std::cin >> c;
    }
    std::cout << "Bye Bye!";
    return 0;
}
