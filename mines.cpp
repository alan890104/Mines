#include <bits/stdc++.h>
#include<windows.h>

using namespace std;



const int xx[8] = {1,1,0,-1,-1,-1,0,1};
const int yy[8] = {0,1,1,1,0,-1,-1,-1};

void SetColor(int color = 7)
{
  HANDLE hConsole;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole,color);
}

void selection(int type,int &n,int &m,int &mines)
{
    switch(type)
    {
        case 1:
            {
                n = m = 9;
                mines = 10;
                break;
            }
        case 2:
            {
                n = m = 16;
                mines = 25;
                break;
            }
        case 3:
            {
                n = 30;
                m = 16;
                mines = 99;
                break;
            }
        default : return;
    }
}


bool bound_check(int row,int col,int n,int m)
{
    if(row<n && row>=0 && col<m && col>=0) return 1;
    return 0;
}

void hint_calculate(vector<vector<int> > &board,int row,int col,int n,int m)
{
    int total = 0;
    for(int i=0;i<8;i++)
    {
            int x = xx[i]+row;
            int y = yy[i]+col;
            if(bound_check(x,y,n,m))
            {
                if(board[x][y]!=-1) board[x][y]++;
            }
    }
}


bool generate_board(int type,vector<vector<int> > &board)
{
    switch(type)
    {
        case 1 :
            {
                vector<vector<int> > a(9,vector<int>(9,0));
                board = a;
                break;
            }
        case 2:
            {
                vector<vector<int> > a(16,vector<int>(16,0));
                board = a;
                break;
            }
        case 3:
            {
                vector<vector<int> > a(30,vector<int>(16,0));
                board = a;
                break;
            }
        default : return false;
    }
    return true;
}

void random_filled_mines(int type,vector<vector<int> > &board)
{
    int n,m,mines;
    selection(type,n,m,mines);

    int max_rand = n*m-1;

    random_device rd;
    default_random_engine gen{static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())};
    uniform_int_distribution<int> dis(0,max_rand);

    set<int> location_of_mines;
    set<int> :: iterator it;

    while(location_of_mines.size()<mines)
    {
        location_of_mines.insert(dis(gen));
    }

    //random fill mines

    for(it=location_of_mines.begin();it!=location_of_mines.end();it++)
    {
        int temp = *it;
        int row = temp/n , col = temp%m;
        board[row][col] = -1;
    }

    // fill the hint of mines

    for(it=location_of_mines.begin();it!=location_of_mines.end();it++)
    {
        int temp = *it;
        int row = temp/n , col = temp%m;
        hint_calculate(board,row,col,n,m);
    }

}


void initialize_show_board(int type,vector<vector<int> > &show,vector<vector<int> > &board)
{
    int n,m,mines;
    selection(type,n,m,mines);
    vector<vector<int> > b(n,vector<int>(m,0));
    show = b;
    int max_rand = n*m-1;
    int max_show = (int)round(sqrt((double)n*(double)m));

    random_device rd;
    default_random_engine gen{static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())};
    uniform_int_distribution<int> dis(0,max_rand);

    while(max_show)
    {
        int temp = dis(gen);
        int row = temp/n , col = temp%m;
        if(board[row][col]!=-1 && !show[row][col])
        {
            show[row][col] = 1;
            max_show--;
        }
    }

}



void print(int type,vector<vector<int> > &show,vector<vector<int> > &board)
{
    int n,m,mines;
    selection(type,n,m,mines);

    cout<<setw(5)<<"";

    for(int i=0;i<m;i++)
    {
        SetColor(11);
        cout<<setw(5)<<i;
    }
    cout<<"\n\n";

    for(int i=0;i<n;i++)
    {

            SetColor(11);
            cout<<setw(5)<<i;

        for(int j=0;j<m;j++)
        {
            if(show[i][j]==1)
            {
                if(board[i][j]!=-1)
                {
                    SetColor(10);
                    cout<<setw(5)<<board[i][j];
                }
                else
                {
                    SetColor(12);
                    cout<<setw(5)<<'x';
                }
            }
            else if(show[i][j]==0)
            {
                SetColor();
                cout<<setw(5)<<'?';
            }
            else
            {
                SetColor(14);
                cout<<setw(5)<<'m';
            }

        }
        cout<<"\n\n";
    }
}


