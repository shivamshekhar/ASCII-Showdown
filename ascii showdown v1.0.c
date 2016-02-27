/*ASCII Showdown
version 1.0
made by: Shivam Shekhar
*/
#include<stdio.h>
#include<windows.h>
#include<time.h>
char grid[20][41]=                                    ///arena
                      {"XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX", /// X: wall
                       "X          c                           X", /// c: cpu
                       "X                                      X", /// u: user
                       "X                                      X", /// b: user's bullet
                       "X                                      X", /// B: cpu's bullet
                       "X                                      X", ///boomerang(array): boomerang
                       "X                                      X", ///wall(array): user's wall
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X                                      X",
                       "X          u                           X",
                       "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"};
///function declarations
void titlescreen(void);
void printbuffer(void);
void win(int);
void printborder(int,int,COORD,int);
void score(int,int,int,int,int,int,int);
void printscoreboard(void);
void screenflash(void);
///main program
int main()
{
    int i,j,k,upos=11,cpos=11,cmove,winner=0,uhealth=10,chealth=40,bullets=500,qtyofboomerang=20,qtyofwall=50,boost=0,nofboost=1;
    int boomerang[4]={188,187,201,200},wall[4]={219,178,177,176};
    clock_t start=-1,reloadtime=-1;
    SetConsoleTitle("ASCII Showdown");
    printf("controls:\n'a': left\n'd': right\n'w': fire bullet\n'o': fire homing boomerang\n'p': place a wall\n'i': Engage triple bullet firing mode\n");
    system("pause");
    titlescreen();
    system("cls");
    printbuffer();
    printscoreboard();
    getch();
    srand(time(NULL));         ///seeds random numbers using system time
    while(1)
    {
        ///user's turn
        if(kbhit()) ///listens if user pressed a key
        {
            switch(getch()) ///takes the key input
            {
            case 'w':
                if(bullets>0)
                {
                    if(boost==0)
                    {
                        grid[17][upos]='b';           ///fires a bullet
                        bullets--;
                    }
                    else
                    {
                        if(bullets>2)
                        {
                            for(i=-1;i<=1;i++)
                            {
                                if(upos+i>0 && upos+i<39)
                                {
                                    grid[17][upos+i]='b';          ///fires 3 bullets via weapon boost
                                    bullets--;
                                }
                            }
                        }
                    }
                }
                break;
            case 'a':              ///moves user left
                if(upos>1)
                {
                    grid[18][upos]=' ';
                    upos=upos-1;
                    grid[18][upos]='u';
                }
                break;
            case 'd':             ///moves user right
                if(upos<38)
                {
                    grid[18][upos]=' ';
                    upos=upos+1;
                    grid[18][upos]='u';
                }
                break;
            case 'o':
                if(qtyofboomerang>0)
                {
                    grid[17][upos]=boomerang[0];      ///fires homing boomerang
                    qtyofboomerang--;
                }
                break;
            case 'p':
                if(qtyofwall>0)
                {
                    grid[16][upos]=219;                 ///sets up a wall
                    qtyofwall--;
                }
                break;
            case 'i':
                if(nofboost>0)
                {boost=1;                              ///starts weapon boost
                start=clock();
                nofboost--;
                }
                break;
            }
        }
        ///cpu's turn
        cmove=rand()%4;        ///randomizing cpu's move
        switch(cmove)
        {
        case 0:
            if(chealth>10)
                grid[2][cpos]='B';    ///fires a bullet
            else
                {
                    for(i=-1;i<=1;i++)
                        {
                            if(cpos+i>0 && cpos+i<39)
                                grid[2][cpos+i]='B';          ///fires 3 bullets when health goes 10 or below 10
                        }
                }
            break;
        case 1:
            if(cpos>1)            ///moves left
            {
                grid[1][cpos]=' ';
                cpos=cpos-1;
                grid[1][cpos]='c';
            }
            break;
        case 2:                 ///moves right
            if(cpos<38)
            {
                grid[1][cpos]=' ';
                cpos=cpos+1;
                grid[1][cpos]='c';
            }
            break;
        case 3:                 ///does nothing
            break;
        }
        ///prints the arena
        printbuffer();
        ///prints score
        score(uhealth,chealth,bullets,qtyofboomerang,qtyofwall,boost,nofboost);
        ///time delay
        Sleep(70);
        ///user bullets
        for(i=0;i<20;i++)
        {
            for(j=0;j<40;j++)
            {
                if(grid[i][j]=='b' && i>1 && grid[i-1][j]==' ')
                {
                    grid[i][j]=' ';
                    grid[i-1][j]='b';             ///moves the bullets forward
                }
                if(grid[i][j]=='b' && i==1)
                    grid[i][j]=' ';   ///bullets collision with wall
                if(grid[i][j]=='b' && grid[i-1][j]=='B')
                {
                    grid[i][j]=' ';                      ///bullets collision
                    grid[i-1][j]=' ';
                }
                if(grid[i][j]=='b' && grid[i-1][j]=='c')
                {
                    grid[i][j]=' ';              ///bullet hits cpu
                    chealth--;
                    screenflash();
                }
                for(k=0;k<4;k++)
                {
                    if(grid[i][j]==boomerang[k]-256 && (grid[i-1][j]=='c' || grid[i-1][j-1]=='c' || grid[i-1][j+1]=='c'))
                    {
                        grid[i][j]=' ';              ///boomerang hits cpu
                        chealth-=1;
                        screenflash();
                        screenflash();
                    }
                    if(grid[i][j]==boomerang[k]-256 && i>1 && grid[i-1][j]==' ')
                    {
                        grid[i][j]=' ';
                        if(j>cpos)
                            grid[i-1][j-1]=boomerang[(k+1)%4];         /// boomerang moves forward
                        if(j<cpos)
                            grid[i-1][j+1]=boomerang[(k+1)%4];           ///homing boomerang
                        if(j==cpos)
                            grid[i-1][j]=boomerang[(k+1)%4];
                        break;
                    }
                    if(grid[i][j]==boomerang[k]-256 && grid[i-1][j]=='B')
                    {
                        grid[i][j]=' ';                      ///bullet and boomerang collision
                        grid[i-1][j]=boomerang[(k+1)%4];
                    }
                    if(grid[i][j]==boomerang[k]-256 && i==1)
                        grid[i][j]=' ';                       ///boomerang hits wall
                }
                for(k=0;k<4;k++)
                {
                    if(grid[i][j]=='b' && grid[i-1][j]==wall[k]-256)
                    {
                        grid[i][j]=' ';
                        grid[i-2][j]='b';                                           ///bullets collision with user's wall
                    }
                    if(grid[i][j]==boomerang[0]-256 && grid[i-1][j]==wall[k]-256)
                    {
                        grid[i][j]=' ';
                        grid[i-1][j]=' ';                                       ///boomerang collision with user's wall
                        grid[i-2][j]=boomerang[2];
                    }
                }
            }
        }
        ///cpu bullets
        for(i=19;i>-1;i--)
        {
            for(j=0;j<40;j++)
            {
                if(grid[i][j]=='B' && i<18 && grid[i+1][j]==' ')
                {
                    grid[i][j]=' ';                ///moves the bullets forward
                    grid[i+1][j]='B';
                }
                if(grid[i][j]=='B' && i==18)
                    grid[i][j]=' ';                 ///bullets collision with wall
                if(grid[i][j]=='B' && grid[i+1][j]=='u')
                {
                    grid[i][j]=' ';           ///bullet hits user
                    uhealth--;
                    screenflash();
                }
                for(k=0;k<4;k++)
                {
                    if(grid[i][j]=='B' && grid[i+1][j]==wall[k]-256)
                    {
                        grid[i][j]=' ';                             ///cpu's bullets collision with user's wall
                        if(k<3)
                            grid[i+1][j]=wall[k+1];
                        else
                            grid[i+1][j]=' ';
                    }
                }
            }
        }
        if(clock()-start>5000 && start>0)
        {
            boost=0;                                 ///stops the weapon boost
            start=-1;
            reloadtime=clock();
        }
        if(clock()-reloadtime>10000 && reloadtime>0)
        {
            if(nofboost==0)
                nofboost++;                              ///reloads the weapon boost
            reloadtime=-1;
        }
        if(chealth<=0)
        {
            winner=1;                               ///user wins
            break;
        }
        if(uhealth<=0 || (bullets<=0 && qtyofboomerang==0))
        {
            winner=2;                                            ///cpu wins
            break;
        }
    }
    win(winner);   ///selects the winner. 0 : no winner, 1 : user wins, 2 : cpu wins
    return 0; ///end of program
}
/// function definitions
void titlescreen()         ///displays the title screen
{

}
///
void printbuffer()         ///prints the arena
{
    int i,j,ucol=10,ccol=12,color=15;
    CHAR_INFO gridbuffer[800];
    COORD zerozero={0,0},gridsz={40,20};
    SMALL_RECT gridrect={10,2,50,22};
    HANDLE out;
    out=GetStdHandle(STD_OUTPUT_HANDLE);
    for(i=0;i<20;i++)
    {
        for(j=0;j<40;j++)
        {
            gridbuffer[j+40*i].Attributes=color;
            if(grid[i][j]=='X')
                gridbuffer[j+40*i].Char.AsciiChar=219;
            else if(grid[i][j]=='u')
            {
                gridbuffer[j+40*i].Char.AsciiChar=1;
                gridbuffer[j+40*i].Attributes=ucol;
            }
            else if(grid[i][j]==' ')
                gridbuffer[j+40*i].Char.AsciiChar=' ';
            else if(grid[i][j]=='b')
                gridbuffer[j+40*i].Char.AsciiChar=30;
            else if(grid[i][j]=='B')
                gridbuffer[j+40*i].Char.AsciiChar=31;
            else if(grid[i][j]=='c')
            {
                gridbuffer[j+40*i].Char.AsciiChar=2;
                gridbuffer[j+40*i].Attributes=ccol;
            }
            else
            {
                gridbuffer[j+40*i].Char.AsciiChar=grid[i][j];
            }
        }
    }
    WriteConsoleOutput(out,gridbuffer,gridsz,zerozero,&gridrect);
}
///
void win(int winner) ///prints winning message
{
    COORD wintextpos={37,12};
    switch(winner)
    {
    case 1:
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),wintextpos);
        printf("You win!\n");
        Sleep(10000);
        break;
    case 2:
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),wintextpos);
        printf("You lose!\n");
        Sleep(10000);
        break;
    }
}
///
void printborder(int _length,int _width,COORD _coordinates,int _color)     ///border printing function
{
    int i,j;
    COORD zerozero={0,0},bordersz;
    SMALL_RECT _rect;
    CHAR_INFO _border[_length*_width];
    HANDLE out;
    out=GetStdHandle(STD_OUTPUT_HANDLE);
    bordersz.X=_length;
    bordersz.Y=_width;
    _rect.Left=_coordinates.X;
    _rect.Top=_coordinates.Y;
    _rect.Right=_rect.Left+_length;
    _rect.Bottom=_rect.Top+_width;
    for(i=0;i<_width;i++)
    {
        for(j=0;j<_length;j++)
        {
            if(i==0 || i==_width-1)
            {
                _border[j+_length*i].Char.AsciiChar=205;
                _border[j+_length*i].Attributes=_color;
                continue;
            }
            if(j==0 || j==_length-1)
            {
                _border[j+_length*i].Char.AsciiChar=186;
                _border[j+_length*i].Attributes=_color;
                continue;
            }
            _border[j+_length*i].Char.AsciiChar=' ';
            _border[j+_length*i].Attributes=_color;
        }
    }
    _border[0].Char.AsciiChar=201;
    _border[_length-1].Char.AsciiChar=187;
    _border[_length*_width - 1].Char.AsciiChar=188;
    _border[_length*(_width-1)].Char.AsciiChar=200;
    _border[0].Attributes=_color;
    _border[_length-1].Attributes=_color;
    _border[_length*_width - 1].Attributes=_color;
    _border[_length*(_width-1)].Attributes=_color;
    WriteConsoleOutput(out,_border,bordersz,zerozero,&_rect);
}
///
void score(int uhealth,int chealth,int ubullets,int uboomerang,int uwall,int boost,int nofboost)   ///prints score
{
    HANDLE out;
    out=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD ctext={68,4},utext={68,16},utext1={68,17},utext2={68,18},utext3={68,19},utext4={68,20};
    SetConsoleCursorPosition(out,ctext);
    printf("   ");
    SetConsoleCursorPosition(out,utext);
    printf("   ");
    SetConsoleCursorPosition(out,utext1);
    printf("   ");
    SetConsoleCursorPosition(out,utext2);
    printf("  ");
    SetConsoleCursorPosition(out,utext3);
    printf("  ");
    SetConsoleCursorPosition(out,utext4);
    printf("   ");
    SetConsoleCursorPosition(out,ctext);
    printf("%d",chealth);
    SetConsoleCursorPosition(out,utext);
    printf("%d",uhealth);
    SetConsoleCursorPosition(out,utext1);
    printf("%d",ubullets);
    SetConsoleCursorPosition(out,utext2);
    printf("%d",uboomerang);
    SetConsoleCursorPosition(out,utext3);
    printf("%d",uwall);
    if(nofboost==1 && boost==0)
    {
        SetConsoleCursorPosition(out,utext4);
        SetConsoleTextAttribute(out,12);
        printf("OFF");
        SetConsoleTextAttribute(out,7);
    }
    else if(nofboost==0 && boost==1)
    {
        SetConsoleCursorPosition(out,utext4);
        SetConsoleTextAttribute(out,10);
        printf("ON");
        SetConsoleTextAttribute(out,7);
    }
    else if(nofboost==0 && boost==0)
    {
        SetConsoleCursorPosition(out,utext4);
        SetConsoleTextAttribute(out,14);
        printf("...");
        SetConsoleTextAttribute(out,7);
    }
}
///
void printscoreboard()    ///prints scoreboard
{
    COORD cpuscoresz={55,3},userscoresz={55,15},ctextpos={56,4},utextpos={56,16};
    HANDLE out;
    out=GetStdHandle(STD_OUTPUT_HANDLE);
    printborder(18,3,cpuscoresz,15);
    printborder(18,7,userscoresz,15);
    SetConsoleCursorPosition(out,ctextpos);
    printf("CPU health: ");
    SetConsoleCursorPosition(out,utextpos);
    printf("USER health: ");
    utextpos.Y++;
    SetConsoleCursorPosition(out,utextpos);
    printf("Bullets: ");
    utextpos.Y++;
    SetConsoleCursorPosition(out,utextpos);
    printf("Boomerang: ");
    utextpos.Y++;
    SetConsoleCursorPosition(out,utextpos);
    printf("Wall: ");
    utextpos.Y++;
    SetConsoleCursorPosition(out,utextpos);
    printf("Boost: ");
}
///
void screenflash()      ///flashes the screen
{
    char str[9]="color   ";
    char sym[17]="0123456789ABCDEF";
    str[6]=sym[rand()%17];
    str[7]=sym[rand()%17];
    system(str);
    Sleep(3);
    str[6]=sym[rand()%17];
    str[7]=sym[rand()%17];
    system(str);
    Sleep(3);
    str[6]=sym[rand()%17];
    str[7]=sym[rand()%17];
    system(str);
    Sleep(3);
    system("color 07");
}
