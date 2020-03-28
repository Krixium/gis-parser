#pragma once

#include <memory>
#include <sstream>
#include <vector>

#include "utils.h"

class Point {
public:
    float x;
    float y;

    std::vector<std::size_t> indicies;

public:
    Point(const float x = 0, const float y = 0) : x(x), y(y), indicies() {}
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
        float x1 = lhs.x;
        float y1 = lhs.y;

        float x2 = rhs.x;
        float y2 = rhs.y;

        return x1 - x2 == 0 && x1 - x2 == 0;
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
    float halfWidth = 0;
    float halfHeight = 0;

    Point center;
    Point lowerLeft;
    Point topRight;

public:
    Quad(const float x = 0, const float y = 0, const float halfWidth = 0, const float halfHeight = 0)
        : center(Point(x, y)), halfWidth(halfWidth), halfHeight(halfHeight) {
        this->calculateBounds();
    }

    Quad(const Quad& other) { *this = other; }
    Quad(Quad&& other) { *this = std::move(other); }
    ~Quad() = default;

    Quad& operator=(const Quad& other);
    Quad& operator=(Quad&& other);

    inline float left() const { return this->lowerLeft.x; }
    inline float right() const { return this->topRight.x; }
    inline float top() const { return this->topRight.y; }
    inline float bottom() const { return this->lowerLeft.y; }

    bool contains(const Point& p) const { return this->containsInternal(p.x, p.y); }
    bool contains(const float x, const float y) const { return this->containsInternal(x, y); }

    bool intersects(const Quad& other) const;

    inline const Point& getCenter() const { return this->center; }
    inline float getHalfWidth() const { return this->halfWidth; }
    inline float getHalfHeight() const { return this->halfHeight; }

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
        return lhs.center == rhs.center && lhs.lowerLeft == rhs.lowerLeft && lhs.topRight == rhs.topRight;
    }

    inline friend bool operator!=(const Quad& lhs, const Quad& rhs) { return !(lhs == rhs); }

private:
    void calculateBounds();
    bool containsInternal(const float x, const float y) const;
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
    QuadTree(const float x = 0, const float y = 0, const float halfWidth = 0, const float halfHeight = 0) {
        this->setBound(x, y, halfWidth, halfHeight);
        this->clear();
    }

    QuadTree(const QuadTree& other) { *this = other; }
    QuadTree(QuadTree& other) { *this = std::move(other); }

    QuadTree& operator=(const QuadTree& other);
    QuadTree& operator=(QuadTree&& other);

    bool isUsable() const { return this->isBoundSet; }

    void setBound(const float x, const float y, const float halfWidth, const float halfHeight) {
        this->bounds = std::make_unique<Quad>(x, y, halfWidth, halfHeight);
        this->isBoundSet = true;
    }

    bool insert(const Point& p);

    void queryPoint(const float x, const float y, std::vector<const Point*>& output);
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