bool lose_game(vector<vector<int> > &board,int row,int col)
{
    return (board[row][col]==-1);
}

bool win_game(vector<vector<int> > &show , vector<vector<int> > &board,int n,int m)
{
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            if(board[i][j]!=-1 && show[i][j]==0) return 0;
        }
    }
    return 1;

}

void lose_print(int type,vector<vector<int> > &show,vector<vector<int> > &board,int row,int col)
{
    int n,m,mines;
    selection(type,n,m,mines);

    cout<<setw(5)<<"";

    for(int i=0;i<m;i++)
    {
        SetColor(11);
        cout<<setw(5)<<i;
    }
    cout<<"\n\n";

    for(int i=0;i<n;i++)
    {

            SetColor(11);
            cout<<setw(5)<<i;

        for(int j=0;j<m;j++)
        {
            if(board[i][j]==-1)
            {
                show[i][j] = 1;
            }
            if(show[i][j]==1)
            {
                if(board[i][j]!=-1)
                {
                    SetColor(10);
                    cout<<setw(5)<<board[i][j];
                }
                else
                {
                    if(i==row && j==col)
                    {
                        SetColor();
                        cout<<"    ";
                        SetColor(207);
                        cout<<'x';
                    }
                    else
                    {
                     SetColor(12);
                    cout<<setw(5)<<'x';
                    }

                }
            }
            else if(show[i][j]==0)
            {
                SetColor();
                cout<<setw(5)<<'?';
            }
            else
            {
                SetColor(14);
                cout<<setw(5)<<'m';
            }

        }
        cout<<"\n\n";
    }
}




int main()
{
int type,n=0,m=0,mines=0,flags=0;
SetColor();
cout<<"Input type (1(is 9*9) or 2(is 16*16) or 3(is 30*16)) : ";
cin>>type;
selection(type,n,m,mines);
flags = mines;
vector<vector<int> > board;
vector<vector<int> > show;
generate_board(type,board);
random_filled_mines(type,board); //-1 is mine
initialize_show_board(type,show,board);
system ("CLS");
print(type,show,board);

clock_t start,stop;
start = clock();

while(1)
{
    int row,col;
    char c;
    SetColor();
    cout<<"You have "<<flags<<" flags.\n\n"<<endl;
    SetColor();
    cout<<"Input index row col mode('m' for flag and 'k' for open): ";
    cin>>row>>col>>c;
    if(!bound_check(row,col,n,m))
    {
        SetColor();
        cout<<"Index out of range. "<<endl;
        system("Pause");
        system ("CLS");
        print(type,show,board);
        continue;
    }

    system ("CLS");

    if(c=='m' || c=='M')
    {
        if(flags==0)
        {
            SetColor();
            cout<<"There are no more flags\n";
        }
        else if(show[row][col]==0)
        {
            show[row][col]=2;
            flags--;
        }
        else if(show[row][col]==1)
        {
            SetColor();
            cout<<'('<<row<<','<<col<<") is already opened."<<endl;
        }
        else
        {
            show[row][col] = 0;
            flags++;
        }

        print(type,show,board);
    }
    else if(c=='K' || c=='k')
    {
        show[row][col] = 1;
        print(type,show,board);
        if(lose_game(board,row,col))
        {
            system ("CLS");
            lose_print(type,show,board,row,col);
            SetColor(12);
            cout<<"BOOOOOOOOMB!! YOU LOSE !";
            break;
        }
        if(win_game(show,board,n,m))
        {
            stop = clock();
            SetColor(14);
            cout<<"YOU WIN! in "<<double(stop - start) / CLOCKS_PER_SEC <<" seconds. "<<endl;
            break;
        }
    }
    else
    {
        SetColor();
        cout<<"Wrong instruction (only have m and k)"<<endl;
        print(type,show,board);
    }

}



SetColor();
return 0;
}
