#include <iostream>
#include<CImg.h>
#include<string>
#include<cmath>
#define PI 3.14159



using namespace std;
using namespace cimg_library;


int main()
{
  
  
  int i,j,r;

  //cimg::imagemagick_path( "/usr/local/bin/convert" );
  //CImg<unsigned char> image("kate_sp.jpg");
  CImg<unsigned char> image("sp_ex2.jpg");
  CImg<unsigned char> visu(image);
  CImgDisplay disp(visu);
  CImgDisplay disp2;
  CImgDisplay disp3;
  CImgDisplay disp4;

  
  visu.display(disp);
  int rows = image.height();
  int cols = image.width();
  cout<<"height: "<<rows<<endl;
  cout<<"width: "<<cols<<endl;

  int row_degreeToPixlfactor= rows/180;
  int col_degreeToPixlfactor= cols/360;
  double row_radToPixlfactor= rows/PI;
  double col_radToPixlfactor= cols/(2*PI);
  
  double az_rot=0;
  double ze_rot=0;
  
  int theta_c= 360/2 ;
  int phi_c= 180/2 ;
  double c [2]={theta_c,phi_c};
  
  
  double x_dist, y_dist, z_dist;
  double vec [3], l_theta,l_phi, theta_pix, phi_pix;
  int temp_x, temp_y, val[3];
  int img_pos_x,img_pos_y;
  
  double theta, phi;
  
  
  //Siddhartha - 30 Oct
  int cartesianHeight = 360;
  int cartesianWidth = 360;

  unsigned char col[]={ 0,0,255 };
  CImg<unsigned char> foveal(cartesianWidth,cartesianHeight,1,3,0);
  CImg<unsigned char> foveal2(cartesianWidth,cartesianHeight,1,3,0);
  CImg<unsigned char> foveal3(cartesianWidth,cartesianHeight,1,3,0);

  double degToRad = PI/180;
  double radToDeg = 180/PI;

  double dblC  = cartesianWidth / 2;
  z_dist=dblC;

  double thetaC = theta_c * degToRad;
  double phiC = phi_c * degToRad;

  
  // CImg<unsigned char> imageDepiction(cartesianWidth, cartesianWidth, cartesianWidth,3,0);
  // int x_coord, y_coord, z_coord;
  // unsigned char colSphere[]={ 0,0,0 };
  // for(i=0; i<360; i++)
  // {  
  //   for (j=0; j<720; j++)
  //     {

  //       theta = j / col_radToPixlfactor ;
  //       phi = i / row_radToPixlfactor;
  //       x_coord = 180 * cos(theta) * cos(phi);
  //       y_coord = 180 * cos(theta) * sin(phi);

  //       x_coord+=180;
  //       y_coord+=180;

  //       z_coord = floor(sqrt( cartesianWidth*cartesianWidth - (x_coord*x_coord) - (y_coord*y_coord)));

  //       cout << "coords: ["<<x_coord<<" "<<y_coord<<" "<<z_coord<<"]"<<endl;

  //       colSphere[0]=image(j,i,0);
  //       colSphere[1]=image(j,i,1);
  //       colSphere[2]=image(j,i,2);

  //       imageDepiction.draw_point(x_coord, y_coord, z_coord, colSphere, 1);  
  //     }
  // }
  
  // imageDepiction.display(disp4);

  //CImgList<unsigned int> faces3d;
  //const CImg<float> points3d = CImg<float>::sphere3d(faces3d,100,5);
  //CImg<unsigned char>().display_object3d("Sphere3d",points3d,faces3d);
  //getchar();


  disp2.empty();
  foveal.display(disp2);
  disp2.show_mouse ();

  int transX;
  int transY;

  int refX=-1;
  int refY=-1;
  int currX;
  int currY;
  int mover;
  double currDist;


  while(!disp2.is_closed())
    {

      if(refX==-1 && refY==-1)
        {
          //transX =-2;
          //transY =-2;
          if (az_rot!=0 || ze_rot!=0)
          {
              currDist = sqrt(az_rot*az_rot + ze_rot*ze_rot);
              az_rot = (az_rot/currDist) * (currDist- 0.1*currDist);
              ze_rot = (ze_rot/currDist)* (currDist- 0.1*currDist);
          }
          
            visu.display(disp);
            foveal.display(disp2);

         } 


          //disp2.wait();
        
      //mouse events
      if(disp2.button())
      {
        
        //transX = disp2.mouse_x()-180;
        //transY = disp2.mouse_y()-180;

        if(refX==-1 && refY==-1)
        {
          refX = disp2.mouse_x();
          refY = disp2.mouse_y();

          transX =0;
          transY =0;
        }
        else
        {
          currX = disp2.mouse_x();
          currY = disp2.mouse_y();

          transX = currX - refX;
          transY = currY - refY;
        }
        
        cout << "ref: "<<refX<<" "<<refY<<endl;
        cout << "curr: "<<currX<<" "<<currY<<endl;
        cout << "trans: "<<transX<<" "<<transY<<endl;

        az_rot=az_rot+ ((transX/2)* degToRad);
        ze_rot=ze_rot+ ((transY/2)* degToRad);

      }
      else
        {
          refX=-1;
          refY=-1;
        }


      //left key pressed
      if (disp2.key()==65361)
         {
              az_rot=az_rot-(5*degToRad);
         }


      //top key pressed

      else if (disp2.key()==65362)
         {
      //       //if(ze_rot>-(54*degToRad))
               ze_rot=ze_rot- (5*degToRad);
      //       //cout<<"top key pressed: "<<ze_rot*radToDeg<<endl;
         }
      //rightkey pressed
      else if (disp2.key()==65363)
         {
      //       //if(az_rot< (144 *degToRad ))
               az_rot=az_rot+ (5* degToRad);
      //       //cout<<"right key pressed: "<<az_rot*radToDeg<<endl;
         }
      //bottom key pressed
      else if (disp2.key()==65364)
        {
            //if(ze_rot< (54*degToRad))
              ze_rot=ze_rot+ (5*degToRad) ;

            //cout<<"down key pressed: "<<ze_rot*radToDeg<<endl;
        }
        
      else if (disp2.key()=='[')
        {
            if(cartesianWidth>100)
              { 
                cartesianWidth--;
                cartesianHeight--;
              }
            //cout<<"top key pressed: "<<ze_rot*radToDeg<<endl;
        }

      else if (disp2.key()==']')
        {
            if(cartesianWidth<800)
              { 
                cartesianWidth++;
                cartesianHeight++;
              }
            //cout<<"top key pressed: "<<ze_rot*radToDeg<<endl;
        }


      //decrease magnification
      else if (disp2.key()=='-')
        {

            if(z_dist<300)
              z_dist=z_dist+4;
        }
      //increase magnification
      else if (disp2.key()=='=')
        {
            

            if(z_dist>50)
              z_dist=z_dist-4;

            //cout<<"increasing mag: "<<proj_z<<endl;
        }
      

        int xcoord, ycoord;
        visu=image;

        // Siddhartha 30th Oct

        //r = 100;

        float v3dIn[3];
        for (i=0; i< cartesianHeight ;i++)
        {
          for(j=0; j< cartesianWidth ;j++)
           {
           
             x_dist   = j - (int)dblC;  
             y_dist    = (int)dblC - i;  
             
             r= sqrt( (x_dist*x_dist) + (y_dist*y_dist) + (z_dist*z_dist) );


             //Get unit vector in direction of pixel on the cartesian frame where we would project a porion of spherical panorama
             v3dIn[0]= x_dist/r;
             v3dIn [1]= y_dist/r;  
             v3dIn[2]= z_dist/r;

            

             //l_phi = atan2(v3dIn[1], v3dIn[0]);
             //l_theta = atan2( sqrt(v3dIn[0]*v3dIn[0] + v3dIn[1]*v3dIn[1]), v3dIn[2]);


             l_phi = v3dIn[1];
             l_theta = thetaC + v3dIn[0];

             // cout<<"i-->height: "<<i<<endl;
             // cout<<"j-->width: "<<j<<endl;
             // cout<<"dblC: "<<dblC<<endl;    
             // cout<<"x dist: j-dblC = "<<x_dist<<endl;
             // cout<<"y dist: i-dblC = "<<y_dist<<endl;
             // cout<<"z dist: "<<z_dist<<endl;    
             // cout<<"r: "<<r<<endl;     

             //l_theta = atan2((double)-x_dist, r) - (thetaC  - PI);
             //l_phi    = atan(-y_dist/sqrt(x_dist*x_dist + r*r)) - (phiC  - PI/2);

             //cout<< "local_phi: "<< l_phi*radToDeg<<endl;
             //cout<< "local_theta: "<< l_theta*radToDeg<<endl;
             //getchar();
              
              
            theta = l_theta +az_rot - thetaC - PI;
            phi = l_phi +ze_rot - phiC + PI;
      

             
             //theta=l_theta+ thetaC + az_rot;
             //phi=l_phi+ phiC +ze_rot;

             // cout<< "before phi: "<< phi<<endl;
             // cout<< "before theta: "<< theta<<endl;       


             if (theta  < 0) theta +=  2*PI;
             if (theta  > (2*PI)) theta =  theta - 2*PI;


             if (phi    < 0) 
                phi = -phi;

             if (phi    > PI) 
             {
                phi = PI - (phi - PI);

                theta = 2* PI - theta;
                //if (theta<thetaC)
                  
                //else 
                  //theta = 2* PI + theta;
             }


             if ( (i==0 && j==0) || (i==cartesianHeight-1 && j==0))
             {
                   //cout<< "phi: "<< phi * radToDeg<<endl;
                   //cout<< "theta: "<< theta * radToDeg<<endl;        
             }


             //x = R * (Lon - LonP) * COS(LatP)
             //y = R * Lat

             theta_pix= floor(theta*col_radToPixlfactor);
             phi_pix= floor(phi*row_radToPixlfactor);

             // theta_pix= floor((theta-thetaC)*cos(phiC)*col_radToPixlfactor);
             // phi_pix= floor(phi*row_radToPixlfactor);


            if(theta_pix<0)
              theta_pix = abs(theta_pix);

             //if ( (i==0 && j==0) || (i==cartesianHeight-1 && j==0))
             //{
                  //cout<< "theta_pix: "<< theta_pix<<endl;
                  //cout<< "phi_pix: "<< phi_pix<<endl;       
             //}
             

             visu.draw_circle(theta_pix, phi_pix, 0.5, col, 0.1,0.1);  
            
             xcoord=cartesianWidth-1-j;
             ycoord=cartesianHeight-1-i;

             //xcoord=1+j;
             //ycoord=1+i;

             // cout<< "xcoord: "<< xcoord<<endl;
             // cout<< "ycoord: "<< ycoord<<endl;       



             foveal3(xcoord,ycoord,0)=image(theta_pix,phi_pix,0);
             foveal3(xcoord,ycoord,1)=image(theta_pix,phi_pix,1);
             foveal3(xcoord,ycoord,2)=image(theta_pix,phi_pix,2);

             theta_pix= theta*col_radToPixlfactor;
             phi_pix= phi*row_radToPixlfactor;

             val[0] = image.linear_atXY(theta_pix,phi_pix,0);
             val[1] = image.linear_atXY(theta_pix,phi_pix,1);
             val[2] = image.linear_atXY(theta_pix,phi_pix,2);

             foveal(xcoord,ycoord,0)=val[0];
             foveal(xcoord,ycoord,1)=val[1];
             foveal(xcoord,ycoord,2)=val[2];

             val[0] = image.cubic_atXY(theta_pix,phi_pix,0);
             val[1] = image.cubic_atXY(theta_pix,phi_pix,1);
             val[2] = image.cubic_atXY(theta_pix,phi_pix,2);

             foveal2(xcoord,ycoord,0)=image(theta_pix,phi_pix,0);
             foveal2(xcoord,ycoord,1)=image(theta_pix,phi_pix,1);
             foveal2(xcoord,ycoord,2)=image(theta_pix,phi_pix,2);

             //getchar();
             //visu.display(disp);
        
           }
           //visu.display(disp);
           //getchar();
           //visu=image;
        }

        visu.display(disp);
        //visu=foveal2;
        disp2.empty();
        foveal.display(disp2);
        disp2.set_title("Linear interpolation");

        // foveal2.display(disp3);
        // disp3.set_title("Cubic interpolation");

        // foveal3.display(disp4);
        // disp4.set_title("No interpolation");
    }
  return 0;

}
