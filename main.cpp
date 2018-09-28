
#include <igl/eigs.h>
#include <igl/readOFF.h>
#include <igl/massmatrix.h>
#include <igl/opengl/glfw/Viewer.h>
#include <Eigen/Sparse>
#include <iostream>
#include <fstream>
#include <armadillo>

#include <igl/cotmatrix.h>
#include <vector>

using namespace std;

arma::mat example_cast_arma(Eigen::MatrixXd eigen_A) {

  arma::mat arma_B = arma::mat(eigen_A.data(), eigen_A.rows(), eigen_A.cols(),
                               false, false);

  return arma_B;
}

vector<double> getGPS(string filename, int eigen,int K, bool rever)
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    string off=filename;
    int numero=eigen;
    Eigen::SparseMatrix<double> OLB,M;

    // Load a mesh in OFF format
    igl::readOFF("/home/bryce/Documents/Libraries/libigl/example/libigl-example-project/build/animales/" + off, V, F);
    igl::cotmatrix(V,F,OLB);
    igl::massmatrix(V,F,igl::MASSMATRIX_TYPE_DEFAULT,M);
    OLB=(-OLB).eval();
    

    Eigen::VectorXd values;
    Eigen::MatrixXd vectors;

    if(! igl::eigs(OLB,M,numero,igl::EIGS_TYPE_SM,vectors,values) )
    {
        //cout<<" Decomposition Fail"<<endl;
        return {};
    }
    else{
        //cout<<"Decomposition Success"<<endl;
    }
    // cout<<values<<endl;
    // cout<<vectors<<endl;

    vector<double> Tgps;
    double prom;

    for(int i = 0; i < V.size(); i++)
    {
        prom = 0;
        for (int n = 0; n < numero - 1; n++)   
        {
            prom += abs(vectors.coeff (i,n)/sqrt(values(n)));
        }
        prom=prom/(numero-1);
        Tgps.push_back(prom);
    }
    


    sort(Tgps.begin(),Tgps.end());
    if (rever)
    {
        reverse(Tgps.begin(),Tgps.end());
    }
    vector <double> res;
    
    for(int i = 0; i < K; i++)
    {
        res.push_back(Tgps[i]);
    }
    return res;

}


void imp(vector<double> & n)
{
    //cout<<"vector: ";
    for (auto i : n)
    {
        cout<<i <<" ";
    }
    cout<<endl;
}

void process(string filename, int eigenNUmber, int K)
{
    fstream file(filename);
    string modelName;
    vector<double> modelGPS;
    while (!file.eof())
    {
        file>>modelName;
        modelGPS=getGPS(modelName,eigenNUmber,K,false);
        cout<<modelName<<" -> ";
        imp(modelGPS);

    }
}

int main(int argc, char *argv[])
{
    process("animales.pro",stoi(argv[1]),stoi(argv[2]));
    return 0;
}


int maino(int argc, char *argv[])
{
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    string off;
    int numero;
    if (argc>1)
    {
        cout<<"Archivo: "<<argv[1]<<" Numero: "<<argv[2]<<endl;
        off=argv[1];
        numero=atoi(argv[2]);
    }
    else{
        off="0003.sampling.3.off";
        numero=10;
    }
    Eigen::SparseMatrix<double> OLB,M,Minv;

    igl::readOFF("/home/bryce/Documents/Libraries/libigl/example/libigl-example-project/build/" + off, V, F);
    
    igl::cotmatrix(V,F,OLB);
    //OLB=(-OLB).eval();
    igl::massmatrix(V,F,igl::MASSMATRIX_TYPE_DEFAULT,M);
    //igl::invert_diag(M,Minv);

    cout<<OLB<<endl;
    Eigen::VectorXd values;
    Eigen::MatrixXd vectors;

    //cout<<"OLB : "<<endl<<OLB<<endl;

    if(! igl::eigs(OLB,M,numero,igl::EIGS_TYPE_SM,vectors,values) )
    {
        cout<<" Decomposition Fail"<<endl;
        return 0;
    }
    else{
        cout<<"Decomposition Success"<<endl;
    }
    // cout<<values<<endl;
    // cout<<vectors<<endl;

    vector<double> Tgps;
    vector<double> gps;
    double prom;

    for(int i = 0; i < 10; i++)
    {
        gps.clear();
        prom = 0;
        for (int n = 0; n < numero - 1; n++)   
        {
            prom += abs(vectors.coeff (i,n)/sqrt(values(n)));
        }
        prom=prom/(numero-1);
        Tgps.push_back(prom);
    }
    cout<<"GPS: "<<endl;
    imp(Tgps);

    // Plot the mesh
    igl::opengl::glfw::Viewer viewer;




    

    viewer.data().set_mesh(V, F);
    viewer.data().set_vertices(V);
    viewer.launch();
}