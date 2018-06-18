/** CSci-4611 Assignment 2:  Car Soccer
 */

#include "car_soccer.h"
#include "config.h"


CarSoccer::CarSoccer() : GraphicsApp(1024,768, "Car Soccer") {
    // Define a search path for finding data files (images and shaders)
    searchPath_.push_back(".");
    searchPath_.push_back("./data");
    searchPath_.push_back(DATA_DIR_INSTALL);
    searchPath_.push_back(DATA_DIR_BUILD);
}

CarSoccer::~CarSoccer() {
}


Vector2 CarSoccer::joystick_direction() {
    Vector2 dir;
    if (IsKeyDown(GLFW_KEY_LEFT))
        dir[0]--;
    if (IsKeyDown(GLFW_KEY_RIGHT))
        dir[0]++;
    if (IsKeyDown(GLFW_KEY_UP))
        dir[1]++;
    if (IsKeyDown(GLFW_KEY_DOWN))
        dir[1]--;
    return dir;
}

float rand_FloatRange(float a, float b){

    return ((b-a)*((float)rand()/RAND_MAX))+a;
}

void CarSoccer::launch() {

    float randomX = rand_FloatRange(1, -1);
    float randomY = rand_FloatRange(1, 0.1);
    float randomZ = rand_FloatRange(1, 0.1);
    
    speed_ = 40.0;
    accel = -1;
    ball_.set_velocity(Vector3(randomX,randomY,randomZ).ToUnit());
    ball_.set_acceleration(Vector3(0,-1,0));
    std::cout << "Launch Vector: " << ball_.velocity()*10 << std::endl;
}

void CarSoccer::OnSpecialKeyDown(int key, int scancode, int modifiers) {
    if (key == GLFW_KEY_SPACE) {
        // Here's where you could call some form of launch_ball();
       // car_.Reset();
        ball_.Reset();
        
        launch();
        time_ = 0.0;
    }
}

// Used to calc ball position for drawing debugging vectors
Point3 CarSoccer::CalcBallPos(double t) {
    float e = t;
    return ball_.position() + ball_.velocity() * e;
}

// used
Vector3 CarSoccer::CalcBallVelocity(double t) {
    float e = t;
    return ball_.velocity() + ball_.acceleration() * e;

}

