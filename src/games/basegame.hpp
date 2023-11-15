namespace base
{
    class BaseGame
    {
        public:
            virtual bool step(double delta);
            virtual void render();
            virtual void reset();
    };
}
