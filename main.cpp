#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <time.h>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <chrono>

enum State{Start,Running};
enum Direction{Stopped,Left,Right,Up,Down};
std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd()); // seed the generator
std::uniform_int_distribution<> distr(1, 4);

//function to check if sprite will be inside of wall
bool checkConflict(Direction currDir, sf::Sprite Pac, std::vector<sf::RectangleShape> bounds)
{
    sf::Vector2f currPos = Pac.getPosition();
    float offsetX = 0;
    float offsetY = 0;
    if (currDir == Left)
    {
        offsetX = -1;
    }
    else if (currDir == Right)
    {
        offsetX = 1;
    }
    else if (currDir == Up)
    {
        offsetY = -1;
    }
    else if (currDir == Down)
    {
        offsetY = 1;
    }

    float newCorners[4][2] = {{currPos.x+5+offsetX, currPos.y+5+offsetY},
                              {currPos.x + 31+offsetX, currPos.y+5+offsetY},
                              {currPos.x+5+offsetX, currPos.y + 29+offsetY},
                              {currPos.x + 31+offsetX, currPos.y + 29+offsetY}};


    bool pacWallCollision = 0;
    for (int corner = 0; corner < 4; corner++) {
        for (int i=0; i<bounds.size();i++)
        {
            if (bounds[i].getGlobalBounds().contains(newCorners[corner][0],newCorners[corner][1])) {
                pacWallCollision = 1;
                break;
            }
        }
    }
    return pacWallCollision;
}

//modified function for ghosts
bool checkConflictGhost(Direction currDir, sf::Sprite Pac, std::vector<sf::RectangleShape> bounds)
{
    sf::Vector2f currPos = Pac.getPosition();
    float offsetX = 0;
    float offsetY = 0;
    if (currDir == Left)
    {
        offsetX = -1;
    } else if (currDir == Right) {
        offsetX = 1;
    } else if (currDir == Up) {
        offsetY = -1;
    } else if (currDir == Down) {
        offsetY = 1;
    }

    float newCorners[4][2] = {{currPos.x+5+offsetX, currPos.y+5+offsetY},
                              {currPos.x + 31+offsetX, currPos.y+5+offsetY},
                              {currPos.x+5+offsetX, currPos.y + 29+offsetY},
                              {currPos.x + 31+offsetX, currPos.y + 29+offsetY}};


    bool pacWallCollision = 0;
    for (int corner = 0; corner < 4; corner++) {
        for (int i=0; i<bounds.size();i++)
        {
            if (bounds[i].getGlobalBounds().contains(newCorners[corner][0],newCorners[corner][1])) {
                pacWallCollision = 1;
                break;
            }
        }
    }
    return pacWallCollision;
}

