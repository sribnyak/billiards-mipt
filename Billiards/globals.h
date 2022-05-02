#ifndef BILLIARDS_GLOBALS_H_
#define BILLIARDS_GLOBALS_H_

#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>

using real = float;

namespace sizes {

constexpr real ballDiameter = 0.068;
constexpr real centralPocketWidth = 0.083;
constexpr real cornerPocketWidth = 0.073;
constexpr real tableHeight = 1.270;
constexpr real tableWidth = 2 * tableHeight;
constexpr real borderWidth = 0.052;
constexpr real cueLength = 1.600;

} // namespace sizes

#endif // BILLIARDS_GLOBALS_H_
