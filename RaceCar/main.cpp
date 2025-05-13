//
// Programmer: Fam Trinli
// Fam Trinli's Source Code from: https://www.youtube.com/watch?v=YzhhVHb0WVY
// Description: This program is a car racing game
//
// Modified by: Braden Miller
// Date: 10-25-23
// Assignment: HW5
//
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;
/*******************FOR PART 6**********************
 * Alteration: Added 2 new images, boost pad and oil spill
 * Description: Boost pad speed car up and oil spill slows car down for half second
 ************************************************************************
 * */
const int num = 9; //checkpoints
                    //points to the coordinates of where the cars need to turn  to go around the track properly
int points[num][2] = {300, 610,
                      1270, 430,
                      1380, 2380,
                      1900, 2460,
                      1970, 1700,
                      2550, 1680,
                      2560, 3000,
                      500, 3300,
                      325, 1800};

class Car {        //make these variables private, by using getters and setters
public:

    float getX() const{return x;}
    float getY() const{return y;}
    /*name: setX
     * description: checks if the x value is within bounds of image
     * inputs: float of myX of the position on car
     * action: checks if the car is within 0-width of image
     * output: float x of the value of car position
     * */
    void setX(float myX){
        if(myX >= 0 && myX <= backgroundWidth){
            x = myX;
        }
    }
    /*name: setY
     * description: checks if the y value is within bounds of image
     * inputs: float of myY of the position on car
     * action: checks if the car is within 0-height of image
     * output: float y of the value of car position
     * */
    void setY(float myY){
        if(myY >= 0 && myY <= backgroundHeight){
            y=myY;
        }
    }
    float getSpeed() const{return speed;}
    void setSpeed(float mySpeed){speed = mySpeed;}
    float getAngle() const{return angle;}
    void setAngle(float myAngle){angle = myAngle;}
    float getN() const{return n;}
    void setN(int myN){n = myN;}
    float getWidth() const{return backgroundHeight;}
    float getHeight() const{return backgroundHeight;}

    /*name: setWidth
     * description: sets the width of the background image chosen
     * input: float w as the background width
     * action: checks if w > 0
     * output: backgroundWidth is a float of w for the width
     * */
    void setWidth(float w) {
        if(w > 0){
            backgroundWidth = w;
        }else{
            backgroundWidth = 0;
        }
    }

    /*name: setHeight
     * description: sets the height of the background image chosen
     * input: float h as the background height
     * action: checks if h > 0
     * output: backgroundHeight is a float of h for the height
     * */
    void setHeight(float h) {
        if(h > 0){
            backgroundHeight = h;
        }else{
            backgroundHeight = 0;
        }
    }

    int getCheckpoint()const {return checkpoint;}
    /*name: checkpointAdd
     * description: increments counter for checkpoints
     * inputs: if checkpoint has been reached
     * action: adds 1 to checkpoint
     * output: int checkpoint = checkpoint+1
     * */
    void checkpointAdd(){
        checkpoint++;
    }
    /*Name: reset
     * description: resets values needed to 0 when game is restarted
     * inputs: every car inside of the program
     * action: resets car values to 0 to every car inside of the game
     * output: int checkpoint=0, int n=0, int angle=0 for all the cars in the game
     * */
    void reset(){
        checkpoint =0;
        n = 0;
        angle = 0;
    }

    Car(float bWidth, float bHeight)  //default constructor
    {
        x = 0;
        y=0;
        speed = 2;
        angle = 0;
        n = 0;
        backgroundWidth = bWidth;
        backgroundHeight = bHeight;
        checkpoint = 0;

    }
/*Name: move
 * description: moving the car on the screen
 * Inputs: where the car is
 * Action: moves the car with x and y, from the speed and angles
 * output: moves the car based on updating the x, y with new values from angle and speed
 * */
    void move() { //moving the car
        float newX = x + sin(angle) * speed; //updates x position of car
        float newY = y - cos(angle) * speed; //updates y position of car

        //set bounds to x, y :: x is width minus radius and 0, y is 0 and height minus the radius
        if(newX >= 0 && newX <= (backgroundWidth - 43) && newY >= 0 && newY <= (backgroundHeight-45)){

            x = newX;
            y = newY;
        }
    }
/*Name: findTarget
 * Description: Cars go throughout the track and go to each checkpoint
 * Inputs: original array of all checkpoints
 * action: updates the cars of the target locations to go throughout the track
 * output: the cars go to the next targets on the screen
 * */
    void findTarget() { //how the cars go to the next checkpoint in the track
        float tx = points[n][0]; //get the x coordinate of the target checkpoint
        float ty = points[n][1]; //get the y coordinate of the target checkpoint
        float beta = angle - atan2(tx - x, -ty + y); //find angle difference of the cars
        if (sin(beta) < 0) { angle += 0.005 * speed; } else { angle -= 0.005 * speed; }//adjust angle if needed of the car
        if ((x - tx) * (x - tx) + (y - ty) * (y - ty) < 25 * 25) { n = (n + 1) % num; }//move to the next checkpoint in the array if reached
    }
private:
    float x;     // x location of the center of the car
    float y;    //y location center of car
    float speed; //speed of car
    float angle;    //angle of car on track
    int n;  //current target checkpoint
    float backgroundWidth; //background size
    float backgroundHeight;
    int checkpoint; //checkpoints reached

};


