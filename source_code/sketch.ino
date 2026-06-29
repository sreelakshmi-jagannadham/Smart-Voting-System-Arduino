#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_RST);

// Buttons
const int c1Btn = 2;
const int c2Btn = 3;
const int c3Btn = 4;
const int c4Btn = 5;

const int resultBtn = 6;
const int resetBtn  = 7;

const int lockBtn   = A0;
const int buzzer    = A1;
const int unlockBtn = A2;

//password variables
int password[4] = {1,2,3,4};

int entered[4];

int passIndex = 0;

// Vote storage
int votes[4] = {0,0,0,0};

bool votingLocked = false;

String names[4] =
{
  "Candidate A",
  "Candidate B",
  "Candidate C",
  "Candidate D"
};

//----------------------------------
// Buzzer
//----------------------------------
void beep()
{
  tone(buzzer,1000,100);
}

//----------------------------------
// Total Votes
//----------------------------------
int totalVotes()
{
  return votes[0] +
         votes[1] +
         votes[2] +
         votes[3];
}

//----------------------------------
// Home Screen
//----------------------------------
void showHome()
{
  tft.fillScreen(ILI9341_NAVY);

  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);

  tft.setCursor(20,10);
  tft.println("ELECTRONIC");

  tft.setCursor(20,35);
  tft.println("VOTING MACHINE");

  tft.drawLine(0,60,320,60,ILI9341_WHITE);

  tft.setCursor(20,80);
  tft.println("D2 - Candidate A");

  tft.setCursor(20,105);
  tft.println("D3 - Candidate B");

  tft.setCursor(20,130);
  tft.println("D4 - Candidate C");

  tft.setCursor(20,155);
  tft.println("D5 - Candidate D");

  tft.setTextColor(ILI9341_YELLOW);

  tft.setCursor(20,200);
  tft.print("Votes : ");
  tft.println(totalVotes());

  if(votingLocked)
  {
    tft.setTextColor(ILI9341_RED);

    tft.setCursor(180,200);
    tft.println("LOCKED");
  }
}

//----------------------------------
// Vote Recorded Screen
//----------------------------------
void voteScreen(String candidate)
{
  beep();

  tft.fillScreen(ILI9341_GREEN);

  tft.setTextColor(ILI9341_BLACK);

  tft.setTextSize(3);

  tft.setCursor(40,60);
  tft.println("VOTE");

  tft.setCursor(20,100);
  tft.println("RECORDED");

  tft.setTextSize(2);

  tft.setCursor(20,170);
  tft.println(candidate);

  delay(1200);

  showHome();
}

//----------------------------------
// Voting Closed Screen
//----------------------------------
void votingClosedScreen()
{
  tft.fillScreen(ILI9341_RED);

  tft.setTextColor(ILI9341_WHITE);

  tft.setTextSize(3);

  tft.setCursor(20,90);
  tft.println("VOTING");

  tft.setCursor(20,130);
  tft.println("CLOSED");

  delay(1500);

  showHome();
}
//---------------------------
// password screen
//---------------------------

void passwordScreen()
{
  tft.fillScreen(ILI9341_BLUE);

  tft.setTextColor(ILI9341_WHITE);

  tft.setTextSize(2);

  tft.setCursor(20,40);
  tft.println("ADMIN RESET");

  tft.setCursor(20,90);
  tft.println("ENTER PASSWORD");

  tft.setCursor(20,140);
  tft.println("A B C D");
}

//----------------------
// correct password
//----------------------

void passwordCorrect()
{
  tft.fillScreen(ILI9341_GREEN);

  tft.setTextColor(ILI9341_BLACK);

  tft.setTextSize(3);

  tft.setCursor(20,100);
  tft.println("ACCESS");

  tft.setCursor(20,140);
  tft.println("GRANTED");

  delay(1500);
}

//---------------------
//wrong password
//--------------------

void passwordWrong()
{
  tft.fillScreen(ILI9341_RED);

  tft.setTextColor(ILI9341_WHITE);

  tft.setTextSize(3);

  tft.setCursor(20,100);
  tft.println("WRONG");

  tft.setCursor(20,140);
  tft.println("PASSWORD");

  delay(1500);

  showHome();
}
//------------------------------
// password verification 
//----------------------------

bool verifyPassword()
{
  passIndex = 0;

  passwordScreen();

  // Wait until reset button is released
  while(digitalRead(resetBtn)==LOW);

  delay(300);

  while(passIndex < 4)
  {
    if(digitalRead(c1Btn)==LOW)
    {
      entered[passIndex] = 1;
      passIndex++;

      while(digitalRead(c1Btn)==LOW);
      delay(300);
    }

    else if(digitalRead(c2Btn)==LOW)
    {
      entered[passIndex] = 2;
      passIndex++;

      while(digitalRead(c2Btn)==LOW);
      delay(300);
    }

    else if(digitalRead(c3Btn)==LOW)
    {
      entered[passIndex] = 3;
      passIndex++;

      while(digitalRead(c3Btn)==LOW);
      delay(300);
    }

    else if(digitalRead(c4Btn)==LOW)
    {
      entered[passIndex] = 4;
      passIndex++;

      while(digitalRead(c4Btn)==LOW);
      delay(300);
    }
  }

  
}

