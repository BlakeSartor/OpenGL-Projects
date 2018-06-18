# My solution to Assignment 2


README.txt
CSci-4611 Assignment #2 Car Soccer
Created by Blake Sartor - sarto019@umn.edu

This program allows the user to control the car (red rectangle) and hit the ball across the field.
By pressing spacebar the ball is launched toward the user at a random intial velocity.

The balls position is updated based on the velocity and delta time. The velocity
is based on a arbitrary acceleration determined. (default -1 in the z axis to simulate gravity).

A box is drawn around the border of the game field to show boundaries the ball can go.
This program detects collision betweens the ball and the planes that create the field boundaries.
If the ball hits a surface the ball is reflected across the normal of the plane the ball is colliding with.

The program also detects collision between the ball and the car. The ball is reflected across the collision normal
created by the car and the ball.

The boundaries do NOT allow the car to bass it in the NORTH and SOUTH walls, but the car can pass
the boundaries in the east and west directions. For some reason adding these checks into the program slows
the whole thing down, thus it was decided to leave it out.

Every time the ball is bounced on the ground plane only, the speed of the ball is decreased to simulate friction.

DEBUGGING:
I have included direction vectors for the ball and the car to help with debugging.
