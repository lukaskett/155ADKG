#include "algorithms.h"
#include <list>
#include "sortbyx.h"
#include <QMessageBox>

Algorithms::Algorithms()
{

}

int Algorithms::getPointLinePosition(QPoint3D &q,QPoint3D &p1,QPoint3D &p2){

// Analyze point and line position
// 1 point in the left half plane
// 0 point in the right half plane
// -1 point on the line
double ux = p2.x() - p1.x();
double uy = p2.y() - p1.y();

double vx = q.x() - p1.x();
double vy = q.y() - p1.y();

double t = ux * vy - uy * vx;

//Point in the left half plane
if (t>0)
    return 1;
if (t<0)
    return 0;
return -1;
}

double Algorithms::getCircleRadius(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, QPoint3D &c){

    //Return radius of the circle given bz 3 points
    //Modify center point of the circle

    double x1 = p1.x();
    double y1 = p1.y();

    double x2 = p2.x();
    double y2 = p2.y();

    double x3 = p3.x();
    double y3 = p3.y();


    //Calculate coeficients k1-k12
    double k1 = x1 * x1 + y1 * y1;
    double k2 = x2 * x2 + y2 * y2;
    double k3 = x3 * x3 + y3 * y3;
    double k4 = y1 - y2;
    double k5 = y1 - y3;
    double k6 = y2 - y3;
    double k7 = x1 - x2;
    double k8 = x1 - x3;
    double k9 = x2 - x3;
    double k10 = x1 * x1;
    double k11 = x2 * x2;
    double k12 = x3 * x3;

    //Calculate radius
    //Midpoint of the circle
    double m_u = k12 * (-k4) + k11 * k5 - (k10 + k4 * k5) * k6;
    double m_d = x3 * (-k4) + x2 * k5 + x1 * (-k6);
    double m = 0.5 * (m_u / m_d);

    double n_u = k1 * (-k9) + k2 * k8 + k3 * (-k7);
    double n_d = y1 * (-k9) + y2 * k8 + y3 * (-k7);
    double n = 0.5 * (n_u / n_d);

    //Set center coordinates of the circle
    c.setX(m);
    c.setY(n);

    double r = sqrt((x1 - m) * (x1 - m) + (y1 - n) * (y1 - n));

    return r;
}

double Algorithms::getDistance(QPoint3D &p1, QPoint3D &p2)
{
    //Calculate distance betweeen two points
    double dx = p1.x() - p2.x();
    double dy = p1.y() - p2.y();
    double dist = sqrt(dx * dx + dy * dy);

    return dist;
}

int Algorithms::getNerestPoint(QPoint3D &p, std::vector<QPoint3D> &points)
{
    //Find the nearest point, return index of the point
    int i_min = 1;
    double d_min = getDistance(p, points[1]);

    //Browse all points
    for (int i = 2; i < points.size(); i++)
    {
        double d = getDistance(p, points[i]);

        //Actualyze min distance and min distance
        if(d < d_min)
        {
            d_min = d;
            i_min = i;
        }
    }

    return i_min;
}

int Algorithms::getDelaunayPoint(QPoint3D &s, QPoint3D &e, std::vector<QPoint3D> &points)
{
    //Find optimal Delaunay point
    int i_min = -1;
    double r_min = INFINITY;

    //Browse all points
    for (unsigned int i = 0; i < points.size(); i++)
    {
        //Test if points[i] is different from start, end
        if((s != points[i]) && (e != points[i]))
        {
            //Points[i] in the left half plane
            if(getPointLinePosition(points[i], s, e) == 1 )
            {
                //Calculate circle radius
                QPoint3D c;
                double r = getCircleRadius(s, e, points[i], c);

                //Position of circle center in the right plane
                if(getPointLinePosition(c, s, e) == 0)
                    r = -r;
                //Actualyze min radius
                if(r < r_min)
                {
                    r_min = r;
                    i_min = i;
                }
            }
        }
    }
    return i_min;
}

