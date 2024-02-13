#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
using namespace std;

class Point
{
    double x_, y_;

public:
    Point(double x = 0, double y = 0) : x_(x), y_(y) {}
    double dist(const Point &other)
    {
        return sqrt((x_ - other.x_) * (x_ - other.x_) + (y_ - other.y_) * (y_ - other.y_));
    }

    friend istream &operator>>(istream &stream, Point &p)
    {
        stream >> p.x_ >> p.y_;
        return stream;
    }

    friend class Polygon;
};

class Polygon
{
    vector<Point> vertices;

public:
    Polygon(int numOfSides = 0) : vertices(numOfSides) {}

    void readPolygon()
    {
        int n = vertices.size();
        for (int i = 0; i < n; i++)
        {
            cout << "Vertex " << (i + 1) << ": ";
            cin >> vertices[i];
        }
    }

    bool CheckConvex()
    {
        int n = vertices.size();
        if (n < 3)
        {
            cout << "Not a closed polygon" << endl;
            exit(0);
        }

        double prev = 0, curr = 0;
        for (int i = 0; i < n; i++)
        {
            // Coordinates
            double x0, y0, x1, y1, x2, y2;
            x0 = vertices[i].x_;
            y0 = vertices[i].y_;
            x1 = vertices[(i != 0) ? i - 1 : n - 1].x_;
            y1 = vertices[(i != 0) ? i - 1 : n - 1].y_;
            x2 = vertices[(i + 1) % n].x_;
            y2 = vertices[(i + 1) % n].y_;

            // Vectors representing adjacent sides
            double ux, uy, vx, vy;
            ux = x1 - x0, uy = y1 - y0;
            vx = x2 - x0, vy = y2 - y0;

            curr = ux * vy - uy * vx; // Cross Product

            if (curr * prev < 0)
                return false;

            prev = curr;
        }
        return true;
    }

    double CalculateArea()
    {
        if (!CheckConvex())
        {
            cout << "Polygon is not Convex" << endl;
            exit(0);
        }

        int n = vertices.size();
        double area = 0;
        for (int i = 1; i < n - 1; i++)
        {
            // Triangulation
            double a = vertices[0].dist(vertices[i]);
            double b = vertices[0].dist(vertices[i + 1]);
            double c = vertices[i].dist(vertices[i + 1]);
            double s = (a + b + c) / 2;
            area += sqrt(s * (s - a) * (s - b) * (s - c));
        }
        return area;
    }
};

int main()
{
    int n;
    cout << "Enter number of sides/vertices of polygon: ";
    cin >> n;

    Polygon poly(n);
    cout << "Enter Vertices:" << endl;
    poly.readPolygon();

    if (poly.CheckConvex())
        cout << "Polygon is Convex" << endl;
    else
        cout << "Polygon is Concave" << endl;

    cout << "Area: " << poly.CalculateArea() << endl;
    return 0;
}