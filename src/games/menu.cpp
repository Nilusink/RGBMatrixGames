#include "controller.hpp"
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


bool Menu::settingsPage(uint8_t button_presses)
{
    if (button_presses & JOY_UP)
        selected_item--;
    
    else if (button_presses & JOY_DOWN)
        selected_item++;

    printf("Selected: %d\n", selected_item);

    if (selected_item < 0)
        return false;
    
    m.fillRect(0, 0, 64, 32, matrix::rgb(BG_COLOR));

    // general settings
    m.setTextColor(matrix::rgb(FG_COLOR));

    uint i_setting = 0;
    // for (auto &[name, value]: g[current_page]->getSettings())

    std::vector<std::pair<const char*, std::pair<double, double>>> settings_values = g[current_page]->getSettings();
    
    printf("size: %d", settings_values.size());
    if (selected_item >= settings_values.size())
        selected_item = settings_values.size() - 1;

    // change settings
    if (button_presses & JOY_LEFT)
    {
        settings_values[selected_item].second.first -= settings_values[selected_item].second.second;
        g[current_page]->setSetting(selected_item, settings_values[selected_item].second.first);

    }

    else if (button_presses & JOY_RIGHT)
    {
        settings_values[selected_item].second.first += settings_values[selected_item].second.second;
        g[current_page]->setSetting(selected_item, settings_values[selected_item].second.first);
    }

    for (; i_setting < settings_values.size(); i_setting++)
    {
        const char *name = settings_values[i_setting].first;
        double value = settings_values[i_setting].second.first;
        m.setCursor(2, 3 + 10 * i_setting);

        if (i_setting == selected_item)
        {
            m.setTextColor(matrix::rgb(BG_COLOR));
            m.fillRect(0, 3 + 10 * i_setting - 1, 64, 9, matrix::rgb(FG_COLOR));
        }
        else
        {
            m.setTextColor(matrix::rgb(FG_COLOR));
        }
        m.printf("%s: %.2lf", name, value);
    }

    return true;
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

    // reset menu values
    selected_item = 0;
}


base::BaseGame &Menu::getGame()
{
    return *g[current_page];
}