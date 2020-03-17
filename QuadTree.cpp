#include "QuadTree.h"

Point& Point::operator=(const Point& other) {
    if (this != &other) {
        this->x = other.x;
        this->y = other.y;
        std::copy(other.indicies.begin(), other.indicies.end(), this->indicies.begin());
    }
    return *this;
}

Point& Point::operator=(Point&& other) {
    if (this != &other) {
        std::swap(this->x, other.x);
        std::swap(this->y, other.y);
        this->indicies.swap(other.indicies);
    }
    return *this;
}

Quad& Quad::operator=(const Quad& other) {
    if (this != &other) {
        this->halfSize = other.halfSize;
        this->center = other.center;
        this->lowerLeft = other.lowerLeft;
        this->upperRight = other.upperRight;
    }
    return *this;
}

Quad& Quad::operator=(Quad&& other) {
    if (this != &other) {
        std::swap(this->center, other.center);
        std::swap(this->halfSize, other.halfSize);
    }
    return *this;
}

bool Quad::intersects(const Quad& other) const {
    if (this->lowerLeft.x > other.upperRight.x) return false;
    if (other.lowerLeft.x > this->upperRight.x) return false;
    if (this->lowerLeft.y < other.upperRight.y) return false;
    if (other.lowerLeft.y < this->upperRight.y) return false;
    return true;
}

void Quad::calculateBounds() {
    Point half(halfSize, halfSize);
    this->lowerLeft = center - half;
    this->upperRight = center + half;
}

bool Quad::containsInternal(const double x, const double y) const {
    bool greaterThanLower = (x >= this->lowerLeft.x) && (y >= this->lowerLeft.y);
    bool lessThanUpper = (x < this->upperRight.x) && (y < this->upperRight.y);
    return greaterThanLower && lessThanUpper;
}

QuadTree& QuadTree::operator=(const QuadTree& other) {
    if (this != &other) {
        this->bounds = other.bounds;
        std::copy(other.nodes.begin(), other.nodes.end(), this->nodes.begin());
        if (other.topLeft != nullptr) this->topLeft = std::make_unique<QuadTree>(*other.topLeft);
        if (other.topRight != nullptr) this->topRight = std::make_unique<QuadTree>(*other.topRight);
        if (other.botLeft != nullptr) this->botLeft = std::make_unique<QuadTree>(*other.botLeft);
        if (other.botRight != nullptr) this->botRight = std::make_unique<QuadTree>(*other.botRight);
    }
    return *this;
}

QuadTree& QuadTree::operator=(QuadTree&& other) {
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

std::vector<Point> QuadTree::queryRange(const Quad& range) {
    std::vector<Point> output;

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

    std::vector<Point> l1 = this->topLeft->queryRange(range);
    std::vector<Point> l2 = this->topRight->queryRange(range);
    std::vector<Point> l3 = this->botLeft->queryRange(range);
    std::vector<Point> l4 = this->botRight->queryRange(range);

    output.insert(output.end(), l1.begin(), l1.end());
    output.insert(output.end(), l2.begin(), l2.end());
    output.insert(output.end(), l3.begin(), l3.end());
    output.insert(output.end(), l4.begin(), l4.end());

    return output;
}

bool QuadTree::insertInternal(const Point& p) {
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

void QuadTree::subdivide() {
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

