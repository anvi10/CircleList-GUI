#include "plot.h"
#include <assert.h>

int window1;
Points inpoints;
Points intersectionPoints;

//window configurations and registering callbacks
void graphicsInit (int argc, char **argv, int wsize, int linewidth, 
		   float pointsize)
{
  glutInit(&argc, argv);
  glutInitWindowSize(wsize, wsize);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  window1 = glutCreateWindow("OpenGlut Demo");
  glutMouseFunc(mouse);
  glutDisplayFunc(display);//callback function invoked to refresh window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, wsize, 0, wsize);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glLineWidth(linewidth);
  glPointSize(pointsize);
  menuInit();// menu displayed on right-click
  glutMainLoop();
}

void menuInit ()
{
  glutCreateMenu(menu);
  glutAddMenuEntry("clear", 1);
  glutAddMenuEntry("read points", 2);
  glutAddMenuEntry("write points", 3);
  glutAddMenuEntry("testing", 4);
  glutAddMenuEntry("exit", 5);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void menu (int value)
{
  string name;
  switch (value) {
  case 1:
    inpoints.clear();
    intersectionPoints.clear();
    glutPostRedisplay(); // marks the current window as needing to be redisplayed.
    break;
  case 2:
    readPoints(inpoints);
    glutPostRedisplay();
    break;
  case 3:
    writePoints(cerr, inpoints);
    glutPostRedisplay();
    break;
  case 4:
  {
    cout <<"Enter test1 | test2 | test3| test4\n";
    string name;
    cin >> name;

    if(name == "test1")
       plot_test1();
     else if(name == "test2")
      plot_test2();
    else if(name == "test3")
      plot_test3();
    else if(name == "test4")
      plot_test4();
    else
      cerr<<"unknown input"<<endl;

     glutPostRedisplay();
  }
break;

  case 5:
    exit(0);
  }
}


void display ()
{
  float white[3] = {1.0f, 1.0f, 1.0f};
  clearScreen(white);
  glColor3f(1.0f, 0.0f, 0.0f); // red
  drawPoints(inpoints);
  glColor3f(0.0f, 0.0f, 0.0f); // black
  drawLoop(inpoints);
  glColor3f(0.0f, 1.0f, 0.0f); // green
  drawPoints(intersectionPoints);

  glFlush();
}

// left click adds point to (global list) inpoints
// shift + left click removes point from inpoints
// alt + left click moves the closest point to the click location
// right click displays context menu

void mouse (int button, int buttonState, int x, int y)
{
  static Point oldp;
  Point p = point(x, y);
  if (button == GLUT_LEFT_BUTTON && buttonState == GLUT_UP)
    return;
  
  if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) {
    if (inpoints.size() != 0);{
      removePoint(p, inpoints);
    }
  }
  else if (button == GLUT_LEFT_BUTTON){
    if(inpoints.size() < 3) inpoints.addBack(p);
    else if (inpoints.size() != 0)
      addPoint(p,inpoints);
  }
  
 else if (buttonState == GLUT_DOWN)
    oldp = p;

  else{
   if (inpoints.size() != 0){
    updatePointLocation(p, inpoints);
  }
}
  
  glutPostRedisplay();
}

Point point (int x, int y)
{
  return Point(x, glutGet(GLUT_WINDOW_WIDTH) - y);
}