std::vector<Edge> Algorithms::DT(std::vector<QPoint3D> &points)
{
    //Create Delaunay triangulation
    std::vector<Edge> dt;
    std::list<Edge> ael; //active edges list

    //Sort points by X
    std::sort(points.begin(), points.end(), SortByX());

    //Pivot
    QPoint3D q = points[0];

    //Find nearest point
    int index = getNerestPoint(q, points);
    QPoint3D qn = points[index];

    //Create the first Delaunay edge
    Edge e1(q, qn);

    //Find optimal Delaunay point
    int i_o = getDelaunayPoint(q, qn, points);

    //No suitable point found -> change edge orientation,
    if(i_o == -1)
    {
        //Change orientation
        e1.changeOrientation();

        //Find optimal Delaunay point
        i_o = getDelaunayPoint(e1.getStart(), e1.getEnd(), points); //body v primce by mohly nastat, predpokladame, ze jsme bod nasli
    }

    //Third point of the triangle
    QPoint3D q3 = points[i_o];

    //Create initial triangle - represented by sequence of three edges
    Edge e2(e1.getEnd(), q3);
    Edge e3(q3, e1.getStart());

    //Add edges to dt
    dt.push_back(e1);
    dt.push_back(e2);
    dt.push_back(e3);

    //Add edges to active edges list (ael)
    ael.push_back(e1);
    ael.push_back(e2);
    ael.push_back(e3);

    //Start main cycle - repeate until any triangle exist
    while(!ael.empty())
    {
        Edge ed1 = ael.back();
        ael.pop_back();

        //Change edge orientation
        ed1.changeOrientation();

        //Find optimal Delaunay point
        int i2 = getDelaunayPoint(ed1.getStart(), ed1.getEnd(), points);

        if(i2 != -1)
        {
            //Third point of the triangle
            QPoint3D q3n = points[i2];

            //Create new triangle
            Edge ed2(ed1.getEnd(), q3n);
            Edge ed3(q3n, ed1.getStart());

            //Add edges to dt
            dt.push_back(ed1);
            dt.push_back(ed2);
            dt.push_back(ed3);
            \
            //Change orientation
            ed2.changeOrientation();
            ed3.changeOrientation();

            //Is ed2 and ed3 in ael?
            std::list<Edge>::iterator ie2 = find(ael.begin(), ael.end(), ed2);
            std::list<Edge>::iterator ie3 = find(ael.begin(), ael.end(), ed3);

            //Add edge ed2 to active edges list (ael), if not existing
            if(ie2 == ael.end())
            {
                ed2.changeOrientation();
                ael.push_back(ed2);
            }

            //Erase ed2
            else ael.erase(ie2);

            //Add edge ed3 to active edges list (ael), if not existing
            if(ie3 == ael.end())
            {
                ed3.changeOrientation();
                ael.push_back(ed3);
            }

            //Erase ed3
            else ael.erase(ie3);
        }

    }
    return dt;
}

QPoint3D Algorithms::getContourPoint(QPoint3D &p1,QPoint3D &p2, double z)
{
    //Compute contour point

    //Compute coordinates
    double x = (p2.x() - p1.x())/(p2.getZ() - p1.getZ()) * (z - p1.getZ()) + p1.x();
    double y = (p2.y() - p1.y())/(p2.getZ() - p1.getZ()) * (z - p1.getZ()) + p1.y();

    //Create contour point and assign coordinates
    QPoint3D A(x, y, z);

    return A;
}

