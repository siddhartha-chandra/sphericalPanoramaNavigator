#include <iostream>
#include <CImg.h>
#include <string>
#include <cmath>
#include <utility>
#include <vector>

#define PI 3.14159
#define DEG2RAD 3.14159/180
#define RAD2DEG 180/3.14159

using namespace std;
using namespace cimg_library;

void toggle_gridVisible(bool *toggle)
{

  *toggle = !(*toggle);

}


int main(int argc, char * argv[])
{

  if (argc!=2)
  {
    cout<<"Usage: "<<endl;
    cout<<"./nav <catadioptric image file>"<<endl;
    exit(1);
  }

  CImg<unsigned char> image(argv[1]);
  int height = image.height();
  int width = image.width();


  CImg<unsigned char> visu(image);
  CImgDisplay cataDisp;
  CImgDisplay perspDisp;

  image.display(cataDisp);
  int clicked =0;

  /*calculate following:
  1. cx, cy - cataImg centre (based on user input)
  2. R1, R2 - inner and outer radius of cataImg (based on user input)
  3. height_persp, width_persp - height and width of perspective image

  */


  int cx, cy, R1, R2, height_persp, width_persp;

  
  cx = width/2;
  cy = height/2;
  
  R2 = cx>=cy?cy:cx;
  R1 = 0;
  height_persp = R2-R1;
  width_persp = 2 * PI * R2;

  double th=0;

  unsigned char col[] = {0, 0 , 255};
  CImg<unsigned char> persp(width_persp,height_persp,1,3,0);

  perspDisp.empty();
  persp.display(perspDisp);

  int i,j;

  /*calculations for perspecive projected image

  1. r - radial distance of pixel (r=R1 for y=0 and r=R2 for y=height)
  2. theta - lies in the range of 0 to 2PI ...0 for x=0 and 2Pi for x = width
  3. xs - the correspondnig x-coordinate from cataImg
  4. ys - the correspondnig y-coordinate from cataImg
  */

  double r, theta, x_s, y_s;
  double val[3];
  //while (!cataDisp.is_closed() && !perspDisp.is_closed())
  //{
    for (i = 0; i < width_persp; ++i)
    {
      for (j = 0; j < height_persp; ++j)
      {
        r =  ((j/double(height_persp)) * height_persp) + R1;
        theta = (i/double(width_persp)) * 2*PI;
        //theta -= PI/4;

        x_s = cx + r*sin(theta);
        y_s = cy + r*cos(theta);

        val[0] = image.linear_atXY(x_s,y_s,0);
        val[1] = image.linear_atXY(x_s,y_s,1);
        val[2] = image.linear_atXY(x_s,y_s,2);

        // persp(i, height_persp-j,0)=val[0];
        // persp(i, height_persp-j,1)=val[1];
        // persp(i, height_persp-j,2)=val[2];

        persp(i, j,0)=val[0];
        persp(i, j,1)=val[1];
        persp(i, j,2)=val[2];

        visu.draw_circle((int)x_s, (int)y_s, 0.5, col, 0.1, 0.1);

      }
        
    }
    
    visu.display(cataDisp);
    perspDisp.empty();
    
    persp.display(perspDisp);
    perspDisp.set_title("What the eye sees");
    
    while(!perspDisp.button() && !perspDisp.is_closed() && !cataDisp.is_closed())
    {
      perspDisp.wait();
    }
    //string saveImg = "persp_" + string(argv[1]);
    persp.save("perspective.jpg");

//main ends here
}
