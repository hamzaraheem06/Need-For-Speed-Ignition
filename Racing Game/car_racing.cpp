#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<iostream>
#include<cmath>
#include<fstream>
#include<iomanip>
#include<sstream>
using namespace std;
using namespace sf;

// Asset Paths
const string fontPath = "assets/fonts/arial.ttf";
const string collisionSoundPath = "assets/sounds/collision.wav";
const string backgroundMusicPath = "assets/sounds/background.ogg";
const string carShapePath = "assets/car.png";
const string backgroundTrack = "assets/background/background.png";
const string menuTitleImage = "assets/background/image.jpg";
const string pauseTitleImage = "assets/background/image1.jpg";
const string carImage1 = "assets/cars/image1.png";
const string carImage2 = "assets/cars/image2.png";
const string carImage3 = "assets/cars/image3.png";
const string carImage4 = "assets/cars/image4.png";
const string carImage5 = "assets/cars/image5.png";
const string carImage6 = "assets/cars/image6.png";
const string clickSound = "assets/sounds/click.wav";
const string save_load = "config/setting.txt";
const string countDownSoundPath = "assets/sounds/countdown.wav";
// Constants for track dimensions
const int trackWidth = 1920;
const int trackHeight = 1080;
int lastCheckpointScore = 0;  // Last checkpoint score 
bool passedCheckpoint = false;  // Flag to check if player passed a checkpoint
Texture selectedCarTexture;










void checkForCheckpoint(int& score) {
    //// If player crosses a checkpoint (e.g., 200, 400, etc.)
    //if (score >= lastCheckpointScore + 200) {
    //    lastCheckpointScore = score;  // Update the last checkpoint to the current score
    //    passedCheckpoint = true;      // Mark that the player passed a checkpoint
    //}
    switch (score / 200) {
    case 0:
        lastCheckpointScore = 0;
        break;
    case 1:
        lastCheckpointScore = 200;
        break;
    case 2:
        lastCheckpointScore = 400;
        break;
    case 3:
        lastCheckpointScore = 600;
        break;
    case 4:
        lastCheckpointScore = 800;
        break;
    case 5:
        lastCheckpointScore = 1000;
        break;

    }
    passedCheckpoint = true;

}

void countDown(RenderWindow& window, Font& font) {
    int countDownTime = 3;
    Clock countDownClock;
    int targetCharacterSize = 100;

    // Create the countdown text object
    Text countDownText(to_string(countDownTime), font);
    countDownText.setFillColor(Color::Green);

    // Initial position calculation (centered)
    countDownText.setPosition(window.getSize().x / 2 - countDownText.getLocalBounds().width / 2,
        window.getSize().y / 2 - countDownText.getLocalBounds().height / 2);

    SoundBuffer buffer;
    if (!buffer.loadFromFile(countDownSoundPath)) {
        cout << "Sound Not Found!";
        window.close();
    }

    Sound countDownSound(buffer);

    // Play the countdown sound once
    countDownSound.play();

    while (countDownTime > 1) { // Stop at 1, not 0
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        // Check if a second has passed
        if (countDownClock.getElapsedTime().asSeconds() >= 1.0f) {
            countDownTime--;
            countDownClock.restart();
        }

        // Set text properties based on the countdown time
        if (countDownTime == 3) {
            countDownText.setFillColor(Color::Green);
            targetCharacterSize = 100;
        }
        else if (countDownTime == 2) {
            countDownText.setFillColor(Color::Yellow);
            targetCharacterSize = 150;
        }
        else if (countDownTime == 1) {
            countDownText.setFillColor(Color::Red);
            targetCharacterSize = 200;
        }

        // Set the character size directly to the target size
        countDownText.setCharacterSize(targetCharacterSize);

        // Recalculate position based on the new size
        countDownText.setPosition(window.getSize().x / 2 - countDownText.getLocalBounds().width / 2,
            window.getSize().y / 2 - countDownText.getLocalBounds().height / 2);

        // Smoothly fade the text
        int alpha = (countDownTime * 255) / 3;
        countDownText.setFillColor(Color(countDownText.getFillColor().r, countDownText.getFillColor().g, countDownText.getFillColor().b, alpha));

        // Update the text for the current countdown number
        countDownText.setString(to_string(countDownTime));

        // Draw the countdown text
        window.clear();
        window.draw(countDownText);
        window.display();

        // Wait briefly before the next frame to avoid double counting
        sleep(milliseconds(200));
    }

    // Fade out the text after countdown finishes
    for (int i = 255; i >= 0; i -= 5) {
        window.clear();
        countDownText.setFillColor(Color(countDownText.getFillColor().r, countDownText.getFillColor().g, countDownText.getFillColor().b, i));
        window.draw(countDownText);
        window.display();
        sleep(milliseconds(10));
    }
}







void resetToCheckpoint(int& lives, int& score, float& currentFuel, vector<RectangleShape>& obstacles, Sprite& carSprite, const int trackWidth, const int trackHeight, bool& carSelected, vector<CircleShape>& powerUps) {
    // If the player has passed a checkpoint, reset the game to the last checkpoint state
    if (passedCheckpoint) {
        score = lastCheckpointScore;  // Set score to the last checkpoint score
        lives = 10;                   // Reset lives (or any other values)
        currentFuel = 100.0f;         // Reset fuel
        carSprite.setPosition(trackWidth / 2, trackHeight - 120); // Reset car position
        carSelected = false;          // Reset car selection

        // Reset obstacles and power-ups to their state at the checkpoint
        for (auto& obstacle : obstacles) {
            obstacle.setPosition(rand() % trackWidth, rand() % (trackHeight / 2)); // Reposition obstacles
        }

        for (auto& powerUp : powerUps) {
            powerUp.setPosition(rand() % trackWidth, rand() % (trackHeight / 2)); // Reposition power-ups
        }

        passedCheckpoint = false;  // Reset the checkpoint flag
    }
}










void drawRoundedButton(RenderWindow& window, Vector2f position, Vector2f size, const string& text, Font& font, bool isHovered, Color fillColor, Color hoverColor) {
    Color buttonColor = isHovered ? hoverColor : fillColor;

    // Draw base rectangle
    float cornerRadius = 20.0f;
    RectangleShape baseRect(Vector2f(size.x - 2 * cornerRadius, size.y));
    baseRect.setFillColor(buttonColor);
    baseRect.setPosition(position.x + cornerRadius, position.y);
    window.draw(baseRect);

    // Draw rounded corners
    CircleShape corner(cornerRadius);
    corner.setFillColor(buttonColor);

    corner.setPosition(position);
    window.draw(corner);

    corner.setPosition(position.x + size.x - 2 * cornerRadius, position.y);
    window.draw(corner);

    corner.setPosition(position.x, position.y + size.y - 2 * cornerRadius);
    window.draw(corner);

    corner.setPosition(position.x + size.x - 2 * cornerRadius, position.y + size.y - 2 * cornerRadius);
    window.draw(corner);

    // Draw text
    Text buttonText(text, font, 50);
    buttonText.setFillColor(Color::White);
    FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setPosition(
        position.x + (size.x - textBounds.width) / 2,
        position.y + (size.y - textBounds.height) / 2 - 10
    );
    window.draw(buttonText);
}










struct GameState {
    bool isMusicPaused;
    float volume;
    bool isFullscreen;

    int score;             // Save score
    int lives;             // Save number of lives
    Vector2f playerPos;    // Save player position
    float songPosition;    // Save current position of the song

    // Save settings and game state to a file
    void save(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            file << "MusicPaused=" << isMusicPaused << "\n";
            file << "Volume=" << volume << "\n";
            file << "Fullscreen=" << isFullscreen << "\n";

            // Save game-specific data
            file << "Score=" << score << "\n";
            file << "Lives=" << lives << "\n";
            file << "PlayerPosX=" << playerPos.x << "\n";
            file << "PlayerPosY=" << playerPos.y << "\n";
            file << "SongPosition=" << songPosition << "\n";

            file.close();
        }
        else {
            cout << "Failed to save the game state." << endl;
        }
    }

    // Load settings and game state from a file
    void load(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                if (line.find("MusicPaused=") != string::npos) {
                    isMusicPaused = (line.substr(12) == "1");
                }
                else if (line.find("Volume=") != string::npos) {
                    volume = stof(line.substr(7));
                }
                else if (line.find("Fullscreen=") != string::npos) {
                    isFullscreen = (line.substr(11) == "1");
                }
                else if (line.find("Score=") != string::npos) {
                    score = stoi(line.substr(6));  // Load score
                }
                else if (line.find("Lives=") != string::npos) {
                    lives = stoi(line.substr(6));  // Load lives
                }
                else if (line.find("PlayerPosX=") != string::npos) {
                    playerPos.x = stof(line.substr(11));  // Load player X position
                }
                else if (line.find("PlayerPosY=") != string::npos) {
                    playerPos.y = stof(line.substr(11));  // Load player Y position
                }
                else if (line.find("SongPosition=") != string::npos) {
                    songPosition = stof(line.substr(13));  // Load song position
                }
            }
            file.close();
        }
        else {
            cout << "Failed to load the game state." << endl;
        }
    }
};


