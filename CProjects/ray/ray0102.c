//
// Torbert, 8 February 2016
//
#include <stdio.h>
#include <math.h>
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
         shadow = 0;
         triple ray;
	      triple curr = {(x + 0.5)/640 - 1/6, 1 - (y + 0.5)/480, 0};
	      diff(&ray, curr, e); 
         double Tf = checkSphere(e, ray, a[0].c, a[0].r);
         double Tr = checkSphere(e, ray, a[1].c, a[1].r);
         double Tg = checkSphere(e, ray, a[2].c, a[2].r);
         double Tb = checkSphere(e, ray, a[3].c, a[3].r);
         double Tmine = min(min(Tf, Tr), min(Tg, Tb));

         double Tsunr = MAX;
         double Tsung = MAX;
         double Tsunb = MAX;
         // printf("Tmine: %f, Tf: %f, Tr: %f, Tg: %f, Tb: %f\n", Tmine, Tf, Tr, Tg, Tb);
         printf("%f\n", e.x + Tmine * ray.x);

         if(Tmine != MAX)
         {
            triple sunray;
            triple normal;
            double spherex = e.x + Tmine * ray.x;
            double spherey = e.y + Tmine * ray.y;
            double spherez = e.z + Tmine * ray.z;
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

            if(Tsunr != MAX || Tsung != MAX || Tsunb != MAX)
            {
               rgb[y][x][0] = a[spherenum].h.r/2   ; // red
               rgb[y][x][1] = a[spherenum].h.g/2   ; // green
               rgb[y][x][2] = a[spherenum].h.b/2   ; // blue
            }
            else
            {
               double mag = sqrt(sunray.x * sunray.x + sunray.y * sunray.y + sunray.z * sunray.z);
               sunray.x = sunray.x/mag;
               sunray.y = sunray.y/mag;
               sunray.z = sunray.z/mag;
               double dotproduct = dotp(sunray, normal);
               double factor = 0.5 + 0.5 * dotproduct;
               rgb[y][x][0] = a[spherenum].h.r * factor  ; // red
               rgb[y][x][1] = a[spherenum].h.g * factor  ; // green
               rgb[y][x][2] = a[spherenum].h.b * factor  ; // blue
            }
         }
         else
         {
            rgb[y][x][0] = 0   ; // red
            rgb[y][x][1] = 0   ; // green
            rgb[y][x][2] = 0   ; // blue
         }
      }
   }
   //
   //
   //
   fout = fopen( "picture.ppm" , "w" ) ;
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
   //
   return 0 ;
}
//
// end of file
//
