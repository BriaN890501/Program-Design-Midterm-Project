#include <iostream> // "#" represents issues or possible changes
#include <cmath>
#include <time.h>
using namespace std;

// max parameters
const int max_n = 1000, max_m = 10000;

const int max_point = 100, max_T = 10000, max_k = 1; // points at most 100

// definition
int n, m, w, d; // map size / threat point amount / turning cost /max distance

int X[max_m] = {0}, Y[max_m] = {0}, R[max_m] = {0}, P[max_m] = {0}; // coordinate (X, Y) / range / threat point weight  # initilaization completed

int sx, sy, ex, ey; // coordinate of start and end points

// defining functions
void SimulatedAnnealing(); //  (main algorithm)

void SearchForNewSolution(double X[], double Y[], int &T);

void UpdateSolution(double tempbestRouteX[], double tempbestRouteY[], double tempRouteX[], double tempRouteY[], int T);

int LengthOfRoute(double route[]);

double threat(double routeX[], double routeY[]);

int main()
{
    // setting time as the seed of rand()
    srand((unsigned)time(NULL)); 
    
    // cin
    cin >> n >> m >> w >> d;
    
    for(int i = 0; i < m; i++)
    {
        cin >> X[i];
    }
    
    for(int i = 0; i < m; i++)
    {
        cin >> Y[i];
    }
    
    for(int i = 0; i < m; i++)
    {
        cin >> R[i];
    }
    
    for(int i = 0; i < m; i++)
    {
        cin >> P[i];
    }
    
    cin >> sx >> sy >> ex >> ey;
    
    // program start
    SimulatedAnnealing();
    
    return 0;
}
void SimulatedAnnealing()
{
    // definition and initialization
    double routeX[max_point], routeY[max_point]; // the x,y coordinate of the i th point in the route
    routeX[0] = sx;
    routeY[0] = sy;
    routeX[1] = ex;
    routeY[1] = ey;
    
    for(int i = 2; i < max_point; i++) // assign -1 to other values 
    {
        routeX[i] = -1;
        routeY[i] = -1;
    }
    
    // setting the temperature
    int T = max_T;
    while(T > 0)
    {
        SearchForNewSolution(routeX,routeY,T);
    }
    
    int len = LengthOfRoute(routeX);
    
    cout << len-2;
    
    for(int i = 1; i < len-1; i++) // output result
    {
        cout << " " << routeX[i] << " " << routeY[i];
    }
    
}

void SearchForNewSolution(double X[], double Y[], int &T)
{
    // finding other routes
    int k=max_k;
    
    double tempbestRouteX[max_point], tempbestRouteY[max_point]; // array to save temporary best route
    
    for(int i = 0; i < max_point; i++) 
    {
        tempbestRouteX[i] = X[i];
        tempbestRouteY[i] = Y[i];
    }
    
    // decide which to choose
    int choose = rand() % 15; // # adjustment can be implemented (which interval)
    
    // 0~2 for adding a point between s and e
    if(choose <=2  && LengthOfRoute(tempbestRouteX) != max_point) // check range and valid operation
    {
        for(; k > 0; k--)
        {
            // making a temporary route
            double tempRouteX[max_point], tempRouteY[max_point];
            
            for(int i = 0; i < max_point; i++) 
            {
                tempRouteX[i] = X[i];
                tempRouteY[i] = Y[i];
            }
            
            int len = LengthOfRoute(tempRouteX); // point amount
            
            // making a random integer between 1 to len-1
            int a = rand() % (len - 1) + 1;
            
            // finding a good coordinate(near the line of a th and a+1 th point) for new point
            double x = rand() % static_cast<int> (tempRouteX[a] - tempRouteX[a - 1] + 1) + tempRouteX[a - 1];
            double y = rand() % static_cast<int> (tempRouteY[a] - tempRouteY[a - 1] + 1) + tempRouteY[a - 1];
            
            // shift points after a th point right
            if( ( x!=tempRouteX[a] || y!=tempRouteY[a] ) && ( x!=tempRouteY[a+1] || y!=tempRouteY[a+1] ) )
            {
                for(int i = len; i > a; i--)
                {
                    tempRouteX[i] = tempRouteX[i - 1];
                    tempRouteY[i] = tempRouteY[i - 1];
                }
                
                // insert the point
                tempRouteX[a] = x;
                tempRouteY[a] = y;
                
                UpdateSolution(tempbestRouteX, tempbestRouteY, tempRouteX, tempRouteY,T);
            }
        }
        T--; // only if operation is effective that T minus
    }
    
    // 3~10 for taking a point out between s and e
    else if(choose <= 10 && LengthOfRoute(tempbestRouteX) != 2) // check range and valid operation
    {
        for(; k > 0; k--)
        {
            // making a temporary route
            double tempRouteX[max_point], tempRouteY[max_point];
            for(int i = 0; i < max_point; i++) 
            {
                tempRouteX[i] = X[i];
                tempRouteY[i] = Y[i];
            }
            
            int len = LengthOfRoute(tempRouteX);
            
            // making a random integer between 2 to len-1
            int a = rand() % (len - 2) + 2;
            
            // shift point after a th point left
            for(int i = a; i < len; i++)
            {
                tempRouteX[i - 1] = tempRouteX[i];
                tempRouteY[i - 1] = tempRouteY[i];
            }
            
            // delete the last point
            tempRouteX[len - 1] = -1;
            tempRouteY[len - 1] = -1;

            UpdateSolution(tempbestRouteX, tempbestRouteY, tempRouteX, tempRouteY, T);
        }
        T--; // only if operation is effective that T minus
    }
    
    // 11~14 for changing the coordinate of a point
    else if(choose <= 14 && LengthOfRoute(tempbestRouteX) != 2)
    {
        for(; k > 0; k--)
        {
            // making a temp route
            double tempRouteX[max_point], tempRouteY[max_point];
            
            for(int i = 0; i < max_point; i++)
            {
                tempRouteX[i] = X[i];
                tempRouteY[i] = Y[i];
            }
            
            int len = LengthOfRoute(tempRouteX);
            
            // making a random integer between 2 to len-1
            int a = rand() % (len - 2) + 2;
            
            // finding a good coordinate(near the line of a-1 th and a+1 th point) for new point
            double x = rand() % static_cast<int>(tempRouteX[a] - tempRouteX[a - 2] + 1) + tempRouteX[a - 2];
            double y = rand() % static_cast<int>(tempRouteY[a] - tempRouteY[a - 2] + 1) + tempRouteY[a - 2];
            
            // changing the coordinate
            tempRouteX[a - 1] = x;
            tempRouteY[a - 1] = y;

            UpdateSolution(tempbestRouteX, tempbestRouteY, tempRouteX, tempRouteY,T);
        }
        T--; // only if operation is effective that T minus
    }
    
    for(int i = 0; i < max_point; i++) // record the changed route
    {
        X[i] = tempbestRouteX[i];
        Y[i] = tempbestRouteY[i];
    }
}

