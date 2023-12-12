/*
 * File: Shape.h
 * Author: kissholic
 * Copyright (c) 2023 kissholic. All wrongs reserved.
 */

#pragma once

#include "Math.h"


template<int Dim>
class Segment
{
public:
    using PointType = Pointf<Dim>;

    Segment()
    {
        for (int i = 0; i < )
    }

    Segment(PointType P1, PointType P2);

    Segment(Segment const& Seg);

    PointType& operator[](int index) noexcept
    {
        return data[index];
    }

    PointType const& operator[](int index) const noexcept
    {
        return data[index];
    }

private:
    PointType data[Dim];
};