std::vector<Edge> Algorithms::createContourLines(std::vector<Edge> &dt, double z_min, double z_max, double dz, std::vector<double> &metadata)
{
    //Generate contour lines

    //Generated contours
    std::vector<Edge> contours;

    //Browse all triangles and search for contours
    for (unsigned int i = 0; i < dt.size(); i+=3)
    {
        //Get triangle points
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd(); //QPoint3D p1 = dt[i+2].getStart();

        //Find all plane-triangle intersections
        for (double z = z_min; z <= z_max; z += dz )
        {
            //Calculate height differences
            double dz1 = z - p1.getZ();
            double dz2 = z - p2.getZ();
            double dz3 = z - p3.getZ();

            //Test criterions
            double t12 = dz1 * dz2;
            double t23 = dz2 * dz3;
            double t31 = dz3 * dz1;

            //Triangle in the plane(coplanar)
            if((dz1 == 0) && (dz2 == 0) && (dz3 == 0))
                continue;

            //I) Edge e12 of the triangle in the plane
            else if((dz1 == 0) && (dz2 == 0))
                contours.push_back(dt[i]);

            //II) Edge e23 of the triangle in the plane
            else if((dz2 == 0) && (dz3 == 0))
                contours.push_back(dt[i+1]);

            //III) Edge e31 of the triangle in the plane
            else if((dz3 == 0) && (dz1 == 0))
                contours.push_back(dt[i+2]);

            //Triangle intersects the plane
            //1)Edge e12 and edge e23 are intersected
            else if((t12 < 0 && t23 <= 0) || (t12 <= 0 && t23 < 0))
            {
                //Compute intersections points
                QPoint3D a = getContourPoint(p1, p2, z);
                QPoint3D b = getContourPoint(p2, p3, z);

                // Create fragment of contour line
                Edge e(a, b);
                // Add fragment to the contours
                contours.push_back(e);
                metadata.push_back(z);
            }

            // 2)Edge e23 and edge e31 are intersected
            else if((t23 < 0 && t31 <= 0) || (t23 <= 0 && t31 < 0))
            {
                //Compute intersections points
                QPoint3D a = getContourPoint(p2, p3, z);
                QPoint3D b = getContourPoint(p3, p1, z);

                // Create fragment of contour line
                Edge e(a, b);
                // Add fragment to the contours
                contours.push_back(e);
                metadata.push_back(z);
            }

            // 3)Edge e31 and edge e12 are intersected
            else if((t31 < 0 && t12 <= 0) || (t31 <= 0 && t12 < 0))
            {
                //Compute intersections points
                QPoint3D a = getContourPoint(p3, p1, z);
                QPoint3D b = getContourPoint(p1, p2, z);

                // Create fragment of contour line
                Edge e(a, b);
                // Add fragment to the contours
                contours.push_back(e); //edge
                metadata.push_back(z); //height
            }

        }
    }
    return contours;
}

double Algorithms::calculateSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Calculate slope of the triangle

    //Calculate direction vector
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double uz = p2.getZ() - p1.getZ();

    double vx = p3.x() - p1.x();
    double vy = p3.y() - p1.y();
    double vz = p3.getZ() - p1.getZ();

    //Calculate normal vector and its norm
    double nx = uy * vz - vy * uz;
    double ny = -(ux * vz - vx * uz);
    double nz = ux * vy - vx * uy;
    double nt = sqrt(nx * nx + ny * ny + nz * nz);

    return acos(nz / nt) / M_PI * 180;
}

double Algorithms::calculateAspect(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Calculate aspect of the triangle

    //Calculate direction vector
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double uz = p2.getZ() - p1.getZ();

    double vx = p3.x() - p1.x();
    double vy = p3.y() - p1.y();
    double vz = p3.getZ() - p1.getZ();

    //Calculate normal vector and its norm
    double nx = uy * vz - vy * uz;
    double ny = -(ux * vz - vx * uz);

    return atan2(nx, ny) / M_PI * 180;
}

std::vector<Triangle> Algorithms::analyzeDMT(std::vector<Edge> &dt)
{
    //Analyze DMT, compute aslope and aspect
    std::vector<Triangle> triangles;

    //Browse triangulation one by one
    for (int i = 0; i < dt.size(); i += 3)
    {
        //Triangle vertices
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        //Compute slope and aspect
        double slope = calculateSlope(p1, p2, p3);
        double aspect = calculateAspect(p1, p2, p3);

        //Create triangle and assign values
        Triangle triangle(p1, p2, p3, slope, aspect);

        //Store triangle
        triangles.push_back(triangle);
    }

    return triangles;
}

