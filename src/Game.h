class Game {
private:

public:
    Game(); // constructor
    ˜Game(); // this is a destructor
    void Initialize();
    void Run();
    void Update();
    void Render();
    void Destroy();
    void ProcessInput();

};