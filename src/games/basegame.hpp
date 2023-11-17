namespace base
{
    class BaseGame
    {
        public:
            virtual bool step(double delta);
            virtual void render();
            virtual void game_over_screen();
            virtual void reset();
    };
}
