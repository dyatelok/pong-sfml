#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

using namespace std;
using namespace sf;

Font font;

class Game{
public:
            Game();
    void    run();
private:
    void    processEvents();
    void    update(Time deltTime);
    void    render();
    void    handlePlayerInput(Keyboard::Key , bool IsPressed);
private:
    RenderWindow Window;
    Time timePerFrame;

private:
    RectangleShape FirstRacket;
    RectangleShape SecondRacket;
    CircleShape Ball;
    bool IsFirstRacketMovingUp;
    bool IsFirstRacketMovingDown;
    bool IsSecondRacketMovingUp;
    bool IsSecondRacketMovingDown;
    Vector2f BallDirection;
    Text score;
    int score1;
    int score2;
    bool IsGameFinished;
};

Game::Game()
:Window(VideoMode(800,600),"SFML_TEST")
{
    IsGameFinished = 0;

    timePerFrame = seconds(1.f / 60.f);

    score1 = 0;
    score2 = 0;

    score.setFont(font);
    score.setCharacterSize(24);
    score.setFillColor(Color::White);
    char ch1 = score1 + '0';
    char ch2 = score2 + '0';

    string s = {ch1,' ',':',' ',ch2};
    score.setString(s);
    score.setOrigin(score.getLocalBounds().width / 2.f , score.getLocalBounds().height / 2.f );
    score.setPosition(Vector2f(400, 40));
    //score.setString((char)(score1 + (int)'0') + " : " + (char)(score2 + (int)'0'));

    BallDirection = Vector2f(sqrt(3.f) / 2.f , (1.f / 2.f));
    int r1 = rand() % 2;
    int r2 = rand() % 2;
    if (r1 == 0) r1 = -1;
    if (r2 == 0) r2 = -1;
    BallDirection.x *= r1;
    BallDirection.y *= r2;

    Ball.setRadius(10);
    Ball.setPosition(400-10,300-10);
    Ball.setFillColor(Color::White);

    FirstRacket.setSize(Vector2f(20,80));
    FirstRacket.setPosition(20,300-40);
    FirstRacket.setFillColor(Color::White);

    SecondRacket.setSize(Vector2f(20,80));
    SecondRacket.setPosition(760,300-40);
    SecondRacket.setFillColor(Color::White);
}

void Game::run()
{
    Clock clock;
    Time timeSinceLastUpdate = Time::Zero;

    while (Window.isOpen())
    {
        processEvents();
        timeSinceLastUpdate += clock.restart();
        while (timeSinceLastUpdate > timePerFrame)
        {
            timeSinceLastUpdate -= timePerFrame;
            processEvents();
            update(timePerFrame);
        }
        render();
    }
}

void Game::processEvents()
{
    Event event;
    while (Window.pollEvent(event))
    {
        switch(event.type)
        {
            case Event::Closed: {
                Window.close();
                break;
            }
            case Event::KeyPressed:
            {
                handlePlayerInput(event.key.code , 1);
                break;
            }
            case Event::KeyReleased:
            {
                handlePlayerInput(event.key.code , 0);
                break;
            }
        }
    }
}

void Game::handlePlayerInput(Keyboard::Key key ,bool IsPressed)
{
    switch (key)
    {
        case Keyboard::Key::W: {
            IsFirstRacketMovingUp = IsPressed;
            break;
        }
        case Keyboard::Key::S: {
            IsFirstRacketMovingDown = IsPressed;
            break;
        }
        case Keyboard::Key::Up: {
            IsSecondRacketMovingUp = IsPressed;
            break;
        }
        case Keyboard::Key::Down: {
            IsSecondRacketMovingDown = IsPressed;
            break;
        }
    }
}

