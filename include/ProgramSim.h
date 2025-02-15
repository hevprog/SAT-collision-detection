#include <raylib.h>
#include <raymath.h>

#define col WHITE

#ifndef prog
#define prog
float dotproduct(Vector2 a, Vector2 b);
Vector2 rotationalV(Vector2 a,Vector2 center,float n);
bool operator==(Color &a, const Color &b);
struct shape
{
    public:
        Color color = WHITE;
        Vector2 init_pos;
        Vector2 rotation(float angle);
        virtual void drawshape()=0;
        virtual void set_pos(Vector2 a)=0;
        virtual Vector2 get_pos()=0;
};

class  circle: public shape
{
protected:
    int rad;
public:
     circle(int _rad, Vector2 pos){
        rad = _rad;
        init_pos = pos;
     }
     circle(){
        rad = 30;
        init_pos = {400,200};
    }
    void drawshape(){
        if(color == WHITE)
            DrawCircleLinesV(init_pos,rad, col);
        else{
            drawshape(color);
        }
    }
    void drawshape(Color a){
        DrawCircleLinesV(init_pos,rad, a);
    }

    Vector2 get_pos(){
        return init_pos;
    }
    void set_pos(Vector2 a){
        init_pos = a;
    }
    float get_rad(){
        return rad;
    }
};
class rectangle: public shape{
    protected:
        Vector2 size;
        Vector2 centerRotation;
        Vector2 vert [4];
    public:

        rectangle(){
            init_pos = {500,300};
            size = {100,100};
            setvert(size);
            centerRotation = avCenter();
        }
        rectangle(Vector2 pos, Vector2 _size){
            init_pos = pos;
            size = _size;
            setvert(_size);
            centerRotation = avCenter();
        }
        void drawshape(){
            if(color == WHITE){
                for(int i = 0; i<=2;i++){
                    DrawLineV(vert[i],vert[i+1],col);
                }
                DrawLineV(vert[3],vert[0],col);
            }else{
                drawshape(color);
            }
        }
        void drawshape(Color a){
            for(int i = 0; i<=2;i++){
             DrawLineV(vert[i],vert[i+1],a);
            }
            DrawLineV(vert[3],vert[0],a);
         }
        void set_pos(Vector2 a){
            init_pos = a;
        }
        Vector2 get_pos(){
            return init_pos;
        }
        void setvert(Vector2 _size){
            vert[0] = get_pos();
            vert[1] = {get_pos().x + _size.x, get_pos().y};
            vert[2] = {get_pos().x + _size.x, get_pos().y -_size.y};
            vert[3] = {get_pos().x, get_pos().y -_size.y};
        }
        Vector2 avCenter(){
            Vector2 x;
            x.x = (get_pos().x + get_pos().x+size.x)/2;
            x.y = (get_pos().y + get_pos().y-size.y)/2;
            
            return x;
        }
        void rotate(float a){
            for(int i = 0;i<=3;i++){
                vert[i] = rotationalV(vert[i],centerRotation,a);
            }
        }
        Vector2 get_vertex(int vertex){
            return vert[vertex%4];
        }
};
class player{
    protected:
        Vector2 position;
        Vector2 velocity;
        float speed = 100;
        float rotation;
    public:
        Vector2 controls();
        void update(Vector2 b);
        Vector2 get_playerpos();
        void drawplayer();
        void drawplayer(float b);
};
class playerCircle: public player,public circle{
    public:
        playerCircle(){
            //init_pos = {20,20};
            position = {20,20};
        }
        void drawplayer();
        void drawplayer(float b);
};
class playerRec: public player,public rectangle{
    public:
        playerRec(){
            init_pos = {20,20};
            setvert({100,100});
            centerRotation = avCenter();
        }
        playerRec(Vector2 pos){
            init_pos = pos;
            setvert({100,100});
            centerRotation = avCenter();
        }
        void drawplayer();
        void drawplayer(Vector2 size);
        void update(Vector2 in);
};

bool operator==(Color &a, const Color &b){ //for collision debugging
    return (a.a == b.a) &&(a.b == b.b)&&(a.g ==b.g)&&(a.r==b.r);
}
#endif //end prog 

//interfaces

/** About the project and iscollideRec functions:
 * Need to understand by pass by reference of rectangles paramenter in project and iscollideRec works, Maybe it has smth to do with how cache works?
 * in contrast to the iscollideCircle func which does not need to be passing reference. Why?
 */
void project(rectangle &a, Vector2 normal,float &min,float &max);//why pass by reference? when I dont modify the rec itself but access the internal data
bool iscollideRec(rectangle &a, rectangle &b);// same here.

bool iscollideCircle(circle b,circle a);
Vector2 minimumvalue(circle b);
Vector2 maximumvalue(circle b);
