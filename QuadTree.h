#pragma once

#include <memory>
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

    Point& operator=(const Point& other) {
        if (this != &other) {
            this->x = other.x;
            this->y = other.y;
            std::copy(other.indicies.begin(), other.indicies.end(), this->indicies.begin());
        }
        return *this;
    }

    Point& operator=(Point&& other) {
        if (this != &other) {
            std::swap(this->x, other.x);
            std::swap(this->y, other.y);
            this->indicies.swap(other.indicies);
        }
        return *this;
    }

    friend Point operator+(const Point& p1, const Point& p2) {
        return Point(p1.getX() + p2.getX(), p1.getY() + p2.getY());
    }

    friend Point operator-(const Point& p1, const Point& p2) {
        return Point(p1.getX() - p2.getX(), p1.getY() - p2.getY());
    }
};

class Quad {
private:
    double halfSize = 0;

    Point center;
    Point lowerLeft;
    Point upperRight;

    void calculateBounds() {
        Point half(halfSize, halfSize);
        this->lowerLeft = center - half;
        this->upperRight = center + half;
    }

    bool containsInternal(const double x, const double y) {
        bool greaterThanLower = (x >= this->lowerLeft.getX()) && (y >= this->lowerLeft.getY());
        bool lessThanUpper = (x < this->upperRight.getX()) && (y < this->upperRight.getY());
        return greaterThanLower && lessThanUpper;
    }

public:
    Quad(const double x = 0, const double y = 0, const double half = 0)
        : center(Point(x, y)), halfSize(half) {
        this->calculateBounds();
    }

    Quad(const Quad& other) { *this = other; }
    Quad(Quad&& other) { *this = std::move(other); }
    ~Quad() = default;

    Quad& operator=(const Quad& other) {
        this->halfSize = other.halfSize;
        this->center = other.center;
        this->lowerLeft = other.lowerLeft;
        this->upperRight = other.upperRight;
    }

    Quad& operator=(Quad&& other) {
        if (this != &other) {
            std::swap(this->center, other.center);
            std::swap(this->halfSize, other.halfSize);
        }
        return *this;
    }

    bool contains(const Point& p) { this->containsInternal(p.getX(), p.getY()); }
    bool contains(const double x, const double y) { this->containsInternal(x, y); }

    bool intersects(const Quad& other) {
        if (this->lowerLeft.getX() > other.upperRight.getX()) {
            return false;
        }

        if (other.lowerLeft.getX() > this->upperRight.getX()) {
            return false;
        }

        if (this->lowerLeft.getY() < other.upperRight.getY()) {
            return false;
        }

        if (other.lowerLeft.getY() < this->upperRight.getY()) {
            return false;
        }

        return true;
    }
};

class QuadTree {
private:
    static const int MAX_CAPACITY;

    Quad bounds;

    std::vector<Point> nodes;

    std::unique_ptr<QuadTree> topLeft;
    std::unique_ptr<QuadTree> topRight;
    std::unique_ptr<QuadTree> botLeft;
    std::unique_ptr<QuadTree> botRight;

    bool insertInternal(const Point& p) {
        if (!this->bounds.contains(p)) {
            return false;
        }

        if (this->nodes.size() < MAX_CAPACITY && this->topLeft == nullptr) {
            this->nodes.push_back(p);
            return true;
        }

        if (this->topLeft == nullptr) {
            this->subdivide();
        }

        if (this->topLeft->insert(p)) return true;
        if (this->topRight->insert(p)) return true;
        if (this->botLeft->insert(p)) return true;
        if (this->botRight->insert(p)) return true;
        return false;
    }

    void subdivide() {
        this->topLeft = std::make_unique<QuadTree>();
        this->topRight = std::make_unique<QuadTree>();
        this->botLeft = std::make_unique<QuadTree>();
        this->botRight = std::make_unique<QuadTree>();

        for (Point& p : this->nodes) {
            if (this->topLeft->insert(p)) continue;
            if (this->topRight->insert(p)) continue;
            if (this->botLeft->insert(p)) continue;
            if (this->botRight->insert(p)) continue;
        }

        this->nodes.clear();
    }

public:
    QuadTree(const double x = 0, const double y = 0, const double halfWidth = 0) : bounds(x, y, halfWidth), nodes() {
        this->topLeft.reset(nullptr);
        this->topRight.reset(nullptr);
        this->botLeft.reset(nullptr);
        this->botRight.reset(nullptr);
    }

    // No copies because I don't want to do recursive deep copy
    QuadTree(const QuadTree& other) = delete;
    QuadTree& operator=(const QuadTree& other) = delete;

    QuadTree(QuadTree& other) {
        *this = std::move(other);
    }

    QuadTree& operator=(QuadTree&& other) {
        if (this != &other) {
            std::swap(this->bounds, other.bounds);
            this->nodes.swap(other.nodes);
            std::swap(this->topLeft, other.topLeft);
            std::swap(this->topRight, other.topRight);
            std::swap(this->botLeft, other.botLeft);
            std::swap(this->botRight, other.botRight);
        }
        return *this;
    }

    bool insert(const double x, const double y) { return this->insertInternal(Point(x, y)); }
    bool insert(const Point& p) { return this->insertInternal(p); }

    std::vector<const Point&> queryRange(const Quad& range) {
        std::vector<const Point&> output;

        if (!this->bounds.intersects(range)) {
            return output;
        }

        for (const Point& p : this->nodes) {
            if (range.contains(p)) {
                output.push_back(p);
            }
        }

        if (this->topLeft == nullptr) {
            return output;
        }

        std::vector<const Point&> l1 = this->topLeft->queryRange(range);
        std::vector<const Point&> l2 = this->topRight->queryRange(range);
        std::vector<const Point&> l3 = this->botLeft->queryRange(range);
        std::vector<const Point&> l4 = this->botRight->queryRange(range);
        
        output.insert(output.end(), l1.begin(), l1.end());
        output.insert(output.end(), l2.begin(), l2.end());
        output.insert(output.end(), l3.begin(), l3.end());
        output.insert(output.end(), l4.begin(), l4.end());

        return output;
    }
};

const int QuadTree::MAX_CAPACITY = 4;
