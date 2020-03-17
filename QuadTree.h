#pragma once

#include <memory>
#include <sstream>
#include <vector>

class Point {
public:
    double x;
    double y;

    std::vector<std::size_t> indicies;

public:
    Point(const double x = 0, const double y = 0) : x(x), y(y), indicies() {}
    Point(const Point& other) { *this = other; }
    Point(Point&& other) { *this = std::move(other); }
    ~Point() = default;

    Point& operator=(const Point& other);
    Point& operator=(Point&& other);

    inline std::string toString() const {
        std::ostringstream oss;
        oss << "(" << this->x << "," << this->y << ")";
        return oss.str();
    }

    inline friend Point operator+(const Point& p1, const Point& p2) { return Point(p1.x + p2.x, p1.y + p2.y); }
    inline friend Point operator-(const Point& p1, const Point& p2) { return Point(p1.x - p2.x, p1.y - p2.y); }

    inline friend std::ostream& operator<<(std::ostream& os, const Point& point) {
        os << point.toString();
        return os;
    }
};

class Quad {
private:
    double halfSize = 0;

    Point center;
    Point lowerLeft;
    Point upperRight;

public:
    Quad(const double x = 0, const double y = 0, const double half = 0)
        : center(Point(x, y)), halfSize(half) {
        this->calculateBounds();
    }

    Quad(const Quad& other) { *this = other; }
    Quad(Quad&& other) { *this = std::move(other); }
    ~Quad() = default;

    Quad& operator=(const Quad& other);
    Quad& operator=(Quad&& other);

    bool contains(const Point& p) const { return this->containsInternal(p.x, p.y); }
    bool contains(const double x, const double y) const { return this->containsInternal(x, y); }

    bool intersects(const Quad& other) const;

private:
    void calculateBounds();
    bool containsInternal(const double x, const double y) const;
};

class QuadTree {
public:
    static const int MAX_CAPACITY = 4;

private:
    Quad bounds;

    std::vector<Point> nodes;

    std::unique_ptr<QuadTree> topLeft;
    std::unique_ptr<QuadTree> topRight;
    std::unique_ptr<QuadTree> botLeft;
    std::unique_ptr<QuadTree> botRight;

public:
    QuadTree(const double x = 0, const double y = 0, const double halfWidth = 0) : bounds(x, y, halfWidth), nodes() {
        this->topLeft.reset(nullptr);
        this->topRight.reset(nullptr);
        this->botLeft.reset(nullptr);
        this->botRight.reset(nullptr);
    }

    QuadTree(const QuadTree& other) { *this = other; }
    QuadTree(QuadTree& other) { *this = std::move(other); }

    QuadTree& operator=(const QuadTree& other);
    QuadTree& operator=(QuadTree&& other);

    bool insert(const double x, const double y) { return this->insertInternal(Point(x, y)); }
    bool insert(const Point& p) { return this->insertInternal(p); }

    std::vector<Point> queryRange(const Quad& range);

private:
    bool insertInternal(const Point& p);
    void subdivide();
};