// Function to show the pause menu
void carSelection(RenderWindow& window,Font font, bool& carSelected, Texture& selectedCarTexture);
void menuSection(RenderWindow& window, Font& font, Texture& selectedCarTexture,bool isPaused,bool& carSelected,Music& backgroundMusic);
void generateObstacles(vector<RectangleShape>& obstacles, int numObstacles, int obstacleSize, Vector2f trackBounds);
void settingsWindow(RenderWindow& window, Font& font, Texture& selectedCarTexture, bool& carSelected,bool ispaused, Music& backgroundMusic);
int showPauseMenu(RenderWindow& window, Font& font, bool& ispaused, Texture selectedCarTexture,bool carSelected, Music& backgroundMusic);
void showMessage(RenderWindow& window, Font& font);
void tryAgain(RenderWindow& window, Font& font, bool& isPaused, bool& carSelected, int& lives, int& score, float& currentFuel, vector<RectangleShape>& obstacles, Sprite& carSprite, const int trackWidth, const int trackHeight, Music& backgroundMusic, vector<CircleShape>& powerUps);
void resetGame(int& lives, int& score, float& currentFuel, vector<RectangleShape>& obstacles, Sprite& carSprite, const int trackWidth, const int trackHeight,bool& carSelected);
void managePowerUps(RenderWindow& window, Sprite& carSprite, vector<CircleShape>& powerUps, float& currentFuel, int& score, int& speedBoostDuration,int& lives, int powerUpSize, int numPowerUps, Vector2f trackBounds);
void addProfile(RenderWindow& window, Font& font, Sound& clickSound);

// Function to generate obstacles
void generateObstacles(vector<RectangleShape>& obstacles, int numObstacles, int obstacleSize, Vector2f trackBounds) {
    obstacles.clear();
    srand(static_cast<unsigned>(time(0))); // Initialize random seed
    for (int i = 0; i < numObstacles; ++i) {
        RectangleShape obstacle(Vector2f(obstacleSize, obstacleSize));
        obstacle.setFillColor(Color::Red);

        // Randomize both x and y positions within bounds
        float randomX = 300 + rand() % (1600 - 300 - obstacleSize); // Consistent x within 300-1600
        float randomY = rand() % int(trackBounds.y / 2);            // Random y within the top half of the track
        obstacle.setPosition(randomX, randomY);

        obstacles.push_back(obstacle);
    }
}
void addProfile(RenderWindow& window, Font& font, Sound& clickSound) {
    Vector2f buttonSize(200, 80);
    float cornerRadius = 20.0f; // Radius for rounded corners
    Color buttonFillColor = Color(70, 130, 180); // Steel blue for buttons
    Color hoverColor = Color(100, 149, 237); // Light steel blue for hover effect

    // Set up text for entering the profile name
    Text profileInputText("Enter Profile Name: ", font, 50);
    profileInputText.setFillColor(Color::White);
    profileInputText.setPosition(500, 300);

    string newProfileName = "";
    bool isProfileNameBeingEntered = true;

    // Set up text for profile name input
    Text inputText("", font, 50);
    inputText.setFillColor(Color::White);
    inputText.setPosition(500, 400);

    // Buttons Text
    Text okButtonText("OK", font, 40);
    Text cancelButtonText("Cancel", font, 40);

    // Set up buttons
    RectangleShape okButton(Vector2f(buttonSize.x, buttonSize.y));
    okButton.setFillColor(buttonFillColor);
    okButton.setPosition(500, 500);

    RectangleShape cancelButton(Vector2f(buttonSize.x, buttonSize.y));
    cancelButton.setFillColor(buttonFillColor);
    cancelButton.setPosition(800, 500);

    // Helper function to draw rounded buttons
    auto drawRoundedButton = [&](RectangleShape& button, Text& buttonText, Vector2f position, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;

        // Draw base rectangle with rounded corners
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);

        // Draw rounded corners
        CircleShape topLeftCorner(cornerRadius);
        topLeftCorner.setFillColor(fillColor);
        topLeftCorner.setPosition(position);

        CircleShape topRightCorner(cornerRadius);
        topRightCorner.setFillColor(fillColor);
        topRightCorner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y);

        CircleShape bottomLeftCorner(cornerRadius);
        bottomLeftCorner.setFillColor(fillColor);
        bottomLeftCorner.setPosition(position.x, position.y + buttonSize.y - 2 * cornerRadius);

        CircleShape bottomRightCorner(cornerRadius);
        bottomRightCorner.setFillColor(fillColor);
        bottomRightCorner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius);

        // Draw corners and rectangle
        window.draw(topLeftCorner);
        window.draw(topRightCorner);
        window.draw(bottomLeftCorner);
        window.draw(bottomRightCorner);
        window.draw(baseRect);

        // Draw button text
        buttonText.setFillColor(Color::White);
        FloatRect textBounds = buttonText.getLocalBounds();
        buttonText.setPosition(position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10);
        window.draw(buttonText);
        };

    // Handle user input for the profile name
    while (isProfileNameBeingEntered) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }

            // Handle typing in the text box
            if (event.type == Event::TextEntered) {
                if (event.text.unicode == 8 && newProfileName.length() > 0) { // Backspace
                    newProfileName.pop_back();
                }
                else if (event.text.unicode < 128) {
                    newProfileName += static_cast<char>(event.text.unicode);
                }
            }

            // Check button clicks (OK and Cancel)
            Vector2i mousePos = Mouse::getPosition(window);

            // OK button click
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (okButton.getGlobalBounds().contains(Vector2f(mousePos))) {
                    // Save profile to file
                    ofstream profileFile("profiles.txt", ios::app);
                    profileFile << newProfileName << endl;
                    profileFile.close();

                    // Play sound and update message
                    clickSound.play();
                    profileInputText.setString("Profile Added: " + newProfileName);
                    isProfileNameBeingEntered = false;
                }
                // Cancel button click
                if (cancelButton.getGlobalBounds().contains(Vector2f(mousePos))) {
                    clickSound.play();
                    isProfileNameBeingEntered = false;  // Exit without saving
                }
            }
        }

        // Update the input text as the user types
        inputText.setString(newProfileName);

        // Draw UI
        window.clear();
        window.draw(profileInputText);
        window.draw(inputText);

        // Check hover effects for buttons
        bool isOkHovered = okButton.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window)));
        bool isCancelHovered = cancelButton.getGlobalBounds().contains(Vector2f(Mouse::getPosition(window)));

        // Draw the rounded OK and Cancel buttons with hover effect
        drawRoundedButton(okButton, okButtonText, Vector2f(500, 500), isOkHovered);
        drawRoundedButton(cancelButton, cancelButtonText, Vector2f(800, 500), isCancelHovered);

        window.display();
    }
}

void savePlayTime(float playTime) {
    ofstream file("playtime.txt");
    if (file.is_open()) {
        file << playTime;
        file.close();
    }
}

// Function to load playtime from a file
float loadPlayTime() {
    ifstream file("playtime.txt");
    float playTime = 0.0f;
    if (file.is_open()) {
        file >> playTime;
        file.close();
    }
    return playTime;
}


