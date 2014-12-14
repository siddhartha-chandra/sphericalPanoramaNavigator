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
    cout<<"./nav <spherical panorama file>"<<endl;
    exit(1);
  }

  CImg<unsigned char> image(argv[1]);
  CImg<unsigned char> visu(image);
  CImgDisplay disp(visu);
  CImgDisplay disp2;
  CImgDisplay disp3;

  visu.display(disp);

  int rows = image.height();
  int cols = image.width();

  int row_degreeToPixlfactor= rows/180;
  int col_degreeToPixlfactor= cols/360;

  double row_radToPixlfactor= rows/PI;
  double col_radToPixlfactor= cols/(2*PI);

  int cartesianHeight = 300;
  int cartesianWidth = 300;

  //Initial Theta and phi angles
  // theta - angle in X-Y plane
  // phi - angle wrt z-axis
  // rho - distance of projection plane from center of sphere. 
  //       Initially set to (cartesianHeight/2) for 90 degree FOV

  double theta = 180;
  double phi = 90;
  double rho = cartesianHeight/2;

  unsigned char col[] = {0, 0 , 255};
  CImg<unsigned char> foveal(cartesianWidth,cartesianHeight,1,3,0);
  CImg<unsigned char> foveal1(cartesianWidth,cartesianHeight,1,3,0);

  disp2.empty();
  foveal.display(disp2);

  // disp3.empty();
  // foveal1.display(disp3);


  disp2.show_mouse ();


  //variables for mouse related events
  int refTheta=-1;
  int refPhi=-1;
  int currTheta;
  int currPhi;
  int theta_del;
  int phi_del;

  bool gridVisible = false;


  //Projected Plane related variables
  //defining projected Plane as:
  vector<vector<pair<double, double> > > projectedPlane;

  int i,j;
  for (i = 0; i<cartesianHeight; i++)
  {
    vector<pair<double, double> > temp;
    projectedPlane.push_back(temp);
    projectedPlane[i].resize(cartesianWidth);
  }

  int height = int(projectedPlane.size());
  int width = int(projectedPlane[0].size());


  int centre[2];
  centre[0] = width/2;
  centre[1] = height/2;

  vector<double> point3d(3,0);
  vector<double> point3d_rotatedZ(3,0);
  vector<double> point3d_rotatedYCap(3,0);
  
  vector<double> ycap(3,0);
  vector<double> zcap(3,0);
  zcap[2] = 1;

  double theta_calculated;
  double ph_calculated;
  double dist;
  double theta_pix;
  double phi_pix;
  double val[3];

  int xcoord;
  int ycoord;
  // Plane related variables end

  while (!disp2.is_closed())
  {
    
    if (disp2.key() == 'g')
          toggle_gridVisible(&gridVisible);

    //todo: key press for resizing window

    //key press for changing distance of focal distance
    else if (disp2.key() == '-')
      rho -= 5;
    else if(disp2.key() == '=')
      rho +=5;

    else if(refTheta==-1 && refPhi==-1)
    {
          // This part retracts the viewing window to original state
          // if theta and phi do not have originally assigned values, the viewing window is displaced and shud be returned back
          if (theta!=180 || phi!=90)
          {
              theta += 0.2*(180-theta);
              phi += 0.2*(90-phi);

          }
          
            //visu.display(disp);
            //foveal.display(disp2);
    }

    //mouse events
    if(disp2.button())
    {
        
        if(refTheta==-1 && refPhi==-1)
        {
          refTheta = disp2.mouse_x();
          refPhi = disp2.mouse_y();
          theta_del =0;
          phi_del =0;
        }
        else
        {
          currTheta = disp2.mouse_x();
          currPhi = disp2.mouse_y();
          theta_del = currTheta - refTheta;
          phi_del = currPhi - refPhi;
        }
        

        //if ( (theta + theta_del >= 0) && (theta+theta_del <= 360) )
        if ( theta + theta_del < 0)
          theta = 0;
        else if(theta+theta_del > 360)
          theta = 360;
        else
          theta=theta+ 0.1*theta_del;        

        if (phi +phi_del <0 )
          phi = 0;
        else if (phi +phi_del >180 )
          phi = 180;
        else
            phi=phi+ 0.1*phi_del;          


    }
    else
    {
      refTheta=-1;
      refPhi=-1;
      
    }

    //Rendering projected plane starts

    //algorithm.
             //1. calculate point3d
             //2. rotate point3d about z-axis(fixed) by theta
             //3. get new y-basis 
             //4. rotate by phi about new y-axis

    visu = image;
    double costheta = cos(theta * DEG2RAD);
    double sintheta = sin(theta * DEG2RAD);

    double cosphi = cos(phi * DEG2RAD);
    double sinphi = sin(phi * DEG2RAD);

    for(i = 0; i< height; i++)
    {
      for (j = 0; j < width; ++j)
      {

        //1. calculate point3d
        //shift Plane centre and get Coordinate Positions
        point3d[0] = j - centre[0];
        point3d[1] = centre[1] - i;
        point3d[2] = rho; 

        //2. rotate point3d about z-axis(fixed) by theta
        point3d_rotatedZ[0] = point3d[0] * costheta;
        point3d_rotatedZ[0] -= point3d[1]* sintheta;
        point3d_rotatedZ[1] = point3d[1]* costheta;
        point3d_rotatedZ[1] += point3d[0] * sintheta;
        point3d_rotatedZ[2] = point3d[2];
        //rotation ends

        //3. rotate by phi about new x-axis
        //point3d_rotatedYCap = axisAngleRotate(point3d_rotatedZ, ycap, phi); 

        double scalar = -point3d_rotatedZ[0] * sintheta;
        scalar += point3d_rotatedZ[1] * costheta;
        scalar *= (1- cosphi);

        point3d_rotatedYCap[0] = point3d_rotatedZ[0] * cosphi;
        point3d_rotatedYCap[0] += scalar* (-sintheta);
        point3d_rotatedYCap[0] +=  point3d[2] * sinphi *costheta;

        point3d_rotatedYCap[1] = point3d_rotatedZ[1] * cosphi;
        point3d_rotatedYCap[1] +=  scalar* costheta;
        point3d_rotatedYCap[1] += point3d_rotatedZ[2] * sintheta *sinphi;

        point3d_rotatedYCap[2] = point3d_rotatedZ[2] * cosphi;
        point3d_rotatedYCap[2] += (-sinphi) * ( (point3d_rotatedZ[1] * sintheta)  + (point3d_rotatedZ[0] * costheta) );
        //rotate ends

        //calculate theta and phi for the point and store in plane
        dist = sqrt((point3d_rotatedYCap[0]*point3d_rotatedYCap[0]) + (point3d_rotatedYCap[1]*point3d_rotatedYCap[1])); 
        //point3d[2] = rho * cos(phi);  

        theta_calculated = atan2(point3d_rotatedYCap[1], point3d_rotatedYCap[0]);
        ph_calculated = atan2(dist,point3d_rotatedYCap[2]);   

        if(theta_calculated < 0)
            theta_calculated += 2*PI; 

        projectedPlane[j][i] = make_pair(theta_calculated, ph_calculated);  

        theta_pix = theta_calculated * col_radToPixlfactor;
        phi_pix = ph_calculated * row_radToPixlfactor;

        if(gridVisible)
          visu.draw_circle(theta_pix, phi_pix, 0.5, col, 0.1,0.1);

        val[0] = image.linear_atXY(theta_pix,phi_pix,0);
        val[1] = image.linear_atXY(theta_pix,phi_pix,1);
        val[2] = image.linear_atXY(theta_pix,phi_pix,2);

        xcoord = (int)projectedPlane.size()-i;
        ycoord = j;
 
        foveal(xcoord, ycoord,0)=val[0];
        foveal(xcoord, ycoord,1)=val[1];
        foveal(xcoord,ycoord,2)=val[2];

        foveal1(xcoord, ycoord,0) = image(int(theta_pix),int(phi_pix),0);
        foveal1(xcoord, ycoord,1) = image(int(theta_pix),int(phi_pix),1);
        foveal1(xcoord, ycoord,2) = image(int(theta_pix),int(phi_pix),2);

        if (gridVisible)
        {
          if((int(projectedPlane[j][i].first*RAD2DEG) % 10 == 0))
            foveal.draw_point(xcoord, ycoord, 0.1, col, 0.2);
        }


        if (gridVisible)
        {
          if (int(projectedPlane[j][i].second*RAD2DEG) % 10 == 0)
            foveal.draw_point(xcoord, ycoord, 0.1, col, 0.2);
        }
          
            

      }
    }


    visu.display(disp);
    disp2.empty();
    
    //foveal.display(disp2);
    foveal.get_sqrt().normalize(0,255).display(disp2);
    disp2.set_title("What the eye sees");

    // foveal1.display(disp3);
    // disp3.set_title("no interpolation");

  }

//main ends here
}
