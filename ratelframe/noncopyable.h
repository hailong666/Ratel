#pragma once

//被继承以后派生类对象可以正常地进行构造和析构，但是派生类对象无法进行构造和赋值
class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};