void managePowerUps(RenderWindow& window, Sprite& carSprite, vector<CircleShape>& powerUps, float& currentFuel, int& score, int& speedBoostDuration, int& lives, int powerUpSize, Vector2f trackBounds) {
    // Generate power-ups based on score milestones
    if (score % 50 == 0 && score != 0) {
        // Add a fuel power-up if not already present
        if (std::none_of(powerUps.begin(), powerUps.end(), [](CircleShape& p) { return p.getFillColor() == Color::Green; })) {
            CircleShape fuelPowerUp(powerUpSize);
            fuelPowerUp.setFillColor(Color::Green); // Fuel power-up color
            fuelPowerUp.setPosition(300 + rand() % (1600 - 300 - powerUpSize), 0); // Restrict within width 300 to 1600
            powerUps.push_back(fuelPowerUp);
        }
    }

    if (score % 100 == 0 && score != 0) {
        // Add a score power-up if not already present
        if (std::none_of(powerUps.begin(), powerUps.end(), [](CircleShape& p) { return p.getFillColor() == Color::Blue; })) {
            CircleShape scorePowerUp(powerUpSize);
            scorePowerUp.setFillColor(Color::Blue); // Boost power-up color
            scorePowerUp.setPosition(300 + rand() % (1600 - 300 - powerUpSize), 0); // Restrict within width 300 to 1600
            powerUps.push_back(scorePowerUp);
        }

        // Add a lives power-up if not already present
        if (std::none_of(powerUps.begin(), powerUps.end(), [](CircleShape& p) { return p.getFillColor() == Color::Red; })) {
            CircleShape livesPowerUp(powerUpSize);
            livesPowerUp.setFillColor(Color::Red); // Lives power-up color
            livesPowerUp.setPosition(300 + rand() % (1600 - 300 - powerUpSize), 0); // Restrict within width 300 to 1600
            powerUps.push_back(livesPowerUp);
        }
    }

    // Draw, move, and check for collisions with the car
    for (auto it = powerUps.begin(); it != powerUps.end();) {
        // Move the power-up downward
        it->move(0, 0.7f); // Adjust speed as needed

        // Remove power-up if it moves out of bounds
        if (it->getPosition().y > trackBounds.y) {
            it = powerUps.erase(it);
            continue;
        }

        window.draw(*it); // Render the power-up

        // Check for collision
        if (carSprite.getGlobalBounds().intersects(it->getGlobalBounds())) {
            // Determine the type of the power-up based on its color
            if (it->getFillColor() == Color::Green) {
                // Fuel Power-Up
                currentFuel = std::min(currentFuel + 20.0f, 100.0f); // Add fuel up to max
                cout << "Collected Power-Up: Fuel Refilled!\n";
            }
            else if (it->getFillColor() == Color::Blue) {
                // Score Power-Up
                score += 50; // Speed boost for 300 frames
                cout << "Collected Power-Up: Speed Boost Activated!\n";
            }
            else if (it->getFillColor() == Color::Red) {
                // Lives Power-Up
                lives++; // Increase lives
                cout << "Collected Power-Up: Extra Life!\n";
            }

            it = powerUps.erase(it); // Remove collected power-up
        }
        else {
            ++it;
        }
    }

    // Handle Speed Boost Duration
    if (speedBoostDuration > 0) {
        --speedBoostDuration;
    }
}








void carSelection(RenderWindow& window, Font font, bool& carSelected, Texture& selectedCarTexture) {
    Vector2f buttonSize(400, 80);
    float cornerRadius = 20.0f;
    Color buttonFillColor = Color(70, 130, 180);
    Color hoverColor = Color(100, 149, 237);

    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!";
        window.close();
    }
    Sound clickSound(clickBuffer);

    // Background texture
    Texture menuTitlebackgroundTexture;
    if (!menuTitlebackgroundTexture.loadFromFile(menuTitleImage)) {
        cout << "Image not found!" << endl;
        window.close();
    }

    Sprite menuTitlebackgroundTrackSprite;
    menuTitlebackgroundTrackSprite.setTexture(menuTitlebackgroundTexture);
    menuTitlebackgroundTrackSprite.setScale(
        float(1980) / menuTitlebackgroundTexture.getSize().x,
        float(1080) / menuTitlebackgroundTexture.getSize().y
    );

    // Helper function to draw rounded buttons
    auto drawRoundedButton = [&](Vector2f position, const string& text, Text& textObject, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;

        // Base rectangle
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);

        // Circles for corners
        CircleShape corner(cornerRadius);
        corner.setFillColor(fillColor);

        // Text
        textObject.setString(text);
        textObject.setCharacterSize(50);
        textObject.setFillColor(Color::White);
        FloatRect textBounds = textObject.getLocalBounds();
        textObject.setPosition(
            position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10
        );

        // Draw components
        window.draw(baseRect);

        // Draw corners
        for (const auto& pos : vector<Vector2f>{
                {position.x, position.y},
                {position.x + buttonSize.x - 2 * cornerRadius, position.y},
                {position.x, position.y + buttonSize.y - 2 * cornerRadius},
                {position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius}
            }) {
            corner.setPosition(pos);
            window.draw(corner);
        }

        window.draw(textObject);
        };

    // Load car images for selection
    vector<Texture> carTextures(6);
    bool texturesLoaded = true;
    for (int i = 0; i < 6; ++i) {
        if (!carTextures[i].loadFromFile("assets/cars/Image" + to_string(i + 1)+".png")) {
            texturesLoaded = false;
            break;
        }
    }
    if (!texturesLoaded) {
        cout << "One or more car images failed to load!" << endl;
        window.close();
    }

    vector<Sprite> carSprites(6);
    for (int i = 0; i < 6; ++i) {
        carSprites[i].setTexture(carTextures[i]);
        carSprites[i].setScale(2.5f, 2.5f);
        carSprites[i].setPosition(135 + i * 300, 1080 / 2 - 120);
    }

    vector<RectangleShape> carBoxes(6);
    for (int i = 0; i < 6; ++i) {
        carBoxes[i].setSize(Vector2f(250, 150));
        carBoxes[i].setFillColor(Color(70, 130, 180, 180));
        carBoxes[i].setOutlineColor(Color::White);
        carBoxes[i].setOutlineThickness(5);
        carBoxes[i].setPosition(100 + i * 300, 1080 / 2 - 120);
    }

    int selectedCarIndex = -1;

    // Function to draw the car selection box with rounded corners
    auto drawCarBox = [&](int i, bool isHovered, bool isSelected) {
        RectangleShape& carBox = carBoxes[i];

        // Highlight on hover
        carBox.setFillColor(isHovered ? Color(135, 206, 250) : Color(70, 130, 180, 180));

        // Outline glow effect when selected
        carBox.setOutlineColor(isSelected ? Color(255, 215, 0) : Color::White);
        carBox.setOutlineThickness(isSelected ? 7 : 5);

        // Draw shadow
        RectangleShape shadow(Vector2f(250, 150));
        shadow.setFillColor(Color(0, 0, 0, 100));
        shadow.setPosition(carBox.getPosition().x + 5, carBox.getPosition().y + 5);
        window.draw(shadow);

        // Draw box and sprite
        window.draw(carBox);
        window.draw(carSprites[i]);
        };

    // Event loop to manage window interactions
    while (window.isOpen()) {
        Event event;
        bool isBackHovered = false;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            Vector2i mousePos = Mouse::getPosition(window);

            // Car selection logic
            for (int i = 0; i < 6; ++i) {
                bool isHovered = carBoxes[i].getGlobalBounds().contains(Vector2f(mousePos));
                if (isHovered) {
                    carSprites[i].setColor(Color::Yellow);
                    if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                        selectedCarIndex = i;
                        selectedCarTexture = carTextures[selectedCarIndex];
                        carSelected = true;
                    }
                }
                else {
                    carSprites[i].setColor(Color::White);
                }

                // Draw car box
                drawCarBox(i, isHovered, selectedCarIndex == i);
            }

            // "Back to Menu" button logic
            FloatRect backBounds(1980 / 2 - buttonSize.x / 2, 1080 - 100, buttonSize.x, buttonSize.y);
            if (backBounds.contains(Vector2f(mousePos))) {
                isBackHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    return;
                }
            }
        }
        Text backOption("Back", font, 50);
        window.clear();
        window.draw(menuTitlebackgroundTrackSprite);
        drawRoundedButton(Vector2f(1980 / 2 - buttonSize.x / 2, 1080 - 100), "Back", backOption, isBackHovered);

        // Display all the car boxes and sprites
        for (int i = 0; i < 6; ++i) {
            drawCarBox(i, false, selectedCarIndex == i);
        }

        window.display();
    }
}





void resetGame(int& lives, int& score, float& currentFuel, vector<RectangleShape>& obstacles, Sprite& carSprite, const int trackWidth, const int trackHeight, bool& carSelected, vector<CircleShape>& powerUps, float& totalTime) {
    // Reset game variables
    lives = 10; // Reset lives
    score = 0; // Reset score
    currentFuel = 100.0f; // Reset fuel
    carSprite.setPosition(trackWidth / 2, trackHeight - 120); // Reset car position
    carSelected = false; // Reset car selection
    totalTime = 0.0f;

    // Reset obstacles - reposition randomly within the screen bounds
    for (auto& obstacle : obstacles) {
        obstacle.setPosition(rand() % trackWidth, rand() % (trackHeight / 2)); // Place obstacles at random positions within upper half of the track
    }

    // Reset power-ups - reposition them randomly
    for (auto& powerUp : powerUps) {
        powerUp.setPosition(rand() % trackWidth, rand() % (trackHeight / 2)); // Place power-ups at random positions
    }

    // Optionally, you can also reset other things, like power-up states or durations, if applicable.
}





