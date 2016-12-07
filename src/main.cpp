#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <queue>

#include "router.h"

void Path (int i, int j, int iFinal, int jFinal);
void dfs (int u);
double stdRandom ();
double nonStdRandom (int a, int b);

using namespace std;

int numberOfNodes;
double links[NUMBER_OF_NODES][NUMBER_OF_NODES];  //used for floyd warshal
double delay[NUMBER_OF_NODES][NUMBER_OF_NODES];  //holds link delays
int next[NUMBER_OF_NODES][NUMBER_OF_NODES];     //eventually holds routing table

bool seen [NUMBER_OF_NODES];
int seed;
int counter = 0;

/*
struct thing{
    int a;
    char c;

    bool operator<(const thing &o) const
    {
        return a < o.a;
    }
};
*/
//priority_queue<thing> wirePQ[NUMBER_OF_NODES];


int main () {

  seed = 17;
  srand(seed);
  ifstream graphFile ("graph.txt");
  int i;
  int j;

  //check if file opened
  if (!graphFile.is_open())
  {
      cout << "Unable to open file";
      return 0;
  }
  cout << "Opened file\n";

  graphFile >> i >> j;
  numberOfNodes = i;

  //fill adjacency matrix with all infinity except for
  //    links to self which are 0
  //fill next matrix will all -1 representing forward directly
  //    to that node
  for ( int k = 0; k < numberOfNodes; k++){
      for ( int l = 0; l < numberOfNodes; l++){
          if (k == l){
            links[k][l] = 0;
          }
          else{
            links[k][l] = INFINITY;
          }
          next[k][l] = -1;
      }
  }
  cout << "Filled links and next with empty values\n";

  //read from file to update adjacency matrix
  //using non standard random generated values representing the link's delay
  //copy accross diagonal
  while ( graphFile >> i >> j )
  {
    links[i][j] = links[j][i] = delay[i][j] = delay[j][i] = nonStdRandom(1,10);
  }
  graphFile.close();

  //cout << nonStdRandom(1,10) << endl;

/*
  //output delay matrix
  int linkcounter = 0;
  cout << "Initial adacency matrix.\n";
  for ( int k = 0; k < numberOfNodes; k++){
      for ( int l = 0; l < numberOfNodes; l++){
          cout << delay[k][l] << " ";
          if (delay[k][l] != 0)
            linkcounter++;
      }
      cout << endl;
  }
  cout << endl << linkcounter << endl;
*/

  //fill seen with all 0's
  for (int i = 0; i < numberOfNodes; i++){
      seen[i] = 0;
  }

  //dfs of links.  changes seen[i] to 1 if found
  dfs (0);

  //print out seen[] to see if dfs found a completely connected graph
  for (int i = 0; i < numberOfNodes; i++){
      //cout << seen[i] << " ";
      if (seen[i] == 0){
          cout << "The graph is not fully connected\n";
          return 0;
      }
  }
  cout << "Graph is completely connected\n\n";

  //floyd warshall
  cout << "Floyd warshall\n";
  for (int k = 0; k < numberOfNodes; k++){
      for (int i = 0; i < numberOfNodes; i++){
          for (int j = 0; j < numberOfNodes; j++){
              if ( links[i][k] + links[k][j] < links[i][j]){
                  links[i][j] = links[i][k] + links[k][j];
                  next[i][j] = k;
              }
          }
       }
   }

  /*
  //output floyd warshall distances
  cout << "Floyd warshall distances between nodes.\n";
  for ( int k = 0; k < numberOfNodes; k++){
      for ( int l = 0; l < numberOfNodes; l++){
          cout << links[k][l] << " ";
      }
      cout << endl;
  }
  cout << endl;
  */

  //Build the routing table (next) and output results
  cout << "Build Routing table.\n";
  for ( int k = 0; k < numberOfNodes; k++){
      for ( int l = 0; l < numberOfNodes; l++){
          //cout << "path " << k << " " << l << ":";
          Path(k,l,k,l);
          //cout << endl;
          //cout << next[k][l] << " ";
      }
      //cout << endl;
  }
  //cout << endl;

  //replace the -1's in the routing table (next) table with the actual next node number
  for ( int k = 0; k < numberOfNodes; k++){
      for ( int l = 0; l < numberOfNodes; l++){
          if (next[i][j] == -1)
            next[i][j] == j;
      }
  }




  //create routers
  Router router[NUMBER_OF_NODES];
  for (int i = 0; i < NUMBER_OF_NODES; i++){
      router[i] = new Router( i, next[i] , delay[i] );
  }


  int tempRouterNumber;
  int sourceRouter[NUMBER_OF_NODES];
  std::fill(sourceRouter, sourceRouter + NUMBER_OF_NODES, -1);
  //build source destination pairs
  for (int i = 0; i < 20; i++){
    tempRouterNumber = (int) nonStdRandom(0,149);
    if ( sourceRouter[tempRouterNumber] == -1){
        sourceRouter[tempRouterNumber] = ((int) (nonStdRandom(0,149)));
    else
        i--;
  }

  /*
  //output source destination pairs
  for(int i = 0; i < NUMBER_OF_NODES; i++){
      cout << i << " -> " << sourceRouter[i] << " \n";
  }
  */

  /*
  //practice with priority queue of structs
  thing a;
  thing b;
  thing c;

  a.a = 1;
  b.a = 3;
  c.a = 2;

  wirePQ[0].push(a);
  wirePQ[1].push(b);
  wirePQ[0].push(c);
  int output = wirePQ[0].top().a;
  cout << endl << endl << output;
  output = wirePQ[1].top().a;
  cout << endl << endl << output;
  */
  return 0;
}

void Path (int i, int j, int iFinal, int jFinal)
{
   counter++;
   int intermediate = next[i][j];
   if(intermediate != -1)
   {
      Path(intermediate , j, iFinal, jFinal);
      //cout << intermediate << "(" << counter << ")";
      next[iFinal][jFinal] = intermediate;
      Path(i, intermediate, iFinal, jFinal);
   }
}

void dfs (int u)
{
    //cout << u << endl;
    seen[u] = true;
    for ( int v = 0; v < numberOfNodes; v++){
        if(!seen[v] && (links[u][v] != INFINITY && links[u][v] != 0))
            dfs ( v );
    }
}

double stdRandom ()
{
    return (double)rand()/RAND_MAX;
}

double nonStdRandom (int a, int b)
{
    double str = stdRandom();
    str = a + str * (b - a);
    return str;
}
