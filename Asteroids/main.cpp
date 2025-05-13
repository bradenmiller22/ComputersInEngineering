//Programmer: Fam Trinli
//Source code is from: https://www.youtube.com/watch?v=rWaSo2usU4A
//Description: Asteroids game with player movement and player shooting
//
//Modified by Braden Miller
//Assignment: Asteroids
//11-29-23

//////////////////////////* EXTRA CREDIT /////////////////////////
/// for extra credit I added a score , this score updates after every asteroid or ufo is destroyed
/// I also added a lives counter, after every impact with asteroid or ufo, lives is decreased by 1
/// I also added an end screen that displays the final score of the game when the player runs out of lives.
/// I also added a restart feature to the code on the end screen
////Lastly I added a sound to whenever a bullet is fired from the player
/// */

#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <cmath>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

const int W = 1200;//dimensions for window
const int H = 800;

float DEGTORAD = 0.017453f;//conversion

//animation class to manage sprite animations
class Animation
{
public:
	float Frame, speed;//variables for animation
	Sprite sprite;//create a sprite
    std::vector<IntRect> frames;//vector to be able to make the animations actually work
    //defualt constructor
	Animation(){}
    //parameterized constructor for class
    Animation (Texture &t, int x, int y, int w, int h, int count, float Speed)
	{
	    Frame = 0;
        speed = Speed;
    //get frames vector with values from intrect for every frame in the sprite
		for (int i=0;i<count;i++) {
            frames.push_back(IntRect(x + i * w, y, w, h));
            //add texture to sprite and set them to screen
            sprite.setTexture(t);
            sprite.setOrigin(w / 2, h / 2);//go to screen
            sprite.setTextureRect(frames[0]);//add to start of frames vector
        }
	}

    //update code to accept updating the frames
	void update()
	{
    	Frame += speed;
		int n = frames.size(); //get the size
		if (Frame >= n) {Frame -= n;}//check if the frame should be updated
		if (n>0) {sprite.setTextureRect( frames[int(Frame)] );}//make the sprite move and set it to sprite
	}
    //check if the animation has reached the end
	bool isEnd()
	{
	  return Frame+speed>=frames.size();//change the frame of the animation for the sprite
	}

};

//game object class
class Entity
{
public://variables for entity
float x,y,dx,dy,R,angle;
bool life;
std::string name;
Animation anim;
//default constructor
Entity()
{
  life=1;
}
//create settings for the entity object created
void settings(Animation &a,int X,int Y,float Angle=0,int radius=1)
{
  anim = a; //default variables
  x=X; y=Y;
  angle = Angle;
  R = radius;
}
//virtual update to be overiden by derived classes
virtual void update(){};
//draw the entity to the window
void draw(RenderWindow &app)
{
  anim.sprite.setPosition(x,y);//set position
  anim.sprite.setRotation(angle+90);//rotation
  app.draw(anim.sprite);//draw to screen

  CircleShape circle(R);//create circle and default it
  circle.setFillColor(Color(255,0,0,170));
  circle.setPosition(x,y);
  circle.setOrigin(R,R);
  //app.draw(circle);
}
//destructor
virtual ~Entity(){};
};

//subclass UFO part 2 hw
class UFO: public Entity
{
public:
    UFO(){
        dx=2;//speed of 3
        dy=0.0;

        name ="UFO";
    }
    void update(){
        x+=dx;//update the ufo speed and position
        y+=dy;
        if (x>W){//check if ufo is off screen/if so then destroy it
            life=0;
        }
    }
};

//subclass asteroid, inherits from entity
class asteroid: public Entity
{
public:
  static int asteroidCount;//static count variable
    //default constructor
  asteroid()
  {
    asteroidCount++;//add to count when created
    dx=rand()%8-4;
    dy=rand()%8-4;//set random inition velocitys
    name="asteroid";
  }
  ~asteroid(){
    asteroidCount--;//decrement the count when asteroid is deleted
  }
//change the movements of the asteriods
void  update()
  {
   x+=dx;//set x position to change with speed
   y+=dy;//y
//go around the screen if out of bounds
   if (x > W){
       x = 0;
   }
   if (x<0) {
       x = W;
   }
   if (y > H){
       y = 0;
   }
   if(y<0) {
       y=H;
   }
   }
};


//bullet class, inherits from entity
class bullet: public Entity
{
public:
    //defualt constructor
  bullet()
  {
    name="bullet";
  }
//update method for bullets
void  update()
  {
   dx=cos(angle*DEGTORAD)*6;//set new velocities based on angle of ship
   dy=sin(angle*DEGTORAD)*6;
  // angle+=rand()%6-3;
   x+=dx;//move the bullet
   y+=dy;
//destroy the bullet if out of bounds
   if (x>W || x<0 || y>H || y<0){
       life=0;
   }
  }

};

