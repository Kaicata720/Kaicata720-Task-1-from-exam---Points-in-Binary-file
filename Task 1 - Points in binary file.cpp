

#include <iostream>
#include <fstream>
using namespace std;

struct Point {
    float x, y;

       void printPoint()const {
       cout << x << ' ' << y << endl;
    }
   void SumPoint(const Point p2) {
       x += p2.x; 
       y += p2.y;
      
   }
};


void writeOnBinaryFile(const char* filename) {
    ofstream outBinaryStream = ofstream(filename, ios::binary);
    if (!outBinaryStream) {
        cerr << filename << "couldn't open" << endl;
        return;
    }
    const int sizeArray = 5;
    Point array[sizeArray] = { {1, 2},{3, 4},{5, 6},{5, 7},{99, 99} };
    
    //outBinaryStream.write(reinterpret_cast<const char*>(&sizeArray), sizeof(sizeArray));
    for (size_t i = 0; i < sizeArray; i++)
    {
        outBinaryStream.write(reinterpret_cast<const char*>(&array[i]), sizeof(array[i]));

    }
    outBinaryStream.close();
}

void readArrayFromBinaryFile(const char* filename,Point* array,int sizeArray) {
    ifstream inBinaryStream = ifstream(filename, ios::binary);
    if (!inBinaryStream) {
        cerr << filename << "couldn't open" << endl;
        return;
    }
    
   // int sizeArray;
  //  inBinaryStream.read(reinterpret_cast<char*>(&sizeArray), sizeof(sizeArray));
   // Point* array = new Point[sizeArray];
    if (!array) {
        cerr << "Can't allocate memory";
        return;
    }
   
   for(int i=0;i<sizeArray;i++)
    {
        inBinaryStream.read(reinterpret_cast<char*>(&array[i]), sizeof(array[i]));
       

    }
   
    
    inBinaryStream.close();
}

Point CenterofWeight(Point*array,int sizeArray) {
    Point center = { 0,0 };
    for (int i = 0; i < sizeArray; i++)
    {
        center.SumPoint(array[i]);
    }
    center.x /= sizeArray;
    center.y /= sizeArray;
    return center;
}

double DistanceCenterToPoint(Point point, Point center) {
    double distance = pow(point.x - center.x, 2) + pow(point.y - center.y, 2);
    return sqrt(distance);
}


void sortArrayByDistance(Point* array, int sizeArray) {
    Point center = CenterofWeight(array, sizeArray);

    for (int i = 0; i < sizeArray-1; i++) {
       
        int index = i;
        for (int j = i + 1; j < sizeArray; j++) {
            if (DistanceCenterToPoint(center, array[index]) > DistanceCenterToPoint(center, array[j])) {
                index = j;
            }
        }
        Point x = array[i];
        array[i] = array[index];
        array[index] = x;
    }
}

void ReplacePointsOutsideOfCircle(Point* array, int sizeArray, double R) {
    Point center = CenterofWeight(array, sizeArray);
    for (size_t i = 0; i < sizeArray; i++)
    {
        if (DistanceCenterToPoint(array[i], center) > R) {
            array[i] = center;
        }
    }
}
void print3FurthestPoints(Point* array, int sizeArray) {
    cout << "The 3 furthest points from centre:" << endl;
    for (size_t i = sizeArray - 3; i < sizeArray; i++)
    {

        array[i].printPoint();
       

    }
}



int main()
{
    writeOnBinaryFile("input-points.dat");

    ifstream InBinaryFile = ifstream("input-points.dat", ios::binary | ios::ate);
    // ==> We get the position of the end of the file and size of file
    streampos fileSize = InBinaryFile.tellg();
    InBinaryFile.seekg(0, ios::beg);
   
    // ==> size of array for points from file
    int sizeArray = fileSize / sizeof(Point);
   

    Point* array = new Point[sizeArray];
    if (!array) {
        cerr << "Couldn't allocate memory for array";
        return -1;
    }

    readArrayFromBinaryFile("input-points.dat", array,sizeArray);
    InBinaryFile.close();
    // ==> second array so we don't lose the sequence of points after sorting
    Point* tmpArray = new Point[sizeArray];
    for (size_t i = 0; i < sizeArray; i++)
    {
        tmpArray[i] = array[i];
    }
    sortArrayByDistance(tmpArray, sizeArray);
    print3FurthestPoints(tmpArray, sizeArray);
    delete[] tmpArray;



    Point center = CenterofWeight(array, sizeArray);
    cout << "Center= ";
    center.printPoint();
   
   

   
   

    float R;
    cout << "Radius= ";
    cin >> R;
    
    cout << "\n\n";

    ReplacePointsOutsideOfCircle(array, sizeArray, R);

    //---------------------------
    // ==> We put the points back in the file after changing
    ofstream outBinaryStream = ofstream("input-points.dat", ios::binary | ios::trunc);
    if (!outBinaryStream) {
        cerr << "Couldn't open file for writing";
        return -1;
    }
    
    for (size_t i = 0; i < sizeArray; i++)
    {
        outBinaryStream.write(reinterpret_cast<const char*>(&array[i]), sizeof(array[i]));
        array[i].printPoint();
    }
    
    outBinaryStream.close();
}




