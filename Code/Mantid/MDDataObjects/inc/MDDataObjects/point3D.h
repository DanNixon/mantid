#ifndef H_POINT3D
#define H_POINT3D

namespace Mantid{
       namespace MDDataObjects{

struct data_point{
    double s;
    double err;
    unsigned long npix;
};
//
class DLLExport point3D
{

public:
    point3D(void):x(0),y(0),z(0),npix(0),s(0),err(0){};
    point3D(double x0, double y0, double z0):x(x0),y(y0),z(z0),npix(0),s(0),err(0){};

    ~point3D(){};

    inline double GetX() const{return x;}
    inline double GetY() const{return y;}
    inline double GetZ() const{return z;}
    double GetS()const{return s;}
    double GetErr()const{return err;}
    unsigned int GetNpix()const{return npix;}

    double &X(){return x;}
    double &Y(){return y;}
    double &Z(){return z;}
    double &S(){return s;}
    double &Err(){return err;}
    unsigned long &Npix(){return npix;}

    point3D &operator=(const data_point &data){
        this->s  =data.s;
        this->err=data.err;
        this->npix=data.npix;
        return *this;
    }
private:
    double x,y,z;
    double s;   // signal field;
    double err; // error field
    unsigned long   npix; // number of the pixels (in) which contribute to this particular data point;

};

}
}
#endif