Vector3 CarSoccer::collision(Vector3 normal, Vector3 velocity) {
    //normal.ToUnit();
    //velocity.ToUnit();

    float dot = velocity.Dot(normal);
    return velocity - 2 * (dot) * normal;
}
void CarSoccer::UpdateSimulation(double timeStep) {
    // Here's where you shound do your "simulation", updating the positions of the
    // car and ball as needed and checking for collisions.  Filling this routine
    // in is the main part of the asssignment.
    time_ += timeStep;

    // Find new velocity
    // v' = v + a * deltaTime
    Vector3 vel = CalcBallVelocity(timeStep);
    // Set new velocity
    ball_.set_velocity(vel);
    // Set new position
    // p' = p + v * deltaTime
    
    Vector2 carDir = joystick_direction();
    carDir = 0.2 * carDir.ToUnit();
    Vector3 CarDirection = Vector3(carDir[0],0,-carDir[1]);
    CarDirection = CarDirection.ToUnit();
    
    if (speed_ != 0) {
        ball_.set_position(ball_.position() + vel * (timeStep * speed_)) ;
    }
    if (speed_ == 0) {
   
        ball_.set_velocity(Vector3(0,0,0));
        ball_.set_acceleration(Vector3(0,-1,0));
    }
    // Grab direciton of car from joystick

    
    // Ground Normal
    Vector3 groundNorm = Vector3(0,1,0);
    // Distance from ball to a point on the ground plane
    Vector3 groundCollision = Vector3(ball_.position() - Point3(0,0,0));
    // Calculate the distance from the center of the ball to the ground plane
    float groundDistance = groundNorm.Dot(groundCollision);
    // Ground Collision
    if (groundDistance <= ball_.radius()) {
        // velocity vector reflected across the normal
        Vector3 newVeloc = (collision(groundNorm, ball_.velocity()));
        // set ball position so it does not penetrade plane
        ball_.set_position(ball_.position() + .1 * newVeloc.ToUnit());
        // set new velocity
        ball_.set_velocity(newVeloc);
        
        if (speed_ >= 2) {
            accel = accel - 1.25;
            speed_ = speed_ /1.25;
            ball_.set_acceleration(Vector3(0,accel,0));
        }
        if (speed_ > 0 && speed_ < 3) {
            speed_ = 0;
        }

    }
    if (!carCollision_) {
        car_.set_position(car_.position() + Vector3(carDir[0],0,-carDir[1]));
    }
    
    Vector3 southWallNormal = Vector3(0,0,-1);
    Vector3 southWallCollision = Vector3(ball_.position()- Point3(0,0,50));
    float southWallDistance = southWallNormal.Dot(southWallCollision);
    // BALL // South Wall Collision
    if (southWallDistance <= ball_.radius()) {
        Vector3 newVeloc = (collision(southWallNormal, ball_.velocity()));
        ball_.set_position(ball_.position() + .1 * newVeloc.ToUnit());
        ball_.set_velocity(newVeloc);
    }
    // CAR
    Vector3 southWallCollisionCar = Vector3(car_.position() - Point3(0,0,50));
    float southWallDistanceCar = southWallNormal.Dot(southWallCollisionCar);
    if (southWallDistanceCar <= car_.collision_radius()) {
        carCollision_ = true;
        //std::cout << "Car southwall collision" << std::endl;
        if (carDir[1] < 0) {
            carCollision_ = true;
        }
        else {
            car_.set_position(car_.position() + .1 * Vector3(carDir[0],0,-carDir[1]));
            carCollision_ = false;

        }
    }

    Vector3 northWallNormal = Vector3(0,0,1);
    Vector3 northWallColiision = Vector3(ball_.position()- Point3(0,0,-50));
    
    float northWallDistance = northWallNormal.Dot(northWallColiision);
    if (northWallDistance <= ball_.radius()) {

        Vector3 newVeloc = (collision(northWallNormal, ball_.velocity()));
        ball_.set_position(ball_.position() + .1 * newVeloc.ToUnit());
        ball_.set_velocity(newVeloc);
    }
    Vector3 northWallCollisionCar = Vector3(car_.position() - Point3(0,0,-50));
    float northWallDistanceCar = northWallNormal.Dot(northWallCollisionCar);
    if (northWallDistanceCar <= car_.collision_radius()) {
        carCollision_ = true;
        //std::cout << "Car northwall collision" << std::endl;
        if (carDir[1] > 0) {
            carCollision_ = true;

        }
        else {
            car_.set_position(car_.position() + .1 * Vector3(carDir[0],0,-carDir[1]));
            carCollision_ = false;
        }
    }
    
    
    Vector3 westWallNormal = Vector3(1,0,0);
    Vector3 westWallColiision = Vector3(ball_.position()- Point3(-40,0,0));
    
    float westWallDistance = westWallNormal.Dot(westWallColiision);
    if (westWallDistance <= ball_.radius()) {
        
        Vector3 newVeloc = (collision(westWallNormal, ball_.velocity()));
        ball_.set_position(ball_.position() + .1 * newVeloc.ToUnit());
        ball_.set_velocity(newVeloc);
    }
    
    
    Vector3 eastWallNormal = Vector3(-1,0,0);
    Vector3 eastWallColiision = Vector3(ball_.position()- Point3(40,0,0));
    
    float eastWallDistance = eastWallNormal.Dot(eastWallColiision);
    if (eastWallDistance <= ball_.radius()) {
        
        Vector3 newVeloc = (collision(eastWallNormal, ball_.velocity()));
        ball_.set_position(ball_.position() + .1 * newVeloc.ToUnit());
        ball_.set_velocity(newVeloc);
    }
    Vector3 ceilingNormal = Vector3(0,-1,0);
    Vector3 ceilingColiision = Vector3(ball_.position()- Point3(0,35,0));
    
    float ceilingDistance = ceilingNormal.Dot(ceilingColiision);
    if (ceilingDistance <= ball_.radius()) {
        
        Vector3 newVeloc = (collision(ceilingNormal, ball_.velocity()));
        ball_.set_position(ball_.position() + .1 * newVeloc.ToUnit());
        ball_.set_velocity(newVeloc);
    }
    

    Vector3 collisionNorm = Vector3(ball_.position() - car_.position());
    collisionNorm = collisionNorm.ToUnit();

    Vector3 collision2 = Vector3(car_.position() - ball_.position());
    if (collision2.Length() <= (ball_.radius() + car_.collision_radius())) {
        Vector3 relVelocity = ball_.velocity() - CarDirection;
        Vector3 newVeloc = (collision(collisionNorm, relVelocity));
        ball_.set_position(ball_.position() + 2* newVeloc);
        ball_.set_velocity(newVeloc);
        speed_ = 40.0;
        ball_.set_acceleration(Vector3(0,-1,0));
        accel = -1;

    }

}


