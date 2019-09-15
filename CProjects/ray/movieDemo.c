//
// Torbert, 28 February 2017
//
#include <stdio.h>
#include <stdlib.h>
//
int main( int argc , char* argv[] )
{
   FILE* fout ;
   //
   char fname[50] ;
   //
   int frameCount ;
   //
   for( frameCount = 1 ; frameCount <= 100 ; frameCount++ )
   {
       //
       // something changes in the scene - position, angle, etc
       //
       // render all the pixel colors
       //
       // write out the PPM file - same code as before
       //
       fout = fopen( "allgreen.ppm" , "w" ) ;
       //
       fprintf( fout , "P3\n" ) ;
       fprintf( fout , "%d %d\n" , M , N ) ;
       fprintf( fout , "255\n" ) ;
       //
       for( y = 0 ; y < N ; y++ )
       {
          for( x = 0 ; x < M ; x++)
          {
             fprintf( fout , "%d %d %d\n" ,
              rgb[y][x][0] , rgb[y][x][1] , rgb[y][x][2] ) ;
          }
       }
       fclose( fout ) ;
       //
       // convert the temporary PPM file to a numbered frame file
       //
       sprintf( fname , "convert allgreen.ppm f%07d.png" , frameCount ) ;
       //
       system( fname ) ;
       //
       // after the run there will be 100 different PNG files
       //
       // f0000001.png
       // f0000002.png
       // f0000003.png
       //      :
       // f0000099.png
       // f0000100.png
       //
       // time convert -loop 0 -delay 10 *png movie.gif
       //
   }
   //
   return 0 ;
}
//
// end of file
//
