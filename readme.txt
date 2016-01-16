IMPLEMENTATION SPECIFICATIONS

Action priorities in descending order:
1. Picking of a control point
2. Clicking inside a curve's convex hull
3. Clicking somewhere else (i.e. not on a control point and not inside a convex hull of some curve) in order to zoom-in\zoom-out

Implementation:
1. zoom-im\zoom-out: click and hold the right mouse button outside of all ocnvex hulls and not on any control point. Moving the mouse upwards from the bottom of the screen zooms in, moving the mouse downwards from the top of the screen zooms out.
2. At any time, hitting 'r' or 'R' on the keyboard resets the scene.
3. At any time, hitting one of the numbered keys on the keyboard will reset the scene, but each curve will have inner control points as the number that has been pressed, i.e. if the user hits '5', then the curve will have 7 points - 2 external (start and end), 5 internal.
4. Curve removal: if there are more than 2 curves, you can delete a curve by left-clicking its leftmost control point. The deletion takes place after the user releases the left mouse button. If an inner curve is removed, point p_n of the curve to the left gets clamped to point p_0 of the curve to the right.
5. Convex hull selection - if the user clicks on more than one convex hull, a single convex hull will be selected arbitrarily, so only one curve can be affected.
6. 






TODOS
1. control points should stick to the mouse pointer
2. fix scene rotation
3. 3D object looks like a blob of color instead of having a nice soft transition of matte color
4/ 3D object has some transperancy