// https://pubs.opengroup.org/onlinepubs/7908799/xcurses/curses.h.html

#include <curses.h>
#include <stdlib.h>
#include <time.h>

// 8 colors for standard Curses
#define PALLETE_SIZE (COLOR_WHITE - COLOR_BLACK + 1)

// Internal state of the application
typedef struct treeData
{
    int lastKey;
    int treeSize;

} treeData;

//////////////////////////////////////////////////////////////////////
// Curses auxiliary functions
//////////////////////////////////////////////////////////////////////
void initScreen()
{
#ifdef XCURSES
    Xinitscr(argc, argv);
#else
    initscr();
#endif

    if (has_colors())
    {
        int bg = 0, fg = 0;

        start_color();

        for(bg = COLOR_BLACK; bg <= COLOR_WHITE; bg++)
            for(fg = COLOR_BLACK; fg <= COLOR_WHITE; fg++)
                init_pair(bg*PALLETE_SIZE + fg + 1, fg, bg); // color 0 is system default (reserved)
    }

    //Enter creates a newline \n
    noecho();
    // 0 invisible mouse cursor
    curs_set(0);
    // Define getch as non-blocking  according to the timeout below
    nodelay(stdscr, TRUE);
    // Timeout in 0 determines getch as non-blocking, doesn't wait for user input
    timeout(0);
    //enables F keys (f1, f2...)
    keypad(stdscr, TRUE);
}

void setColor(short int fg, short int bg, chtype attr)
{
    chtype color = COLOR_PAIR(bg*PALLETE_SIZE + fg + 1);
    color |= attr;
    attrset(color);
}


//////////////////////////////////////////////////////////////////////
// Application functions
//////////////////////////////////////////////////////////////////////
void initKey(treeData * tree)
{
    tree->lastKey = -1;
}


void handleInputs(treeData * tree)
{
    int input = getch();

    if(input != -1)
        tree->lastKey = input;

    switch(tree->lastKey)
    {
        case 'q':
        case 'Q':
            curs_set(1);
            endwin();
            exit(EXIT_SUCCESS);
            break;
        case KEY_UP:
            if (tree->treeSize < 50)
            {
               tree->treeSize++;
            }
            tree->lastKey = -1;
            break;
        case KEY_DOWN:
            if(tree->treeSize > 1)
            {
                tree->treeSize--;
            }
            tree->lastKey = -1;
            break;
        case KEY_RESIZE:
            endwin();
            initScreen();

            clear();
            refresh();

            break;
    }
}



void drawScreen(treeData * tree)
{
   
    int i = 0, j = 0;
    clear();


    for(i = getmaxy(stdscr) - tree->treeSize; i < getmaxy(stdscr); i++ )
    {
        for(j = getmaxx(stdscr)/2 - tree->treeSize - i + 62 ; j < getmaxx(stdscr)/2 + tree->treeSize + i - 61; j++)
        {
            setColor(COLOR_YELLOW, COLOR_GREEN, 0);
            mvaddch(i, j, '*');
            if (tree->treeSize > 10)
            {
                setColor(COLOR_RED, COLOR_BLUE, 0);
                mvaddch(i, j, '*');
            }

            if (tree->treeSize > 20)
            {
                setColor(COLOR_BLUE, COLOR_YELLOW, 0);
                mvaddch(i, j, '*');
            }

            if (tree->treeSize > 30)
            {
                setColor(COLOR_WHITE, COLOR_RED, 0);
                mvaddch(i, j, '*');
            }
            if (tree->treeSize > 40)
            {
                setColor(COLOR_CYAN, COLOR_WHITE, 0);
                mvaddch(i, j, '*');
            }        
            if (tree->treeSize == 50)
            {
                setColor(COLOR_YELLOW, COLOR_GREEN, 0);
                mvaddch(i, j, '*');
            }
            
        }
    }

    
   for(i = getmaxy(stdscr)-tree->treeSize; i <= getmaxy(stdscr); i++)
    {
        setColor(COLOR_RED, COLOR_RED, 0);
        mvaddch(i, getmaxx(stdscr)/2,'|');
    }
    j = getmaxx(stdscr)/2;
    setColor(COLOR_WHITE,COLOR_YELLOW, 0);
    mvprintw((i-2)-tree->treeSize, (j-1)+1, "*");
    mvprintw((i-3)-tree->treeSize, (j)+1, "*");
    mvprintw((i-3)-tree->treeSize, (j-2)+1, "*");
    mvprintw((i-4)-tree->treeSize, (j-1)+1, "*");

    refresh();
}

int main(int argc, char *argv[])
{
    //internal struct of the tree
    treeData tree;

    // initializes ncurses screen
    initScreen();
    initKey(&tree);

    // main loop
	while(1)
    {
        // manages user input
        handleInputs(&tree);

        // Atualiza a tela
        drawScreen(&tree);

        // controls the fps of the application
        napms(144);
    }

    // closes the window
    endwin();

	return 0;
}
