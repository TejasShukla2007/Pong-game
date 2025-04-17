#include <iostream>
#include <raylib.h>

using namespace std;
int player_score=0,cpu_score=0;
Color Blue = Color{43,100,211,255};
Color Yellow = Color{240,180,13,255};
class Ball{
    public:
        float x,y;
        int speed_x,speed_y;
        int radius;
        void Draw(){

            DrawCircle(x,y,radius,Yellow);

        }
        void Update(){
            x+=speed_x;
            y+=speed_y;

            if((y+radius>=GetScreenHeight()) || (y-radius<=0)){
                speed_y*=-1;
            }

            if(x+radius>=GetScreenWidth()){
                cpu_score++;
                ResetBall();
            }
            
            if(x-radius<=0){
                player_score++;
                ResetBall();
            }
        }
        void ResetBall(){
            x = GetScreenWidth()/2;
            y = GetScreenHeight()/2;
            int speed_choices[] = {-1,1};
            speed_x *= speed_choices[GetRandomValue(0,1)];
            speed_y *= speed_choices[GetRandomValue(0,1)];
        }
};
Ball ball;

class Paddle{
    protected:
        void LimitMovement(){
            if(y<=0){
                y=0;
            }
            if(y>=GetScreenHeight()-height){
                y = GetScreenHeight()-height;
            }
        }
    public:
        float x,y;
        float width,height;
        int speed;
    void Draw(){
        DrawRectangleRounded(Rectangle{x,y,width,height},0.8,0,WHITE); 
    }
    void Update(){
        if(IsKeyDown(KEY_UP)){
            y-=speed;
        }
        if(IsKeyDown(KEY_DOWN)){
            y+=speed;
        }
        LimitMovement();
    }
    
};

class CpuPaddle:public Paddle{
    public:
        void Update(int ball_y){
            if((y+(height/2))> ball_y){
                y=y-speed;
            }
            if((y+(height/2)) <= ball_y){
                y=y+speed;
            }
            LimitMovement();
        }
        
};
Paddle player;
CpuPaddle cpu;
int main () {

    cout<<"Starting the game"<<endl;
    const int screen_width = 1400, screen_height = 850;
    InitWindow(screen_width,screen_height,"Pong game");
    SetTargetFPS(60);

    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width -10;
    player.y = screen_height/2-player.height/2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 6;


    while(WindowShouldClose()==false){
        BeginDrawing();
        
        
        


        //Drawing
        ClearBackground(Blue);
        DrawLine(screen_width/2,0,screen_width/2,screen_height,WHITE);
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("CPU: %i",cpu_score),screen_width/4-120,20,80,WHITE);
        DrawText(TextFormat("Player: %i",player_score),3*screen_width/4-140,20,80,WHITE);
        
        //Updating
        
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        //Checking for collision
        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{player.x,player.y,player.width,player.height})){
                ball.speed_x*=-1;
        }

        if(CheckCollisionCircleRec(Vector2{ball.x,ball.y},ball.radius,Rectangle{cpu.x,cpu.y,cpu.width,cpu.height})){
            ball.speed_x*=-1;
        }
        
        
        
        EndDrawing();
    }

    CloseWindow();
}