void clearScreen (float *color)
{
  glClearColor(color[0], color[1], color[2], color[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void drawPoints (const Points &points)
{
  glBegin(GL_POINTS);
  
  for(int i = 0; i < points.size(); i++) {
  	glVertex(points[i]);
  }
  
  glEnd();
}

void drawLoop (const Points &points)
{
  glBegin(GL_LINE_LOOP);
  
  for(int i = 0; i < points.size(); i++) {
  	glVertex(points[i]);
  }
  
  glEnd();
}

void glVertex (const Point &p)
{
  glVertex2d(p.getX(), p.getY());
}

// read points from file
void readPoints (Points &inpoints)
{
  cerr << "Enter filename: ";
  string name;
  cin >> name;
  ifstream istr(name.c_str());
  if (!istr.good()) return;
  inpoints.clear();
  readPoints(istr, inpoints);
}

void readPoints (istream &istr, Points &pts)
{
  int n;
  istr >> n;
  for (int i = 0; i < n; ++i) {
  double x, y;
  istr >> x >> y;
  pts.addBack(Point(x, y));
  }
}
// print out points
void writePoints (ostream &ostr, const Points &pts)
{
  ostr << pts.size() << endl;
  for (int i = 0; i < pts.size(); i++) {
  	Point p = pts[i];
    ostr << p.getX() << " " << p.getY() << endl;
  }
}

int closestPoint (const Point &p, const Points &pts)
{
  if (pts.size() ==0){
    cerr<<"function called with empty list\n";
    exit(1);
  }

  Point q =  pts[0];
  int ret = 0;
  for (int i = 1; i < pts.size(); ++i) {
    if (p.distanceSquared(pts[i]) < p.distanceSquared(q)){
      q = pts[i];
      ret = i;
    }
  }

  return ret;
}


bool leftTurn (Point a, Point b, Point c, Point d) {
// (d-c) x (a-b) > 0 to return true

int abX = b.getX() - a.getX();
int abY = b.getY() - a.getY();
int dcX = c.getX() - d.getX();
int dcY = c.getY() - d.getY();

int crossProduct = (abX * dcY) - ( abY * dcX );
cout << crossProduct;
	if (crossProduct > 0) {
		return true;
	} else {
		return false;
	}
}

void addPoint (const Point &p, Points &pts)
{
  int index = closestPoint(p, pts); //2
  Point closestPoint = pts[index]; //pts[2]
  Point leftPoint = pts[index-1]; //pts[1]
  Point rightPoint = pts[index+1]; //pts[3] == pts [2]
  int rightDistance = p.distanceSquared(rightPoint);
  int leftDistance = p.distanceSquared(leftPoint);
  
  if(rightDistance < leftDistance) {
    //add on the right
    pts.addAt(index+1, p);
  }
  else {
    //add on the left
    pts.addAt(index, p);
  }

//Begin checking for intersection points over here!
 for (int i =0; i < pts.size()-1; i ++) {
	//to make sure array index does not go out of bounds
	if (i == pts.size() - 2) {
		break;
	}
	Point p1 = pts[i];
	Point p2 = pts[i+1];
	for (int j = i+2; j < pts.size()-1; j++) {
		//make sure array index doesn't go out of bounds
		if (j == pts.size() - 2) {
			break;
		}
	Point p3 = pts[j];
	Point p4 = pts[j+1];

	bool intersects = leftTurn(p1, p2, p3, p4 ); //call left turn predicate function
//if left turn returns true, then a point of intersection does indeed exist. now let us find the point of intersection.

	if (intersects == true) {
		//call a function that will calculate the intersection point!!
	}


	}
	 
 }


}


void removePoint (const Point &p, Points &pts)
{
    //TODO 3
    //p is location of user click
    //find closest point to user click and remove that 
    int index = closestPoint(p, pts);
    pts.removeAt(index);
}

void updatePointLocation(const Point &p, Points &pts)
{
    //TODO 4
    int index = closestPoint(p, pts);
    pts.setElemAt(index, p);
}

void plot_test1(){
  cerr<<"test1 inserting without self intersection\n ";
  inpoints.clear();
  intersectionPoints.clear();

  inpoints.addBack(Point(213, 369));
  inpoints.addBack(Point(346, 149));
  inpoints.addBack(Point(65, 146));

  addPoint(Point(342, 272), inpoints);
  addPoint(Point(139, 302), inpoints);
  addPoint(Point(174, 119), inpoints);
  
  assert(inpoints[0].getX() == 139 && inpoints[0].getY() == 302);
  assert(inpoints[1].getX() ==  213 && inpoints[1].getY() == 369);
  assert(inpoints[2].getX() ==  342 && inpoints[2].getY() == 272);
  assert(inpoints[3].getX() ==  346 && inpoints[3].getY() == 149);
  assert(inpoints[4].getX() == 174 && inpoints[4].getY() == 119 );
  assert(inpoints[5].getX() == 65 && inpoints[5].getY() == 146 );
  
  cerr<<"test1 passed\n";

}

void plot_test2(){
      	cerr<<"test2 inserting with self intersection\n ";
  inpoints.clear();
  intersectionPoints.clear();
  inpoints.addBack(Point(214, 399));
  inpoints.addBack(Point(348, 232));
  inpoints.addBack(Point(128, 238));
  addPoint(Point(159, 225), inpoints);

  intersectionPoints.print();

  assert(intersectionPoints[0].getX() == 162 && intersectionPoints[0].getY()== 237);
  cerr<<"test2 passed\n";


}

void plot_test3(){
  cerr<<"test3 delete closest point\n ";
  inpoints.clear();
  intersectionPoints.clear();
  
  inpoints.addBack(Point(208, 400 ));
  inpoints.addBack(Point(421, 190  ));
  inpoints.addBack(Point(71, 169));

  removePoint(Point(249, 455), inpoints);
  assert(inpoints[0].getX() ==  421 && inpoints[0].getY() == 190);
  assert(inpoints[1].getX() == 71  && inpoints[1].getY() == 169);

  cerr<<"test3 passed\n";


}

void plot_test4(){
  cerr<<"test4 update one point\n ";
  inpoints.clear();
  intersectionPoints.clear();
  inpoints.addBack(Point(93, 411));
  inpoints.addBack(Point(402, 359 ));
  inpoints.addBack(Point(87, 235));

  updatePointLocation(Point(392,54), inpoints);

  assert(inpoints[0].getX() ==  93 && inpoints[0].getY() == 411);
  assert(inpoints[1].getX() ==  392 && inpoints[1].getY() == 54);
  assert(inpoints[2].getX() ==  87 && inpoints[2].getY() == 235);

  cerr<<"test4 passed\n";
}

int main (int argc, char **argv)
{
  //launch GUI
  int wsize = 500; 
  int linewidth = 1;
  float pointsize = 5.0f;
  graphicsInit(argc, argv, wsize, linewidth, pointsize);
  return 0;
}
