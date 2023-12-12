/*
 * File: Math.h
 * Author: kissholic
 * Copyright (c) 2023 kissholic. All wrongs reserved.
 */

#pragma once


template<typename T, int Dim>
struct Point
{
    T& operator[](int index) noexcept
    {
        return data[index];
    }

    T const& operator[](int index) const noexcept
    {
        return data[index];
    }

    static int Dimension() noexcept
    {
        return Dim;
    }

    T data[Dim];
};


template<int Dim>
using Pointi = Point<int, Dim>;

template<int Dim>
using Pointf = Point<float, Dim>;


using Point2i = Pointi<2>;
using Point3i = Pointi<3>;
using Point2f = Pointf<2>;
using Point3f = Pointf<3>;


