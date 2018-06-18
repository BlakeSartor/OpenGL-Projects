CSci-4611 Assignment 4: So You Think Ants Can Dance
created by Blake Sartor

This program uses mocap data from http://mocap.cs.cmu.edu
to animate ants dancing.

Two of the characters perform motion from the mocap data in a contious loop,
while the other character is in an idle animation loop until one of the five motion buttons.

Using the recursive DrawBonesRecursive function, a current transformation matrix is set for drawing
each bone. Each bone is translated from its join space to the bone space, it is then rotated based on the
current pose rotation, and is translated back to join space. The child_transform matrix is also set
using the ctm and bone direction and length.

A basic stick figure is drawn using the Drawlinesegment call at each bone with the proper length and direction.

A basic character model of an ant is applied using a multiple of matrix transformations.

The buttons that control the character on the left attempt to the best of my abilities to seamlessly extend
into the triggered animation.




