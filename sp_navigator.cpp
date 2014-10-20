#include <iostream>
#include<CImg.h>
#include<string>
#include<cmath>

using namespace std;
using namespace cimg_library;

int main()
{
  
  //printf("Hello\n");
  //CImg<unsigned char> img;
  //cimg::imagemagick_path( "/usr/local/bin/convert" );
  CImg<unsigned char> image("kate_sp.jpg");
  CImg<unsigned char> visu(image);
  CImgDisplay disp(visu);
  CImgDisplay disp2;

  visu.display(disp);
  int rows = image.height();
  int cols = image.width();
  
  //cout<<"height= "<<rows<<endl;
  //cout<<"width= "<<cols<<endl;

  //Let fov=45 degrees
  int fov=45;
  int proj_z=180;
  
  float x_mid= cols/2;
  float y_mid= rows/2;
 
  //int c [3]={x_mid, y_mid, 180};
  int az_rot=0;
  int ze_rot=0;
  
  int theta_c= 180 + az_rot;
  int phi_c= 90+ze_rot;

  if(theta_c<fov)
    theta_c=fov;

  if(theta_c>(360-fov))
    theta_c=360-fov;

  if(phi_c<fov)
    phi_c=fov;
      
  if(phi_c>(180-fov))
    phi_c=fov;

  int c [3]={theta_c*2, phi_c*2, 180};
  
  int theta_bound_l=(theta_c-fov);
  int theta_bound_u=(theta_c+fov);
  int phi_bound_l=(phi_c-fov);
  int phi_bound_u=(phi_c+fov);
  
  int i,j;
  int rho=180;
  
  int x_dist, y_dist, x_dist2, y_dist2, z_dist;
  float vec [3];
  
  // cout<<"bounds are: "<<endl;
  // cout<<theta_bound_l<<":"<<theta_bound_u<<endl;
  // cout<<phi_bound_l<<":"<<phi_bound_u<<endl<<endl;

  CImg<unsigned char> foveal(180,180,1,3,0); 
  for (i=theta_bound_l*2; i<=theta_bound_u*2;i++)
    {
      for(j=phi_bound_l*2; j<=phi_bound_u*2;j++)
	     {
	     x_dist=i-c[0];
	     y_dist=j-c[1];
	     x_dist2= x_dist*x_dist;
	     y_dist2=y_dist*y_dist;
	     z_dist= sqrt((rho*rho)-(x_dist2+y_dist2));
	  
	     vec[0]=floor(proj_z*x_dist/z_dist);
	     vec[1]=floor(proj_z*y_dist/z_dist);
	     vec[2]=floor(proj_z*z_dist/z_dist);

	  
        foveal(i-(theta_bound_l*2),j-(phi_bound_l*2),0)=image(c[0]+vec[0],c[1]+vec[1],0);
        foveal(i-(theta_bound_l*2),j-(phi_bound_l*2),1)=image(c[0]+vec[0],c\
                  [1]+vec[1],1);
        foveal(i-(theta_bound_l*2),j-(phi_bound_l*2),2)=image(c[0]+vec[0],c\
                  [1]+vec[1],2);

	      // cout<<"position assigned on foveal: ["<<j-(phi_bound_l*2)<<", "<<i-(theta_bound_l*2)<<"]"<<endl;
       //  cout<<"centre: "<<c[0]<<":"<<c[1]<<endl;
       //  cout<<"vec is: ["<<vec[0]<<", "<<vec[1]<<", "<<vec[2]<<"]"<<endl<<endl;
	      // cout<<"position from image: ["<<c[1]+vec[1]<<","<<c[0]+vec[0]<<"]"<<endl;
	      // cout<<"value: ["<<int(image(c[1]+vec[1],c[0]+vec[0],0))<<","<<int(image(c[1]+vec[1],c[0]+vec[0],1))<<","<<int(image(c[1]+vec[1],c[0]+vec[0],2))<<"]"<<endl;
	  
	     }
    }

 
  visu=foveal;
  visu.display(disp2);

  while(!disp2.is_closed())
    {
      disp2.wait();

      //left key pressed
      if (disp2.key()==65361)
        {
            if(az_rot+theta_bound_l>0)
              az_rot=az_rot-4;

            //cout<<"left key pressed: "<<az_rot<<endl;
        }
      //top key pressed
      else if (disp2.key()==65362)
        {
            if(ze_rot+phi_bound_l>0)
              ze_rot=ze_rot-4;
            //cout<<"top key pressed: "<<ze_rot<<endl;
        }
      //rightkey pressed
      else if (disp2.key()==65363)
        {
            if(az_rot+theta_bound_u<360)
              az_rot=az_rot+4;
            //cout<<"right key pressed: "<<az_rot<<endl;
        }
      //bottom key pressed
      else if (disp2.key()==65364)
        {
            if(ze_rot+phi_bound_u<180)
              ze_rot=ze_rot+4;

            //cout<<"down key pressed: "<<ze_rot<<endl;
        }

      //decrease magnification
      else if (disp2.key()=='-')
        {

            // if(theta_c-fov>0 && phi_c-fov>0)
            // {
            //   fov=fov-1;
            //   theta_bound_l=theta_c-fov;
            //   phi_bound_l=phi_c-fov;
            // }

            if(proj_z<200)
              proj_z=proj_z+4;
        }
      //increase magnification
      else if (disp2.key()=='=')
        {
            // if(theta_c+fov<360 && phi_c+fov<180)
            // {
            //   fov=fov+1;
            //   theta_bound_u=theta_c+fov;
            //   phi_bound_u=phi_c+fov;
            // }

            if(proj_z>50)
              proj_z=proj_z-4;

            //cout<<"increasing mag: "<<proj_z<<endl;
        }

        //new centre
        theta_c= 180 + az_rot;
        phi_c= 90+ze_rot;
        c[0]=theta_c*2;
        c[1]=phi_c*2;
          
        //setting new bounds  
        theta_bound_l=(theta_c-fov);
        theta_bound_u=(theta_c+fov);
        phi_bound_l=(phi_c-fov);
        phi_bound_u=(phi_c+fov);

        for (i=theta_bound_l*2; i<=theta_bound_u*2;i++)
        {
          for(j=phi_bound_l*2; j<=phi_bound_u*2;j++)
          {
            x_dist=i-c[0];
            y_dist=j-c[1];
            x_dist2= x_dist*x_dist;
            y_dist2=y_dist*y_dist;
            z_dist= sqrt((rho*rho)-(x_dist2+y_dist2));
    
            vec[0]=floor(proj_z*x_dist/z_dist);
            vec[1]=floor(proj_z*y_dist/z_dist);
            vec[2]=floor(proj_z*z_dist/z_dist);

    
            foveal(i-(theta_bound_l*2),j-(phi_bound_l*2),0)=image(c[0]+vec[0],c[1]+vec[1],0);
            foveal(i-(theta_bound_l*2),j-(phi_bound_l*2),1)=image(c[0]+vec[0],c\
                  [1]+vec[1],1);
            foveal(i-(theta_bound_l*2),j-(phi_bound_l*2),2)=image(c[0]+vec[0],c\
                  [1]+vec[1],2);
          }
        }

        visu=foveal;
        visu.display(disp2);

   
    }
  return 0;

}