void UpdateSolution(double tempbestRouteX[], double tempbestRouteY[], double tempRouteX[], double tempRouteY[], int T)
{
    if( threat( tempbestRouteX, tempbestRouteY ) > threat( tempRouteX, tempRouteY ) ) // update solution if better (less threat)
    {
        for(int i = 0; i < max_point; i++)
        {
            tempbestRouteX[i] = tempRouteX[i];
            tempbestRouteY[i] = tempRouteY[i];
        }
    }
    
    else
    {
        double deltathreat = threat(tempRouteX,tempRouteY) - threat(tempbestRouteX,tempbestRouteY);
        if(rand() <= exp((-deltathreat) / T) * RAND_MAX) // decide whether to accept worse situation # can be adjusted
        {
            for(int i = 0; i < max_point; i++)
            {
                tempbestRouteX[i] = tempRouteX[i];
                tempbestRouteY[i] = tempRouteY[i];
            }
        }
    }
}

int LengthOfRoute(double route[]) // # counting points? suggest change function name in case of misinterpretation
{
    int i = 2;
    while(route[i] >= 0)
    {
        i++;
        if(i == max_point)
        {
            break;
        }
    }
    return i;
}

// temporarily consider threat as points
double threat(double routeX[], double routeY[]) // # must implement right threat function, then consider runtime
{
	int len = LengthOfRoute(routeX);	
	double newX = routeX[0];
	double newY = routeY[0];
	double threat = 0;
	double leftdis = 0;
	double dis;
	for(int i = 1; i < len; i++)
	{
		double DisBetDots = sqrt(pow(routeX[i]-routeX[i-1],2) + pow(routeY[i]-routeY[i-1],2));
		for(int j = 0; j < DisBetDots-1; j++)
		{
			if(j==0)
			{
				newX = newX + (1-leftdis)*(routeX[i]-routeX[i-1])/DisBetDots;
				newY = newY + (1-leftdis)*(routeY[i]-routeY[i-1])/DisBetDots;
			}
			else
			{
				newX = newX + (routeX[i]-routeX[i-1])/DisBetDots;
				newY = newY + (routeY[i]-routeY[i-1])/DisBetDots;
			}
			if(DisBetDots-2<j&&j<DisBetDots-1)
				leftdis = sqrt(pow(routeX[i] - newX, 2) + pow(routeY[i] - newY, 2));
			
			
			for(int k = 0;k < m; k++)
			{
				dis = sqrt(pow(X[k] - newX, 2)+pow(Y[k] - newY, 2));
				if(dis>R[k])
					continue;
				threat = threat + P[k]*((R[k]-dis)/R[k]);
			}
			
		}
	}
	
	
	
		
    return threat;
}