//player class, inherits from entity
class player: public Entity
{
public:
   bool thrust;

   player()//default constructor
   {
     name="player";
   }
//update method for player
   void update()
   {
       //accelerate the player if thrust is active
     if (thrust)
      { dx+=cos(angle*DEGTORAD)*0.2;//change accelerations
        dy+=sin(angle*DEGTORAD)*0.2;
      }
     else//apply drag to the player if no thrust
      { dx*=0.99;
        dy*=0.99; }

    int maxSpeed=15;//max
    float speed = sqrt(dx*dx+dy*dy);//check the players speed
    if (speed>maxSpeed)//check if its more than max
     { dx *= maxSpeed/speed;//if so lower it
       dy *= maxSpeed/speed; }

    x+=dx;//move the player
    y+=dy;

    if (x>W) {x=0;}//check if out of bounds, and go to other side if
    if (x<0) {x=W;}
    if (y>H) {y=0;}
    if (y<0) {y=H;}
   }

};

//check if two entities have collided
bool isCollide(Entity *a,Entity *b)
{//change the positions
  return (b->x - a->x)*(b->x - a->x)+
         (b->y - a->y)*(b->y - a->y)<//all the positions from the two entitys, using pointers to get x, y values
         (a->R + b->R)*(a->R + b->R);
}

int asteroid::asteroidCount = 0;//initialize static member variable outside

int main()
{
    srand(time(0));
//default the window
    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60);
//load all the images to the game
    Texture t1,t2,t3,t4,t5,t6,t7,t8,t9;//load textures
    t1.loadFromFile("images/spaceship.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/explosions/type_C.png");
    t4.loadFromFile("images/rock.png");
    t5.loadFromFile("images/fire_blue.png");
    t6.loadFromFile("images/rock_small.png");
    t7.loadFromFile("images/explosions/type_B.png");
    //ufo free image download from https://www.flaticon.com/free-icon/ufo_3306571?term=ufo&page=1&position=9&origin=search&related_id=3306571 (free license for personal use)
    t8.loadFromFile("images/ufo.png");
    //sound spawn from https://freesound.org/people/colorsCrimsonTears/sounds/607408/(artist is colorsCrimsonTears, free use to reuse)
    SoundBuffer ufo;
    if(!ufo.loadFromFile("sounds/ufoSound.ogg")){ return EXIT_FAILURE;}
    Sound sound;//load sound to game
    sound.setBuffer(ufo);
    sound.setVolume(60);
    ////extra credit bullet sound
    SoundBuffer bullet1;
    //sound bullet from https://freesound.org/people/Jofae/sounds/363698/ (free use sound from artist)
    if(!bullet1.loadFromFile("sounds/bulletSound.ogg")){ return EXIT_FAILURE;}
    Sound bulletSound;
    bulletSound.setBuffer(bullet1);
    bulletSound.setVolume(30);

    Font font; //Lato script by Łukasz Dziedzic, https://www.1001freefonts.com/lato.font, license free for personal use
    if(!font.loadFromFile("fonts/LatoRegular.ttf")){ return EXIT_FAILURE;}

    t1.setSmooth(true);
    t2.setSmooth(true);

    Sprite background(t2);
    //make new animations
    Animation sExplosion(t3, 0,0,256,256, 48, 0.5);//explosion animation
    Animation sRock(t4, 0,0,64,64, 16, 0.2);//asteroid animation
    Animation sRock_small(t6, 0,0,64,64, 16, 0.2);//small asteroid animation
    Animation sBullet(t5, 0,0,32,64, 16, 0.8);//bullet animation
    Animation sPlayer(t1, 40,0,40,40, 1, 0);//create player animtion
    Animation sPlayer_go(t1, 40,40,40,40, 1, 0);//create player moving animation
    Animation sExplosion_ship(t7, 0,0,192,192, 64, 0.5);//ship explosion
    Animation sUFO(t8,0,0,128,128,1,0.2);//ufo animation

    std::list<Entity*> entities;//all entities

    for(int i=0;i<15;i++)//create new asteriods
    {
      asteroid *a = new asteroid();//new asteroid
      a->settings(sRock, rand()%W, rand()%H, rand()%360, 25);//defualt random them for 15 new ones
      entities.push_back(a);
    }


    //clock for ufo timer
    Clock ufoClock;
    float ufoDelay = 5.0;
    int ufoOnScreenStart = 0;//initializers for if a ufo is on the screen or not
    int ufoOnScreen = 1;

    int score = 0;//default 0 score
    int lives = 5;//set starting lives to 5
    Text scoreText("",font,30);//create text
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(15,15);//top left

    Text livesText("",font,30);//create text with loaded font
    livesText.setFillColor(Color::Red);//red
    livesText.setPosition(15,45);//top left


