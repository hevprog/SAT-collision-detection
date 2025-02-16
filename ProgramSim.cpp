
#include <raylib.h>
#include <ProgramSim.h>



class initshapes{
    public:
        circle c1 = circle();
        rectangle r1;
        rectangle r2 = rectangle({400,400},{200,60});
};
//functions

float dotproduct(Vector2 a, Vector2 b){
    return ((a.x*b.x)+(a.y*b.y));
}


Vector2 rotationalV(Vector2 a,Vector2 center,float n){ //rotational vector based on the rotational matrix model in wikipedia.
    Vector2 rot;
    rot.x = center.x+(a.x -center.x)*cos(n) - (a.y-center.y)*sin(n);
    rot.y = center.y+(a.x -center.x)*sin(n) + (a.y-center.y)*cos(n);
    return rot; //returns a rotational vector

}
//basic collision for circles
bool iscollideCircle(circle a,circle b){
    float distance = Vector2Distance(a.get_pos(), b.get_pos());
    return distance < (a.get_rad() + b.get_rad());
}

//collision circle and rec
Vector2 closestPointOnLine(Vector2 v1, Vector2 v2, Vector2 p) {
    Vector2 lineVec = Vector2Subtract(v2, v1);
    Vector2 pointVec = Vector2Subtract(p, v1); //only get the direction of v1 from the p's position
    float t = dotproduct(pointVec, lineVec) / dotproduct(lineVec, lineVec);
    t = (t < 0) ? 0 : (t > 1) ? 1 : t; //Only range between 0 to 1 so that I can multiply it to the vector between vertex[n] and vertex[n+1]
    Vector2 closest = Vector2Add(v1, Vector2Scale(lineVec, t));//add back the v1 to add back original positon.
    return closest;
}
bool iscollideCircleRec(circle a, rectangle &b) {
    for (int i = 0; i < 4; i++) {
        Vector2 v1 = b.get_vertex(i);
        Vector2 v2 = b.get_vertex((i + 1) % 4);
        Vector2 closestPoint = closestPointOnLine(v1, v2, a.get_pos());
        if (Vector2Distance(a.get_pos(), closestPoint) <= a.get_rad()) {
            return true;
        }
    }

    return false;
}


//collision for Rectangle usig SAT
void project(rectangle &a, Vector2 normal,float &min,float &max){
    min = INFINITY;
    max = -INFINITY;
    for(int i = 0; i<4;i++){
        float project = dotproduct(a.get_vertex(i),normal);
        min = (min>project)?project:min;
        max = (max<project)?project:max;
    }
}
bool iscollideRec(rectangle &a, rectangle &b){
    Vector2 normal;
    float aMin,aMax,bMin,bMax;
    

        for(int i = 0; i<4;i++){
            normal = Vector2Subtract(a.get_vertex((i+1)%4), a.get_vertex(i));
            normal = {-normal.y,normal.x};
            //DrawLineV({500,500},Vector2Add(normal,{500,500}),(i==4)?GREEN:ORANGE); //Debugger
            project(a,normal,aMin,aMax);
            project(b,normal,bMin,bMax);
            if(aMin > bMax || bMin > aMax){
                return false;
            }
        }
        for(int i = 0; i<4;i++){
            normal = Vector2Subtract(b.get_vertex((i+1) % 4), b.get_vertex(i));
            normal = {-normal.y,normal.x};
            project(a,normal,aMin,aMax);
            project(b,normal,bMin,bMax);
            if(aMin > bMax || bMin > aMax){
                return false;
            }
        }
    return true;
}



//playerclass
Vector2 player::controls(){
    Vector2 input;
    input.x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    input.y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
    return input;
}
void player::update(Vector2 in){
    velocity.x = speed*in.x*GetFrameTime();velocity.y = speed *in.y*GetFrameTime();
    position.x += velocity.x;position.y += velocity.y;
}
void playerRec::update(Vector2 in){
    velocity.x = speed*in.x*GetFrameTime();velocity.y = speed *in.y*GetFrameTime();
    for(int i = 0; i <=3;i++){
        vert[i].x += velocity.x; vert[i].y += velocity.y;
    }
    centerRotation.x += velocity.x; centerRotation.y += velocity.y;
}
Vector2 player::get_playerpos(){
    return position;
}

//Draw player shapes
void playerCircle::drawplayer(float r){
    rad = r;
    drawshape();
}
void playerRec::drawplayer(){
    drawshape();
}




int main(){
    SetTargetFPS(60);
    InitWindow(1080,720,"SAT Collision test");
    initshapes Shapes;
    playerRec PlayerRec;
    playerCircle playerCircle;
    //set
    Shapes.r2.rotate(180);



    int playerShape =2;
    while(!WindowShouldClose())
    {
        ClearBackground(BLACK);
        //processes
        if(IsKeyPressed(KEY_C)){
            playerShape++;
        }
        int rota = ((int)IsKeyDown(KEY_LEFT)-(int)IsKeyDown(KEY_RIGHT));
        PlayerRec.rotate(rota*0.01);

        //drawing
        BeginDrawing();
        DrawText("press c to switch shapes and arrow keys to rotate",20, 50,24,WHITE);
        Shapes.r1.rotate(0.005);
        Shapes.c1.drawshape();
        Shapes.r1.drawshape();
        Shapes.r2.drawshape();
        DrawCircleV(Shapes.r1.avCenter(),10,RED);
        


        //drawplayer
        switch (playerShape)
        {
        case 1: //circle
            playerCircle.drawplayer(20);
            playerCircle.update(playerCircle.controls());
            playerCircle.set_pos(playerCircle.get_playerpos());

            
            if(iscollideCircle(Shapes.c1,playerCircle)){
                Shapes.c1.color = RED;
            }else{
                Shapes.c1.color = WHITE;
            }
            if(iscollideCircleRec(playerCircle,Shapes.r1)){
                Shapes.r1.color = RED;
            }else{
                Shapes.r1.color = WHITE;
            }
            if(iscollideCircleRec(playerCircle,Shapes.r2)){
                Shapes.r2.color = RED;
            }else{
                Shapes.r2.color = WHITE;
            }
            break;
        case 2: //rectangle
            PlayerRec.drawplayer();
            PlayerRec.update(PlayerRec.controls());
            PlayerRec.set_pos(PlayerRec.get_playerpos());


            if(iscollideRec(PlayerRec,Shapes.r1)){
                Shapes.r1.drawshape(RED);

            }else{
                Shapes.r1.color = WHITE;
            }if(iscollideRec(PlayerRec,Shapes.r2)){
                Shapes.r2.drawshape(RED);

            }else{
                Shapes.r2.color = WHITE;
            }
            if(iscollideCircleRec(Shapes.c1,PlayerRec)){
                Shapes.c1.drawshape(RED);
            }else{
                Shapes.c1.color = WHITE;
            }
            break;
        default:
            playerShape = 1;
        }
        
        EndDrawing();
    }
    CloseWindow();
}
