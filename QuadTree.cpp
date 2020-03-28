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
        this->topRight = other.topRight;
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
    if (this->left() > other.right()) return false;
    if (other.left() > this->right()) return false;
    if (this->bottom() > other.top()) return false;
    if (other.bottom() > this->top()) return false;
    return true;
}

void Quad::calculateBounds() {
    Point half(halfWidth, halfHeight);
    this->lowerLeft = center - half;
    this->topRight = center + half;
}

bool Quad::containsInternal(const float x, const float y) const {
    if (x <= this->left() || x > this->right()) {
        return false;
    }

    if (y <= this->bottom() || y > this->top()) {
        return false;
    }

    return true;
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

void QuadTree::queryPoint(const float x, const float y, std::vector<const Point*>& output) {
    static const float DEVIATION = 0.0001;
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
    float newHalfWidth = this->bounds->getHalfWidth() / 2;
    float newHalfHeight = this->bounds->getHalfHeight() / 2;
    float left = center.x - newHalfWidth;
    float right = center.x + newHalfWidth;
    float up = center.y + newHalfHeight;
    float down = center.y - newHalfHeight;

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