void tryAgain(RenderWindow& window, Font& font, bool& isPaused, bool& carSelected, int& lives, int& score, float& currentFuel, vector<RectangleShape>& obstacles, Sprite& carSprite, const int trackWidth, const int trackHeight, Music& backgroundMusic, vector<CircleShape>& powerUps, float totalTime) {
    Vector2f buttonSize(400, 80); // Button Size
    float cornerRadius = 20.0f;   // Radius for rounded corners
    Color buttonFillColor = Color(70, 130, 180); // Steel blue for buttons
    Color hoverColor = Color(100, 149, 237);     // Light steel blue for hover effect

    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!";
        window.close();
    }
    Sound clickSound(clickBuffer);

    // Text for buttons
    Text tryAgainText("Try Again", font, 30);
    Text mainMenuText("Main Menu", font, 30);
    Text exitText("Exit", font, 30);
    Text lastCheckpointText("Last Checkpoint", font, 30);

    // Total Time Text
    ostringstream timeStream;
    timeStream << fixed << setprecision(1) << totalTime << "s";
    Text timeText("Total Time: " + timeStream.str(), font, 40);
    timeText.setFillColor(Color::White);

    // Helper function to draw rounded buttons with hover effects
    auto drawRoundedButton = [&](Vector2f position, const std::string& text, Text& textObject, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;
        float scale = isHovered ? 1.05f : 1.0f; // Slight scaling on hover

        // Base rectangle
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);
        baseRect.setScale(scale, scale);

        // Circles for corners
        CircleShape corner(cornerRadius);
        corner.setFillColor(fillColor);

        // Text
        textObject.setString(text);
        textObject.setCharacterSize(40);
        textObject.setFillColor(Color::White);
        FloatRect textBounds = textObject.getLocalBounds();
        textObject.setPosition(
            position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10
        );

        // Draw components
        window.draw(baseRect);

        // Draw corner circles
        corner.setPosition(position);
        window.draw(corner); // Top-left
        corner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y);
        window.draw(corner); // Top-right
        corner.setPosition(position.x, position.y + buttonSize.y - 2 * cornerRadius);
        window.draw(corner); // Bottom-left
        corner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius);
        window.draw(corner); // Bottom-right

        // Draw text
        window.draw(textObject);
        };

    // Layout positions
    float centerX = window.getSize().x / 2 - buttonSize.x / 2; // Center horizontally
    float startY = 400; // Starting Y position for buttons

    // Game Over and Final Score Texts
    Text gameOverText("Game Over!", font, 80);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2, 80);

    Text finalScoreText("Final Score: " + to_string(score), font, 50);
    finalScoreText.setFillColor(Color::White);
    finalScoreText.setPosition(window.getSize().x / 2 - finalScoreText.getGlobalBounds().width / 2, 180);

    Text lossReasonText("", font, 40);
    lossReasonText.setFillColor(Color::Yellow);

    // Set loss reason text
    if (currentFuel == 0) {
        lossReasonText.setString("Out of Fuel!");
    }
    else if (lives == 0) {
        lossReasonText.setString("No Lives Left!");
    }
    lossReasonText.setPosition(window.getSize().x / 2 - lossReasonText.getGlobalBounds().width / 2, 250);

    timeText.setPosition(window.getSize().x / 2 - timeText.getGlobalBounds().width / 2, 310);

    // Background Overlay
    RectangleShape backgroundOverlay(Vector2f(window.getSize().x, window.getSize().y));
    backgroundOverlay.setFillColor(Color(0, 0, 0, 150));

    while (window.isOpen()) {
        bool isTryAgainHovered = false, isMainMenuHovered = false, isExitHovered = false, isLastCheckpointHovered = false;

        Event pauseEvent;
        while (window.pollEvent(pauseEvent)) {
            if (pauseEvent.type == Event::Closed) window.close();

            Vector2i mousePos = Mouse::getPosition(window);

            // Buttons hover and click detection
            FloatRect lastCheckpointBounds(centerX, startY, buttonSize.x, buttonSize.y);
            FloatRect tryAgainBounds(centerX, startY + 90, buttonSize.x, buttonSize.y);
            FloatRect mainMenuBounds(centerX, startY + 180, buttonSize.x, buttonSize.y);
            FloatRect exitBounds(centerX, startY + 270, buttonSize.x, buttonSize.y);

            if (lastCheckpointBounds.contains(Vector2f(mousePos))) {
                isLastCheckpointHovered = true;
                if (pauseEvent.type == Event::MouseButtonPressed && pauseEvent.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    resetToCheckpoint(lives, score, currentFuel, obstacles, carSprite, trackWidth, trackHeight, carSelected, powerUps);
                    countDown(window, font);
                    return;
                }
            }
            if (tryAgainBounds.contains(Vector2f(mousePos))) {
                isTryAgainHovered = true;
                if (pauseEvent.type == Event::MouseButtonPressed && pauseEvent.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    isPaused = false;
                    resetGame(lives, score, currentFuel, obstacles, carSprite, trackWidth, trackHeight, carSelected, powerUps,totalTime);
                    countDown(window, font);
                    return;
                }
            }
            if (mainMenuBounds.contains(Vector2f(mousePos))) {
                isMainMenuHovered = true;
                if (pauseEvent.type == Event::MouseButtonPressed && pauseEvent.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    resetGame(lives, score, currentFuel, obstacles, carSprite, trackWidth, trackHeight, carSelected, powerUps,totalTime);
                    menuSection(window, font, selectedCarTexture, isPaused, carSelected, backgroundMusic);
                    return;
                }
            }
            if (exitBounds.contains(Vector2f(mousePos))) {
                isExitHovered = true;
                if (pauseEvent.type == Event::MouseButtonPressed && pauseEvent.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    window.close();
                }
            }
        }

        // Draw everything
        window.clear();
        window.draw(backgroundOverlay);

        window.draw(gameOverText);
        window.draw(finalScoreText);
        window.draw(lossReasonText);
        window.draw(timeText);

        drawRoundedButton(Vector2f(centerX, startY), "Last Checkpoint", lastCheckpointText, isLastCheckpointHovered);
        drawRoundedButton(Vector2f(centerX, startY + 90), "Try Again", tryAgainText, isTryAgainHovered);
        drawRoundedButton(Vector2f(centerX, startY + 180), "Main Menu", mainMenuText, isMainMenuHovered);
        drawRoundedButton(Vector2f(centerX, startY + 270), "Exit", exitText, isExitHovered);

        window.display();
    }
}





    






