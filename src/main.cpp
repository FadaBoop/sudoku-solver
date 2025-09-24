#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>
#include <chrono>
#include <thread>

using Array3D = std::array<std::array<int, 9>, 9>;
int iterations = 0;

int validateNumber(Array3D arrayToCheck, int verticalPos, int horizontalPos){
    int count = 0;
    // horizontal check
    for(int j = 0; j<9; j++)
        if(arrayToCheck[verticalPos][j]==arrayToCheck[verticalPos][horizontalPos])
            count++;
    if (count > 1)
        return 0;

    // vertical check
    count = 0;
    for(int i = 0; i<9; i++)
        if(arrayToCheck[i][horizontalPos]==arrayToCheck[verticalPos][horizontalPos])
            count++;
    if (count > 1)
        return 0;

    // square check
    count = 0;
    for(int i = (verticalPos/3)*3; i<(verticalPos/3)*3+3; i++){
        for (int j = (horizontalPos/3)*3; j<(horizontalPos/3)*3+3; j++){
            if(arrayToCheck[i][j]==arrayToCheck[verticalPos][horizontalPos])
                count++;
        }
    }
    
    if (count > 1)
        return 0;
    
    return 1;
}

void solve(Array3D originalArray, Array3D &modifiedArray){
    int i = 0, j;
    while(i<9){ // loop for numbers
        j = 0;
        while(j<9){ // loop for numbers
            if (originalArray[i][j] == 0){
                while(validateNumber(modifiedArray, i, j) == 0 || modifiedArray[i][j]==0)
                    modifiedArray[i][j] = modifiedArray[i][j]+1;
                if (modifiedArray[i][j] > 9){
                    modifiedArray[i][j] = 0;
                    do{
                        if(j>0)
                            j--;
                        else if (j<=0){
                            j = 8;
                            i--;
                        }
                    }while(originalArray[i][j]!=0);
                    modifiedArray[i][j] = modifiedArray[i][j]+1;
                }
                else
                    j++;
            }
            else
                j++;
            iterations++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            std::cout << "\033[2J\033[1;1H";
            std::cout << iterations << std::endl;
        }
        i++;
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 800}), "WINDOWS?? NAH LINUX");

    sf::Vector2u windowSize = window.getSize();

    // LINES
    float xPosBigLines = windowSize.x / 3.0f; // 1/3rd of the screen
    float yPosBigLines = windowSize.y / 3.0f; // 1/3rd of the screen

    float xPosSmallLines = windowSize.x / 9.0f; // 1/9th of the screen
    float yPosSmallLines = windowSize.y / 9.0f; // 1/9th of the screen

    float xTextStart = windowSize.x / 25.0f;
    float yTextStart = windowSize.y / 50.0f; 
    
    sf::RectangleShape vertBigRectangle({4.0f, 1000.0f});
    vertBigRectangle.setFillColor(sf::Color::Black);
    sf::RectangleShape horiBigRectangle({1000.0f, 4.0f});
    horiBigRectangle.setFillColor(sf::Color::Black);

    sf::RectangleShape vertSmallRectangle({1.0f, 1000.0f});
    vertSmallRectangle.setFillColor(sf::Color::Black);
    sf::RectangleShape horiSmallRectangle({1000.0f, 1.0f});
    horiSmallRectangle.setFillColor(sf::Color::Black);

    // TEXT
    sf::Font textFont("ARIAL.TTF");
    sf::Text text(textFont);
    text.setString("Default");
    text.setCharacterSize(48);

    // ARRAY
    Array3D sudokuNums = {{
    {{8,0,0, 0,0,0, 0,0,0}},
    {{0,0,3, 6,0,0, 0,0,0}},
    {{0,7,0, 0,9,0, 2,0,0}},

    {{0,5,0, 0,0,7, 0,0,0}},
    {{0,0,0, 0,4,5, 7,0,0}},
    {{0,0,0, 1,0,0, 0,3,0}},

    {{0,0,1, 0,0,0, 0,6,8}},
    {{0,0,8, 5,0,0, 0,1,0}},
    {{0,9,0, 0,0,0, 4,0,0}}}};

    Array3D solvedNums = sudokuNums;
    std::thread solving(solve, sudokuNums, std::ref(solvedNums));

    while (window.isOpen())
    {
        text.setFillColor(sf::Color::Black);

        vertBigRectangle.setPosition({xPosBigLines,0.0f});
        horiBigRectangle.setPosition({0.0f,yPosBigLines});

        vertSmallRectangle.setPosition({xPosSmallLines,0.0f});
        horiSmallRectangle.setPosition({0.0f,yPosSmallLines});

        text.setPosition({xTextStart,yTextStart});

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::White);
        // just drawing
        for(int i=0;i<9;i++){ // loop for small lines
            window.draw(vertSmallRectangle);
            vertSmallRectangle.move({xPosSmallLines, 0.0f});

            window.draw(horiSmallRectangle);
            horiSmallRectangle.move({0.0f, yPosSmallLines});
        }
        for(int i=0;i<3;i++){ // loop for big lines
            window.draw(vertBigRectangle);
            vertBigRectangle.move({xPosBigLines, 0.0f});

            window.draw(horiBigRectangle);
            horiBigRectangle.move({0.0f, yPosBigLines});
        }
        for(int i=0;i<9;i++){ // loop for numbers
            for(int j=0;j<9;j++){ // loop for numbers
                if (solvedNums[i][j] == 0)
                    text.setFillColor(sf::Color::White);
                else if(sudokuNums[i][j] != 0)
                    text.setFillColor(sf::Color::Black);
                else
                    text.setFillColor(sf::Color::Blue);
                text.setString(std::to_string(solvedNums[i][j]));
                window.draw(text);
                text.move({xPosSmallLines, 0.0f});
            }
            text.setPosition({xTextStart,yTextStart});
            text.move({0.0f, yPosSmallLines*(i+1)});
        }

        window.display();
    }
}