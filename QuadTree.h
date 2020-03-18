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

    inline friend bool operator==(const Point& lhs, const Point& rhs) {
        float x1 = std::floor((lhs.x * 100) + 0.5) / 100;
        float y1 = std::floor((lhs.y * 100) + 0.5) / 100;
        float x2 = std::floor((rhs.x * 100) + 0.5) / 100;
        float y2 = std::floor((rhs.y * 100) + 0.5) / 100;
        return x1 == x2 && y1 == y2;
    }

    inline friend bool operator!=(const Point& lhs, const Point& rhs) { return !(lhs == rhs); }

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

    inline const Point& getCenter() const { return this->center; }
    inline double getHalfSize() const { return this->halfSize; }

    inline friend bool operator==(const Quad& lhs, const Quad& rhs) {
        return lhs.center == rhs.center && lhs.lowerLeft == rhs.lowerLeft && lhs.upperRight == rhs.upperRight;
    }

    inline friend bool operator!=(const Quad& lhs, const Quad& rhs) { return !(lhs == rhs); }

private:
    void calculateBounds();
    bool containsInternal(const double x, const double y) const;
};

class QuadTree {
public:
    static const int MAX_CAPACITY = 4;

private:
    std::unique_ptr<Quad> bounds;
    bool isBoundSet = false;

    std::vector<Point> nodes;

    std::unique_ptr<QuadTree> topLeft;
    std::unique_ptr<QuadTree> topRight;
    std::unique_ptr<QuadTree> botLeft;
    std::unique_ptr<QuadTree> botRight;

public:
    QuadTree(const double x = 0, const double y = 0, const double halfWidth = 0) {
        this->setBound(x, y, halfWidth);
        this->clear();
    }

    QuadTree(const QuadTree& other) { *this = other; }
    QuadTree(QuadTree& other) { *this = std::move(other); }

    QuadTree& operator=(const QuadTree& other);
    QuadTree& operator=(QuadTree&& other);

    bool isUsable() const { return this->isBoundSet; }

    void setBound(const double x, const double y, const double halfWidth) {
        this->bounds = std::make_unique<Quad>(x, y, halfWidth);
        this->isBoundSet = true;
    }

    bool insert(const Point& p) { return this->insertInternal(p); }

    void QuadTree::queryPoint(const double x, const double y, std::vector<const Point*>& output);
    void queryRange(const Quad& range, std::vector<const Point*>& output);

    void clear();

private:
    bool insertInternal(const Point& p);
    void subdivide();
};
