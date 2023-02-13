#pragma once
#include <list>
#include <utility>
#include <vector>

typedef std::pair<double, double> Point2D;

template<class ID, class T>
class QuadTree {
private:
    // punkter som definerer hjørner til objectene
    Point2D m_a;
    Point2D m_b;
    Point2D m_c;
    Point2D m_d;

    // pointers to subtrees, this could be an array to
    // nw | ne
    // -------
    // sw | se
    QuadTree* _se;
    QuadTree* _sw;
    QuadTree* _nw;
    QuadTree* _ne;

    std::list<std::pair<ID, T>> _objects;

    bool IsLeaf() const;

    // help variable to save all Ts
    // for later iteration
    std::vector<T> _allObjects;
public:
    QuadTree();
    QuadTree(const Point2D &v1,const Point2D &v2,const Point2D &v3,const Point2D &v4);
    void Init(const Point2D &v1,const Point2D &v2,const Point2D &v3,const Point2D &v4);
    void Subdivide(int n);
    void Print() const;
    void PrintAll() const;
    QuadTree* Insert(const Point2D& point, const ID& id, const T& object);
    QuadTree* Find(const Point2D& p);
    const QuadTree* find(const Point2D& p) const;
    T& find(const ID& id);
    T& operator[] (ID id);

    // teachers note: not real iteration functions for Quadtree, but for a contrainer of T
    typename std::vector<T>::iterator begin() {
        traverse_all(_allObjects);
        return _allObjects.begin();
    }

    typename std::vector<T>::iterator end() {
        return _allObjects.end();
    }
};
