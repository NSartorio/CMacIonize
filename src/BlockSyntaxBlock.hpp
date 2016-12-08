/*******************************************************************************
 * This file is part of CMacIonize
 * Copyright (C) 2016 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
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
 * @file BlockSyntaxBlock.hpp
 *
 * @brief Block structure used in the BlockSyntaxDensityFunction.
 *
 * @author Bert Vandenbroucke (bv7@st-andrews.ac.uk)
 */
#ifndef BLOCKSYNTAXBLOCK_HPP
#define BLOCKSYNTAXBLOCK_HPP

#include "CoordinateVector.hpp"
#include "Error.hpp"

/**
 * @brief Block structure used in the BlockSyntaxDensityFunction.
 */
class BlockSyntaxBlock {
private:
  /*! @brief Origin of the block. */
  CoordinateVector<> _origin;

  /*! @brief Side lengths of the block. */
  CoordinateVector<> _sides;

  /*! @brief Exponent of the block. */
  double _exponent;

  /*! @brief Density inside the block (in m^-3). */
  double _density;

  /*! @brief Temperature inside the block (in K). */
  double _temperature;

public:
  /**
   * @brief Empty constructor.
   */
  inline BlockSyntaxBlock() : _density(0.), _temperature(0.) {}

  /**
   * @brief Constructor.
   *
   * @param origin Origin of the block.
   * @param sides Side lengths of the block.
   * @param exponent Exponent of the block.
   * @param density Density inside the block.
   * @param temperature Temperature inside the block.
   */
  inline BlockSyntaxBlock(CoordinateVector<> origin, CoordinateVector<> sides,
                          double exponent, double density, double temperature)
      : _origin(origin), _sides(sides), _exponent(exponent), _density(density),
        _temperature(temperature) {}

  /**
   * @brief Check if the given position lies inside this block.
   *
   * @param position CoordinateVector specifying a position.
   * @return True if the position lies inside this block.
   */
  inline bool is_inside(CoordinateVector<> position) {
    double r = 0.;
    for (unsigned int i = 0; i < 3; ++i) {
      double x = 2. * std::abs(position[i] - _origin[i]) / _sides[i];
      if (_exponent < 10.) {
        r += std::pow(x, _exponent);
      } else {
        r = std::max(r, x);
      }
    }
    if (_exponent < 10.) {
      r = std::pow(r, 1. / _exponent);
    }
    return r <= 1.;
  }

  /**
   * @brief Get the number density inside this block.
   *
   * @return Number density inside this block (in m^-3).
   */
  inline double get_density() { return _density; }

  /**
   * @brief Get the temperature inside this block.
   *
   * @return Temperature inside this block (in K).
   */
  inline double get_temperature() { return _temperature; }
};

#endif // BLOCKSYNTAXBLOCK_HPP