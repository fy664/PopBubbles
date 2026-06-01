#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>

/**
 * 碰撞检测工具
 * 纯头文件实现，提供圆形/矩形/线段相交检测
 */

namespace Collision {

// 圆形与圆形碰撞
inline bool circleCircle(sf::Vector2f c1, float r1, sf::Vector2f c2, float r2) {
    float dx = c1.x - c2.x;
    float dy = c1.y - c2.y;
    float distSq = dx * dx + dy * dy;
    float radiusSum = r1 + r2;
    return distSq <= radiusSum * radiusSum;
}

// 点是否在圆内
inline bool pointInCircle(sf::Vector2f point, sf::Vector2f center, float radius) {
    float dx = point.x - center.x;
    float dy = point.y - center.y;
    return (dx * dx + dy * dy) <= (radius * radius);
}

// 线段与圆的碰撞检测
// 返回线段上距离圆心最近的点到圆心的距离是否 <= 半径
inline bool lineCircle(sf::Vector2f A, sf::Vector2f B,
                       sf::Vector2f center, float radius) {
    sf::Vector2f AB = B - A;
    sf::Vector2f AC = center - A;
    float lenSq = AB.x * AB.x + AB.y * AB.y;

    // 线段退化情况
    if (lenSq < 0.0001f) {
        return pointInCircle(A, center, radius);
    }

    float t = (AC.x * AB.x + AC.y * AB.y) / lenSq;
    t = std::max(0.f, std::min(1.f, t));

    sf::Vector2f closest = A + t * AB;
    return pointInCircle(closest, center, radius);
}

// 矩形与圆形碰撞
inline bool rectCircle(const sf::FloatRect& rect, sf::Vector2f center, float radius) {
    // 矩形范围内最近点
    float closestX = std::max(rect.left, std::min(center.x, rect.left + rect.width));
    float closestY = std::max(rect.top, std::min(center.y, rect.top + rect.height));
    return pointInCircle({closestX, closestY}, center, radius);
}

} // namespace Collision
