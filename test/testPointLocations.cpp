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
 * @file testPointLocations.cpp
 *
 * @brief Unit test for the PointLocations class.
 *
 * @author Bert Vandenbroucke (bv7@st-andrews.ac.uk)
 */
#include "Assert.hpp"
#include "PointLocations.hpp"
#include "Timer.hpp"
#include "Utilities.hpp"
#include <fstream>

/**
 * @brief Unit test for the PointLocations class.
 *
 * @param argc Number of command line arguments.
 * @param argv Command line arguments.
 * @return Exit code: 0 on success.
 */
int main(int argc, char **argv) {

  /// test PointLocations::ngbiterator::increase_indices()
  {
    // create a reference to the static function, this makes the lines below
    // shorter
    void (&func)(int &, int &, int &, int &) =
        PointLocations::ngbiterator::increase_indices;
    // start from the middle box
    int rx = 0;
    int ry = 0;
    int rz = 0;
    int level = 0;
    // now do a full cycle of the next shell and check every step of the cycle
    func(rx, ry, rz, level);
    assert_condition(rx == -1 && ry == -1 && rz == -1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == -1 && ry == -1 && rz == 0 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == -1 && ry == -1 && rz == 1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == -1 && ry == 0 && rz == -1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == -1 && ry == 0 && rz == 0 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == -1 && ry == 0 && rz == 1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == -1 && ry == 1 && rz == -1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == -1 && ry == 1 && rz == 0 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == -1 && ry == 1 && rz == 1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 0 && ry == -1 && rz == -1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 0 && ry == -1 && rz == 0 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 0 && ry == -1 && rz == 1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 0 && ry == 0 && rz == -1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 0 && ry == 0 && rz == 1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 0 && ry == 1 && rz == -1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 0 && ry == 1 && rz == 0 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 0 && ry == 1 && rz == 1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 1 && ry == -1 && rz == -1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 1 && ry == -1 && rz == 0 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 1 && ry == -1 && rz == 1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 1 && ry == 0 && rz == -1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 1 && ry == 0 && rz == 0 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 1 && ry == 0 && rz == 1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 1 && ry == 1 && rz == -1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 1 && ry == 1 && rz == 0 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == 1 && ry == 1 && rz == 1 && level == 1);
    func(rx, ry, rz, level);
    assert_condition(rx == -2 && ry == -2 && rz == -2 && level == 2);
  }

  const unsigned int numpoint = 10000;
  const unsigned int center = 2;
  const double radius = 0.1;
  const double radius2 = radius * radius;
  std::vector< CoordinateVector<> > positions(numpoint);
  for (unsigned int i = 0; i < numpoint; ++i) {
    positions[i] = Utilities::random_position();
  }

  PointLocations locations(positions, 10);

  const CoordinateVector<> &cpos = positions[center];

  Timer smart_timer;
  smart_timer.start();
  unsigned int smart_ngb_count = 0;
  auto it = locations.get_neighbours(center);
  auto ngbs = it.get_neighbours();
  for (auto ngbit = ngbs.begin(); ngbit != ngbs.end(); ++ngbit) {
    if (*ngbit != center) {
      const CoordinateVector<> &ngbpos = positions[*ngbit];
      if ((ngbpos - cpos).norm2() < radius2) {
        ++smart_ngb_count;
      }
    }
  }
  while (it.get_max_radius2() < radius2 && it.increase_range()) {
    ngbs = it.get_neighbours();
    for (auto ngbit = ngbs.begin(); ngbit != ngbs.end(); ++ngbit) {
      if (*ngbit != center) {
        const CoordinateVector<> &ngbpos = positions[*ngbit];
        if ((ngbpos - cpos).norm2() < radius2) {
          ++smart_ngb_count;
        }
      }
    }
  }
  smart_timer.stop();
  cmac_status("Grid search time: %g s", smart_timer.value());

  Timer bf_timer;
  bf_timer.start();
  unsigned int bf_ngb_count = 0;
  for (unsigned int i = 0; i < numpoint; ++i) {
    if (i != center) {
      const CoordinateVector<> &ngbpos = positions[i];
      if ((ngbpos - cpos).norm2() < radius2) {
        ++bf_ngb_count;
      }
    }
  }
  bf_timer.stop();
  cmac_status("Brute force search time: %g s", bf_timer.value());

  assert_condition(smart_ngb_count == bf_ngb_count);
  assert_condition(smart_timer.value() < bf_timer.value());

  return 0;
}
