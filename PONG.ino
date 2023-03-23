#include <TFT_eSPI.h>

//create the screen variable from the library
TFT_eSPI tft = TFT_eSPI();

// Set up variables for the cursor and counter. Cursor starts in the middle of the screen.
int paddle1X = 50;
int paddle1Y= 100;
int oldPaddle1Y;
int paddle2X = 430;
int paddle2Y = 100;
int oldPaddle2Y;

float ballDirectionX = 1;
float ballDirectionY = 1;
float ballX, ballY, oldBallX, oldBallY;
int ballSpeed = 1; // lower numbers are faster
int paddleSpeed = 2; // higher is faster
int width = 10;
int height = 40;
int player1=0;
int player2=0;

float time1, time2;
int resetCount = 0;

// initialize variables for menu selection
int menuIndex = 0;
const int MENU_ITEMS = 3;

// Define colours in 4-digit hex                                  
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

// Setting the joystick pins here so we can easily change them
#define JOYSTICK_X_PIN A14
#define JOYSTICK_Y_PIN A15
#define JOYSTICK_BUTTON_PIN 23
#define Button_A_PIN 2
#define Button_B_PIN 3
#define Button_X_PIN 4
#define Button_Y_PIN 5



void setup() {
  // put your setup code here, to run once:
  // Initalize the screen and set the orientation correctly, then make sure it's clear.
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);

  //activates push button on joystick
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);

  // Initialize console
  tft.setTextSize(5);
  tft.setCursor(180, 70);
  tft.println("PONG");
  tft.setTextSize(2);
  tft.setCursor(165, 110);
  tft.setTextColor(GREEN);
  tft.println("Made by JOMM");
  tft.setTextColor(WHITE);

  for (int i = 0; i < 4; i++) {
    delay(700);
    tft.setCursor(200+(i*20), 160);
    tft.println(".");
    tone(10,500+(i*150),100);
  }
  tft.fillScreen(TFT_BLACK);
  
}


