#include <iostream>


struct Point
{
  double x;
  double y;
  double z;

   Point(double _x, double _y, double _z):x(_x),y(_y),z(_z){};
};


struct Face
{
  int v1;
  int v2;
  int v3;
  Face(int _x, int _y, int _z):v1(_x),v2(_y),v3(_z){};
};

vector<Point>  Set_Points;
vector<Face>   Set_Face;

int main()
{
     string File_Off;
     cout<<"Ingrese Archivo .off";  cin>>File_Off;
     string File_patch;
     cout<<"Ingrese Archivo .path";  cin>>File_patch;
     char buffer[10];
     ifstream file((char*)File_Off.c_str());
     file>>buffer;
     int nVertex;
     int nFaces;
     int dato;
     double x,y,z;
     int   v1,v2,v3;
     file>>nVertex>>nFaces>>dato;
     while(!file.eof())
     {
           for(int i=0;i<nVertex;i++)
           {
              file>>x>>y>>y;
               Set_Points.push(Point(x,y,z));
           }

           for(int i=0;i<nFaces;i++)
           {
              file>>v1>>v2>>v3;
               Set_Points.push(Face(v1,v2,v3));
           }

                   
     }
     

}
