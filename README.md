# Bézier playground

[Bézier curves](https://en.wikipedia.org/wiki/B%C3%A9zier_curve) playground.

## Dependencies
* OpenGL

## Usage
* Right-click and hold a control point to move it.
* Left-click on the left-most control point of a curve to delete it (a minimum of 2 curves would remain)/
* Left-click on the left-most _inner_ control point of a curve: imagine a line between that point and the left-most control point. The right-most control point of the curve to the left would reposition to that line.
* Middle-click and hold inside a curve's convex hull to move the curve in the XY plane.
* Right-click in a curve's convex hull to split it in half.
* Left-click in a curve's convex hull to transform the curve to a straight line.
* Zoom-in\out when moving the mouse up\down while right-clicking outside of some curve's' convex hull.
* Press `d` to create a [Bézier surface](https://en.wikipedia.org/wiki/B%C3%A9zier_surface) from the curve. Moving the mouse while right-clicking translates the scene, and while left-clicking would rotate it around the respective axes.
* Press the number keys to change the number of inner control points in the curves. This would also reset the scene.
* `esc` to exit.

You can also run this under Linux: a `makefile` is provided, and there's some compiler flag in an appropriate header file.

## Example screenshots
Initial setting:

![image 1](https://github.com/Bbenchaya/Bezier_Playground/blob/master/image1.png)



After moving a couple of control points:

![image 2](https://github.com/Bbenchaya/Bezier_Playground/blob/master/image2.png)



Transitioning to Bézier surface:

![image 3](https://github.com/Bbenchaya/Bezier_Playground/blob/master/image3.png)

## License

The MIT License (MIT)

Copyright (c) 2015 Asaf Chelouche, Ben Ben-Chaya

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