void CarSoccer::InitOpenGL() {
    // Set up the camera in a good position to see the entire field
    projMatrix_ = Matrix4::Perspective(60, aspect_ratio(), 1, 1000);
    modelMatrix_ = Matrix4::LookAt(Point3(0,60,70), Point3(0,0,10), Vector3(0,1,0));
 
    // Set a background color for the screen
    glClearColor(0.8,0.8,0.8, 1);
    
    // Load some image files we'll use
    fieldTex_.InitFromFile(Platform::FindFile("pitch.png", searchPath_));
    crowdTex_.InitFromFile(Platform::FindFile("crowd.png", searchPath_));
    
    accel = -1.0;

    // launch with initial random direction vector
    launch();

}


void CarSoccer::DrawUsingOpenGL() {
    // Draw the crowd as a fullscreen background image
    quickShapes_.DrawFullscreenTexture(Color(1,1,1), crowdTex_);
    
    // Draw the field with the field texture on it.
    //Color col(16.0/255.0, 46.0/255.0, 9.0/255.0);
    //Matrix4 M = Matrix4::Translation(Vector3(0,-0.201,0)) * Matrix4::Scale(Vector3(50, 1, 60));
    //quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, col);
    //M = Matrix4::Translation(Vector3(0,-0.2,0)) * Matrix4::Scale(Vector3(40, 1, 50));
    //quickShapes_.DrawSquare(modelMatrix_ * M, viewMatrix_, projMatrix_, Color(1,1,1), fieldTex_);
    
    // Draw the car
    Color carcol(0.8, 0.2, 0.2);
    Matrix4 Mcar =
        Matrix4::Translation(car_.position() - Point3(0,0,0)) *
        Matrix4::RotationY(-joystick_direction()[0]) *
        Matrix4::Scale(car_.size()) *
    Matrix4::Scale(Vector3(0.5,0.5,0.5));
    quickShapes_.DrawCube(modelMatrix_ * Mcar, viewMatrix_, projMatrix_, carcol);

    
    //Draw boundary
    Matrix4 m_loop;
    std::vector<Point3> loop;
    
    loop.push_back(Point3( 40.0, 0.0, -50.0));
    loop.push_back(Point3( 40.0, 0.0, 50.0));
    loop.push_back(Point3(-40.0, 0.0, 50.0));
    loop.push_back(Point3(-40.0, 0.0, -50.0));
    quickShapes_.DrawLines(modelMatrix_ * m_loop, viewMatrix_,projMatrix_, Color(100,0,0), loop, QuickShapes::LinesType::LINE_LOOP, 0.1);
    Matrix4 trans = Matrix4::Translation(Vector3(0,35,0));
    quickShapes_.DrawLines(modelMatrix_ * m_loop * trans, viewMatrix_,projMatrix_, Color(100,0,0), loop, QuickShapes::LinesType::LINE_LOOP, 0.1);

    Matrix4 m_loop2;
    std::vector<Point3> loop2;
    
    loop2.push_back(Point3( 40.0, 0.0, -50.0));
    loop2.push_back(Point3( 40.0, 0.0, 50.0));
    loop2.push_back(Point3(40.0, 35.0, 50.0));
    loop2.push_back(Point3(40.0, 35.0, -50.0));
    quickShapes_.DrawLines(modelMatrix_ * m_loop2, viewMatrix_,projMatrix_, Color(100,0,0), loop2, QuickShapes::LinesType::LINE_LOOP, 0.1);
    
    Matrix4 trans2 = Matrix4::Translation(Vector3(-80,0,0));
    quickShapes_.DrawLines(modelMatrix_ * m_loop2 * trans2, viewMatrix_,projMatrix_, Color(100,0,0), loop2, QuickShapes::LinesType::LINE_LOOP, 0.1);
    
    //Draw Goals
    Matrix4 m_goal;
    std::vector<Point3> loop_goal;
    loop_goal.push_back(Point3( 10.0, 0.0, -50.0));

    loop_goal.push_back(Point3( 10.0, 10.0, -50.0));
    loop_goal.push_back(Point3( -10.0, 10.0, -50.0));
    loop_goal.push_back(Point3( -10.0, 0.0, -50.0));
    
    //Grid Loop vertical
    for (float i = -10.0; i < 10; i++) {
        loop_goal.push_back(Point3( i, 0.0, -50.0));
        loop_goal.push_back(Point3( i, 10.0, -50.0));
        loop_goal.push_back(Point3( i, 0.0, -50.0));
    }
    //Grid loop horizontal
    for (float i = -0.0; i < 10; i++) {
        loop_goal.push_back(Point3( -10, i, -50.0));
        loop_goal.push_back(Point3( 10, i, -50.0));
        loop_goal.push_back(Point3( -10, i, -50.0));
    }
    loop_goal.push_back(Point3( 10.0, 9, -50.0));
    quickShapes_.DrawLines(modelMatrix_ * m_goal, viewMatrix_,projMatrix_, Color(0.5,0.80,0.92), loop_goal, QuickShapes::LinesType::LINE_LOOP, 0.1);
    
    quickShapes_.DrawLines(modelMatrix_ * m_goal * (Matrix4::Translation(Vector3(0,0,100))), viewMatrix_,projMatrix_, Color(0.5,0.80,0.92), loop_goal, QuickShapes::LinesType::LINE_LOOP, 0.1);

    
    
    // Draw the ball
    Color ballcol(1,1,1);
    Matrix4 Mball =
        Matrix4::Translation(ball_.position() - Point3(0,0,0)) *
        Matrix4::Scale(Vector3(ball_.radius(), ball_.radius(), ball_.radius()));
    quickShapes_.DrawSphere(modelMatrix_ * Mball, viewMatrix_, projMatrix_, ballcol);
    
    
    // Draw the ball's shadow -- this is a bit of a hack, scaling Y by zero
    // flattens the sphere into a pancake, which we then draw just a bit
    // above the ground plane.
    Color shadowcol(0.2,0.4,0.15);
    Matrix4 Mshadow =
        Matrix4::Translation(Vector3(ball_.position()[0], -0.1, ball_.position()[2])) *
        Matrix4::Scale(Vector3(ball_.radius(), 0, ball_.radius())) *
        Matrix4::RotationX(90);
    quickShapes_.DrawSphere(modelMatrix_ * Mshadow, viewMatrix_, projMatrix_, shadowcol);
    
    
    // You should add drawing the goals and the boundary of the playing area
    // using quickShapes_.DrawLines()
    
    //Debugging
    
    //Matrix4 axis =
      //  Matrix4::Scale(Vector3(10,10,10));
    //quickShapes_.DrawAxes(modelMatrix_ * axis, viewMatrix_, projMatrix_);
    
    Vector3 BallDir = CalcBallPos(time_+ 0.1) - CalcBallPos(time_);
    BallDir = 5 * BallDir.ToUnit();
    quickShapes_.DrawArrow(modelMatrix_, viewMatrix_, projMatrix_, Color(50,50,0), ball_.position(),BallDir, 0.1);
    
    Vector2 dir2d = joystick_direction();
    Vector3 CarDir = Vector3(dir2d[0],0,-dir2d[1]);
    CarDir = 5.0 * CarDir.ToUnit();
    quickShapes_.DrawArrow(modelMatrix_,viewMatrix_, projMatrix_, Color(50,50,0),car_.position(), CarDir, 0.1);
}
