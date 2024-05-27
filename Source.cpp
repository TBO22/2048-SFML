#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <vector>
#include<string>
#include <filesystem>
#include <sstream>
#include <SFML/Audio.hpp>
using namespace std;

const int SIZE = 6;
const int TILE_SIZE = 100;
const int GRID_OFFSET_X = 100;
const int GRID_OFFSET_Y = 100;
const int FONT_SIZE = 32;
class StartPage {
public:
    void initButton(sf::Text& button, const std::string& text, float posX, float posY, const std::string& imagePath, int leftPadding, int rightPadding) {
        button.setFont(font_);
        button.setCharacterSize(28);
        button.setString(text);
        // Calculate the width and height of the button text
        float textWidth = button.getGlobalBounds().width;
        float textHeight = button.getGlobalBounds().height;
        // Set the position of the button text to center it within the background
        button.setPosition(posX, posY);

        // Adjust the position of the border rectangle to center it within the background
        float borderRectX = posX - leftPadding - 50;
        float borderRectY = posY - 509;

        // Create the border rectangle
        sf::RectangleShape borderRect(sf::Vector2f(textWidth + leftPadding + rightPadding + 100, button.getGlobalBounds().height + 50));
        borderRect.setPosition(borderRectX, borderRectY);
        borderRect.setFillColor(sf::Color::Transparent);
        borderRect.setOutlineThickness(2);
        borderRect.setOutlineColor(sf::Color::Transparent);

        // Draw the border rectangle and the button text
        window_.draw(borderRect);

        window_.draw(button);
    }

    void initBackground() {
        // Load the background texture
        if (!backgroundTexture.loadFromFile("background.jpeg")) {
            std::cerr << "Failed to load background image!" << std::endl;
            return;
        }

        // Set the texture to the background sprite
        backgroundSprite.setTexture(backgroundTexture);

        // Set the scale of the background sprite to cover the size of the window
        float scaleX = static_cast<float>(window_.getSize().x) / backgroundTexture.getSize().x;
        float scaleY = static_cast<float>(window_.getSize().y) / backgroundTexture.getSize().y;
        backgroundSprite.setScale(scaleX, scaleY);

        // Position the background at the top-left corner
        backgroundSprite.setPosition(0, 0);
    }
    // In the StartPage constructor
    StartPage(sf::RenderWindow& window, const sf::Font& font)
        : window_(window), font_(font) {
        initBackground();

        initButton(startButton_, "Start", window.getSize().x / 2.0f - 40, window.getSize().y / 2.0f + 63, "button_background.png", 50, 50);
        initButton(exitButton_, "Exit", window.getSize().x / 2.0f - 30, window.getSize().y / 2.0f + 135, "button_background.png", 50, 50);
        initButton(aboutButton_, "About", window.getSize().x / 2.0f - 40, window.getSize().y - 60, "button_background.png", 20, 20);

    }

    void draw(sf::RenderWindow& window) {
        // Draw the background first
        window.draw(backgroundSprite);

        // Draw the game logo, buttons, etc.
        window.draw(gameLogoSprite);
        drawButton(startButton_);
        drawButton(exitButton_);
        drawButton(aboutButton_);
    }

    bool isStartButtonPressed(const sf::Event& event) {
        return isButtonPressed(startButton_, event);
    }

    bool isExitButtonPressed(const sf::Event& event) {
        return isButtonPressed(exitButton_, event);
    }

    bool isAboutButtonPressed(const sf::Event& event) {
        return isButtonPressed(aboutButton_, event);
    }

    void handleButtonHover(const sf::Event& event) {
        handleButtonHover(startButton_, event);
        handleButtonHover(exitButton_, event);
        handleButtonHover(aboutButton_, event);
    }

private:
    sf::RenderWindow& window_;
    const sf::Font& font_;
    sf::Sprite gameLogoSprite;
    sf::Texture gameLogoTexture;
    sf::Text startButton_;
    sf::Text aboutButton_;
    sf::Text exitButton_;

    // Add these variables for the background image
    sf::Sprite backgroundSprite;
    sf::Texture backgroundTexture;

    // Draw button with rounded border
    void drawButton(const sf::Text& button) {
        sf::RectangleShape borderRect(sf::Vector2f(button.getGlobalBounds().width + 100, button.getGlobalBounds().height + 10));
        borderRect.setPosition(button.getPosition().x - 50, button.getPosition().y - 0.5);
        borderRect.setFillColor(sf::Color::Transparent);
        borderRect.setOutlineThickness(2);
        borderRect.setOutlineColor(sf::Color::Transparent);
        borderRect.setOutlineThickness(3);

        window_.draw(borderRect);
        window_.draw(button);
    }