int main() {
    RenderWindow app(VideoMode(640, 480), "Car Racing Game!"); //sets the window size
    app.setFramerateLimit(60); //set frame rate limit to 60

    Texture t1, t2, t3, t4;
    t1.loadFromFile("images/background.png"); //load background
    if(!t1.loadFromFile("images/background.png")) { return EXIT_FAILURE;}
    t2.loadFromFile("images/car.png"); //load car
    if(!t2.loadFromFile("images/car.png")) { return EXIT_FAILURE;}
    t3.loadFromFile("images/boost.png"); //boost image was drawn by myself inside of GIMP
    if(!t3.loadFromFile("images/boost.png")){ return EXIT_FAILURE;}
    t4.loadFromFile("images/oilSpill.png");
    if(!t4.loadFromFile("images/oilSpill.png")){ return EXIT_FAILURE;}

    t1.setSmooth(true); //this is texture smoothing
    t2.setSmooth(true);
    t3.setSmooth(true);


    Sprite sBackground(t1), sCar(t2), sBoost(t3), sOil(t4);//creat sprites for the background and of the car
    sBackground.scale(2, 2);//scale the background to fit


    sBoost.rotate(180);//rotate my image 180 degree cause i drew it backwards

    sCar.setOrigin(22, 22);//set origin of car
    float carRadius = 22;//radius of car

    const int numCars = 5; //number of cars
    Car car[numCars]{
            {sBackground.getGlobalBounds().width, sBackground.getGlobalBounds().height},
            {sBackground.getGlobalBounds().width, sBackground.getGlobalBounds().height},
            {sBackground.getGlobalBounds().width, sBackground.getGlobalBounds().height},
            {sBackground.getGlobalBounds().width, sBackground.getGlobalBounds().height},
            {sBackground.getGlobalBounds().width, sBackground.getGlobalBounds().height},
    };//create array of car objects



    for (int i = 0; i < numCars; i++) {
        car[i].setX(300 + i * 50); //set x of cars
        car[i].setY(1700 + i * 80); //set y of car
        car[i].setSpeed(7+i); //set speed of cars
        car[i].setWidth(sBackground.getGlobalBounds().width);
        car[i].setHeight(sBackground.getGlobalBounds().height);
    }
    //intialize variables of the car
    float speed = 0;//inital speed
    float angle = 0;//inital angle
    float maxSpeed = 10.0; //max speed
    float acc = 0.2, dec = 0.3; //accel and decel values
    float turnSpeed = 0.08;//turning speed

    int offsetX = 0, offsetY = 0; //this is the change in postion or offset from the background variable

    Font font; //Lato script by Łukasz Dziedzic, https://www.1001freefonts.com/lato.font, licensce free for personal use
    if(!font.loadFromFile("fonts/LatoRegular.ttf")){ return EXIT_FAILURE;}

    ////music/////
    //Music: Bensound.com/free-music-for-videos
    //License code: ZNRGFN3VKB9UTTUS
    //load sound
    SoundBuffer soundB;
    if(!soundB.loadFromFile("sounds/dreams.ogg")) { return EXIT_FAILURE;} //checking
    //creat a sound instance to use
    Sound sound; //make sound variable
    sound.setBuffer(soundB); //set it to it
    sound.setLoop(true);    //always play
    sound.setVolume(50); //volume
    sound.setPitch(1);//pitch
    sound.play();//play sound

//create circle for a checkpoint
    CircleShape circle(60);
    circle.setOrigin(60,60);//middle of circle
    circle.setFillColor(Color::Blue); //color blue

    float checkpointX = 300; //initial first checkpoint
    float checkpointY = 610; //initial
    float checkpointR = 70; //initial radius

    //initialize a clock and text
    Clock clock;
    Text timerText; //make clock
    timerText.setFont(font);//font
    timerText.setCharacterSize(40);
    timerText.setFillColor(Color::Black);//black
    timerText.setPosition(20,20);//top left
    //initial checkers
    bool finalCheckpoint = false;
    bool waitingForRestart = false;
    bool updateTime = true;
    Time finishTime; //final time
    Clock secClock;//secondary clock and time for oil spill
    Time spinTimer;


    while (app.isOpen()) { //main game loop for the program

        Time elapsed = clock.getElapsedTime();//time elapsed of program

        //this massive if, else checks if the game is complete or not, when complete the user is asked
        //if they want to restart the game
        if(finalCheckpoint){
                if(updateTime){ //this is a way to stop the clock when the game is finished
                    finishTime = clock.getElapsedTime();
                    updateTime = false;
                    sound.pause();
                }
            //display end screen saying game was completed in x amount of time
            timerText.setString("Race Completed!\nTime: " + to_string(static_cast<int>(finishTime.asSeconds())) + " seconds.\nDo you want to restart the race? (R)");
            app.clear(Color::Blue); //end screen is blue
            app.draw(timerText);
            //app.display();

            Event e;//make the event in SFML
            while (app.pollEvent(e)) { //go through each event
                if (e.type == Event::KeyPressed) {
                    if(e.key.code == Keyboard::R){ //check to see if key R is pressed
                        clock.restart(); //restart the clock
                        sound.play();
                        sCar.setOrigin(22, 22);//set origin of car
                        checkpointX = 310; //initial first checkpoint
                        checkpointY = 600; //initial
                        checkpointR = 70; //initial radius
                        //this is to reset the cars back to original locations
                        for (int i = 0; i < numCars; i++) {
                            car[i].setX(300 + i * 50); //set x of cars
                            car[i].setY(1700 + i * 80); //set y of car
                            car[i].setSpeed(7+i); //set speed of cars
                            car[i].setWidth(sBackground.getGlobalBounds().width);
                            car[i].setHeight(sBackground.getGlobalBounds().height);
                            car[i].reset(); //reset to original values
                        }
                        maxSpeed = 10.0; //default back to original values
                        speed = 0;
                        //resets booleans
                        //finishTime = clock.getElapsedTime();
                        updateTime = true; //makes it so finalTime can be updated
                        waitingForRestart = true;
                    }
                }
            }
            //if restart occurs reset boolean checkers to false
            if(waitingForRestart){
                finalCheckpoint = false;
                waitingForRestart = false;
            }

        }else {
            /////
            Event e;//make the event in SFML
            while (app.pollEvent(e)) { //go through each event
                if (e.type == Event::Closed) {
                    app.close();//close window if event is closed
                }
            }
            bool Up = 0, Right = 0, Down = 0, Left = 0;//for keyboard inputs TRUE or False
            //loop for a second when the oil spill is hit, making the car unable to move for half second
            if(spinTimer.asSeconds() == 0 || spinTimer.asSeconds() <= 0.5){
                car[0].setSpeed(1);
                maxSpeed=10.0; //change max back down to 10
                spinTimer = secClock.getElapsedTime(); //increment time by 1 second
            }else {
                if (Keyboard::isKeyPressed(Keyboard::Up)) { Up = 1; } //up key (true)
                if (Keyboard::isKeyPressed(Keyboard::Right)) { Right = 1; } //right key(true)
                if (Keyboard::isKeyPressed(Keyboard::Down)) { Down = 1; } //down key(true)
                if (Keyboard::isKeyPressed(Keyboard::Left)) { Left = 1; }//left key(true)
            }
            //car movement
            if (Up && speed < maxSpeed) { //if up arrow is pressed move the car forward
                if (speed < 0) { speed += dec; } //if no movement, accelerate car
                else { speed += acc; } //still accelerate car
            }

            if (Down && speed > -maxSpeed) { //if down arrow is pressed slow the car down to speed 0
                if (speed > 0) { speed -= dec; } //change speed by decelleration value
                else { speed -= acc; } //otherwise change by acceleration
            }

            if (!Up && !Down) { //if up and down are false
                if (speed - dec > 0) { speed -= dec; } //change speed by decelleration value
                else if (speed + dec < 0) { speed += dec; } //if its lower than 0 add deceleration value
                else { speed = 0; } //speed is 0
            }

            if (Right && speed != 0) { angle += turnSpeed * speed / maxSpeed; } //turn the car to the right
            if (Left && speed != 0) { angle -= turnSpeed * speed / maxSpeed; } //turn the car to the left

            car[0].setSpeed(speed);//set initial car to speed
            car[0].setAngle(angle); //set initial car to angle

            for (int i = 0; i < numCars; i++) { car[i].move(); } //move all of the other cars in the target
            for (int i = 1; i < numCars; i++) { car[i].findTarget(); } //find checkpoints that the cars need to hit

            //collision
            for (int i = 0; i < numCars; i++) { //loop through for both cars
                for (int j = 0; j < numCars; j++) {
                    int dx = 0, dy = 0; //initial 0
                    bool noCollision = false; //initial false
                    while (dx * dx + dy * dy < 4 * carRadius * carRadius && !noCollision) {
                        car[i].setX(car[i].getX() + dx / 10.0); //modify x coordinate for first car seperating the cars
                        car[i].setY(car[i].getY() + dy / 10.0); //modify y coordinate for first car seperating y values
                        car[j].setX(car[j].getX() - dx / 10.0); //modify x coordinate for second car seperating them
                        car[j].setY(car[j].getY() - dy / 10.0); //modify y coordinate for second car seperating them
                        dx = car[i].getX() - car[j].getX(); //sets dx to the difference of the x values
                        dy = car[i].getY() - car[j].getY();//sets dy to the difference of the y values
                        if (!dx &&
                            !dy) { noCollision = true; } //keeps going until dx and dy are 0 which means the cars arent colliding anymore
                    }
                }
            }


            app.clear(Color::White); //clear the screen

            if (car[0].getX() > 320) {//adjust the offset based on your cars position
                offsetX = car[0].getX() - 320; //change x to cars x value
            }
            if (car[0].getY() > 240) { //adjust the yoffset based on your cars position
                offsetY = car[0].getY() - 240; //change y to cars y value
            }

            //make
            sBackground.setPosition(-offsetX,-offsetY);//set the background position based off of ur cars postion on screen
            app.draw(sBackground); //draw the background
            //set x,y for boost pad
            int xBoost = 1420-offsetX;
            int yBoost = 1400 - offsetY;


            //set it on image
            sOil.setPosition(xBoost+495, yBoost+530);
            //get the distance from car to oil spill accounting for offsett from the background and size
            float oilDistance = sqrt(pow(car[0].getX() - (xBoost+2175),2) + pow(car[0].getY() - (yBoost+2300),2));
            if(oilDistance <= 90){//car radius plus img radius
                secClock.restart(); //reset the secondary clock to make the car unable to move momentarily
                spinTimer = secClock.getElapsedTime();//start the clock again
            }
            app.draw(sOil);//draw oil spill

            //set it
            sBoost.setPosition(xBoost,yBoost);
            //calculate distance from car to boost pad in x,y accounting for offsett from background
            float boostDistance = sqrt(pow(car[0].getX() - (xBoost+1045),2) + pow(car[0].getY() - (yBoost+1100),2));
            //radius of boostpad compared to the distance between it and the car
            if(boostDistance <= (90)){
                maxSpeed = 15.0; //if on boost pad increase speed up to 15
            }
            app.draw(sBoost); //draw pad

            Color colors[10] = {Color::Red, Color::Green, Color::Magenta, Color::Blue,
                                Color::White}; //makes an array of possible car colors

            for (int i = 0; i < numCars; i++) { //check position of all cars
                sCar.setPosition(car[i].getX() - offsetX,
                                 car[i].getY() - offsetY); //set position of each car by using the background offset
                sCar.setRotation(car[i].getAngle() * 180 /
                                 3.141593); //check what the angle of the car is and set it to that on the screen
                sCar.setColor(colors[i]); //set the color of the cars
                app.draw(sCar); //draw each sprite to the screen
            }
            circle.setPosition(checkpointX - offsetX, checkpointY - offsetY);//initialy set it

            /////
            //this is a way to calculate the distance between the car and checkpoints on the image. it uses pow to square them so the distance isnt negative, this allows then the two values to be square rooted
            float disCheckpoint = sqrt(pow(car[0].getX() - checkpointX, 2) + pow(car[0].getY() - checkpointY, 2));
            //check if the distance is less than the radius of the checkpoint
            if (disCheckpoint <= checkpointR) {
                car[0].checkpointAdd(); //increment counter
                checkpointX = points[car[0].getCheckpoint() % num][0]; //next checkpoint
                checkpointY = points[car[0].getCheckpoint() % num][1]; //next checkpoint
                circle.setPosition(checkpointX - offsetX, checkpointY - offsetY); //position the next checkpoint on image
                //check if the final checkpoint is reached
                if (car[0].getCheckpoint() == (num)) {
                    finalCheckpoint = true; //updates this if-else and stops program
                }
            }
            //loop goes through until final checkpoint is reached, incrementing the time
            if (!finalCheckpoint) {
                timerText.setString("Time: " + to_string(static_cast<int>(elapsed.asSeconds()))); //change seconds into an integer
                app.draw(timerText); //draw timer and circles
                app.draw(circle);
            } else {
                app.draw(timerText); //draw timer
            }


        }
        //display to the screen
        app.display();
    }

    return 0;
}