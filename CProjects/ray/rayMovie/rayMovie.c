//
// Torbert, 8 February 2016
//
#include <stdio.h>
#include <math.h>
#include <string.h>
//
#define M 640
#define N 480
#define MAX 99999999999999
#define size 4

//
typedef struct
{
   double x ;
   double y ;
   double z ;
   //
} triple ;

typedef struct
{
   double r ;
   double g ;
   double b ;
   //
} color ;

typedef struct
{
   triple c ;
   double r ;
   color h ;
   //
} sphere ;
//
triple e = { 0.50 , 0.50 , -1.00 } ; // the eye
triple g = { 0.00 , 1.25 , -0.50 } ; // the light
color sky = { 175, 255, 255 } ;
color purple = { 120, 81, 169 } ;
//
sphere a[size];
//
double dotp( triple t , triple u )
{
   return t.x * u.x + t.y * u.y + t.z * u.z ;
}
//
void diff( triple* t , triple u , triple v ) // t = u - v
{
   t->x = u.x - v.x ;
   t->y = u.y - v.y ;
   t->z = u.z - v.z ;
}
//
double min(double num1, double num2) 
{
   return (num1 > num2 ) ? num2 : num1;
}
//
double checkSphere(triple start, triple r, triple center, double radius)
{
   double dx = start.x - center.x;
   double dy = start.y - center.y;
   double dz = start.z - center.z;
   double a = r.x * r.x + r.y * r.y + r.z * r.z;
   double b = 2 * (r.x * dx + r.y * dy + r.z * dz);
   double c = dx * dx + dy * dy + dz * dz - radius * radius;
   double det = b * b - 4 * a * c;
   if(det >= 0)
   {
      double Tone = (-b - sqrt(det))/(2 * a);
      // printf("Tone: %f\n", Tone);
      if(Tone > 0)
      {
         return Tone;
      }
   }
   return MAX;
}

color findColor(int x, int y, triple curr, triple eye, triple* sphere_points[N][M])
{
   int shadow = 0;
   triple ray;
   diff(&ray, curr, eye); 
   double Tf = checkSphere(eye, ray, a[0].c, a[0].r);
   double Tr = checkSphere(eye, ray, a[1].c, a[1].r);
   double Tg = checkSphere(eye, ray, a[2].c, a[2].r);
   double Tb = checkSphere(eye, ray, a[3].c, a[3].r);
   double Tmine = min(min(Tf, Tr), min(Tg, Tb));

   double Tsunr = MAX;
   double Tsung = MAX;
   double Tsunb = MAX;
   // printf("Tmine: %f, Tf: %f, Tr: %f, Tg: %f, Tb: %f\n", Tmine, Tf, Tr, Tg, Tb);
   // printf("%f\n", e.x + Tmine * ray.x);

   if(Tmine != MAX)
   {
      triple sunray;
      triple normal;
      triple reversenormal;
      double spherex = eye.x + Tmine * ray.x;
      double spherey = eye.y + Tmine * ray.y;
      double spherez = eye.z + Tmine * ray.z;
      int spherenum;
      if(Tmine == Tr)
      {
         spherenum = 1;
         normal = (triple) {(spherex - a[1].c.x)/a[1].r, (spherey - a[1].c.y)/a[1].r, (spherez - a[1].c.z)/a[1].r};
      }
      else if(Tmine == Tg)
      {
         spherenum = 2;
         normal = (triple) {(spherex - a[2].c.x)/a[2].r, (spherey - a[2].c.y)/a[2].r, (spherez - a[2].c.z)/a[2].r};
      }
      else if(Tmine == Tb)
      {
         spherenum = 3;
         normal = (triple) {(spherex - a[3].c.x)/a[3].r, (spherey - a[3].c.y)/a[3].r, (spherez - a[3].c.z)/a[3].r};
      }
      else
      {
         spherenum = 0;
         normal = (triple) {(spherex - a[0].c.x)/a[0].r, (spherey - a[0].c.y)/a[0].r, (spherez - a[0].c.z)/a[0].r};
      }
      
      sphere_points[y][x] = &(triple) {spherex + 0.001 * normal.x, spherey + 0.001 * normal.y, spherez + 0.001 * normal.z};
      diff(&sunray, g, *sphere_points[y][x]);
      Tsunr = checkSphere(*sphere_points[y][x], sunray, a[1].c, a[1].r);
      Tsung = checkSphere(*sphere_points[y][x], sunray, a[2].c, a[2].r);
      Tsunb = checkSphere(*sphere_points[y][x], sunray, a[3].c, a[3].r);

      reversenormal = (triple) {-normal.x, -normal.y, -normal.z};
      if(Tsunr != MAX || Tsung != MAX || Tsunb != MAX)
      {
         if(spherenum > 0)
         {
            return (color) {a[spherenum].h.r/2, a[spherenum].h.g/2, a[spherenum].h.b/2};
         }
         else
         {
            // double u = 0.5 + atan2(reversenormal.z, reversenormal.x)/(2 * M_PI);
            // double v = 0.5 - asin(reversenormal.y)/M_PI;
            // printf("u: %f, v: %f\n", u, v);
            // double sum = roundf((u + v) * 100) / 100;
            // int sum =  round(u + v);
            double checkerx = sphere_points[y][x] -> x;
            if(checkerx < 0)
            {
               checkerx += -1;
            }
            int sum = round(checkerx) + round(sphere_points[y][x] -> z);
            // printf("sum: %d\n", sum);
            if(fmod(sum, 2) == 0)
            {
               return (color) {a[spherenum].h.r/2, a[spherenum].h.g/2, a[spherenum].h.b/2};

            }
            else
            {
               return (color) {purple.r/2, purple.g/2, purple.b/2};
            }
         }
      }
      else
      {
         double mag = sqrt(sunray.x * sunray.x + sunray.y * sunray.y + sunray.z * sunray.z);
         sunray.x = sunray.x/mag;
         sunray.y = sunray.y/mag;
         sunray.z = sunray.z/mag;
         double dotproduct = dotp(sunray, normal);
         double factor = 0.5 + 0.5 * dotproduct;
         if(spherenum > 0)
         {
            return (color) {a[spherenum].h.r * factor, a[spherenum].h.g * factor, a[spherenum].h.b * factor};
         }
         else
         {
            // double u = 0.5 + atan2(reversenormal.z, reversenormal.x)/(2 * M_PI);
            // double v = 0.5 - asin(reversenormal.y)/M_PI;
            // double sum = roundf((u + v) * 100) / 100;
            // int sum =  round(u + v);
            double checkerx = sphere_points[y][x] -> x;
            if(checkerx < 0)
            {
               checkerx += -1;
            }
            int sum = round(checkerx) + round(sphere_points[y][x] -> z);
            // printf("u: %f, v: %f\n", u, v);
            // printf("sum: %d\n", sum);
            if(fmod(sum, 2) == 0)
            {
               return (color) {a[spherenum].h.r * factor, a[spherenum].h.g * factor, a[spherenum].h.b * factor};
            }
            else
            {
               return (color) {purple.r * factor, purple.g * factor, purple.b * factor};
            }
         }
      }
   }
   else
   {
      return sky;
   }
}

