#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

bool seen [150];
int numberOfNodes = 150;
int numberOfEdges = 0;
float links[150][150];

void
dfs ( int u ) ;


int main( int argc , char ** argv )
{
   if ( argc < 2 )
   {
       std::cout << "Invalid input. Please run './create_graph <seed>' where <seed> is an int." << std::endl ;
       exit (-1) ;
   }
   srand( atoi( argv[ 1 ] ) ) ;

   //create file
   bool graph_passed = false;

   while(graph_passed == false){

       //fill links of adjacency matrix with "coin flips"
       for ( int i = 0; i < numberOfNodes; i++){
           for ( int j = i+1; j < numberOfNodes; j++){
               if( (float)rand()/RAND_MAX <= .95f ){
                   links[i][j] = links[j][i] = 0;
               }
               else {
                   links[i][j] = links[j][i]= 1;
                   numberOfEdges++;
               }
               //cout<< links[i][j]<< " ";
           }
           //cout<< endl;
       }

     //fill seen with all 0's
     for (int i = 0; i < numberOfNodes; i++){
         seen[i] = 0;
     }
     //cout << "\ndfs\n";
     dfs(0);

     //assume
     graph_passed = true;
     //print out seen[] to see if dfs found a completely connected graph
     for (int i = 0; i < numberOfNodes; i++){
         //cout << seen[i] << " ";
         if (seen[i] == 0){
             graph_passed = false;
         }
     }
  }

   ofstream graph;
   graph.open ("graph.txt");
   //write adjacency matrix connections to file
   graph << numberOfNodes << " " << numberOfEdges << endl;
   for ( int i = 0; i < numberOfNodes; i++){
       for ( int j = i+1; j < numberOfNodes; j++){
           if ( links[i][j] == 1)
               graph << i << " " << j << endl;
       }
   }
   graph << "**" << endl;

   return 0;
}

void
dfs ( int u )
{
   //std::cout << u << std::endl;
   seen[u] = true;
   for ( int v = 0 ; v < numberOfNodes ; ++ v )
   {
       if( !seen[ v ] && links[ u ][ v ] != INFINITY && links[ u ][ v ] != 0 )
           dfs ( v ) ;
   }
}