void loop() {
  // menu loop
  
  // use joystick to select menu options
  if (analogRead(A15) < 100) {
    menuIndex--;
    if (menuIndex < 0) {
      menuIndex = MENU_ITEMS - 1;
    }
    delay(50);
  }
  else if (analogRead(A15) > 900) {
    menuIndex++;
    if (menuIndex >= MENU_ITEMS) {
      menuIndex = 0;
    }
    delay(100);
  }

  // ------------MENU------------
  // display current menu selection
  tft.setTextSize(2);
  switch(menuIndex) {
    case 0:
      tft.setCursor(150, 140);
      tft.println("> 2 - Player");

      tft.setTextColor(BLACK);
      tft.setCursor(150, 160);
      tft.println(">");
      tft.setTextColor(WHITE);
      tft.setCursor(150, 160);
      tft.println("  1 - Player");

      tft.setTextColor(BLACK);
      tft.setCursor(150, 180);
      tft.println(">");
      tft.setTextColor(WHITE);
      tft.setCursor(150, 180);
      tft.println("  High Scores");
      break;

    case 1:
      tft.setTextColor(BLACK);
      tft.setCursor(150, 140);
      tft.println(">");
      tft.setTextColor(WHITE);
      tft.setCursor(150, 140);
      tft.println("  2 - Player");

      tft.setCursor(150, 160);
      tft.println("> 1 - Player");
      
      tft.setTextColor(BLACK);
      tft.setCursor(150, 180);
      tft.println(">");
      tft.setTextColor(WHITE);
      tft.setCursor(150, 180);
      tft.println("  High Scores");
      break;
      
    case 2:
      tft.setTextColor(BLACK);
      tft.setCursor(150, 140);
      tft.println(">");
      tft.setTextColor(WHITE);
      tft.setCursor(150, 140);
      tft.println("  2 - Player");

      tft.setTextColor(BLACK);
      tft.setCursor(150, 160);
      tft.println(">");
      tft.setTextColor(WHITE);
      tft.setCursor(150, 160);
      tft.println("  1 - Player");
      
      tft.setCursor(150, 180);
      tft.println("> High Scores");
      break;
  }   

    // wait for user to press button to select menu option
  if (digitalRead(JOYSTICK_BUTTON_PIN) == 0) {
    switch(menuIndex) {
      case 0:
        // 2 - Player start game
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(tft.width()/3, 150);
        tft.println("Starting game...");
        delay(500);
        tft.fillScreen(TFT_BLACK);
        tft.setTextSize(10);
        tft.setCursor(340, 0);
        tft.print(player1);

        tft.setCursor(100, 0);
        tft.print(player2);
        ballMotion();
        while(digitalRead(JOYSTICK_BUTTON_PIN) != 0 && (player1 < 5 && player2 < 5))
          pongGAME(2);
        if (player1 == 5){
          tft.setCursor(tft.width()/3, 150);
          tft.println("Player 1 wins!");
          tone(10,880,1000);
          delay(1000);
          tone(10,110,250);
          delay(250);
          tone(10,932,250);
          delay(250);
          tone(10,1109,250);
          delay(250);
          tone(10,932,250);
          delay(250);
          tone(10,880,1000);
        }
        else if (player2 == 5){
          tft.setCursor(tft.width()/3, 150);
          tft.println("Player 2 wins!");
          tone(10,880,1000);
          delay(1000);
          tone(10,110,250);
          delay(250);
          tone(10,932,250);
          delay(250);
          tone(10,1109,250);
          delay(250);
          tone(10,932,250);
          delay(250);
          tone(10,880,1000);
        }
        player1 = 0;
        player1 = 0;
        tft.fillScreen(TFT_BLACK);


        // need to put code for exit button
        break;

      case 1:
        // 1 - Player start game
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(tft.width()/3, 150);
        tft.println("Starting game...");
        delay(500);
        tft.fillScreen(TFT_BLACK);
        tft.setTextSize(10);
        tft.setCursor(340, 0);
        tft.print(player1);

        tft.setCursor(100, 0);
        tft.print(player2);
        ballMotion();
        while(digitalRead(JOYSTICK_BUTTON_PIN) != 0)
          pongGAME(1);
        tft.fillScreen(TFT_BLACK);
        break;
    
      case 2:
        // High Scores
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(tft.width()/3, 150);
        tft.println("High Scores");
        tft.println();
        tft.println("haha high scores are for losers");
        tft.println("JK NYI, press _____ to go back.");
       
        tft.fillScreen(TFT_BLACK);
        break;
        
    }
    // wait for user to release button
    
  }
  delay(50);

}




void pongGAME(int x) {
  int myWidth = tft.width();
  int myHeight = tft.height();

  paddle1Y = paddle1Y + map(analogRead(A15), 0, 1023, -paddleSpeed, paddleSpeed); //paddle 1

  if (0 >= paddle1Y)
    paddle1Y = 0;

  if (280 <= (paddle1Y))
    paddle1Y = 280;

  if (oldPaddle1Y != paddle1Y) {
    tft.drawRect(paddle1X, oldPaddle1Y, width, height, BLACK);
  }
  else{
    delay(1);
  }

  tft.drawRect(paddle1X, paddle1Y, width, height, WHITE);

  oldPaddle1Y = paddle1Y;

  /////////////////////////////////////////////////
  if (x == 1)
    if (ballY > paddle2Y + 20)
      paddle2Y += paddleSpeed;
    else
      paddle2Y -= paddleSpeed;
  else
  paddle2Y = paddle2Y + map(digitalRead(3), LOW, HIGH, 0, paddleSpeed) + map(digitalRead(4), LOW, HIGH, 0, -paddleSpeed);

  if (0 >= paddle2Y)
    paddle2Y = 0;

  if (280 <= paddle2Y)
    paddle2Y = 280;

  if (oldPaddle2Y != paddle2Y) {
    tft.drawRect(paddle2X, oldPaddle2Y, width, height, BLACK);
  }
  else{
    delay(1);
  }

  tft.drawRect(paddle2X, paddle2Y, width, height, WHITE);

  oldPaddle2Y = paddle2Y;
  /////////////////////////////////////////////

  // update the ball's position and draw it on screen
  if (millis() % ballSpeed < 2) {
    moveBall();
  }



    //2345 digital
  if (digitalRead(JOYSTICK_BUTTON_PIN) == 0){ 
    tft.fillScreen(BLACK);
    ballMotion();
    
    player1=0;
    player2=0;
    paddle2Y = 100;
    paddle1Y = 100;

    tft.setTextSize(10);

    tft.setCursor(340, 0);
    tft.print(player1);

    tft.setCursor(100, 0);
    tft.print(player2);
  }
  if (digitalRead(5) == HIGH) {
    tft.setCursor(tft.width()/2, 150);
    tft.print("Game Paused!");
    delay(500);
    while (digitalRead(5) == LOW) {
      delay(100);

    }
    delay(1000);
    tft.setTextColor(BLACK);
    tft.setCursor(tft.width()/2, 150);
    tft.print("Game Paused!");
  }

}

