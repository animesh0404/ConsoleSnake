#include<ncurses.h>
#include<unistd.h>
#include<cstdlib>
#include<ctime>
 
bool gameOver,gameWon;
int score,maxScore;
int shx,shy;
int fx,fy;
int width,height;
char shch,sbch,fch,bch;

int fraction;
int nTail;
unsigned long *tailX,*tailY;

const int second = 1000000;
int sltime; 
 
enum Direction {LEFT,UP,RIGHT,DOWN};
Direction dir;

int sy,sx,ey,ex;

void drawBoard(void);
void dispScore(void);
void genFruitpos(void);
void draw(void);
void setup(void);
void input(void);
void logic(void);
void gameoversc(void);
void gamewonsc(void);
//void deathAnimation(void);
 
int main()
{
    initscr();
    noecho();
    raw();
    cbreak();
    curs_set(FALSE); // Don't display a cursor
    //scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);    
    getmaxyx(stdscr,height,width);

    setup();
    while(!gameOver){
        logic();
        draw();
        input();
        usleep(sltime);       
    }
    if(!gameWon)
        gameoversc();
    else
        gamewonsc();
    endwin();
    return 0;
}

void gamewonsc()
{
    delete []tailX;
    delete []tailY;
    nodelay(stdscr, FALSE);
    mvprintw(height/2,width/2,"YOU WON");   
    getch();
}

void gameoversc()
{
    
    usleep(second/4);
    
    //Drawing Snake Head
    mvaddch(shy,shx,'X');
    refresh();
    
    //Drawing Snake Body
    for(int i=0;i<nTail;i++)
    {
        usleep(second/4);
        mvaddch(tailY[i],tailX[i],'.');
        mvaddch(shy,shx,'X');
        refresh();
    }
    
    usleep(second);
    

    nodelay(stdscr, FALSE);
    mvprintw(height/2,(ex/2),"GAME OVER");
    delete []tailX;
    delete []tailY;
    getch();
}

void setup()
{
    gameOver = false;
    gameWon  = false;
    nTail = 3;  
    shch = '@';
    sbch = 'o'; 
    bch  = '#';
    fch  = 'F';
    
    fraction = 24; 
    sltime = (second/fraction);
    
    //Board Limits
    sy = 1; ey = height-2;
    sx = 1; ex = (int)(width * 0.5);
    
    score = 0;
    maxScore = ((ey-sy-1)*(ex-sx-1))-1;
    
    tailX = new unsigned long[(ey-sy-1)*(ex-sx-1)];
    tailY = new unsigned long[(ey-sy-1)*(ex-sx-1)];
    //Snake Head Start Position
    shx = ((ex-sx)/2)+sx;
    shy = ((ey-sy)/2)+sy; 
    
    //Fruit Starting Postion
    genFruitpos();

        //mvprintw(sy+22,((width-ex)/4)+ex ,"maxTail: %u",maxScore);
        //mvprintw(sy+24,((width-ex)/4)+ex ,"fx: %d fy: %d shx: %d shy: %d",fx,fy,shx,shy);
}

void genFruitpos()
{
    int i;
    /* Intializes random number generator */
    srand((int)time(0));
    
    regen:
    fx = (rand()%(ex-sx-1))+sx+1;
    fy = (rand()%(ey-sy-1))+sy+1;
    
    if(fx == shx && fy == shy)
        goto regen;
    for(i=0;i<nTail;i++)
    {
        if(tailX[i] == fx && tailY[i] == fy)
        {
            goto regen;
        }
    }
}

void drawBoard()
{
    int i;
    
    for(i=sx;i<ex;i++)               //Drawing Upper Border
        mvaddch(sy,i,bch);
        
    for(i=sy;i<ey;i++)               //Drawing Right Border
        mvaddch(i,ex,bch);
        
    for(i=ex;i>sx;i--)               //Drawing Bottom Border
        mvaddch(ey,i,bch);
        
    for(i=ey;i>sy;i--)               //Drawing Left Border
        mvaddch(i,sx,bch);
}
 
void draw()
{
    int i;
    clear();
    box(stdscr,0,0);
    
    //Drawing Fruit 
    mvaddch(fy,fx,fch);
    
    //Drawing Snake Head
    mvaddch(shy,shx,shch);
    
    //Drawing Snake Body
    for(i=0;i<nTail;i++)
    {
        mvaddch(tailY[i],tailX[i],sbch);
    }
            
    //Drawing Game board 
    drawBoard();
    
    //Displaying Score
    dispScore();
    refresh();
    //getch(); 
}

void dispScore()
{
    mvprintw(sy+10,((width-ex)/4)+ex ,"Score: %d",score);
    mvprintw(sy+12,((width-ex)/4)+ex ,"x: Exit ");
    mvprintw(sy+14,((width-ex)/4)+ex ,"Use Arrow keys to Move");
    mvprintw(sy+16,((width-ex)/4)+ex,"Snake by Animesh Kahara");
}

void input()
{ 
    int ch;
    ch = getch();
	switch(ch)
		{	case KEY_LEFT:
		        if(dir != RIGHT){
                    dir = LEFT;
                }
				break;
				
			case KEY_RIGHT:
			    if(dir != LEFT){
                    dir = RIGHT;
				}
				break;
				
			case KEY_UP:
			    if(dir != DOWN){
                    dir = UP;
                }
				break;
				
			case KEY_DOWN:
			    if(dir != UP){
                    dir = DOWN;
                }
				break;

			case 'x':
			    gameOver = true;
			    break;
		}
}

void logic()
{
    if(score == maxScore-1)
    {
        gameWon  = true;
        gameOver = true;    
        return;
    }
    
     //Checking for game border
    // if(shx < sx+1 || shx > ex-1 || shy < sy+1 || shy > ey-1) 
    //  {   
    //     gameOver = true;
    //     return;
    //  }
    
    int i,prevX,prevY,prev2X,prev2Y;
    prevX    = tailX[0];
    prevY    = tailY[0];
    tailX[0] = shx;
    tailY[0] = shy;
    
    for(i=1;i<nTail;i++)
    {
        prev2X   = tailX[i];
        prev2Y   = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX    = prev2X;
        prevY    = prev2Y;
    } 
    
    //Checking for Self-Body Collision
    for(i=1;i<nTail;i++)
    {
        if(shx == tailX[i] && shy == tailY[i])
        {
            gameOver = true;
            return;
        }
    }
    

     
        
    // //4D-wall
    if(shx < sx)  { shx = ex; }
    if(shx > ex)  { shx = sx; }
    if(shy < sy)  { shy = ey; }
    if(shy > ey)  { shy = sy; }

    //Fruit Collision Check
    if(fx == shx && fy == shy)
    {
        score += 1;
        nTail++;
        tailX[nTail-1] = tailX[nTail-2];
        tailY[nTail-1] = tailY[nTail-2];
        genFruitpos();
    }
    
    //if( nTail!=0 && nTail%5==0 )
     //       fraction += 1;
    
    
    switch(dir)
    {
        case LEFT:
            shx--;
            sltime = (second/fraction);
            break;
            
        case UP:
            shy--;
            sltime = (second/(fraction-4));
            break;
            
        case RIGHT:
            shx++;
            sltime = (second/fraction);
            break;
            
        case DOWN:
            shy++;
            sltime = (second/(fraction)-8);
            break;
    }    
}