void showMessage(RenderWindow& window, Font& font) {
    // Create gradient background shading for better visibility of the text
    RectangleShape background(Vector2f(window.getSize()));
    background.setFillColor(Color(0, 0, 0, 200));  // Semi-transparent black


    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!";
        window.close();
    }
    Sound clickSound(clickBuffer);


    // Create the message text
    Text message("You Cannot Select a Car while Playing!", font, 30);
    message.setFillColor(Color::Red);  // White color for the text
    FloatRect messageBounds = message.getLocalBounds();
    message.setOrigin(messageBounds.width / 2, messageBounds.height / 2);
    message.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 150); // Center the text
    message.setStyle(Text::Bold);  // Bold text style for emphasis

    // Animation variables for message scaling
    float scale = 0.7f;  // Initial scale
    bool scalingUp = true;

    // Helper function to draw a rounded button with hover and shadow effects
    auto drawCurvedButton = [&](Vector2f position, const std::string& text, Text& textObject, bool isHovered) {
        // Button colors
        Color buttonColor = Color(70, 130, 180);     // Steel blue for default
        Color hoverColor = Color(100, 149, 237);     // Light steel blue for hover
        Color shadowColor = Color(0, 0, 0, 100);     // Semi-transparent shadow

        // Determine fill color based on hover state
        Color fillColor = isHovered ? hoverColor : buttonColor;

        // Button dimensions
        float width = 400, height = 80, cornerRadius = height / 2;

        // Shadow effect (slightly offset)
        RectangleShape shadow(Vector2f(width - 2 * cornerRadius, height));
        shadow.setFillColor(shadowColor);
        shadow.setPosition(position.x + cornerRadius + 5, position.y + 5);

        CircleShape shadowLeftCorner(cornerRadius);
        shadowLeftCorner.setFillColor(shadowColor);
        shadowLeftCorner.setPosition(position.x + 5, position.y + 5);

        CircleShape shadowRightCorner(cornerRadius);
        shadowRightCorner.setFillColor(shadowColor);
        shadowRightCorner.setPosition(position.x + width - 2 * cornerRadius + 5, position.y + 5);

        // Draw shadow
        window.draw(shadow);
        window.draw(shadowLeftCorner);
        window.draw(shadowRightCorner);

        // Create button shape with rounded sides
        RectangleShape button(Vector2f(width - 2 * cornerRadius, height)); // Central rectangle
        button.setFillColor(fillColor);
        button.setPosition(position.x + cornerRadius, position.y);

        CircleShape leftCorner(cornerRadius);
        leftCorner.setFillColor(fillColor);
        leftCorner.setPosition(position.x, position.y);

        CircleShape rightCorner(cornerRadius);
        rightCorner.setFillColor(fillColor);
        rightCorner.setPosition(position.x + width - 2 * cornerRadius, position.y);

        // Draw the button
        window.draw(button);
        window.draw(leftCorner);
        window.draw(rightCorner);

        // Set up button text
        textObject.setString(text);
        textObject.setCharacterSize(40); // Smaller font size for buttons
        textObject.setFillColor(Color::White);
        FloatRect textBounds = textObject.getLocalBounds();
        textObject.setOrigin(textBounds.width / 2, textBounds.height / 2);
        textObject.setPosition(position.x + width / 2, position.y + height / 2 - 5);

        // Draw the text
        window.draw(textObject);
        };

    // "OK" button text
    Text okText("OK", font, 40); // Adjust font size
    bool isOkButtonHovered = false;

    // Main animation and event loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }

            // Check if the OK button is hovered or clicked
            Vector2i mousePos = Mouse::getPosition(window);
            FloatRect okBounds(
                window.getSize().x / 2 - 200,
                window.getSize().y / 2 + 50,
                400, 80
            ); // Button bounds
            if (okBounds.contains(Vector2f(mousePos))) {
                isOkButtonHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    return;  // Close the message
                }
            }
            else {
                isOkButtonHovered = false;
            }
        }

        // Animation: Scale the message text up and down
        if (scalingUp) {
            scale += 0.002f;
            if (scale >= 1.0f) scalingUp = false;
        }
        else {
            scale -= 0.002f;
            if (scale <= 0.9f) scalingUp = true;
        }
        message.setScale(scale, scale);

        // Clear the window
        window.clear();
        window.draw(background);  // Draw the semi-transparent background
        window.draw(message);     // Draw the animated message text

        // Draw the "OK" button
        drawCurvedButton(
            Vector2f(window.getSize().x / 2 - 200, window.getSize().y / 2 + 50), // Adjusted position
            "OK", okText, isOkButtonHovered
        );

        window.display(); // Display all the elements
    }
}












void showChooseCarMessage(RenderWindow& window, Font& font, bool& carSelected) {
    // Semi-transparent black background for better visibility of the text
    RectangleShape background(Vector2f(window.getSize()));
    background.setFillColor(Color(0, 0, 0, 200));

    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!";
        window.close();
    }
    Sound clickSound(clickBuffer);

    // Message text with scaling animation
    Text message("Please select a car from the Car Selection before starting!", font, 30);
    message.setFillColor(Color::White);
    message.setStyle(Text::Bold);
    message.setOrigin(message.getLocalBounds().width / 2, message.getLocalBounds().height / 2);
    message.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 150);

    // Animation variables for message scaling
    float scale = 0.7f;
    bool scalingUp = true;

    // Helper function to draw rounded buttons with hover effect
    auto drawRoundedButton = [&](Vector2f position, const std::string& text, Text& textObject, bool isHovered) {
        Color buttonColor = Color(70, 130, 180);     // Default color
        Color hoverColor = Color(100, 149, 237);     // Hover color
        float width = 400, height = 80, cornerRadius = height / 2;

        // Draw shadow
        Color shadowColor = Color(0, 0, 0, 100);  // Shadow color
        RectangleShape shadow(Vector2f(width - 2 * cornerRadius, height));
        shadow.setFillColor(shadowColor);
        shadow.setPosition(position.x + cornerRadius + 5, position.y + 5);
        CircleShape shadowCorner(cornerRadius);
        shadowCorner.setFillColor(shadowColor);
        shadowCorner.setPosition(position.x + 5, position.y + 5);
        window.draw(shadow);
        window.draw(shadowCorner);
        shadowCorner.setPosition(position.x + width - 2 * cornerRadius + 5, position.y + 5);
        window.draw(shadowCorner);

        // Button
        Color fillColor = isHovered ? hoverColor : buttonColor;
        RectangleShape button(Vector2f(width - 2 * cornerRadius, height));
        button.setFillColor(fillColor);
        button.setPosition(position.x + cornerRadius, position.y);
        CircleShape leftCorner(cornerRadius), rightCorner(cornerRadius);
        leftCorner.setFillColor(fillColor);
        rightCorner.setFillColor(fillColor);
        leftCorner.setPosition(position.x, position.y);
        rightCorner.setPosition(position.x + width - 2 * cornerRadius, position.y);
        window.draw(button);
        window.draw(leftCorner);
        window.draw(rightCorner);

        // Button text
        textObject.setString(text);
        textObject.setCharacterSize(40);
        textObject.setFillColor(Color::White);
        textObject.setOrigin(textObject.getLocalBounds().width / 2, textObject.getLocalBounds().height / 2);
        textObject.setPosition(position.x + width / 2, position.y + height / 2 - 5);
        window.draw(textObject);
        };

    // "OK" button text
    Text okText("OK", font, 40);
    bool isOkButtonHovered = false;

    // Main loop
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return;
            }

            // Check for OK button hover and click
            Vector2i mousePos = Mouse::getPosition(window);
            FloatRect okBounds(window.getSize().x / 2 - 200, window.getSize().y / 2 + 50, 400, 80);
            if (okBounds.contains(Vector2f(mousePos))) {
                isOkButtonHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    return;  // Close the message
                }
            }
            else {
                isOkButtonHovered = false;
            }
        }

        // Scale the message text up and down
        scale += (scalingUp ? 0.002f : -0.002f);
        if (scale >= 1.0f) scalingUp = false;
        if (scale <= 0.9f) scalingUp = true;
        message.setScale(scale, scale);

        // Clear the window and draw elements
        window.clear();
        window.draw(background); // Background overlay
        window.draw(message);    // Message with scaling animation

        // Draw the "OK" button with hover effect
        drawRoundedButton(Vector2f(window.getSize().x / 2 - 200, window.getSize().y / 2 + 50), "OK", okText, isOkButtonHovered);

        window.display(); // Update the window
    }
}