void moveBall() {

  if (ballX > tft.width()) {
    player2 += 1;
    
    point();
    ballMotion();
  }

  if (ballX < 0) {
    player1 += 1;
    point();
    ballMotion();
  }

  if (ballY > tft.height() || ballY < 0) {

    ballDirectionY = -ballDirectionY;

  }

  if (inPaddle(ballX, ballY, paddle1X, paddle1Y, width, height)) {

    ballDirectionX = -ballDirectionX;
    tone(10,500,50);

    float r = random(1,9);
    float sum = 0.5+r/8;
    Serial.print(sum);
    
    ballDirectionY = sum*ballDirectionY;
    
    
  }
  if (inPaddle(ballX, ballY, paddle2X, paddle2Y, width, height)) {

    ballDirectionX = -ballDirectionX;
    tone(10,500,50);

    float r = random(1,9);
    float sum = 0.5+r/8;
    Serial.print(sum);
    ballDirectionY = sum*ballDirectionY;

  }

  ballX += ballDirectionX;
  ballY += ballDirectionY;

  if (oldBallX != ballX || oldBallY != ballY) {

    tft.drawCircle(oldBallX, oldBallY, 5, BLACK);

  }

  tft.drawCircle(ballX, ballY, 5, WHITE);

  oldBallX = ballX;

  oldBallY = ballY;
}

void point() {
  tone(10, 800, 80);
  delay(80);
  tone(10, 600, 80);
  delay(80);
  tone(10, 400, 80);
  tft.setTextColor(WHITE);
  tft.setTextSize(16);
  tft.setCursor(130, 60);
  tft.println("POINT!");
  
  delay(1000);
  tft.setTextSize(4);
  tft.setTextSize(4);
  tft.setCursor(0, 120);
  tft.println("player 2: ");

  tft.setCursor(240, 120);
  tft.println("player 1: ");

  tft.setTextSize(10);
  tft.setCursor(340, 160);
  tft.print(player1);

  tft.setCursor(100, 160);
  tft.print(player2);
  delay(1000);

  tft.fillScreen(BLACK);
  ballDirectionX = 1;
  ballDirectionY = 1;
  ballX = tft.width()/2;
  ballY = tft.height()/2;
  
  paddle2Y = 100;
  paddle1Y = 100;

  tft.setTextSize(10);

  tft.setCursor(340, 0);
  tft.print(player1);

  tft.setCursor(100, 0);
  tft.print(player2);
  
}

boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight) {

  boolean result = false;

  if ((x >= rectX && x <= (rectX + rectWidth)) &&

    (y >= rectY && y <= (rectY + rectHeight))) {

    result = true;

  }

  return result;
}

void ballMotion() {
  ballDirectionX =random(2) ? -1:1;
  ballDirectionY =random(2) ? -1:1;
  if (ballDirectionX > 0) {
    ballX = 70;
  } else {
    ballX = 410;
  }
}