//-------------------------------
// reset 
//------------------------------- 

void resetScreen()
{
 if(digitalRead(resetBtn)==LOW)
{
  if(verifyPassword())
  {
    passwordCorrect();

    votes[0]=0;
    votes[1]=0;
    votes[2]=0;
    votes[3]=0;

    resetScreen();

    showHome();
  }
  else
  {
    passwordWrong();
  }

  while(digitalRead(resetBtn)==LOW);

  delay(200);
}
}

//-------------------
//result 
//-----------------

void showResults()
{
  int total =
  votes[0]+votes[1]+votes[2]+votes[3];

  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_YELLOW);

  tft.setTextSize(2);

  tft.setCursor(80,10);
  tft.println("RESULTS");

  for(int i=0;i<4;i++)
  {
    float percentage = 0;

    if(total > 0)
    {
      percentage =
      (votes[i] * 100.0) / total;
    }

    tft.setTextColor(ILI9341_WHITE);

    tft.setCursor(10,50 + i*45);

    tft.print(char('A'+i));

    tft.print(": ");

    tft.print(votes[i]);

    tft.print(" (");

    tft.print((int)percentage);

    tft.print("%)");

    int barLength =
    map(votes[i],
        0,
        total==0?1:total,
        0,
        150);

    tft.fillRect(
      150,
      50 + i*45,
      barLength,
      20,
      ILI9341_BLUE
    );
  }

  tft.setTextColor(ILI9341_GREEN);

  tft.setCursor(10,230);

  tft.print("Total Votes: ");
  tft.print(total);

  delay(6000);

  showWinner();
}
//-------------------
// winner annoncement
//-------------------

void showWinner()
{
  int winner = 0;

  for(int i=1;i<4;i++)
  {
    if(votes[i] > votes[winner])
      winner = i;
  }

  tft.fillScreen(ILI9341_GREEN);

  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(3);

  tft.setCursor(70,20);
  tft.println("WINNER");

  tft.drawRoundRect(40,70,240,100,10,ILI9341_BLACK);

  tft.setTextSize(2);

  tft.setCursor(70,100);
  tft.println(names[winner]);

  tft.setCursor(90,135);
  tft.print(votes[winner]);
  tft.println(" Votes");

  delay(5000);

  showHome();
}
//---------------------------
//  screen unlock 
//---------------------------

void unlockedScreen()
{
  tft.fillScreen(ILI9341_GREEN);

  tft.setTextColor(ILI9341_BLACK);

  tft.setTextSize(3);

  tft.setCursor(20,80);
  tft.println("VOTING");

  tft.setCursor(20,120);
  tft.println("RESUMED");

  delay(1500);

  showHome();
}

//----------------------------------
void setup()
{
  Serial.begin(9600);
  pinMode(c1Btn,INPUT_PULLUP);
  pinMode(c2Btn,INPUT_PULLUP);
  pinMode(c3Btn,INPUT_PULLUP);
  pinMode(c4Btn,INPUT_PULLUP);

  pinMode(resultBtn,INPUT_PULLUP);
  pinMode(resetBtn,INPUT_PULLUP);

  pinMode(lockBtn,INPUT_PULLUP);

  pinMode(buzzer,OUTPUT);
  pinMode(unlockBtn, INPUT_PULLUP);
  
  tft.begin();

  tft.setRotation(1);

  showHome();
}

//----------------------------------
void loop()
{
  // LOCK BUTTON
  if(digitalRead(lockBtn)==LOW && !votingLocked)
{
  votingLocked = true;

  votingClosedScreen();

  while(digitalRead(lockBtn)==LOW);

  delay(200);
}
  if(digitalRead(unlockBtn)==LOW)
{
  votingLocked = false;

  unlockedScreen();

  while(digitalRead(unlockBtn)==LOW);

  delay(200);
}

  // RESET BUTTON
  if(digitalRead(resetBtn)==LOW)
{
   if(verifyPassword())
  {
    passwordCorrect();

    votes[0] = 0;
    votes[1] = 0;
    votes[2] = 0;
    votes[3] = 0;

    resetScreen();

    showHome();
  }
   else
  {
    passwordWrong();
  }

   while(digitalRead(resetBtn)==LOW);

   delay(200);
}
  // RESULT BUTTON
  if(digitalRead(resultBtn)==LOW)
{

    showResults();

    while(digitalRead(resultBtn)==LOW);

    delay(200);
}

  // Candidate A
  if(digitalRead(c1Btn)==LOW && !votingLocked)
  {
    votes[0]++;
    voteScreen(names[0]);
    delay(300);
  }

  // Candidate B
  if(digitalRead(c2Btn)==LOW && !votingLocked)
  {
    votes[1]++;
    voteScreen(names[1]);
    delay(300);
  }

  // Candidate C
  if(digitalRead(c3Btn)==LOW && !votingLocked)
  {
    votes[2]++;
    voteScreen(names[2]);
    delay(300);
  }

  // Candidate D
  if(digitalRead(c4Btn)==LOW && !votingLocked)
  {
    votes[3]++;
    voteScreen(names[3]);
    delay(300);
  }
}