//
int main(void)
{
   a[0].c.x =      0.50 ;
   a[0].c.y = -20000.00 ; // the floor
   a[0].c.z =      0.50 ;
   a[0].r   =  20000.25 ;
   a[0].h.r =    205    ; // color is Peru
   a[0].h.g =    133    ;
   a[0].h.b =     63    ;
   //
   a[1].c.x =      0.50 ;
   a[1].c.y =      0.50 ;
   a[1].c.z =      0.50 ;
   a[1].r   =      0.25 ;
   a[1].h.r =      0    ; // color is Blue
   a[1].h.g =      0    ;
   a[1].h.b =    255    ;
   //
   a[2].c.x =      1.00 ;
   a[2].c.y =      0.50 ;
   a[2].c.z =      1.00 ;
   a[2].r   =      0.25 ;
   a[2].h.r =      0    ; // color is Green
   a[2].h.g =    255    ;
   a[2].h.b =      0    ;
   //
   a[3].c.x =      0.00 ;
   a[3].c.y =      0.75 ;
   a[3].c.z =      1.25 ;
   a[3].r   =      0.50 ;
   a[3].h.r =    255    ; // color is Red
   a[3].h.g =      0    ;
   a[3].h.b =      0    ;

   for(int frame = 0; frame < 30; frame++)
   {
      printf("%s\n", "In frame loop");
      int rgb[N][M][3] ; // red-green-blue for each pixel
      triple* sphere_points[N][M] ;

      //
      int y , x ;

      int shadow;
      //
      FILE* fout ;
      //
      for( y = 0 ; y < N ; y++ )
      {
         for( x = 0 ; x < M ; x++)
         {
            triple curr = {(x + 0.5)/640 - 1/6, 1 - (y + 0.5)/480, 0};
            color reflectColor = findColor(x, y, curr, e, sphere_points);

            rgb[y][x][0] = reflectColor.r   ; // red
            rgb[y][x][1] = reflectColor.g   ; // green
            rgb[y][x][2] = reflectColor.b   ; // blue
         }
      }
      //
      //
      //
      char buf[0x100];
      snprintf(buf, sizeof(buf), "picture000%d.jpg", frame);
      fout = fopen( buf , "w" ) ;
      //
      fprintf( fout , "P3\n" ) ;
      fprintf( fout , "%d %d\n" , M , N ) ;
      fprintf( fout , "255\n" ) ;
      //
      for( y = 0 ; y < N ; y++ )
      {
         for( x = 0 ; x < M ; x++)
         {
            fprintf( fout , "%d %d %d\n" , rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
         }
      }
      fclose( fout ) ;
      a[1].r += 0.01;
   }
   //
   return 0 ;
}
//
// end of file
//