void Game::update(Time deltaTime)
{
    if (IsGameFinished == 0) {
        Vector2f FirstMove = Vector2f(0, 0);
        Vector2f SecondMove = Vector2f(0, 0);

        if (FirstRacket.getPosition().y < 20) {
            IsFirstRacketMovingUp = 0;
            FirstRacket.setPosition(FirstRacket.getPosition().x, 20);
        }
        if (500 < FirstRacket.getPosition().y) {
            IsFirstRacketMovingDown = 0;
            FirstRacket.setPosition(FirstRacket.getPosition().x, 500);
        }

        if (SecondRacket.getPosition().y < 20) {
            IsSecondRacketMovingUp = 0;
            SecondRacket.setPosition(SecondRacket.getPosition().x, 20);
        }
        if (500 < SecondRacket.getPosition().y) {
            IsSecondRacketMovingDown = 0;
            SecondRacket.setPosition(SecondRacket.getPosition().x, 500);
        }

        if (IsFirstRacketMovingUp) { FirstMove.y -= 1; }
        if (IsFirstRacketMovingDown) { FirstMove.y += 1; }
        if (IsSecondRacketMovingUp) { SecondMove.y -= 1; }
        if (IsSecondRacketMovingDown) { SecondMove.y += 1; }

        if (Ball.getPosition().x < 40)
            if (FirstRacket.getPosition().y < Ball.getPosition().y - 10.f &&
                Ball.getPosition().y - 10.f < FirstRacket.getPosition().y + 80.f) {
                Ball.setPosition(40, Ball.getPosition().y);
                BallDirection.x = -BallDirection.x;
            }
            else {
                score2 += 1;
                string s = " ";
                if (score2 > 9) {
                    IsGameFinished = 1;
                    s = "Second Player Won!";
                    score.setString(s);
                    score.setOrigin(score.getLocalBounds().width / 2.f, score.getLocalBounds().height / 2.f);
                    score.setPosition(Vector2f(400, 40));
                } else {
                    char ch1 = '0' + score1;
                    char ch2 = '0' + score2;
                    s = {ch1, ' ', ':', ' ', ch2};
                    score.setString(s);
                    score.setOrigin(score.getLocalBounds().width / 2.f, score.getLocalBounds().height / 2.f);
                    score.setPosition(Vector2f(400, 40));
                }
                score.setString(s);
                Ball.setPosition(Vector2f(400, 300));
                int r1 = rand() % 2;
                int r2 = rand() % 2;
                if (r1 == 0) r1 = -1;
                if (r2 == 0) r2 = -1;
                BallDirection.x *= r1;
                BallDirection.y *= r2;
            }


        if (Ball.getPosition().x > 740)
            if (SecondRacket.getPosition().y < Ball.getPosition().y - 10.f &&
                Ball.getPosition().y - 10.f < SecondRacket.getPosition().y + 80.f) {
                Ball.setPosition(740, Ball.getPosition().y);
                BallDirection.x = -BallDirection.x;
            }
            else {
                score1 += 1;
                string s = " ";
                if (score1 > 9) {
                    IsGameFinished = 1;
                    s = "First Player Won!";
                    score.setString(s);
                    score.setOrigin(score.getLocalBounds().width / 2.f, score.getLocalBounds().height / 2.f);
                    score.setPosition(Vector2f(400, 40));
                } else {
                    char ch1 = '0' + score1;
                    char ch2 = '0' + score2;
                    s = {ch1, ' ', ':', ' ', ch2};
                    score.setString(s);
                    score.setOrigin(score.getLocalBounds().width / 2.f, score.getLocalBounds().height / 2.f);
                    score.setPosition(Vector2f(400, 40));
                }
                score.setString(s);
                Ball.setPosition(Vector2f(400, 300));
                int r1 = rand() % 2;
                int r2 = rand() % 2;
                if (r1 == 0) r1 = -1;
                if (r2 == 0) r2 = -1;
                BallDirection.x *= r1;
                BallDirection.y *= r2;
            }

        if (Ball.getPosition().y < 0) {
            Ball.setPosition(Ball.getPosition().x, 0);
            BallDirection.y = -BallDirection.y;
        }
        if (Ball.getPosition().y > 580) {
            Ball.setPosition(Ball.getPosition().x, 580);
            BallDirection.y = -BallDirection.y;
        }

        Ball.move(750.f * BallDirection * deltaTime.asSeconds());
        FirstRacket.move(200.f * FirstMove * deltaTime.asSeconds());
        SecondRacket.move(200.f * SecondMove * deltaTime.asSeconds());
    } else
    {
        if (Ball.getPosition().x < 40) {
            Ball.setPosition(40 , Ball.getPosition().y);
            BallDirection.x = -BallDirection.x;
        }
        if (700 < Ball.getPosition().x) {
            Ball.setPosition(700 , Ball.getPosition().y);
            BallDirection.x = -BallDirection.x;
        }
        if (Ball.getPosition().y < 0) {
            Ball.setPosition(Ball.getPosition().x, 0);
            BallDirection.y = -BallDirection.y;
        }
        if (Ball.getPosition().y > 580) {
            Ball.setPosition(Ball.getPosition().x, 580);
            BallDirection.y = -BallDirection.y;
        }

        Ball.move(750.f * BallDirection * deltaTime.asSeconds());
    }
}

void Game::render()
{
    Window.clear();
    Window.draw(Ball);
    Window.draw(FirstRacket);
    Window.draw(SecondRacket);
    Window.draw(score);
    Window.display();
}

int main() {
    srand( time(0) );
    if (!font.loadFromFile("../ArialBlack.ttf")) {cout << "CAN'T LOAD FONT";}
    Game game;
    game.run();
    return 0;
}