    // Check if button is pressed
    bool isButtonPressed(const sf::Text& button, const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window_.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
            if (button.getGlobalBounds().contains(mousePos)) {
                return true;
            }
        }
        return false;
    }

    // Handle button hover effect
    void handleButtonHover(sf::Text& button, const sf::Event& event) {
        if (event.type == sf::Event::MouseMoved) {
            sf::Vector2f mousePos = window_.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
            if (button.getGlobalBounds().contains(mousePos)) {
                button.setFillColor(sf::Color::Transparent);
            }
            else {
                button.setFillColor(sf::Color::Transparent);
            }
        }
    }
};

class Tile {
public:
    Tile(int value = 0) : value_(value) {
    }
    int getValue() const { 
        return value_; 
    }
    void setValue(int value) {
        value_ = value;
    }

private:
    int value_;
};

class Board {
public:
    Board(int size) : size_(size) {
        board_.resize(size, vector<Tile>(size));
    }
    static sf::Font defaultFont;
    const sf::Font& getFont() const {  // Added getter method
        return font;
    }
    bool isEmpty(int row, int col) const { 
        return board_[row][col].getValue() == 0; 
    }
    void setValue(int row, int col, int value) { 
        board_[row][col].setValue(value); 
    }
    int getValue(int row, int col) const {
        return board_[row][col].getValue(); 
    }
    bool isFull() const {
        for (int i = 0; i < size_; i++) {
            for (int k = 0; k < size_; k++) {
                if (isEmpty(i, k)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool addNewTile() {
        if (isFull()) {
            return false;
        }
        vector<pair<int, int> > emptyTiles;
        for (int i = 0; i < size_; i++) {
            for (int k = 0; k < size_; k++) {
                if (isEmpty(i, k)) {
                    emptyTiles.push_back(make_pair(i, k));
                }
            }
        }
        int randomIndex = rand() % emptyTiles.size();
        int value = (rand() % 2 + 2) * 2;  // Either 2 or 4
        setValue(emptyTiles[randomIndex].first, emptyTiles[randomIndex].second, value);
        return true;
    }

    void initializeBoard() {
        for (int i = 0; i < size_; i++) {
            for (int k = 0; k < size_; k++) {
                board_[i][k].setValue(0);
            }
        }
        addNewTile();
        addNewTile();
    }

    void draw(sf::RenderWindow& window, const sf::Font& defaultFont) {
        // Create a gradient rectangle for the background
        sf::VertexArray gradient(sf::Quads, 4);
        gradient[0].position = sf::Vector2f(0, 0);
        gradient[1].position = sf::Vector2f(window.getSize().x, 0);
        gradient[2].position = sf::Vector2f(window.getSize().x, window.getSize().y);
        gradient[3].position = sf::Vector2f(0, window.getSize().y);

        gradient[0].color = sf::Color(115, 192, 223);  // Pinkish
        gradient[1].color = sf::Color(120, 0, 12);    // Purplish
        gradient[2].color = sf::Color(0, 191, 155);     // Blueish
        gradient[3].color = sf::Color(115, 125, 100);   // Pinkish

        window.draw(gradient);

        for (int i = 0; i < size_; i++) {
            for (int k = 0; k < size_; k++) {
                sf::RectangleShape tile(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                if (board_[i][k].getValue() == 0) {
                    tile.setFillColor(sf::Color(155, 192, 203)); // Pinkish
                }
                else {
                    tile.setFillColor(sf::Color(105, 105, 100));   // Purplish
                }
                tile.setOutlineColor(sf::Color::Black);
                tile.setOutlineThickness(2);
                tile.setPosition(GRID_OFFSET_X + k * TILE_SIZE, GRID_OFFSET_Y + i * TILE_SIZE);
                window.draw(tile);

                if (board_[i][k].getValue() > 0) {
                    sf::Text text;
                    text.setCharacterSize(FONT_SIZE);  // Set font size
                    text.setFillColor(sf::Color::Black);  // Set text color
                    text.setFont(defaultFont);

                    stringstream ss;
                    ss << board_[i][k].getValue();
                    text.setString(ss.str());

                    // Centering the text within the tile
                    sf::FloatRect textRect = text.getLocalBounds();
                    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                    text.setPosition(tile.getPosition().x + TILE_SIZE / 2.0f,
                        tile.getPosition().y + TILE_SIZE / 2.0f);

                    window.draw(text);
                }
            }
        }
    }

    bool moveLeft() {
        bool moved = false;

        for (int i = 0; i < size_; i++) {
            int writeIndex = 0;
            for (int k = 0; k < size_; k++) {
                if (!board_[i][k].getValue()) continue;

                if (writeIndex != k) {
                    moved = true;
                    board_[i][writeIndex].setValue(board_[i][k].getValue());
                    board_[i][k].setValue(0);
                }

                if (writeIndex > 0 && writeIndex - 1 >= 0 && board_[i][writeIndex].getValue() == board_[i][writeIndex - 1].getValue()) {
                    board_[i][writeIndex - 1].setValue(board_[i][writeIndex - 1].getValue() * 2);
                    board_[i][writeIndex].setValue(0);
                    score += 1/2; // Increment score by 1 for each successful move
                }

                writeIndex++;
            }
        }

        return moved;
    }

    bool moveRight() {
        bool moved = false;

        for (int i = 0; i < size_; i++) {
            int writeIndex = size_ - 1;
            for (int k = size_ - 1; k >= 0; --k) {
                if (!board_[i][k].getValue()) continue;

                if (writeIndex != k) {
                    moved = true;
                    board_[i][writeIndex].setValue(board_[i][k].getValue());
                    board_[i][k].setValue(0);
                }

                if (writeIndex < size_ - 1 && board_[i][writeIndex].getValue() == board_[i][writeIndex + 1].getValue()) {
                    board_[i][writeIndex + 1].setValue(board_[i][writeIndex + 1].getValue() * 2);
                    board_[i][writeIndex].setValue(0);
                    score += 1/2;
                }

                writeIndex--;
            }
        }

        return moved;
    }

    bool moveUp() {
        // Transpose the board, call moveLeft, and then transpose it back
        // This is equivalent to moving tiles up
        transpose(); // Transpose the board
        bool moved = moveLeft(); 
        transpose(); 

        if (moved) {
            score += 0.39; 
        }

        return moved;
    }

    bool moveDown() {
        transpose();
        bool moved = moveRight(); 
        transpose(); 

        if (moved) {
            score += 0.38; 
        }

        return moved;
    }
    bool isWin() const {
        for (int i = 0; i < size_; i++) {
            for (int k = 0; k < size_; k++) {
                if (board_[i][k].getValue() == 2048) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isGameOver() const {
        if (!isFull()) return false;

        for (int i = 0; i < size_; i++) {
            for (int k = 0; k < size_ - 1; k++) {
                if (board_[i][k].getValue() == board_[i][k + 1].getValue()) return false;
                if (board_[k][i].getValue() == board_[k + 1][i].getValue()) return false;
            }
        }
        return true;
    }

    int getScore() const {
        return score;
    }

private:
    int size_;
    float score = 0;
    vector<vector<Tile> > board_;
    sf::Font font;
    void transpose() {
        vector<vector<Tile> > temp(size_, vector<Tile>(size_));
        for (int i = 0; i < size_; i++) {
            for (int k = 0; k < size_; k++) {
                temp[i][k].setValue(board_[k][i].getValue());
            }
        }
        board_ = temp;
    }
};
sf::Font Board::defaultFont;
void displayAboutInfo(sf::RenderWindow& window, const sf::Font& font) {
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("backgroundabout.jpg")) { 
        cerr << "Failed to load background image!" << endl;
        return;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    sf::Text aboutText;
    aboutText.setFont(font);
    aboutText.setCharacterSize(24); 
    aboutText.setFillColor(sf::Color::Cyan);
    aboutText.setString(
        "-------------------- 2048 --------------------\n"
        "   Created by 4 passionate young data         \n"
        "   scientists with a vision to innovate       \n"
        "   and transform the world through data.      \n"
        "   Meet the Team:                             \n"
        "   - Talha Bin Omar: A master of algorithms   \n"
        "     and data structures, bringing ideas to   \n"
        "     life with code.                          \n"
        "                                              \n"
        "   - Muhammad Haroon Khan: A wizard of        \n"
        "     machine learning, turning data into      \n"
        "     intelligent insights.                    \n"
        "                                              \n"
        "   - Muhammad Ibrahim Arif: A data wrangler   \n"
        "     who makes sense of chaos, ensuring       \n"
        "     accuracy and reliability.                \n"
        "                                              \n"
        "   - Huma Imtiaz: A storyteller with data,    \n"
        "     creating visualizations that speak       \n"
        "     volumes.                                 \n"
        "                                              \n"
        "----------------------------------------------\n"
    );

    // Center the text horizontally and adjust vertically
    sf::FloatRect textRect = aboutText.getLocalBounds();
    aboutText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top);
    aboutText.setPosition(sf::Vector2f(window.getSize().x / 2.0f, 50.0f));

    // Create the back button
    sf::Text backButton;
    backButton.setFont(font);
    backButton.setCharacterSize(24);
    backButton.setFillColor(sf::Color::Red);
    backButton.setString("Back");
    backButton.setPosition(10, window.getSize().y - backButton.getLocalBounds().height - 10);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }

            // Handle back button click
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (backButton.getGlobalBounds().contains(mousePos)) {
                        return; // Exit the about info screen and return to start page
                    }
                }
            }
        }
        window.clear(sf::Color::White); // Clear the window with white color
        window.draw(backgroundSprite); // Draw the background image
        window.draw(aboutText); // Draw the text
        window.draw(backButton); // Draw the back button
        window.display(); // Display what has been drawn
    }
}

int main() {
    srand(time(NULL));
    int score = 0;
    sf::Image icon;
    if (!icon.loadFromFile("logo3.png")) { 
        cerr << "Failed to load icon image!" << endl;
        return EXIT_FAILURE;
    }
 
    sf::RenderWindow window(sf::VideoMode(620, 600), "2048");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    if (!Board::defaultFont.loadFromFile("Roboto-Bold.ttf")) {
        cerr << "Failed to load default font!" << endl;
        return EXIT_FAILURE;
    }

    Board board(SIZE);

    StartPage startPage(window, Board::defaultFont);

    bool isGameStarted = false; // Track if the game has started
    board.initializeBoard();
    // Load the sound buffer and set up the sound object
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("laila.mp3")) { // Adjust the file path accordingly
        cerr << "Failed to load sound effect!" << endl;
        return EXIT_FAILURE;
    }
    sf::Sound sound;
    sound.setBuffer(buffer);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            startPage.handleButtonHover(event);

            if (!isGameStarted) { // Only check buttons if game hasn't started
                if (startPage.isStartButtonPressed(event)) {
                    isGameStarted = true; // Start the game
                }
                else if (startPage.isExitButtonPressed(event)) {
                    window.close();
                }
                else if (startPage.isAboutButtonPressed(event)) {
                    displayAboutInfo(window, Board::defaultFont);
                }

            }//agar mam bole ke keyboard keys change kro jis se control krte movements
            else { // Game is started, handle game events
                if (event.type == sf::Event::KeyPressed) {
                    bool successfulMove = false;
                    switch (event.key.code) {
                    case sf::Keyboard::Left:
                        successfulMove = board.moveLeft();
                        break;
                    case sf::Keyboard::Right:
                        successfulMove = board.moveRight();
                        break;
                    case sf::Keyboard::Up:
                        successfulMove = board.moveUp();
                        break;
                    case sf::Keyboard::Down:
                        successfulMove = board.moveDown();
                        break;
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    }

                    if (successfulMove) {
                        board.addNewTile();
                        score += board.getScore();
                        sound.play();
                    }
                }
            }
        }

