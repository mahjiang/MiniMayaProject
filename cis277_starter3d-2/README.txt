ADAM COLE
CIS-277
HW8 -- Raycasting


This version of the project updates the last by including the
ability to cast rays and select polygon faces.

1. Ray Generation
Clicking on the screen of the "myglwidget" within the GUI will
cast a ray from the view of the camera to the corresponding
world coordinates of where you clicked. Once you click somewhere
on the screen, you can move the camera view with the arrow keys
to see the projected ray. This was implemented by first
converting the screen coordinates to normalized device coordinates
and then calculating some relationships between the camera vectors and
fields of view.

2. Polygon Picking
The user can select the faces of polygons by clicking directly on them.
The program demonstrates that the face was selected by turning blue.
Only the face closest to the user will be selected. Clicking somewhere
another face or outside the polygon entirely will deselect the original
face.


3. Ray-Polygon Intersection
The implementation of the ray-polygon intersection algorithim is
demonstrated by the ability to select faces with ray casting.
I was able to forgo a planarity test because instead of checking
if the ray interescts with every face, I iterate through the polygon
by breaking it down into triangles and check if the ray
intersects one of those triangles. Since triangles are always
planar, I did not have to check if the polygon or its faces were planar.

4. Extra-Credit
Mouse Hovering:
    I implemented mouse-hovering functionality. Hovering over a face
    causes it to turn a light shade of blue. The face can then be
    selected by clicking on it which turns it to a darker shade of blue.
    The program does not run noticably slower in any way from adding this
    functionality. Hovering over a selected face does not affect the color.

Closest Vertex/Edge:
    Made progress towards implementing a "closest vertex to cursor" method.
    Successfuly activates a vertex near the cursor but does not act
    correctly in ambiguous edge cases.