void menuSection(RenderWindow& window, Font& font, Texture& selectedCarTexture, bool ispaused, bool& carSelected, Music& backgroundMusic) {
    Vector2f buttonSize(400, 80);
    float cornerRadius = 20.0f; // Radius for rounded corners
    Color buttonFillColor = Color(70, 130, 180); // Steel blue for buttons
    Color hoverColor = Color(100, 149, 237); // Light steel blue for hover effect

    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!";
        window.close();
    }
    Sound clickSound(clickBuffer);

    Texture menuTitlebackgroundTexture;
    if (!menuTitlebackgroundTexture.loadFromFile(menuTitleImage)) {
        cout << "Image not found!" << endl;
        window.close();
    }

    Sprite menuTitlebackgroundTrackSprite;
    menuTitlebackgroundTrackSprite.setTexture(menuTitlebackgroundTexture);
    menuTitlebackgroundTrackSprite.setScale(
        float(1980) / menuTitlebackgroundTexture.getSize().x,
        float(1080) / menuTitlebackgroundTexture.getSize().y
    );

    // Helper function to draw rounded buttons
    auto drawRoundedButton = [&](Vector2f position, const std::string& text, Text& textObject, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;

        // Base rectangle
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);

        // Circles for corners
        CircleShape corner(cornerRadius);
        corner.setFillColor(fillColor);

        // Text
        textObject.setString(text);
        textObject.setCharacterSize(50);
        textObject.setFillColor(Color::White);
        FloatRect textBounds = textObject.getLocalBounds();
        textObject.setPosition(
            position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10
        );

        // Draw components
        window.draw(baseRect);

        // Draw four corners
        corner.setPosition(position);
        window.draw(corner);
        corner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y);
        window.draw(corner);
        corner.setPosition(position.x, position.y + buttonSize.y - 2 * cornerRadius);
        window.draw(corner);
        corner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius);
        window.draw(corner);

        // Draw text
        window.draw(textObject);
        };

    // Create button text objects
    Text startOption("", font), settingOption("", font), quitOption("", font), carSelectionOption("", font);
    bool isStartHovered = false, isSettingHovered = false, isQuitHovered = false, isCarSelectionHovered = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            Vector2i mousePos = Mouse::getPosition(window);

            // Handle Start Button hover and click
            FloatRect startBounds(130, 200, buttonSize.x, buttonSize.y);
            if (startBounds.contains(Vector2f(mousePos))) {
                isStartHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    if (!carSelected) {
                        clickSound.play();
                        showChooseCarMessage(window, font, carSelected);
                    }
                    else {
                        countDown(window, font);
                        return; // Start the game
                    }
                }
            }
            else {
                isStartHovered = false;
            }

            // Handle Car Selection Button hover and click
            FloatRect carSelectionBounds(130, 300, buttonSize.x, buttonSize.y);
            if (carSelectionBounds.contains(Vector2f(mousePos))) {
                isCarSelectionHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    carSelection(window, font, carSelected, selectedCarTexture);
                }
            }
            else {
                isCarSelectionHovered = false;
            }

            // Handle Settings Button hover and click
            FloatRect settingBounds(130, 400, buttonSize.x, buttonSize.y);
            if (settingBounds.contains(Vector2f(mousePos))) {
                isSettingHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    settingsWindow(window, font, selectedCarTexture, carSelected, ispaused, backgroundMusic);
                }
            }
            else {
                isSettingHovered = false;
            }

            // Handle Quit Button hover and click
            FloatRect quitBounds(130, 500, buttonSize.x, buttonSize.y);
            if (quitBounds.contains(Vector2f(mousePos))) {
                isQuitHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    window.close(); // Close the window
                }
            }
            else {
                isQuitHovered = false;
            }
        }

        // Clear window and draw background
        window.clear();
        window.draw(menuTitlebackgroundTrackSprite);

        // Draw buttons with hover effects
        drawRoundedButton(Vector2f(130, 200), "Start Game", startOption, isStartHovered);
        drawRoundedButton(Vector2f(130, 300), "Car Selection", carSelectionOption, isCarSelectionHovered);
        drawRoundedButton(Vector2f(130, 400), "Settings", settingOption, isSettingHovered);
        drawRoundedButton(Vector2f(130, 500), "Quit Game", quitOption, isQuitHovered);

        window.display();
    }
}




void settingsWindow(RenderWindow& window, Font& font, Texture& selectedCarTexture, bool& carSelected, bool ispaused, Music& backgroundMusic) {
    Vector2f buttonSize(400, 80);
    Vector2f checkboxSize(30, 30);
    float cornerRadius = 20.0f;
    Color buttonFillColor = Color(70, 130, 180); // Steel blue
    Color hoverColor = Color(100, 149, 237); // Light steel blue
    Color checkboxColor = Color(255, 255, 255); // White
    Color checkboxCheckedColor = Color(0, 255, 0); // Green
    SoundBuffer clickBuffer;

    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!" << endl;
        window.close();
    }
    Sound clickSound(clickBuffer);

    // Background texture
    Texture menuTitlebackgroundTexture;
    if (!menuTitlebackgroundTexture.loadFromFile(menuTitleImage)) {
        cout << "Image not found!" << endl;
        window.close();
    }

    Sprite menuTitlebackgroundTrackSprite;
    menuTitlebackgroundTrackSprite.setTexture(menuTitlebackgroundTexture);
    menuTitlebackgroundTrackSprite.setScale(float(1980) / menuTitlebackgroundTexture.getSize().x,
        float(1080) / menuTitlebackgroundTexture.getSize().y);

    // Fullscreen state variable
    bool isFullscreen = false;

    // Helper function to draw rounded buttons
    auto drawRoundedButton = [&](Vector2f position, const string& text, Text& textObject, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);
        CircleShape corner(cornerRadius);
        corner.setFillColor(fillColor);

        // Text
        textObject.setString(text);
        textObject.setCharacterSize(50);
        textObject.setFillColor(Color::White);
        FloatRect textBounds = textObject.getLocalBounds();
        textObject.setPosition(position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10);

        // Draw components
        window.draw(baseRect);

        corner.setPosition(position);
        window.draw(corner);
        corner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y);
        window.draw(corner);
        corner.setPosition(position.x, position.y + buttonSize.y - 2 * cornerRadius);
        window.draw(corner);
        corner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius);
        window.draw(corner);

        window.draw(textObject);
        };

    // Initialize text objects
    Text backOption("", font), musicOption("", font), fullscreenOption("", font), saveOption("", font), loadOption("", font), addProfileOption("", font);

    bool isMusicPaused = true;
    bool isCheckboxChecked = false;

    // Volume control slider
    RectangleShape volumeBar(Vector2f(300, 20));
    volumeBar.setFillColor(Color(169, 169, 169));
    volumeBar.setPosition(1980 / 2 - volumeBar.getSize().x / 2, 1080 - 400);

    CircleShape volumeHandle(10);
    volumeHandle.setFillColor(Color::Green);
    volumeHandle.setPosition(1980 / 2 - volumeBar.getSize().x / 2 + 150, 1080 - 400);

    float volume = 50;
    backgroundMusic.setVolume(volume);

    Text volumeText("Volume", font, 50);
    volumeText.setFillColor(Color::White);
    volumeText.setPosition(1980 / 2 - volumeText.getLocalBounds().width / 2, 1080 - 470);

    GameState gameState;
    gameState.load("settings.txt");  // Load settings

    // Apply loaded settings
    backgroundMusic.setVolume(gameState.volume);
    if (gameState.isMusicPaused) {
        backgroundMusic.pause();
    }
    else {
        backgroundMusic.play();
    }

    isFullscreen = gameState.isFullscreen;

    bool isDraggingVolumeHandle = false;

    while (window.isOpen()) {
        Event event;
        bool isBackHovered = false, isSaveHovered = false, isLoadHovered = false, isAddProfileHovered = false;

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            Vector2i mousePos = Mouse::getPosition(window);

            // "Back to Menu" button logic
            FloatRect backBounds(1980 / 2 - buttonSize.x / 2, 1080 - 150, buttonSize.x, buttonSize.y);
            if (backBounds.contains(Vector2f(mousePos))) {
                isBackHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    return;
                }
            }

            // "Save" button logic
            FloatRect saveBounds(100, 400, buttonSize.x, buttonSize.y);
            if (saveBounds.contains(Vector2f(mousePos))) {
                isSaveHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    // Save the current game settings to the file
                    gameState.isMusicPaused = isMusicPaused;
                    gameState.volume = volume;
                    gameState.isFullscreen = isFullscreen;
                    gameState.save("settings.txt");
                }
            }

            // "Load" button logic
            FloatRect loadBounds(100, 500, buttonSize.x, buttonSize.y);
            if (loadBounds.contains(Vector2f(mousePos))) {
                isLoadHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    // Load the saved settings from the file
                    gameState.load("settings.txt");
                    backgroundMusic.setVolume(gameState.volume);
                    if (gameState.isMusicPaused) {
                        backgroundMusic.pause();
                    }
                    else {
                        backgroundMusic.play();
                    }
                    isFullscreen = gameState.isFullscreen;
                }
            }

            // "Add Profile" button logic
            FloatRect addProfileBounds(100, 600, buttonSize.x, buttonSize.y);
            if (addProfileBounds.contains(Vector2f(mousePos))) {
                isAddProfileHovered = true;
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    // Call the addProfile function
                    addProfile(window, font, clickSound);
                }
            }

            // Music control checkbox logic
            FloatRect checkboxBounds(1980 / 2 - buttonSize.x / 2 - 20, 1080 - 339, checkboxSize.x, checkboxSize.y);
            if (checkboxBounds.contains(Vector2f(mousePos))) {
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    isCheckboxChecked = !isCheckboxChecked;
                    if (isCheckboxChecked) {
                        backgroundMusic.pause();
                        isMusicPaused = true;
                    }
                    else {
                        backgroundMusic.play();
                        isMusicPaused = false;
                    }
                }
            }

            // Fullscreen toggle logic
            FloatRect fullscreenBounds(1980 / 2 - buttonSize.x / 2, 1080 - 270, buttonSize.x, buttonSize.y);
            if (fullscreenBounds.contains(Vector2f(mousePos))) {
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                    clickSound.play();
                    isFullscreen = !isFullscreen;
                    if (isFullscreen) {
                        window.create(VideoMode(1920, 1080), "Racing Game", Style::Fullscreen);
                    }
                    else {
                        window.create(VideoMode(1920, 1080), "Racing Game", Style::Default);
                    }
                }
            }

            // Volume control logic (dragging slider functionality)
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                if (volumeHandle.getGlobalBounds().contains(Vector2f(mousePos))) {
                    isDraggingVolumeHandle = true;
                }
            }

            if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
                isDraggingVolumeHandle = false;
            }

            if (isDraggingVolumeHandle) {
                // Update handle position based on mouse drag
                float newPos = min(max(float(mousePos.x - volumeBar.getPosition().x), 0.f), volumeBar.getSize().x);
                volumeHandle.setPosition(volumeBar.getPosition().x + newPos - volumeHandle.getRadius(), volumeHandle.getPosition().y);

                // Update volume based on handle position
                volume = newPos / volumeBar.getSize().x * 100.f;
                backgroundMusic.setVolume(volume);
            }
        }

        window.clear();
        window.draw(menuTitlebackgroundTrackSprite);

        // Draw buttons and controls
        drawRoundedButton(Vector2f(1980 / 2 - buttonSize.x / 2, 1080 - 150), "Back to Menu", backOption, isBackHovered);
        drawRoundedButton(Vector2f(100, 400), "Save", saveOption, isSaveHovered);
        drawRoundedButton(Vector2f(100, 500), "Load", loadOption, isLoadHovered);
        drawRoundedButton(Vector2f(100, 600), "Add Profile", addProfileOption, isAddProfileHovered);
        drawRoundedButton(Vector2f(1980 / 2 - buttonSize.x / 2, 1080 - 270), "Fullscreen", fullscreenOption, isFullscreen);

        // Draw the volume slider and volume text
        window.draw(volumeBar);
        window.draw(volumeHandle);
        window.draw(volumeText);

        // Draw the music checkbox and label
        if (isCheckboxChecked) {
            RectangleShape checkboxRect(checkboxSize);
            checkboxRect.setFillColor(checkboxCheckedColor);
            checkboxRect.setPosition(1980 / 2 - buttonSize.x / 2 - 20, 1080 - 339);
            window.draw(checkboxRect);
        }
        else {
            RectangleShape checkboxRect(checkboxSize);
            checkboxRect.setOutlineThickness(3);
            checkboxRect.setOutlineColor(checkboxColor);
            checkboxRect.setFillColor(Color::Transparent);
            checkboxRect.setPosition(1980 / 2 - buttonSize.x / 2 - 20, 1080 - 339);
            window.draw(checkboxRect);
        }

        // Draw "Music On/Off" text
        Text musicText(isMusicPaused ? "Music Off" : "Music On", font, 60);
        musicText.setFillColor(Color::Red);
        musicText.setPosition(1980 / 2 - musicText.getLocalBounds().width / 2-10, 1080 - 350);
        window.draw(musicText);

        window.display();
    }
}