//function to change directions based on arrow key inputs
Direction updateDir(Direction curr, sf::Sprite Pac, std::vector<sf::RectangleShape> bounds)
{
    Direction newDir = curr;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        newDir = Left;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        newDir = Right;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        newDir = Up;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        newDir = Down;
    }
    if (checkConflict(newDir,Pac,bounds))
    {
        return curr;
    }
    return newDir;
}
int main()
{
    //font setup
    sf::Font font;
    font.loadFromFile("Freshman.ttf");

    // create window and set size
    sf::RenderWindow window(sf::VideoMode(641, 728), "PAC-MAN");

    //texture declarations
    sf::Texture textureBackground;
    sf::Texture texturePac;
    sf::Texture textureRGhost;
    sf::Texture texturePGhost;
    sf::Texture textureOGhost;
    sf::Texture textureBGhost;

    //load in background image
    textureBackground.loadFromFile("pac-man.bmp");

    //load in pacman
    texturePac.loadFromFile("pacman.bmp");

    textureRGhost.loadFromFile("red_ghosts.bmp");//load in red ghost
    texturePGhost.loadFromFile("pink_ghost.bmp");//load in pink ghost
    textureOGhost.loadFromFile("orange_ghost.bmp");//load in orange ghost
    textureBGhost.loadFromFile("blue_ghost.bmp");//load in blue ghost

    //Sprite declarations
    sf::Sprite spriteBackground;
    sf::Sprite Pac;
    sf::Sprite RGhost;
    sf::Sprite PGhost;
    sf::Sprite OGhost;
    sf::Sprite BGhost;

    //Splash screen setup
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 0);

    //Pacman sprite setup
    Pac.setTexture(texturePac);
    Pac.setPosition(295, 485);

    //ghosts initial setup
    RGhost.setTexture(textureRGhost);
    PGhost.setTexture(texturePGhost);
    OGhost.setTexture(textureOGhost);
    BGhost.setTexture(textureBGhost);
    RGhost.setPosition(305, 250);
    PGhost.setPosition(305, 250);
    OGhost.setPosition(305, 250);
    BGhost.setPosition(305, 250);

    std::vector<sf::Sprite>Ghosts {RGhost,PGhost,OGhost,BGhost};//populate ghost vector


    sf::Text startText;
    startText.setFont(font);
    startText.setString("PRESS ENTER TO START");
    startText.setCharacterSize(40);
    startText.setPosition(70,300);
    startText.setFillColor(sf::Color::White);


    //create outline
    sf::RectangleShape outline;
    outline.setSize(sf::Vector2f(550, 612));
    outline.setOutlineColor(sf::Color::Red);
    outline.setFillColor( sf::Color::Transparent);
    outline.setOutlineThickness(5);
    outline.setPosition(45, 38);

    sf::RectangleShape leftBump1;
    leftBump1.setSize(sf::Vector2f(99, 83));
    leftBump1.setOutlineColor(sf::Color::Red);
    leftBump1.setFillColor( sf::Color::Transparent);
    leftBump1.setOutlineThickness(5);
    leftBump1.setPosition(46, 221);

    sf::RectangleShape leftBump2;
    leftBump2.setSize(sf::Vector2f(99, 83));
    leftBump2.setOutlineColor(sf::Color::Red);
    leftBump2.setFillColor( sf::Color::Transparent);
    leftBump2.setOutlineThickness(5);
    leftBump2.setPosition(46, 345);

    sf::RectangleShape rightBump1;
    rightBump1.setSize(sf::Vector2f(99, 83));
    rightBump1.setOutlineColor(sf::Color::Red);
    rightBump1.setFillColor( sf::Color::Transparent);
    rightBump1.setOutlineThickness(5);
    rightBump1.setPosition(497, 221);

    sf::RectangleShape rightBump2;
    rightBump2.setSize(sf::Vector2f(99, 83));
    rightBump2.setOutlineColor(sf::Color::Red);
    rightBump2.setFillColor( sf::Color::Transparent);
    rightBump2.setOutlineThickness(5);
    rightBump2.setPosition(497, 345);

    sf::RectangleShape topBump;
    topBump.setSize(sf::Vector2f(22, 80));
    topBump.setOutlineColor(sf::Color::Red);
    topBump.setFillColor( sf::Color::Transparent);
    topBump.setOutlineThickness(5);
    topBump.setPosition(308, 39);

    sf::RectangleShape lBump1;
    lBump1.setSize(sf::Vector2f(37, 21));
    lBump1.setOutlineColor(sf::Color::Red);
    lBump1.setFillColor( sf::Color::Transparent);
    lBump1.setOutlineThickness(5);
    lBump1.setPosition(45, 528);

    sf::RectangleShape lBump2;
    lBump2.setSize(sf::Vector2f(37, 21));
    lBump2.setOutlineColor(sf::Color::Red);
    lBump2.setFillColor( sf::Color::Transparent);
    lBump2.setOutlineThickness(5);
    lBump2.setPosition(558, 528);

    //create map
    sf::RectangleShape topRec1;
    topRec1.setSize(sf::Vector2f(62, 40));
    topRec1.setOutlineColor(sf::Color::Red);
    topRec1.setFillColor( sf::Color::Transparent);
    topRec1.setOutlineThickness(5);
    topRec1.setPosition(84, 79);

    sf::RectangleShape topRec2;
    topRec2.setSize(sf::Vector2f(80, 40));
    topRec2.setOutlineColor(sf::Color::Red);
    topRec2.setFillColor( sf::Color::Transparent);
    topRec2.setOutlineThickness(5);
    topRec2.setPosition(187, 80);

    sf::RectangleShape topRec3;
    topRec3.setSize(sf::Vector2f(80, 40));
    topRec3.setOutlineColor(sf::Color::Red);
    topRec3.setFillColor( sf::Color::Transparent);
    topRec3.setOutlineThickness(5);
    topRec3.setPosition(370, 79);

    sf::RectangleShape topRec4;
    topRec4.setSize(sf::Vector2f(60, 40));
    topRec4.setOutlineColor(sf::Color::Red);
    topRec4.setFillColor( sf::Color::Transparent);
    topRec4.setOutlineThickness(5);
    topRec4.setPosition(493, 80);

    sf::RectangleShape topRec5;
    topRec5.setSize(sf::Vector2f(62, 20));
    topRec5.setOutlineColor(sf::Color::Red);
    topRec5.setFillColor( sf::Color::Transparent);
    topRec5.setOutlineThickness(5);
    topRec5.setPosition(84, 160);

    sf::RectangleShape topRec6;
    topRec6.setSize(sf::Vector2f(60, 20));
    topRec6.setOutlineColor(sf::Color::Red);
    topRec6.setFillColor( sf::Color::Transparent);
    topRec6.setOutlineThickness(5);
    topRec6.setPosition(493, 160);

    sf::RectangleShape leftTopTet1;
    leftTopTet1.setSize(sf::Vector2f(20, 140));
    leftTopTet1.setOutlineColor(sf::Color::Red);
    leftTopTet1.setFillColor( sf::Color::Transparent);
    leftTopTet1.setOutlineThickness(5);
    leftTopTet1.setPosition(185, 160);

    sf::RectangleShape leftTopTet2;
    leftTopTet2.setSize(sf::Vector2f(80, 20));
    leftTopTet2.setOutlineColor(sf::Color::Red);
    leftTopTet2.setFillColor( sf::Color::Transparent);
    leftTopTet2.setOutlineThickness(5);
    leftTopTet2.setPosition(185, 220);

    sf::RectangleShape TopTet1;
    TopTet1.setSize(sf::Vector2f(140, 20));
    TopTet1.setOutlineColor(sf::Color::Red);
    TopTet1.setFillColor( sf::Color::Transparent);
    TopTet1.setOutlineThickness(5);
    TopTet1.setPosition(247, 160);

    sf::RectangleShape TopTet2;
    TopTet2.setSize(sf::Vector2f(20, 80));
    TopTet2.setOutlineColor(sf::Color::Red);
    TopTet2.setFillColor( sf::Color::Transparent);
    TopTet2.setOutlineThickness(5);
    TopTet2.setPosition(308, 160);

    sf::RectangleShape rightTopTet1;
    rightTopTet1.setSize(sf::Vector2f(20, 140));
    rightTopTet1.setOutlineColor(sf::Color::Red);
    rightTopTet1.setFillColor( sf::Color::Transparent);
    rightTopTet1.setOutlineThickness(5);
    rightTopTet1.setPosition(431, 160);

    sf::RectangleShape rightTopTet2;
    rightTopTet2.setSize(sf::Vector2f(80, 20));
    rightTopTet2.setOutlineColor(sf::Color::Red);
    rightTopTet2.setFillColor( sf::Color::Transparent);
    rightTopTet2.setOutlineThickness(5);
    rightTopTet2.setPosition(370, 220);

    sf::RectangleShape box;
    box.setSize(sf::Vector2f(140, 80));
    box.setOutlineColor(sf::Color::Red);
    box.setFillColor( sf::Color::Transparent);
    box.setOutlineThickness(5);
    box.setPosition(245, 285);

    sf::RectangleShape mRec1;
    mRec1.setSize(sf::Vector2f(20, 80));
    mRec1.setOutlineColor(sf::Color::Red);
    mRec1.setFillColor( sf::Color::Transparent);
    mRec1.setOutlineThickness(5);
    mRec1.setPosition(185, 345);

    sf::RectangleShape mRec2;
    mRec2.setSize(sf::Vector2f(20, 80));
    mRec2.setOutlineColor(sf::Color::Red);
    mRec2.setFillColor( sf::Color::Transparent);
    mRec2.setOutlineThickness(5);
    mRec2.setPosition(431, 345);

    sf::RectangleShape mTet1;
    mTet1.setSize(sf::Vector2f(140, 20));
    mTet1.setOutlineColor(sf::Color::Red);
    mTet1.setFillColor( sf::Color::Transparent);
    mTet1.setOutlineThickness(5);
    mTet1.setPosition(247, 405);

    sf::RectangleShape mTet2;
    mTet2.setSize(sf::Vector2f(20, 80));
    mTet2.setOutlineColor(sf::Color::Red);
    mTet2.setFillColor( sf::Color::Transparent);
    mTet2.setOutlineThickness(5);
    mTet2.setPosition(310, 405);

    sf::RectangleShape bottomRec1;
    bottomRec1.setSize(sf::Vector2f(80, 20));
    bottomRec1.setOutlineColor(sf::Color::Red);
    bottomRec1.setFillColor( sf::Color::Transparent);
    bottomRec1.setOutlineThickness(5);
    bottomRec1.setPosition(186, 467);

    sf::RectangleShape bottomRec2;
    bottomRec2.setSize(sf::Vector2f(80, 20));
    bottomRec2.setOutlineColor(sf::Color::Red);
    bottomRec2.setFillColor( sf::Color::Transparent);
    bottomRec2.setOutlineThickness(5);
    bottomRec2.setPosition(370, 467);

    sf::RectangleShape leftL1;
    leftL1.setSize(sf::Vector2f(60, 20));
    leftL1.setOutlineColor(sf::Color::Red);
    leftL1.setFillColor( sf::Color::Transparent);
    leftL1.setOutlineThickness(5);
    leftL1.setPosition(82, 467);

    sf::RectangleShape leftL2;
    leftL2.setSize(sf::Vector2f(20, 80));
    leftL2.setOutlineColor(sf::Color::Red);
    leftL2.setFillColor( sf::Color::Transparent);
    leftL2.setOutlineThickness(5);
    leftL2.setPosition(122, 467);

    sf::RectangleShape rightL1;
    rightL1.setSize(sf::Vector2f(60, 20));
    rightL1.setOutlineColor(sf::Color::Red);
    rightL1.setFillColor( sf::Color::Transparent);
    rightL1.setOutlineThickness(5);
    rightL1.setPosition(493, 467);

    sf::RectangleShape rightL2;
    rightL2.setSize(sf::Vector2f(20, 80));
    rightL2.setOutlineColor(sf::Color::Red);
    rightL2.setFillColor( sf::Color::Transparent);
    rightL2.setOutlineThickness(5);
    rightL2.setPosition(493, 467);

    sf::RectangleShape bottomTetLeft1;
    bottomTetLeft1.setSize(sf::Vector2f(185, 20));
    bottomTetLeft1.setOutlineColor(sf::Color::Red);
    bottomTetLeft1.setFillColor( sf::Color::Transparent);
    bottomTetLeft1.setOutlineThickness(5);
    bottomTetLeft1.setPosition(82, 590);

    sf::RectangleShape bottomTetLeft2;
    bottomTetLeft2.setSize(sf::Vector2f(20, 80));
    bottomTetLeft2.setOutlineColor(sf::Color::Red);
    bottomTetLeft2.setFillColor( sf::Color::Transparent);
    bottomTetLeft2.setOutlineThickness(5);
    bottomTetLeft2.setPosition(185, 529);

    sf::RectangleShape bottomTet1;
    bottomTet1.setSize(sf::Vector2f(140, 20));
    bottomTet1.setOutlineColor(sf::Color::Red);
    bottomTet1.setFillColor( sf::Color::Transparent);
    bottomTet1.setOutlineThickness(5);
    bottomTet1.setPosition(247, 529);

    sf::RectangleShape bottomTet2;
    bottomTet2.setSize(sf::Vector2f(20, 80));
    bottomTet2.setOutlineColor(sf::Color::Red);
    bottomTet2.setFillColor( sf::Color::Transparent);
    bottomTet2.setOutlineThickness(5);
    bottomTet2.setPosition(307, 529);

    sf::RectangleShape bottomTetRight1;
    bottomTetRight1.setSize(sf::Vector2f(185, 20));
    bottomTetRight1.setOutlineColor(sf::Color::Red);
    bottomTetRight1.setFillColor( sf::Color::Transparent);
    bottomTetRight1.setOutlineThickness(5);
    bottomTetRight1.setPosition(370, 590);

    sf::RectangleShape bottomTetRight2;
    bottomTetRight2.setSize(sf::Vector2f(20, 80));
    bottomTetRight2.setOutlineColor(sf::Color::Red);
    bottomTetRight2.setFillColor( sf::Color::Transparent);
    bottomTetRight2.setOutlineThickness(5);
    bottomTetRight2.setPosition(430, 529);

    sf::RectangleShape fix1;
    fix1.setSize(sf::Vector2f(276, 220));
    fix1.setOutlineColor(sf::Color::Red);
    fix1.setFillColor( sf::Color::Transparent);
    fix1.setOutlineThickness(5);
    fix1.setPosition(183, 215);

    sf::RectangleShape fix2;
    fix2.setSize(sf::Vector2f(120, 33));
    fix2.setOutlineColor(sf::Color::Red);
    fix2.setFillColor( sf::Color::Transparent);
    fix2.setOutlineThickness(5);
    fix2.setPosition(33, 306);

    sf::RectangleShape fix3;
    fix3.setSize(sf::Vector2f(120, 33));
    fix3.setOutlineColor(sf::Color::Red);
    fix3.setFillColor( sf::Color::Transparent);
    fix3.setOutlineThickness(5);
    fix3.setPosition(495, 306);

    sf::RectangleShape top;
    top.setSize(sf::Vector2f(552, 2));
    top.setOutlineColor(sf::Color::Red);
    top.setFillColor( sf::Color::Transparent);
    top.setOutlineThickness(5);
    top.setPosition(43, 36);

    sf::RectangleShape left;
    left.setSize(sf::Vector2f(2, 612));
    left.setOutlineColor(sf::Color::Red);
    left.setFillColor( sf::Color::Transparent);
    left.setOutlineThickness(5);
    left.setPosition(43, 36);

    sf::RectangleShape right;
    right.setSize(sf::Vector2f(2, 612));
    right.setOutlineColor(sf::Color::Red);
    right.setFillColor( sf::Color::Transparent);
    right.setOutlineThickness(5);
    right.setPosition(595, 36);

    sf::RectangleShape bottom;
    bottom.setSize(sf::Vector2f(552, 2));
    bottom.setOutlineColor(sf::Color::Red);
    bottom.setFillColor( sf::Color::Transparent);
    bottom.setOutlineThickness(5);
    bottom.setPosition(43, 651);

    sf::RectangleShape tunnelL;
    tunnelL.setSize(sf::Vector2f(10, 33));
    tunnelL.setOutlineColor(sf::Color::Red);
    tunnelL.setFillColor( sf::Color::Transparent);
    tunnelL.setOutlineThickness(5);
    tunnelL.setPosition(75, 303);

    sf::RectangleShape tunnelR;
    tunnelR.setSize(sf::Vector2f(10, 33));
    tunnelR.setOutlineColor(sf::Color::Red);
    tunnelR.setFillColor( sf::Color::Transparent);
    tunnelR.setOutlineThickness(5);
    tunnelR.setPosition(565, 303);

    //store all walls in vector
    std::vector<sf::RectangleShape> walls{outline,leftBump1,leftBump2,rightBump1,rightBump2,topBump,lBump1,lBump2,topRec1,topRec2,topRec3,topRec4,topRec5,topRec6,leftTopTet1,leftTopTet2,TopTet1,TopTet2,rightTopTet1,rightTopTet2,box,mRec1,mRec2,mTet1,mTet2,bottomRec1,bottomRec2,leftL1,leftL2,rightL1,rightL2,bottomTetLeft1,bottomTetLeft2,bottomTet1,bottomTet2,bottomTetRight1,bottomTetRight2};

    //modify for dot placement
    std::vector<sf::RectangleShape> bounds{fix1,fix2,fix3,leftBump1,leftBump2,rightBump1,rightBump2,topBump,lBump1,lBump2,topRec1,topRec2,topRec3,topRec4,topRec5,topRec6,leftTopTet1,leftTopTet2,TopTet1,TopTet2,rightTopTet1,rightTopTet2,box,mRec1,mRec2,mTet1,mTet2,bottomRec1,bottomRec2,leftL1,leftL2,rightL1,rightL2,bottomTetLeft1,bottomTetLeft2,bottomTet1,bottomTet2,bottomTetRight1,bottomTetRight2};

    //sprite bounds
    std::vector<sf::RectangleShape> sbounds{top,left,right,bottom,leftBump1,leftBump2,rightBump1,rightBump2,topBump,lBump1,lBump2,topRec1,topRec2,topRec3,topRec4,topRec5,topRec6,leftTopTet1,leftTopTet2,TopTet1,TopTet2,rightTopTet1,rightTopTet2,box,mRec1,mRec2,mTet1,mTet2,bottomRec1,bottomRec2,leftL1,leftL2,rightL1,rightL2,bottomTetLeft1,bottomTetLeft2,bottomTet1,bottomTet2,bottomTetRight1,bottomTetRight2};

    //make tunnel fix
    std::vector<sf::RectangleShape> tunnel {tunnelL,tunnelR};

    //populate grid dots vector
    std::vector<sf::CircleShape>dots;

    //dot placement function
    int xstep=20;
    int ystep=20;
    int counter = 0;
    int counter2 = 0;
    for (int x=62;x<575;x=x+xstep)
    {
        for (int y=57;y<640;y=y+ystep)
        {
            bool conflict = 0;
            for (int i=0; i<bounds.size();i++)
            {
                if (bounds[i].getGlobalBounds().contains(x,y)) {
                    conflict = 1;
                    break;
                }
            }
            if (!conflict) {
                sf::CircleShape dot(2);
                dot.setFillColor(sf::Color(255, 255, 255));
                dot.setPosition(x, y);
                dots.push_back(dot);
            }
            if (counter2%2 == 0)
            {
                ystep = 20;
            }
            else
            {
                ystep=21;
            }
            counter2++;
        }
        if (counter%2 == 0)
        {
            xstep = 20;
        }
        else
        {
            xstep=21;
        }
        counter++;
    }


    //make powerUps
    sf::CircleShape pow1(7);
    pow1.setFillColor(sf::Color(255, 255, 255));
    pow1.setPosition(58,92);

    sf::CircleShape pow2(7);
    pow2.setFillColor(sf::Color(255, 255, 255));
    pow2.setPosition(570,92);

    sf::CircleShape pow3(7);
    pow3.setFillColor(sf::Color(255, 255, 255));
    pow3.setPosition(58,500);

    sf::CircleShape pow4(7);
    pow4.setFillColor(sf::Color(255, 255, 255));
    pow4.setPosition(570,500);

    std::vector<sf::CircleShape>powerUps {pow1,pow2,pow3,pow4};//populate powerup vector

    //display optimization
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);


    State currentState = Start;
    Direction currDir = Stopped;

    srand(time(0));//seed the random

    //vector for ghost directions
    std::vector<Direction>GhostDirs;
    GhostDirs.push_back(static_cast<Direction>((rand()%4)+1));
    GhostDirs.push_back(static_cast<Direction>((rand()%4)+1));
    GhostDirs.push_back(static_cast<Direction>((rand()%4)+1));
    GhostDirs.push_back(static_cast<Direction>((rand()%4)+1));

    //variable for pacman being able to eat ghosts or not
    int PacMode = 0;

    //main game loop
    while (window.isOpen())
    {

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(spriteBackground);

        //switch statement for game states
        switch (currentState) {
            case Start:
                window.draw(startText);

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    currentState = Running;
                }

                //exit condition from escape key
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {
                    window.close();
                }
                break;
            case Running: {

                textureBackground.loadFromFile("maze.bmp");
                spriteBackground.setTexture(textureBackground);

                //exit condition from escape key
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    window.close();
                }

                // figure out offset to center based on time since last check
                currDir = updateDir(currDir, Pac, sbounds);

                sf::Vector2f currPos = Pac.getPosition();

                //Pacman collision detection conditionals
                float offsetX = 0;
                float offsetY = 0;
                if (currDir == Left)
                {
                    offsetX = -1;
                } else if (currDir == Right) {
                    offsetX = 1;
                } else if (currDir == Up) {
                    offsetY = -1;
                } else if (currDir == Down) {
                    offsetY = 1;
                }

                //pacman movement loop
                if (!checkConflict(currDir, Pac, sbounds))
                {
                    Pac.move(offsetX,offsetY);
                }

                //ghost collision system - with move included
                for(int i = 0; i<Ghosts.size(); i++){
                    if (!checkConflictGhost(GhostDirs[i], Ghosts[i], sbounds))
                    {
                        float offsetX = 0;
                        float offsetY = 0;
                        if (GhostDirs[i] == Left)
                        {
                            offsetX = -1;
                        } else if (GhostDirs[i] == Right) {
                            offsetX = 1;
                        } else if (GhostDirs[i] == Up) {
                            offsetY = -1;
                        } else if (GhostDirs[i] == Down) {
                            offsetY = 1;
                        }
                        Ghosts[i].move(offsetX,offsetY);
                    } else {
                        GhostDirs[i] = static_cast<Direction>((rand()%4)+1);
                    }
                }

                //remove dots when pacman eats them
                for (int dot=0;dot<dots.size();dot++)
                {
                    if (Pac.getGlobalBounds().contains(dots[dot].getPosition()))
                    {
                        dots.erase(dots.begin()+dot);
                    }
                }

                //remove powerups when pacman eats them
                for (int po=0;po<powerUps.size();po++)
                {
                    if (Pac.getGlobalBounds().contains(powerUps[po].getPosition()))
                    {
                        powerUps.erase(powerUps.begin()+po);
                        PacMode = 1;
                    }
                }

                //tunnel 1
                if(Pac.getGlobalBounds().intersects(tunnelL.getGlobalBounds()))
                {
                    Pac.setPosition(520,305);
                }

                //tunnel 2
                if(Pac.getGlobalBounds().intersects(tunnelR.getGlobalBounds()))
                {
                    Pac.setPosition(93,305);
                }

                //end game when all dots eaten
                if (dots.size() == 0)
                {
                    window.close();
                }

                //draw dots
                for (int i = 0; i < dots.size(); i++) {
                    window.draw(dots[i]);
                }

                //draw power ups
                for(int p=0;p<powerUps.size();p++)
                {
                    window.draw(powerUps[p]);
                }

                window.draw(Pac);//draw pacman

                //draw ghosts
                for (int i = 0; i < Ghosts.size(); i++)
                {
                    window.draw(Ghosts[i]);
                }

                //kill ghosts if pacman eats them
                for (int g=0;g<Ghosts.size();g++)
                {
                    if (Pac.getGlobalBounds().intersects(Ghosts[g].getGlobalBounds()))
                    {
                        if (PacMode == 1)
                        {
                            Ghosts.erase(Ghosts.begin()+g);
                        }
                        else
                        {
                            window.close();
                        }

                    }
                }
                break;
            }
        }

        window.display();
    }

}