        window.clear(sf::Color::Transparent);

        if (!isGameStarted) { // Draw start page if game hasn't started
            startPage.draw(window);
        }
        else { // Draw game board if game is started
            board.draw(window, Board::defaultFont);
            sf::Text text;
            text.setFont(board.defaultFont);
            text.setCharacterSize(32);
            text.setFillColor(sf::Color::Black);

            std::stringstream ss;
            ss << "Score: " << score;
            text.setString(ss.str());
            text.setPosition(10, 10);
            window.draw(text);

            if (board.isGameOver()) {
                // Display "Game Over!" message and prompt to play again
                sf::Text gameOverText;
                gameOverText.setFont(Board::defaultFont);
                gameOverText.setCharacterSize(32);
                gameOverText.setFillColor(sf::Color::Black);
                gameOverText.setString("Game Over! Play again? (Y/N)");
                gameOverText.setPosition(window.getSize().x / 2.0f - gameOverText.getLocalBounds().width / 2.0f, 50);
                window.draw(gameOverText);

                // Handle user input to play again or quit
                sf::Event gameOverEvent;
                while (window.pollEvent(gameOverEvent)) {
                    if (gameOverEvent.type == sf::Event::Closed) {
                        window.close();
                    }
                    if (gameOverEvent.type == sf::Event::KeyPressed) {
                        if (gameOverEvent.key.code == sf::Keyboard::Y) {
                            // Restart the game
                            board.initializeBoard();
                            score = 0;
                        }
                        else if (gameOverEvent.key.code == sf::Keyboard::N) {
                            window.close();
                        }
                    }
                }
            }
        }

        window.display();
    }
    return 0;
}



