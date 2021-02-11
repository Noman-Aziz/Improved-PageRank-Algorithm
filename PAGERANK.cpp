#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

struct Node
{
    Node * next;
    int Destination;

    Node(const int & d)
    {
        next = NULL;
        Destination = d;
    }
};

struct AdjList
{
    Node * head;
    Node * last;
    int OutDegree;

    AdjList()
    {
        OutDegree = 0;
        head = NULL;
        last = NULL;
    }
};

class Graph
{
    AdjList * arr;
    int vertices;
    double **M;
    double *Rank;

    int iterations ;
    double d ;

    //for fixing dangling pages
    void FixDangling()
    {
        for(int i=0 ; i<vertices ; i++)
        {
            if(arr[i].OutDegree == 0)
                arr[i].OutDegree = vertices;
        }
    }

    void InitializeM()
    {
        double value ;

        for(int i=0 ; i<vertices ; i++)
        {
            //the value to assign
            value = 1.0 / double(arr[i].OutDegree) ;

            //fixing dangling page problem
            if(arr[i].OutDegree == vertices)
            {
                for(int j=0 ; j<vertices ; j++)
                    M[j][i] = ( (1-d) * value ) + ( d * (1.0/double(vertices)) );

                continue;
            }

            //assigning values
            for(auto k = arr[i].head ; k != NULL ; k = k->next)
                M[k->Destination][i] = ( (1-d) * value ) + ( d * (1.0/double(vertices)) );
        }
    }

    //for rank multiplications
    void MultiplyMatrix()
    {
        //creating a temp rank matrix for multiplcation
        double *R = new double [vertices];

        //raising power of matrix
        for(int n=1 ; n<=iterations ; n++)
        {
            for(int i=0 ; i<vertices ; i++)
                R[i] = 0;

            for(int i=0 ; i<vertices ; i++)
                for(int j=0 ; j<vertices ; j++)
                    R[i] += M[i][j] * Rank[j];

            Copy(Rank,R);
        }
  
        delete [] R;
    }

    //for copying the matrix b to a
    void Copy(double *&A, double *&B)
    {
        for(int i=0 ; i<vertices ; i++)
                A[i] = B[i];        
    }

    public:

        Graph()
        {

        }

        Graph(const int &s, const double &damp, const int&i)
        {
            vertices = s;
            iterations = i;
            d = damp;

            arr = new AdjList[s];

            Rank = new double[s];

            //initializing ranks equally at start
            for(int i=0 ; i<vertices ; i++)
                Rank[i] = 1.0 / double(vertices) ;

            M = new double *[s];
            for(int i=0 ; i<s ; i++)
                M[i] = new double[s];

            for(int i=0 ; i<s ; i++)
                for(int j=0 ; j<s ; j++)
                    M[i][j] = d * (1.0/double(vertices));
        }

        ~Graph()
        {
            for(int i=0 ; i<vertices ; i++)
                delete [] M[i];

            delete [] M;
            delete [] Rank;
            delete [] arr;
        }

        void Initialize(const int &s, const double &damp, const int&i)
        {
            vertices = s;
            iterations = i;
            d = damp;

            arr = new AdjList[s];

            Rank = new double[s];

            //initializing ranks equally at start
            for(int i=0 ; i<vertices ; i++)
                Rank[i] = 1.0 / double(vertices) ;

            M = new double *[s];
            for(int i=0 ; i<s ; i++)
                M[i] = new double[s];

            for(int i=0 ; i<s ; i++)
                for(int j=0 ; j<s ; j++)
                    M[i][j] = d * (1.0/double(vertices));
        }

        void AddEdge(int s, int d)
        {
            //Directed Graph
            arr[s].OutDegree++;

            if(arr[s].head == NULL)
            {
                arr[s].head = new Node(d);
                arr[s].last = arr[s].head;
            }

            else
            {
                arr[s].last->next = new Node(d);
                arr[s].last = arr[s].last->next;
            }   
        }

        void PageRank()
        {    
            FixDangling();
            InitializeM();
            MultiplyMatrix();

/*
            ofstream output1;
            output1.open("MatrixM.txt",ios::trunc);
        
            if(!output1)
            {
                cout << "ERROR OPENING OUTPUT FILE\n";
                exit(1);
            }


            output1 << "MATRIX M* :-\n\n";
            for(int i=0 ; i<vertices ; i++)
            {
                for(int j=0 ; j<vertices ; j++)
                    output1 << fixed << setprecision(3) << M[i][j] << " " ;
            
                output1 << "\n";
            }

            output1.close();
*/
            ofstream output;
            output.open("output.txt",ios::trunc);
        
            if(!output)
            {
                cout << "ERROR OPENING OUTPUT FILE\n";
                exit(1);
            }

            output << "PAGERANK VALUES OF PAGES 1 to N :-\n\n";

            double sum = 0;

            for(int i=0 ; i<vertices ; i++)
            {
                output  << Rank[i] << "\n";
                sum += Rank[i];
            }

            output << "\nSUM OF PAGERANK VALUES : " << sum << "\n";
        
            output.close();

            cout << "PAGERANKS OF PAGES WRITTEN TO output.txt ALONG WITH THEIR SUM\n";
        }
};

int main()
{
    Graph Test;

    ifstream file("input.txt");

    if(!file)
    {
        cout << "INPUT FILE (input.txt) NOT OPENED" << endl;
        exit(1);
    }

    int a,b;
    int nodes;
    string temp;
    bool once = false;

    while(file)
    {
        if(!once)
        {
            once = true;
            file >> nodes;
            Test.Initialize(nodes,0.85,20);
            continue;
        }   
        
        file >> a;
        file >> b;

        a--;
        b--;


        Test.AddEdge(a,b); 
    }

    Test.PageRank();

    return 0;
}