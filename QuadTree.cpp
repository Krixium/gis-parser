#include "QuadTree.h"

#include <sstream>

Point& Point::operator=(const Point& other) {
    if (this != &other) {
        this->x = other.x;
        this->y = other.y;
        this->indicies = other.indicies;
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
        this->halfWidth = other.halfWidth;
        this->halfHeight = other.halfHeight;
        this->center = other.center;
        this->lowerLeft = other.lowerLeft;
        this->upperRight = other.upperRight;
    }
    return *this;
}

Quad& Quad::operator=(Quad&& other) {
    if (this != &other) {
        std::swap(this->center, other.center);
        std::swap(this->halfWidth, other.halfWidth);
        std::swap(this->halfHeight, other.halfHeight);
    }
    return *this;
}

bool Quad::intersects(const Quad& other) const {
    // if one is completely to the left of the other
    if (this->lowerLeft.x > other.upperRight.x) return false;
    if (other.lowerLeft.x > this->upperRight.x) return false;

    // if one is above the other
    if (this->lowerLeft.y > other.upperRight.y) return false;
    if (other.lowerLeft.y > this->upperRight.y) return false;
    return true;
}

void Quad::calculateBounds() {
    Point half(halfWidth, halfHeight);
    this->lowerLeft = center - half;
    this->upperRight = center + half;
}

// TODO: FIX THIS
bool Quad::containsInternal(const double x, const double y) const {
    bool greaterThanLower = (x >= this->lowerLeft.x) && (y >= this->lowerLeft.y);
    bool lessThanUpper = (x < this->upperRight.x) && (y < this->upperRight.y);
    return greaterThanLower && lessThanUpper;
}

QuadTree& QuadTree::operator=(const QuadTree& other) {
    if (this != &other) {
        this->bounds = std::make_unique<Quad>(*other.bounds);
        this->nodes = other.nodes;
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

bool QuadTree::insert(const Point& p) {
    if (!this->bounds->contains(p)) {
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

void QuadTree::queryPoint(const double x, const double y, std::vector<const Point*>& output) {
    static const double DEVIATION = 0.0001;
    if (!this->bounds->contains(x, y)) {
        return;
    }

    Point target(x, y);

    for (const Point& p : this->nodes) {
        Point& diff = p - target;
        if (std::abs(diff.x) < DEVIATION && std::abs(diff.y) < DEVIATION) {
            output.push_back(&p);
        }
    }

    if (this->topLeft == nullptr) {
        return;
    }

    this->topLeft->queryPoint(x, y, output);
    this->topRight->queryPoint(x, y, output);
    this->botLeft->queryPoint(x, y, output);
    this->botRight->queryPoint(x, y, output);
}

void QuadTree::queryRange(const Quad& range, std::vector<const Point*>& output) {
    if (!this->bounds->intersects(range)) {
        return;
    }

    for (const Point& p : this->nodes) {
        if (range.contains(p)) {
            output.push_back(&p);
        }
    }

    if (this->topLeft == nullptr) {
        return;
    }

    this->topLeft->queryRange(range, output);
    this->topRight->queryRange(range, output);
    this->botLeft->queryRange(range, output);
    this->botRight->queryRange(range, output);
}

void QuadTree::clear() {
    this->nodes.clear();

    this->topLeft.reset();
    this->topRight.reset();
    this->botLeft.reset();
    this->botRight.reset();
}

std::string QuadTree::toString(const int level) const {
    std::string tab = utils::generateIndent(level);

    std::ostringstream oss;

    oss << tab << "Bounds: " << *this->bounds << std::endl;
    oss << tab << "Points: [";
    if (!this->nodes.empty()) {
        if (this->nodes.size() > 1) {
            for (int i = 0; i < this->nodes.size() - 1; i++) {
                oss << this->nodes[i] << ", ";
            }
        }
        oss << this->nodes.back();
    }
    oss << "]" << std::endl << std::endl;

    if (this->topLeft != nullptr) {
        oss << this->topLeft->toString(level + 1);
    }

    if (this->topLeft != nullptr) {
        oss << this->topRight->toString(level + 1);
    }

    if (this->topLeft != nullptr) {
        oss << this->botLeft->toString(level + 1);
    }

    if (this->topLeft != nullptr) {
        oss << this->botRight->toString(level + 1);
    }

    return oss.str();
}

void QuadTree::subdivide() {
    const Point& center = this->bounds->getCenter();
    double newHalfWidth = this->bounds->getHalfWidth() / 2;
    double newHalfHeight = this->bounds->getHalfHeight() / 2;
    double left = center.x - newHalfWidth;
    double right = center.x + newHalfWidth;
    double up = center.y + newHalfHeight;
    double down = center.y - newHalfHeight;

    this->topLeft = std::make_unique<QuadTree>(left, up, newHalfWidth, newHalfHeight);
    this->topRight = std::make_unique<QuadTree>(right, up, newHalfWidth, newHalfHeight);
    this->botLeft = std::make_unique<QuadTree>(left, down, newHalfWidth, newHalfHeight);
    this->botRight = std::make_unique<QuadTree>(right, down, newHalfWidth, newHalfHeight);

    for (Point& p : this->nodes) {
        if (this->topLeft->insert(p)) continue;
        if (this->topRight->insert(p)) continue;
        if (this->botLeft->insert(p)) continue;
        if (this->botRight->insert(p)) continue;
    }

    this->nodes.clear();
}