int showPauseMenu(RenderWindow& window, Font& font, bool& ispaused, Texture selectedCarTexture, bool carSelected, Music& backgroundMusic) {
    Vector2f buttonSize(400, 80);
    float cornerRadius = 20.0f; // Radius for rounded corners
    Color buttonFillColor = Color(70, 130, 180); // Steel blue for buttons
    Color hoverColor = Color(100, 149, 237); // Light steel blue for hover effect

    SoundBuffer clickBuffer;
    if (!clickBuffer.loadFromFile(clickSound)) {
        cout << "Sound Not Found!" << endl;
        window.close();
    }
    Sound clickSound(clickBuffer);

    // Load pause background texture
    Texture pauseTitleBackgroundTexture;
    if (!pauseTitleBackgroundTexture.loadFromFile(pauseTitleImage)) {
        cout << "Image Not Found!" << endl;
        window.close();
    }

    Sprite pauseTitleBackgroundSprite;
    pauseTitleBackgroundSprite.setTexture(pauseTitleBackgroundTexture);
    pauseTitleBackgroundSprite.setScale(
        float(1980) / pauseTitleBackgroundTexture.getSize().x,
        float(1080) / pauseTitleBackgroundTexture.getSize().y
    );

    // Text for buttons
    Text resumeText("Resume", font, 50);
    Text settingsText("Settings", font, 50);
    Text mainMenuText("Main Menu", font, 50);
    Text exitText("Exit", font, 50);

    // Helper function to draw rounded buttons
    auto drawRoundedButton = [&](Vector2f position, const string& text, Text& textObject, bool isHovered) {
        Color fillColor = isHovered ? hoverColor : buttonFillColor;

        // Base rectangle
        RectangleShape baseRect(Vector2f(buttonSize.x - 2 * cornerRadius, buttonSize.y));
        baseRect.setFillColor(fillColor);
        baseRect.setPosition(position.x + cornerRadius, position.y);

        // Circles for corners
        CircleShape topLeftCorner(cornerRadius);
        topLeftCorner.setFillColor(fillColor);
        topLeftCorner.setPosition(position);

        CircleShape topRightCorner(cornerRadius);
        topRightCorner.setFillColor(fillColor);
        topRightCorner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y);

        CircleShape bottomLeftCorner(cornerRadius);
        bottomLeftCorner.setFillColor(fillColor);
        bottomLeftCorner.setPosition(position.x, position.y + buttonSize.y - 2 * cornerRadius);

        CircleShape bottomRightCorner(cornerRadius);
        bottomRightCorner.setFillColor(fillColor);
        bottomRightCorner.setPosition(position.x + buttonSize.x - 2 * cornerRadius, position.y + buttonSize.y - 2 * cornerRadius);

        // Draw corners
        window.draw(topLeftCorner);
        window.draw(topRightCorner);
        window.draw(bottomLeftCorner);
        window.draw(bottomRightCorner);

        // Draw rectangle
        window.draw(baseRect);

        // Text
        textObject.setString(text);
        textObject.setFillColor(Color::White);
        FloatRect textBounds = textObject.getLocalBounds();
        textObject.setPosition(
            position.x + (buttonSize.x - textBounds.width) / 2,
            position.y + (buttonSize.y - textBounds.height) / 2 - 10
        );

        // Draw text
        window.draw(textObject);
        };

    // Helper function to check if mouse is hovering over a button
    auto isMouseOverButton = [&](FloatRect buttonBounds, Vector2i mousePos) -> bool {
        return buttonBounds.contains(Vector2f(mousePos));
        };

    // Pause menu logic
    while (window.isOpen()) {
        bool isResumeHovered = false, isSettingsHovered = false, isMainMenuHovered = false, isExitHovered = false;
        Event pauseEvent;

        while (window.pollEvent(pauseEvent)) {
            if (pauseEvent.type == Event::Closed) {
                window.close();  // Close the window
            }

            Vector2i mousePos = Mouse::getPosition(window); // Get mouse position in window coordinates

            // Check if mouse is over any button
            isResumeHovered = isMouseOverButton(FloatRect(780, 350, buttonSize.x, buttonSize.y), mousePos);
            isSettingsHovered = isMouseOverButton(FloatRect(780, 450, buttonSize.x, buttonSize.y), mousePos);
            isMainMenuHovered = isMouseOverButton(FloatRect(780, 550, buttonSize.x, buttonSize.y), mousePos);
            isExitHovered = isMouseOverButton(FloatRect(780, 650, buttonSize.x, buttonSize.y), mousePos);

            // Handle button clicks
            if (pauseEvent.type == Event::MouseButtonPressed && pauseEvent.mouseButton.button == Mouse::Left) {
                if (isResumeHovered) {
                    clickSound.play();
                    ispaused = false;
                    return ispaused; // Resume the game
                }
                if (isSettingsHovered) {
                    clickSound.play();
                    settingsWindow(window, font, selectedCarTexture, carSelected, ispaused, backgroundMusic);
                    continue;
                }
                if (isMainMenuHovered) {
                    clickSound.play();
                    menuSection(window, font, selectedCarTexture, ispaused, carSelected, backgroundMusic);
                    continue;
                }
                if (isExitHovered) {
                    clickSound.play();
                    window.close(); // Exit the game
                }
            }
        }

        window.clear(); // Clear the screen
        window.draw(pauseTitleBackgroundSprite);

        // Draw buttons with hover effects
        drawRoundedButton(Vector2f(780, 350), "Resume", resumeText, isResumeHovered);
        drawRoundedButton(Vector2f(780, 450), "Settings", settingsText, isSettingsHovered);
        drawRoundedButton(Vector2f(780, 550), "Main Menu", mainMenuText, isMainMenuHovered);
        drawRoundedButton(Vector2f(780, 650), "Exit", exitText, isExitHovered);

        window.display(); // Update the window
    }

    return 0; // Default return to indicate no action if the loop ends
}









