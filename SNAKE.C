#include<stdlib.h>
#include<conio.h>
#include<stdio.h>
#include<dos.h>

int height;
int width;
int shx,shy;
int nTail, tailX[100],tailY[100];
int fx,fy;
int score;
int gameOver;
char bd = -37;
char snh = '@';
char snb = 'o';
char f   = 3;
enum eDirection { STOP=0,LEFT,RIGHT,UP,DOWN};
enum eDirection dir;

void setup()
{
 gameOver = 0;
 score = 0;
 height = 18;
 width  = 20;
 randomize();
 fx  = (rand() % (width-3) +1);
 fy  = (rand() % (height-3) +1);
 shx = (width/2);
 shy = (height/2);
}

void draw()
{
 int i,j,k,pr;
 clrscr();

 for(i=0;i<width;i++)          //Drawing Upper Border
  printf("%c",bd);
 printf("\n");

 for(i=0;i<height;i++)         //Drawing Middle Border and Body
 {
  for(j=0;j<width-1;j++)
  {
    if(j==0)
    {
     printf("%c",bd);
    }

    else if(i == shy && j == shx)
    {
     printf("%c",snh);
    }

    else if(i==fy && j == fx)
    {
     printf("%c",f);
    }

    else
    {
     pr = 0;
     for(k=0;k<nTail;k++)
     {
      if(tailX[k] == j && tailY[k] == i)
      {
       printf("%c",snb);
       pr = 1;
      }
     }

     if(pr==0)
     {
      printf(" ");
     }
    }

    if(j==width-2)
    {
     printf("%c",bd);
    }
  }
  printf("\n");
 }

 for(i=0;i<width;i++)                      //Drawing Lower Border
  printf("%c",bd);

 printf("\n      Keymap\n  p:pause x:exit\n      w-s-a-d\n      Score: %d",score);
}

void input()
{
 if(kbhit())                         //kbhit returns a non-zero value until any key is pressed
 {
  switch(getch())
  {
   case 'a':
    dir = LEFT;
    break;

   case 'd':
    dir = RIGHT;
    break;

   case 'w':
    dir = UP;
    break;

   case 's':
    dir = DOWN;
    break;

   case 'p':
    getch();
    break;

   case 'x':
    gameOver = 1;
    break;
  }
 }
}

void logic()
{
 int i;
 int prevX = tailX[0];
 int prevY = tailY[0];
 int prev2X,prev2Y;
 tailX[0] = shx;
 tailY[0] = shy;

 for(i=1;i<nTail;i++)          //Traversing over sanke's body evry element
 {
  prev2X   = tailX[i];
  prev2Y   = tailY[i];
  tailX[i] = prevX;
  tailY[i] = prevY;
  prevX = prev2X;
  prevY = prev2Y;
 }

 switch(dir)
 {
  case LEFT:			//Setting snake eat direction to Left
      shx--;
  break;

  case RIGHT:                   //Setting snake eat direction to Right
      shx++;
  break;

  case UP:                      //Setting snake eat direction to Up
      shy--;
  break;

  case DOWN:                    //Setting snake eat direction to Down
      shy++;
  break;
 }

 //Boundary Collision check
 //if(shx < 1  || shx > width-2 || shy < 0 || shy > height-1)
 // gameOver = 1;

 
 if(shx < 1)                              //4-D wall
 {
  shx = width - 2;
 }
 if(shx > width - 2)
 {
  shx = 1;
 }
 if(shy < 0)
 {
  shy = height -1;
 }
 if(shy > height -1)
 {
  shy = 0;
 }
 
 for(i=0;i < nTail;i++)                   //Body Colliosion check
  if(tailX[i] == shx && tailY[i] == shy)
   gameOver = 1;

                                          
 if(shx == fx && shy == fy)           //fruit detection
 {
  score+=1;                           //Score Increase
  fx = (rand() % (width-3)   ) +1;    //Random fruit posx
  fy = (rand() % (height-3)  ) +1;    //Random fruit posy	
  nTail++;
 }
}

int main()
{
 setup();
 _setcursortype(_NOCURSOR);		//Hides the cursor
 while(!gameOver)			//Main Game Loop
 {
  draw();				//Calling the Draw function
  input();				//Calling the Input function
  logic();				//Calling the Logic function
  delay(200);				//Delaying main game for 200 ms. provides a stable gameplay speed
 }
 return 0;
}