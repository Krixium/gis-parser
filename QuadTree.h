#pragma once

#include <memory>
#include <sstream>
#include <vector>

#include "utils.h"

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
        oss << "{";
        oss << "(" << this->x << "," << this->y << "),[";
        if (!this->indicies.empty()) {
            if (this->indicies.size() > 1) {
                for (int i = 0; i < this->indicies.size(); i++) {
                    oss << this->indicies[i] << ",";
                }
            }
            oss << this->indicies.back();
        }
        oss << "]";
        oss << "}";

        return oss.str();
    }

    inline friend bool operator==(const Point& lhs, const Point& rhs) {
        double x1 = utils::round(lhs.x);
        double y1 = utils::round(lhs.y);

        double x2 = utils::round(rhs.x);
        double y2 = utils::round(rhs.y);

        return utils::round(x1 - x2) == 0 && utils::round(x1 - x2) == 0;
    }

    inline friend bool operator!=(const Point& lhs, const Point& rhs) { return !(lhs == rhs); }

    inline friend Point operator+(const Point& p1, const Point& p2) { return Point(utils::round(p1.x + p2.x), utils::round(p1.y + p2.y)); }
    inline friend Point operator-(const Point& p1, const Point& p2) { return Point(utils::round(p1.x - p2.x), utils::round(p1.y - p2.y)); }

    inline friend std::ostream& operator<<(std::ostream& os, const Point& point) {
        os << point.toString();
        return os;
    }
};

class Quad {
private:
    double halfWidth = 0;
    double halfHeight = 0;

    Point center;
    Point lowerLeft;
    Point upperRight;

public:
    Quad(const double x = 0, const double y = 0, const double halfWidth = 0, const double halfHeight = 0)
        : center(Point(x, y)), halfWidth(halfWidth), halfHeight(halfHeight) {
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
    inline double getHalfWidth() const { return this->halfWidth; }
    inline double getHalfHeight() const { return this->halfHeight; }

    inline std::string toString() const {
        std::ostringstream oss;
        oss << "Quad(" << "Center - " << this->center << ", Width - " << this->halfWidth * 2 << ", Height - " << this->halfHeight * 2 << ")";
        return oss.str();
    }

    inline friend std::ostream& operator<<(std::ostream& os, const Quad& quad) {
        os << quad.toString();
        return os;
    }

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
    QuadTree(const double x = 0, const double y = 0, const double halfWidth = 0, const double halfHeight = 0) {
        this->setBound(x, y, halfWidth, halfHeight);
        this->clear();
    }

    QuadTree(const QuadTree& other) { *this = other; }
    QuadTree(QuadTree& other) { *this = std::move(other); }

    QuadTree& operator=(const QuadTree& other);
    QuadTree& operator=(QuadTree&& other);

    bool isUsable() const { return this->isBoundSet; }

    void setBound(const double x, const double y, const double halfWidth, const double halfHeight) {
        this->bounds = std::make_unique<Quad>(x, y, halfWidth, halfHeight);
        this->isBoundSet = true;
    }

    bool insert(const Point& p);

    void QuadTree::queryPoint(const double x, const double y, std::vector<const Point*>& output);
    void queryRange(const Quad& range, std::vector<const Point*>& output);

    void clear();

    std::string toString(const int level = 0) const;

    inline friend std::ostream& operator<<(std::ostream& os, const QuadTree& tree) {
        os << tree.toString();
        return os;
    }

private:
    void subdivide();
};
