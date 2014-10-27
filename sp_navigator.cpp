#include <iostream>
#include<CImg.h>
#include<string>
#include<cmath>
#define PI 3.14159

using namespace std;
using namespace cimg_library;


int main()
{
  
  //cimg::imagemagick_path( "/usr/local/bin/convert" );
  CImg<unsigned char> image("kate_sp.jpg");
  CImg<unsigned char> visu(image);
  CImgDisplay disp(visu);
  CImgDisplay disp2;
  CImgDisplay disp3;
  CImgDisplay disp4;

  visu.display(disp);
  int rows = image.height();
  int cols = image.width();
  

  int row_degreeToPixlfactor= rows/180;
  int col_degreeToPixlfactor= cols/360;
  

  //Let fov=45 degrees
  int fov=45;
  
  int az_rot=0;
  int ze_rot=0;
  
  int theta_c= 360/2 ;
  int phi_c= 180/2 ;
  
  int i,j,r;
  
  float x_dist, y_dist, x_dist2, y_dist2, z_dist;
  float vec [3], l_theta,l_phi, theta_pix, phi_pix;
  int temp_x, temp_y, val[3];
  int img_pos_x,img_pos_y;
  
  float theta, phi;
  
  
  z_dist=180;

  unsigned char col[3]={ 0,0,255 };
  CImg<unsigned char> foveal(360,360,1,3,0);
  CImg<unsigned char> foveal2(360,360,1,3,0);
  CImg<unsigned char> foveal3(360,360,1,3,0);

  float c [2]={theta_c,phi_c};
  

  while(!disp.is_closed())
    {
      disp.wait();

      //left key pressed
      if (disp.key()==65361)
        {
            if(az_rot>-100)
              az_rot=az_rot-2;

            //cout<<"left key pressed: "<<az_rot<<endl;
        }
      //top key pressed
      else if (disp.key()==65362)
        {
            if(ze_rot>-15)
              ze_rot=ze_rot-2;
            //cout<<"top key pressed: "<<ze_rot<<endl;
        }
      //rightkey pressed
      else if (disp.key()==65363)
        {
            if(az_rot<100)
              az_rot=az_rot+2;
            //cout<<"right key pressed: "<<az_rot<<endl;
        }
      //bottom key pressed
      else if (disp.key()==65364)
        {
            if(ze_rot<15)
              ze_rot=ze_rot+2;

            //cout<<"down key pressed: "<<ze_rot<<endl;
        }

      //decrease magnification
      else if (disp.key()=='-')
        {


            if(z_dist<300)
              z_dist=z_dist+4;
        }
      //increase magnification
      else if (disp.key()=='=')
        {
            

            if(z_dist>50)
              z_dist=z_dist-4;

            //cout<<"increasing mag: "<<proj_z<<endl;
        }
      

        visu=image;
        for (j=0; j<360;j++)
        {
          for(i=0; i<360;i++)
           {
           
           x_dist=i-theta_c;
           y_dist=phi_c-j;    

           cout<<"x dist: "<<x_dist<<endl;
           cout<<"y dist: "<<y_dist<<endl;    

           if(x_dist==0 && y_dist==0)
           {
            x_dist=0.001;
            y_dist=0.001;
           }

           
           r= sqrt(x_dist*x_dist + y_dist*y_dist + z_dist*z_dist);

           //r= sqrt(x_dist*x_dist + y_dist*y_dist);
           //l_phi= (asin(y_dist/r))*180/PI;
           //l_theta=(atan(x_dist/z_dist))*180/PI;    

           l_phi= (acos(z_dist/r))*180/PI;
           l_theta=(atan(abs(x_dist/y_dist)))*180/PI;    

           
           if(x_dist<0)
               l_theta=-l_theta;
           // else if(x_dist<0 && y_dist<0)
           //    l_theta=l_theta+180;
           // else if(x_dist>0 && y_dist<0)
           //    l_theta=360-l_theta;
           //  else if(x_dist>0 && y_dist>0)
           //    l_theta=360-l_theta;
           
            theta=l_theta+c[0] + az_rot;

           // if(y_dist>0)
           //    l_phi=-l_phi;
              
            phi=l_phi+c[1]+ze_rot;

           cout<< "local_phi: "<< l_phi<<endl;
           cout<< "local_theta: "<< l_theta<<endl;    

           cout<< "phi: "<< phi<<endl;
           cout<< "theta: "<< theta<<endl;    

            theta_pix= floor(theta*col_degreeToPixlfactor);
            phi_pix= floor(phi*row_degreeToPixlfactor);
            visu.draw_circle(theta_pix, phi_pix, 1, col, 1, 1);


            foveal3(i,j,0)=image(theta_pix,phi_pix,0);
            foveal3(i,j,1)=image(theta_pix,phi_pix,1);
            foveal3(i,j,2)=image(theta_pix,phi_pix,2);


            theta_pix= theta*col_degreeToPixlfactor;
            phi_pix= phi*row_degreeToPixlfactor;

            val[0] = image.linear_atXY(theta_pix,phi_pix,0);
            val[1] = image.linear_atXY(theta_pix,phi_pix,1);
            val[2] = image.linear_atXY(theta_pix,phi_pix,2);

            foveal(i,j,0)=val[0];
            foveal(i,j,1)=val[1];
            foveal(i,j,2)=val[2];

            val[0] = image.cubic_atXY(theta_pix,phi_pix,0);
            val[1] = image.cubic_atXY(theta_pix,phi_pix,1);
            val[2] = image.cubic_atXY(theta_pix,phi_pix,2);

            foveal2(i,j,0)=image(theta_pix,phi_pix,0);
            foveal2(i,j,1)=image(theta_pix,phi_pix,1);
            foveal2(i,j,2)=image(theta_pix,phi_pix,2);

            
            
            //getchar();
            //visu.display(disp);
        
           }
           visu.display(disp);
           //getchar();
           //visu=image;
        }

        
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
