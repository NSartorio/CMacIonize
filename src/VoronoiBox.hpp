/*******************************************************************************
 * This file is part of CMacIonize
 * Copyright (C) 2017 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
 *
 * CMacIonize is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CMacIonize is distributed in the hope that it will be useful,
 * but WITOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with CMacIonize. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/**
 * @file VoronoiBox.hpp
 *
 * @brief Special type of Box used to store a NewVoronoiGrid.
 *
 * @author Bert Vandenbroucke (bv7@st-andrews.ac.uk)
 */
#ifndef VORONOIBOX_HPP
#define VORONOIBOX_HPP

#include "Box.hpp"
#include "VoronoiVariables.hpp"

/**
 * @brief Special type of Box used to store a NewVoronoiGrid.
 */
template < typename _datatype_ > class VoronoiBox {
private:
  /*! @brief Underlying Box. */
  Box< _datatype_ > _box;

  /*! @brief Positions of the large, all-encompassing initial tetrahedron. */
  CoordinateVector< _datatype_ > _tetrahedron[4];

  /**
   * @brief Get a reflective copy of the given generator by mirroring it with
   * the wall with the given index.
   *
   * @param index Wall index.
   * @param generator_position Position of the generator.
   * @return Reflective copy of the generator.
   */
  inline CoordinateVector< _datatype_ > get_wall_copy(
      unsigned int index,
      const CoordinateVector< _datatype_ > &generator_position) const {
    switch (index) {
    case NEWVORONOICELL_BOX_LEFT:
      return CoordinateVector< _datatype_ >(
          2 * _box.get_anchor().x() - generator_position.x(),
          generator_position.y(), generator_position.z());
    case NEWVORONOICELL_BOX_RIGHT:
      return CoordinateVector< _datatype_ >(
          2 * (_box.get_anchor().x() + _box.get_sides().x()) -
              generator_position.x(),
          generator_position.y(), generator_position.z());
    case NEWVORONOICELL_BOX_FRONT:
      return CoordinateVector< _datatype_ >(generator_position.x(),
                                            2 * _box.get_anchor().y() -
                                                generator_position.y(),
                                            generator_position.z());
    case NEWVORONOICELL_BOX_BACK:
      return CoordinateVector< _datatype_ >(
          generator_position.x(),
          2 * (_box.get_anchor().y() + _box.get_sides().y()) -
              generator_position.y(),
          generator_position.z());
    case NEWVORONOICELL_BOX_BOTTOM:
      return CoordinateVector< _datatype_ >(
          generator_position.x(), generator_position.y(),
          2 * _box.get_anchor().z() - generator_position.z());
    case NEWVORONOICELL_BOX_TOP:
      return CoordinateVector< _datatype_ >(
          generator_position.x(), generator_position.y(),
          2 * (_box.get_anchor().z() + _box.get_sides().z()) -
              generator_position.z());
    }
    cmac_error("Unknown box wall index: %u!", index);
    return CoordinateVector< _datatype_ >();
  }

public:
  /**
   * @brief Empty constructor.
   */
  inline VoronoiBox() {}

  /**
   * @brief Constructor.
   *
   * @param box Box.
   */
  inline VoronoiBox(const Box< _datatype_ > box) : _box(box) {

    // the large all-encompassing tetrahedron has one vertex in the anchor of
    // the (extended) box (with side length 3*'max_side')
    // the other vertices are at a distance of sqrt{6}*'max_side'
    // however, since we need to account for integer coordinates, we round this
    // up to 3
    _datatype_ max_side = std::max(box.get_sides().x(), box.get_sides().y());
    max_side = std::max(max_side, box.get_sides().z());

    _tetrahedron[0][0] = box.get_anchor().x() - box.get_sides().x();
    _tetrahedron[0][1] = box.get_anchor().y() - box.get_sides().y();
    _tetrahedron[0][2] = box.get_anchor().z() - box.get_sides().z();
    _tetrahedron[1][0] =
        box.get_anchor().x() - box.get_sides().x() + 9 * max_side;
    _tetrahedron[1][1] = box.get_anchor().y() - box.get_sides().y();
    _tetrahedron[1][2] = box.get_anchor().z() - box.get_sides().z();
    _tetrahedron[2][0] = box.get_anchor().x() - box.get_sides().x();
    _tetrahedron[2][1] =
        box.get_anchor().y() - box.get_sides().y() + 9 * max_side;
    _tetrahedron[2][2] = box.get_anchor().z() - box.get_sides().z();
    _tetrahedron[3][0] = box.get_anchor().x() - box.get_sides().x();
    _tetrahedron[3][1] = box.get_anchor().y() - box.get_sides().y();
    _tetrahedron[3][2] =
        box.get_anchor().z() - box.get_sides().z() + 9 * max_side;
  }

  /**
   * @brief Get the given component of the box.
   *
   * @param index Index of a component.
   * @param generator_position Position of the generator of the cell.
   * @return Value for that component.
   */
  inline CoordinateVector< _datatype_ >
  get_position(unsigned int index,
               const CoordinateVector< _datatype_ > &generator_position) const {
    if (index >= NEWVORONOICELL_BOX_LEFT) {
      return get_wall_copy(index, generator_position);
    } else {
      cmac_assert(index >= NEWVORONOICELL_BOX_CORNER0);
      return _tetrahedron[index - NEWVORONOICELL_BOX_CORNER0];
    }
  }

  /**
   * @brief Get the dimensions of the box that contains the entire VoronoiBox,
   * including the large all-encompassing tetrahedron.
   *
   * @return All-encompassing Box.
   */
  inline Box< _datatype_ > get_box() const {
    const CoordinateVector< _datatype_ > sides = CoordinateVector< _datatype_ >(
        _tetrahedron[1].x() - _tetrahedron[0].x(),
        _tetrahedron[2].y() - _tetrahedron[0].y(),
        _tetrahedron[3].z() - _tetrahedron[0].z());
    return Box< _datatype_ >(_tetrahedron[0], sides);
  }
};

#endif // VORONOIBOX_HPP
