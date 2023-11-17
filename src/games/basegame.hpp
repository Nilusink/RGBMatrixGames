namespace base
{
    struct point_t
    {
        int x;
        int y;
    };

    class BaseGame
    {
        public:
            /**
             * @brief iterate one step
             * 
             * @param delta time since last call (time passed for the game)
             * @return true game contniue
             * @return false game end | error
             */
            virtual bool step(double delta);

            /**
             * @brief render the current game status to the matrix
             * 
             */
            virtual void render();
            virtual void game_over_screen();
            virtual void reset();
    };
}
