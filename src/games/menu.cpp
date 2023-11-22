#include "display.hpp"
#include "menu.hpp"


using namespace menu;


Menu::Menu(
    RGB64x32MatrixPanel_I2S_DMA &mat, 
    std::array<base::BaseGame *, 2> &games
)
 : m(mat), g(games)
{}


void Menu::setPage(int page)
{
    // do nothing if OOR
    if (0 > page || page >= g.size())
        return;

    // set page and upsate
    current_page = page;
    update_page();
}


void Menu::nextPage()
{
    setPage(current_page + 1);
}
void Menu::prevPage()
{
    setPage(current_page - 1);
}


uint8_t Menu::getPage()
{
    return current_page;
}


void Menu::update_page()
{
    // clear screen
    m.fillRect(0, 0, 64, 32, matrix::rgb(BG_COLOR));

    // print current game
    m.setTextColor(matrix::rgb(FG_COLOR));
    g[current_page]->drawTitle();

    // left arrow
    if (current_page > 0)
    {
        m.fillTriangle(
            6, 13,
            1, 16,
            6, 19,
            matrix::rgb(FG_COLOR)
        );
    }

    // right arrow
    if (current_page < g.size() -1)
    {
        m.fillTriangle(
            58, 13,
            63, 16,
            58, 19,
            matrix::rgb(FG_COLOR)
        );
    }
}


void Menu::reset()
{
    // clear screen
    m.fillRect(0, 0, 64, 32, matrix::rgb(BG_COLOR));

    // update menu
    update_page();
}


base::BaseGame &Menu::getGame()
{
    return *g[current_page];
}