std::vector<QPoint3D> Algorithms::importMeasurement(std::string path, int width, int height,  double &min_z, double &max_z)
{
    std::vector<QPoint3D> points;

    //Open the file
    std::ifstream measurement_file;
    measurement_file.open(path);

    //Check if the file is opened
    if(!measurement_file.is_open())
    {
        measurement_file.close();
        return points;
    }

    //Store max and min coordinates
    //Inicialize - https://en.cppreference.com/w/cpp/types/numeric_limits/max

    double min_x = std::numeric_limits<double>::max();
    double min_y = std::numeric_limits<double>::max();
    min_z = std::numeric_limits<double>::max();

    double max_x = std::numeric_limits<double>::min();
    double max_y = std::numeric_limits<double>::min();
    max_z = std::numeric_limits<double>::min();

    //Go through whole file and store all measured points
     while(measurement_file.good())
     {
         double x, y, z;
         measurement_file >> x;
         measurement_file >> y;
         measurement_file >> z;
         points.push_back(QPoint3D(x, y, z));

         //Actualize MIN and MAX values
         if(x > max_x) max_x = x;
         if(y > max_y) max_y = y;
         if(z > max_z) max_z = z;

         if(x < min_x) min_x = x;
         if(y < min_y) min_y = y;
         if(z < min_z) min_z = z;

     }

     measurement_file.close();

     //Scale points to fit Canvas
     int boundary_gap = 4;
     double kx = (width - 2 * boundary_gap) / (max_x - min_x);
     double ky = (height - 2 * boundary_gap) / (max_y - min_y);

     //Reduce x, y coordinates and multiple with coefficient
     for (unsigned i = 0; i < points.size(); i++)
     {
         points[i].setX((points[i].x() - min_x) * kx + boundary_gap);
         points[i].setY((points[i].y() - min_y) * ky + boundary_gap);
     }

     return points;
}

std::vector<QPoint3D> Algorithms::generateShapes(int shape, int width, int height)
{
    //Generate terrain shape: 0 - hill, 1 - valley, 2 - spur, 3 - platform

    //Clear points
    std::vector<QPoint3D> points;

    //Generate hill
    if(shape == 0)
    {
        //Elipse center
        QPoint3D center(width / 2, height / 2, 0);

        //Scale elipse accordint to the window size
        unsigned int major_diam = width * 0.4;
        unsigned int minor_diam = height * 0.25;

        //Define bounding box
        double k = 1.1;
        points.push_back(QPoint3D(center.x() + k * major_diam, center.y() + k * minor_diam, 0));
        points.push_back(QPoint3D(center.x() - k * major_diam, center.y() - k * minor_diam, 0));
        points.push_back(QPoint3D(center.x() + k * major_diam, center.y() - k * minor_diam, 0));
        points.push_back(QPoint3D(center.x() - k * major_diam, center.y() + k * minor_diam, 0));

        //Parameters
        unsigned int slope = 30;
        double density = 0.3;
        double z_angle = 0;
        double z_step = 90 / floor(2 * M_PI / density);

        for(unsigned int s = 0; s < major_diam; s += slope)
        {
            for (double u = 0; u < 2 * M_PI; u += density)
            {
                double x = center.x() + (major_diam - s) * cos(u);
                double y = center.y() + (minor_diam - s) * sin(u);
                double z = sin(z_angle) * 10;
                QPoint3D point(x, y, z);
                points.push_back(point);
            }

            //Increase angle for z-coord generating
            z_angle += z_step;
        }
        //Return z_max value?
    }

    //Generate valley
    else if(shape == 1)
    {
        //Devide into the parts
        int start = 4;
        double end = width - 2 * start;
        double part = end / 4;

        //General parameters
        double density = 10;
        int dz = 30;

        for (int st_z = start; st_z < height - 2 * start - (height / 4); st_z += dz)
        {
            //1)First part
            double start_first = 4;
            double end_first = part;
            for (double step = start_first; step < end_first; step += density)
            {
                double x = step;
                double y = st_z;
                double z = st_z;
                QPoint3D point(x, y, z);
                points.push_back(point);

            }

            //2)Middle part
            double start_valley = end_first;
            double end_valley = end_first + 2 * part;

            int B = 2;
            double v1_a = -M_PI/2/B;
            double v1_step = M_PI / ((end_valley - start_valley)/density);
            int amplitude = height / 4;
            for (double step = start_valley; step < end_valley; step += density)
            {
                double x = step;
                double y = st_z;
                double z = st_z - amplitude/2  + amplitude/2 * sin(B * (-v1_a));
                QPoint3D point(x, y, z);
                points.push_back(point);
                v1_a += v1_step;
            }

            //3)Last part
            double start_last = end_valley;
            double end_last = end;
            for (double step = start_last; step < end_last; step += density)
            {
                double x = step;
                double y = st_z;
                double z = st_z;
                QPoint3D point(x, y, z);
                points.push_back(point);
            }

        }
    }

    //Generate spur
    //else if(shape == 2){}

    //Generate platform
    //else{}

    return points;
}