int main() {
    // Create game window
    RenderWindow window(VideoMode(trackWidth, trackHeight), "Need For Speed: Ignition");
    bool carSelected = false;
    bool isPaused = false;
    
   

    // Load Font
    Font font;
    if (!font.loadFromFile(fontPath)) {
        cout << "Failed to load font from: " << fontPath << endl;
        return -1;
    }

    Texture selectedCarTexture;
    Music bgMusic;
    if (!bgMusic.openFromFile(backgroundMusicPath)) {
        cout << "Failed to load background music from: " << backgroundMusicPath << endl;
        return -1;
    }
    menuSection(window, font, selectedCarTexture, isPaused, carSelected, bgMusic);

    // Load Background Music
    bgMusic.setLoop(true);
    bgMusic.play();

    // Load Collision Sound Effect
    SoundBuffer collisionBuffer;
    if (!collisionBuffer.loadFromFile(collisionSoundPath)) {
        cout << "Failed to load collision sound from: " << collisionSoundPath << endl;
        return -1;
    }
    Sound collisionSound(collisionBuffer);

    // HUD
    Text scoreText("Score: 0", font, 35);
    scoreText.setPosition(20, 20);
    scoreText.setFillColor(Color::White);

    Text livesText("Lives: 3", font, 35);
    livesText.setPosition(20, 80);
    livesText.setFillColor(Color::White);

    Text timeText("Time: 0.0s", font, 35);
    timeText.setPosition(1640, 76);
    timeText.setFillColor(Color::White);

    // Array of level texts
    Text levels[10];
    for (int i = 0; i < 10; ++i) {
        levels[i].setFont(font);
        levels[i].setCharacterSize(50);
        levels[i].setPosition(30, 150);
        levels[i].setFillColor(Color::White);
        levels[i].setString("Level: " + std::to_string(i + 1));
    }

    Sprite carSprite;
    carSprite.setTexture(selectedCarTexture);
    carSprite.setScale(1.0f, 1.0f);
    carSprite.setPosition(trackWidth / 2, trackHeight - 120);

    // Fuel Bar Background
    RectangleShape fuelBarBackground(Vector2f(30, 200));
    fuelBarBackground.setFillColor(Color::Black);
    fuelBarBackground.setPosition(1800, 100);

    // Fuel Bar
    RectangleShape fuelBar(Vector2f(30, 200));
    fuelBar.setFillColor(Color::Green);
    fuelBar.setPosition(1800, 100);

    // Load the background image
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile(backgroundTrack)) {
        cout << "Image not found!" << endl;
        return -1;
    }

    Sprite backgroundTrack1Sprite;
    backgroundTrack1Sprite.setTexture(backgroundTexture);
    backgroundTrack1Sprite.setScale(
        float(trackWidth) / backgroundTexture.getSize().x,
        float(trackHeight) / backgroundTexture.getSize().y
    );

    // Obstacles
    vector<RectangleShape> obstacles;
    generateObstacles(obstacles, 25, 40, Vector2f(trackWidth, trackHeight));

    // Power-Ups
    vector<CircleShape> powerUps;
    int powerUpSize = 20; // Size of power-up
    Vector2f trackBounds(trackWidth, trackHeight);

    // Game variables
    int score = 0;
    int lives = 10;
    float carSpeed = 2.2f;
    int speedBoostDuration = 0;
    Clock clock;

    // Fuel Variables
    float maxFuel = 100.0f;
    float currentFuel = maxFuel;
    float fuelConsumptionRate = 0.05f;
    Clock gameClock;
    Clock updateClock;
  

    // Load previous playtime
    float previousPlayTime = 0.0f;
    // Game loop
    while (window.isOpen()) {
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            isPaused = true;
            bgMusic.pause();
            isPaused = showPauseMenu(window, font, isPaused, selectedCarTexture, carSelected, bgMusic);
            continue;
        }

        if (!isPaused) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
            }
            // Handle player movement
            float dt = clock.restart().asSeconds();
            float elapsedTime = gameClock.getElapsedTime().asSeconds();
            // Update time text
            ostringstream oss;
            oss << "Time: " << fixed << setprecision(1) << elapsedTime + previousPlayTime << "s";
            timeText.setString(oss.str());

            // Moving the car
            if (Keyboard::isKeyPressed(Keyboard::Left) && carSprite.getPosition().x > 240) {
                carSprite.move(-carSpeed, 0);
            }
            if (Keyboard::isKeyPressed(Keyboard::Right) && carSprite.getPosition().x + carSprite.getGlobalBounds().width < 1680) {
                carSprite.move(carSpeed, 0);
            }
            if (Keyboard::isKeyPressed(Keyboard::Up) && carSprite.getPosition().y > 0) {
                carSprite.move(0, -carSpeed);
            }
            if (Keyboard::isKeyPressed(Keyboard::Down) && carSprite.getPosition().y + carSprite.getGlobalBounds().height < 1030) {
                carSprite.move(0, carSpeed);
            }

            // Control fuel
            if (Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Down) || Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right)) {
                currentFuel -= fuelConsumptionRate;
                if (currentFuel <= 0) {
                    currentFuel = 0;
                    bgMusic.stop();
                    tryAgain(window, font, isPaused, carSelected, lives, score, currentFuel, obstacles, carSprite, trackWidth, trackHeight, bgMusic,powerUps, elapsedTime + previousPlayTime);
                    bgMusic.play();
                }
            }
            checkForCheckpoint(score);
            // Calculate fuel bar height and adjust position
            float fuelBarHeight = (currentFuel / maxFuel) * 200;
            fuelBar.setSize(Vector2f(30, fuelBarHeight));
            fuelBar.setPosition(1800, 100 + (200 - fuelBarHeight));

            // Move obstacles only when the game is not paused
            if (!isPaused) {
                for (auto& obstacle : obstacles) {
                    obstacle.move(0, 0.5f * carSpeed);
                    if (obstacle.getPosition().y > trackHeight) {
                        float randomX = 300 + static_cast<float>(rand() % (1600 - 300 - static_cast<int>(obstacle.getSize().x))); // Corrected type handling
                        obstacle.setPosition(randomX, 0);
                        score++;
                    }

                    // Check for collision with the car
                    if (carSprite.getGlobalBounds().intersects(obstacle.getGlobalBounds())) {
                        lives--;
                        collisionSound.play();
                        obstacle.setPosition(rand() % trackWidth, 0);
                        if (lives <= 0) {
                            bgMusic.stop();
                            savePlayTime(elapsedTime + previousPlayTime);
                            tryAgain(window, font, isPaused, carSelected, lives, score, currentFuel, obstacles, carSprite, trackWidth, trackHeight, bgMusic,powerUps, elapsedTime + previousPlayTime);
                            bgMusic.play();
                        }
                    }
                }
            }

            // Manage power-ups (draw, generate, and handle collision)
            managePowerUps(window, carSprite, powerUps, currentFuel, score, speedBoostDuration, lives, powerUpSize, trackBounds);

            // Update HUD
            scoreText.setString("Score: " + to_string(score));
            livesText.setString("Lives: " + to_string(lives));

            // Render everything
            int levelIndex = score / 100;
            if (levelIndex > 9) {
                levelIndex = 9;
            }

            // Set the speed cap for the car
            carSpeed = (score >= 1000) ? 12.0f : 2.0f + (score / 100);

            window.clear();
            window.draw(backgroundTrack1Sprite);
            window.draw(carSprite);

            for (const auto& obstacle : obstacles) {
                window.draw(obstacle);
            }

            managePowerUps(window, carSprite, powerUps, currentFuel, score, speedBoostDuration, lives, powerUpSize, trackBounds);
            window.draw(fuelBarBackground);
            window.draw(fuelBar);
            window.draw(scoreText);
            window.draw(livesText);
            window.draw(timeText);

            if (levelIndex >= 0 && levelIndex < 10) {
                window.draw(levels[levelIndex]);
            }

            window.display();
        }
    }
    return 0;
}