//default the player and asteroids^
    player *p = new player();
    p->settings(sPlayer,W/2,H/2,0,20);//default
    entities.push_back(p);//add player to array list of entities

    /////main loop/////
    while (app.isOpen() && lives > 0)
    {
        Event event;
        while (app.pollEvent(event))//keyboard loop
        {
            if (event.type == Event::Closed){
                app.close();
            }//close the game

            if (event.type == Event::KeyPressed) {//key presses
                if (event.key.code == Keyboard::Space) {
                    bulletSound.play();
                    bullet *b = new bullet();//new bullet if space
                    b->settings(sBullet, p->x, p->y, p->angle, 10);//create bullet
                    entities.push_back(b);

                }//handle if a bullet should be shot, if space is pressed
                //bulletSound.stop();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)){//change player to the right
            p->angle+=4;
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {//change player to the left
            p->angle-=4;
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {//set thrust to true
            p->thrust=true;
        }
        else {
            p->thrust=false;//drag if not
        }



    if(ufoClock.getElapsedTime().asSeconds() >= ufoDelay && (ufoOnScreenStart == 0)){
        //create a ufo at start
        sound.play();
        sound.setLoop(true);
        //sound.setLoop(true);
        UFO *u = new UFO();
        u->settings(sUFO, 0,rand()%H,270, 64);
        entities.push_back(u);
        ufoOnScreenStart = 1;

    }


//collision detection for the game
    for(auto a:entities) {
        for (auto b: entities) {
            //check if collision is with bullet and asteroid
            if (a->name == "asteroid" && b->name == "bullet") {
                if (isCollide(a, b)) {
                    //change the bullet and asteroid to be deleted
                    a->life = false;
                    b->life = false;
                    //create explosion at asteroid position
                    Entity *e = new Entity();//new explosion
                    e->settings(sExplosion, a->x, a->y);
                    e->name = "explosion";
                    entities.push_back(e);//throw loop back

                    score += 250;//add to score

                    //after the asteroid is deleted create smaller ones
                    for (int i = 0; i < 2; i++) {
                        if (a->R == 15) {//check if still under small amount of asteroids
                            continue;
                        }
                        Entity *e = new asteroid();//make new ones
                        e->settings(sRock_small, a->x, a->y, rand() % 360, 15);
                        entities.push_back(e);//default settings^
                    }

                }
            }
            if(a->name == "UFO" && b->name == "bullet"){//check if ufo hit by bullet
                if(isCollide(a,b)){
                    a->life = false;//delete ufo and bullet
                    b->life = false;
                    Entity *x = new Entity();
                    x->settings(sExplosion,a->x,b->y);
                    x->name = "explosion";//name explosion
                    entities.push_back(x);//add to back
                    ufoOnScreen = 0;
                    ufoClock.restart();//stop the sound and restart clock timer for ufo to spawn
                    sound.stop();

                    score+=1000;//add to score

                }
            }//check if player and ufo collide
            if(a->name == "player" && b->name == "UFO"){
                if(isCollide(a,b)){
                    b->life = false;//delete ufo

                    Entity *y = new Entity();//new entinty
                    y->settings(sExplosion_ship,a->x,a->y);//set settings
                    y->name = "explosion";//set name to explosion
                    entities.push_back(y);//add

                    p->settings(sPlayer,W/2,H/2,0,20);//reset player
                    p->dx = 0;//0
                    p->dy = 0;
                    ufoOnScreen = 0;//reset timer for ufo to spawn
                    ufoClock.restart();

                    sound.stop();//stop the sound of UFO

                    score+=500;//add to score
                    lives--;//decrease lives
                }
            }
            //check if player and asteroid collided
            if (a->name == "player" && b->name == "asteroid") {
                if (isCollide(a, b)) {
                    b->life = false;//delete asteroid

                    Entity *e = new Entity();//explosion at player
                    e->settings(sExplosion_ship, a->x, a->y);//set it to x, y, player
                    e->name = "explosion";//new explosion
                    entities.push_back(e);//add to array

                    //reset the player back to origin with default settings
                    p->settings(sPlayer, W / 2, H / 2, 0, 20);
                    p->dx = 0;
                    p->dy = 0;//defaults speed 0

                    score+=100;//add to score
                    lives--;
                }
            }
        }
    }

    //player should be accelerating
    if (p->thrust)  {
        p->anim = sPlayer_go;//change animation
    }
    else   {//player to be normal moving
        p->anim = sPlayer;//change animation
    }

    //explosion check
    for(auto e:entities) {
        if (e->name == "explosion") {//chekc if the explosion should be done or not
            if (e->anim.isEnd()){ e->life = 0;}//if the explosion animation is done then delete
        }
    }

    if(asteroid::asteroidCount == 0){//check if no asteroids left in the game
        for(int i = 0; i<15;i++){//if so spawn 15 new asteroids
            asteroid *a = new asteroid();
            a->settings(sRock,rand()%W,rand()%H,rand()%360,25);//set the asteroids with values
            entities.push_back(a);
        }
    }
    //check if the ufo should spawn or not, if its been 7 seconds or not and if none on screen
    if(ufoClock.getElapsedTime().asSeconds() >= (ufoDelay+2) && (ufoOnScreen == 0)){
            //create a ufo at start
            sound.play();
            sound.setLoop(true);
            //sound.setLoop(true);
            UFO *u = new UFO();
            u->settings(sUFO, 0,rand()%H,270, 64);
            entities.push_back(u);//create ufo object
            ufoOnScreen = 1;

        }

    for(auto i=entities.begin();i!=entities.end();)//check and update the entities array
    {
      Entity *e = *i;//entity at that position

      e->update();//update the position and animation of entity looking at
      e->anim.update();

      if (e->life==false) {//chekc if the entity should be deleted
          if(e->name =="UFO")//check if entity is ufo
          {
              sound.stop();//if ufo off screen then stop sound
              ufoClock.restart();//reset clock
              ufoOnScreen = 0;
          }
          i=entities.erase(i);//erase entities at i and go next
          delete e;//free memory
      }
      else {
          i++;//go to next entity
      }
    }

    //////EXTRA CREDIT END SCREEN-----LIVES-----restart/////
    scoreText.setString("Score: " + to_string(score));
    livesText.setString("Lives: " + to_string(lives));//update scores and lives
    //check to see if game should be over or not
    if(lives <= 0){
        //endscreen window
        sound.stop();
        RenderWindow endScreen(VideoMode(W,H), "Game Over!");
        endScreen.setFramerateLimit(60);
        //default text to load it
        Text endText("",font,65);
        endText.setString("Game Over!\nScore of: " + to_string(score) + "\nPress Escape to Exit. \nR to Restart.");
        endText.setFillColor(Color::White);//set white color
        endText.setPosition(W/5,H/5);//middle of screen
        //end screen loop for game
        while(endScreen.isOpen()){
            Event endEvent;
            while(endScreen.pollEvent((endEvent))){//keep checking if event
                if(endEvent.type == Event::Closed){//if its closed then shut the program off
                    endScreen.close();
                    app.close();//close game
                }

                if(endEvent.type == Event::KeyPressed){//if escape is pressed then shut game off
                    if(endEvent.key.code == Keyboard::Escape){
                        endScreen.close();//close game
                        app.close();
                    }
                    //check for restart
                    if(endEvent.key.code == Keyboard::R){
                        for(auto i=entities.begin();i!=entities.end();)//check and update the entities array
                        {
                            Entity *e = *i;//entity at that position

                            if(e->name == "player"){//check if the player entity, if so dont delete but reset
                                p->settings(sPlayer, W / 2, H / 2, 0, 20);//
                                p->dx = 0;//default player again
                                p->dy = 0;//defaults speed 0
                                i++;//go next
                            }
                            else {
                                i = entities.erase(i);//erase entities
                                delete e;//delete them free memory
                                //i++;//go to next entity
                            }
                        }
                        lives = 5;//reset variables
                        score = 0;
                        asteroid::asteroidCount = 0;

                        //spawn new asteroids after restart
                        for(int i = 0; i<15;i++){
                            asteroid *a= new asteroid();
                            a->settings(sRock,rand()%W,rand()%H,rand()%360,25);
                            entities.push_back(a);
                        }

                        //restart clocks
                        ufoClock.restart();
                        ufoOnScreen=0;

                        //close endscreen
                        endScreen.close();
                    }
                }
            }
            endScreen.clear();//clear the screen from asteroids
            endScreen.draw(endText);//draw the endText to screen
            endScreen.display();//display it
        }
    }

   //////draw//////
   app.draw(background);
   app.draw(scoreText);
   app.draw(livesText);

   for(auto i:entities){
     i->draw(app);//draw the entities
   }

   app.display();//show it
   }

    return 0;
}
