#include "opencv2/opencv.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main()
{
    int row=2;
    int col=10;
    stringstream name;

    Mat img[row][col];
    for(int irow=0;irow<row;irow++){
        for(int icol=1;icol<col;icol++){
            if(icol<10)
                name<<"/home/jo/image/"<<to_string(irow)<<",0"<<to_string(icol)<<".jpg";
            else
                name<<"/home/jo/image/"<<to_string(irow)<<","<<to_string(icol)<<".jpg";
            img[irow][icol]=imread(name.str());
            name.str("");
        }
    }
    
    if(img[0][1].empty())
    {
        cerr<<"Image lode failed!"<<endl;
        return -1;
    }
    for(int irow=0;irow<row;irow++){
        for(int icol=2;icol<col;icol++){
            vconcat(img[irow][1],img[irow][icol],img[irow][1]);
        }
    }
    for(int irow=1;irow<row;irow++){
        hconcat(img[0][1],img[irow][1],img[0][1]);
    }
    //resize(img[0][1],img[0][1],Size(600,300));
    namedWindow("image");
    imshow("image",img[0][1]);
    imwrite("newimg.jpg",img[0][1]);
    return 0;
}