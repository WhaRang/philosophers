#include <ncurses.h>
#include "philosopher.h"

static const int ARG = 6;
static bool isEnd = false;
std::vector<philosopher> phils;
auto *tbls = new tableware[ARG];


void screen() {

    initscr();
    noecho();

    getch();

    halfdelay(1);

    int height = 7, width = 48;
    int start_y = 1, start_x = 1;
    int delta_y = 0, delta_x = 10;
    int max_y, max_x;

    int win_delta_y = 1, win_delta_x = 4;
    int fork_start_x = 49, fork_start_y = 1;
    int fork_height = height * 3, fork_width = delta_x - 1;

    getmaxyx(stdscr, max_y, max_x);

    WINDOW **windows = new WINDOW*[ARG];
    for (int i = 0; i < ARG; i++) {
        WINDOW *window = newwin(height, width, start_y, start_x);
        refresh();
        windows[i] = window;
        start_y += height + delta_y;
        if (start_y + height >= max_y) {
            start_y = 1;
            start_x = width + delta_x;
        }
    }

    start_y = 3;
    start_x = 1;
    std::string hunger;
    std::string sanity;
    std::string status;

    WINDOW *fork_win = newwin(fork_height, fork_width, fork_start_y, fork_start_x);
    fork_start_x = 1;
    fork_start_y = 3;

    for (int i = 0; i < phils.size(); i++) {
        //wclear(fork_win);
        box(fork_win, 0, 0);
        mvwprintw(fork_win, fork_start_y, fork_start_x, tbls[i].getName().c_str());
        fork_start_y += height;
        if (fork_start_y >= max_y + delta_y) {
            fork_start_y = 3;
            fork_start_x += delta_x / 2;
        }
        wrefresh(fork_win);
    }

    while((getch()) != 27) {
        for (int i = 0; i < phils.size(); i++) {
            wclear(windows[i]);
            box(windows[i], 0, 0);
            mvwprintw(windows[i], start_y, start_x, phils[i].getName().c_str());
            hunger = "hunger: " + std::string(phils[i].getHunger() + 1, '|');
            sanity = "sanity: " + std::string(phils[i].getSanity() + 1, '|');
            status = "status: " + phils[i].getStatus();
            mvwprintw(windows[i], start_y - win_delta_y, start_x + win_delta_x, status.c_str());
            mvwprintw(windows[i], start_y, start_x + win_delta_x, hunger.c_str());
            mvwprintw(windows[i], start_y + win_delta_y, start_x + win_delta_x, sanity.c_str());
            wrefresh(windows[i]);
        }
    }

    endwin();
}


int main() {

    std::thread screenT(screen);

    bool *priors = new bool[ARG];
    for (int i = 0; i < ARG; i++) {
        priors[i] = false;
    }

    auto *cVars = new std::condition_variable[ARG];
    auto *muts = new std::mutex[ARG];

    std::vector<bool *> pVect;
    for (int i = 0; i < ARG; i++) {
        pVect.push_back(&priors[i]);
    }

    std::vector<std::condition_variable *> cvVect;
    for (int i = 0; i < ARG; i++) {
        cvVect.push_back(&cVars[i]);
    }

    waiter w(cvVect, pVect);

    for (int i = 0; i < ARG - 1; i++) {
        phils.emplace_back(i, "P" + std::to_string(i + 1), w, priors[i], cVars[i],
                           tbls[i], tbls[i + 1], muts[i]);
    }
    phils.emplace_back(ARG - 1, "P" + std::to_string(ARG), w,
                       priors[ARG - 1], cVars[ARG - 1],
                       tbls[ARG - 1], tbls[0], muts[ARG - 1]);

    std::vector<std::thread> philThs;
    philThs.reserve(ARG);
    for (int i = 0; i < ARG; i++) {
        philThs.emplace_back(phils[i].start());
    }

    w.init();

    screenT.join();

    for (int i = 0; i < ARG; i++) {
        phils[i].finish();
    }

    w.notifyAll();

    for (int i = 0; i < ARG; i++) {
        philThs[i].join();
    }

    return 0;
}
