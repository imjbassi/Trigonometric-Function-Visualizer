// Trigonometric Function Visualizer
// Renders sine, cosine, tangent, cosecant, secant, and cotangent as ASCII
// art plots over [-PI, PI) directly in the terminal.

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace {

constexpr int kWidth = 80;
constexpr int kHeight = 30;
constexpr float kYLimit = 5.0f; // clamp reciprocal functions so asymptotes don't dominate the plot
constexpr float kStep = 0.08f;
constexpr float kPi = 3.14159265358979323846f;

struct FunctionInfo {
  char key;
  const char *name;
  const char *ratio;
};

const FunctionInfo kFunctions[] = {
    {'s', "sine", "opposite/hypotenuse"},
    {'c', "cosine", "adjacent/hypotenuse"},
    {'t', "tangent", "opposite/adjacent"},
    {'C', "cosecant", "hypotenuse/opposite"},
    {'S', "secant", "hypotenuse/adjacent"},
    {'T', "cotangent", "adjacent/opposite"},
};

// Returns false near an asymptote (division by a value too close to zero)
// so the caller can skip plotting that point instead of drawing garbage.
bool EvaluateFunction(float x, char function, float &outValue) {
  constexpr float kEpsilon = 1e-3f;
  switch (function) {
  case 's':
    outValue = std::sin(x);
    return true;
  case 'c':
    outValue = std::cos(x);
    return true;
  case 't':
    if (std::fabs(std::cos(x)) < kEpsilon)
      return false;
    outValue = std::tan(x);
    return true;
  case 'C':
    if (std::fabs(std::sin(x)) < kEpsilon)
      return false;
    outValue = 1.0f / std::sin(x);
    return true;
  case 'S':
    if (std::fabs(std::cos(x)) < kEpsilon)
      return false;
    outValue = 1.0f / std::cos(x);
    return true;
  case 'T':
    if (std::fabs(std::sin(x)) < kEpsilon)
      return false;
    outValue = 1.0f / std::tan(x);
    return true;
  default:
    return false;
  }
}

void PrintMenu() {
  std::cout << "\nTrigonometric Function Visualizer\n";
  std::cout << "----------------------------------\n";
  for (const auto &f : kFunctions) {
    std::cout << "  " << f.key << " = " << f.name << " (" << f.ratio << ")\n";
  }
  std::cout << "  q = quit\n\n";
  std::cout << "Select a function: ";
}

// Builds the ASCII canvas: axes, tick labels, and the plotted curve.
void RenderPlot(char function) {
  std::vector<std::string> plot(kHeight, std::string(kWidth, ' '));

  // Axes
  for (int row = 0; row < kHeight; ++row)
    plot[row][kWidth / 2] = '|';
  for (int col = 0; col < kWidth; ++col)
    plot[kHeight / 2][col] = '-';
  plot[kHeight / 2][kWidth / 2] = '+';

  // X-axis ticks (radians, centered at 0)
  for (int col = 0; col < kWidth; col += kWidth / 10) {
    plot[kHeight / 2][col] = '+';
    if (col == kWidth / 2)
      continue;
    // Ticks are labeled in tenths of a radian relative to x = 0 (plot center).
    int label = static_cast<int>(std::round((col - kWidth / 2) * (2 * kPi / kWidth) * 10));
    std::string text = std::to_string(label);
    int start = col - static_cast<int>(text.size()) / 2;
    for (size_t k = 0; k < text.size(); ++k) {
      int c = start + static_cast<int>(k);
      if (c >= 0 && c < kWidth)
        plot[kHeight / 2 + 1][c] = text[k];
    }
  }

  // Y-axis ticks
  for (int row = 0; row < kHeight; row += kHeight / 10) {
    plot[row][kWidth / 2] = '+';
    if (row == kHeight / 2)
      continue;
    int label = static_cast<int>(std::round(
        (kHeight / 2 - row) * (2 * kYLimit / kHeight)));
    std::string text = std::to_string(label);
    int start = kWidth / 2 + 2;
    for (size_t k = 0; k < text.size() && start + static_cast<int>(k) < kWidth; ++k) {
      plot[row][start + k] = text[k];
    }
  }

  // Curve, mapped from math space (x in [-PI, PI), y in [-kYLimit, kYLimit])
  // to screen space.
  for (float x = -kPi; x < kPi; x += kStep) {
    float y;
    if (!EvaluateFunction(x, function, y))
      continue;
    if (y > kYLimit || y < -kYLimit)
      continue;

    int col = static_cast<int>(kWidth / 2 + (kWidth / (2 * kPi)) * x);
    int row = static_cast<int>(kHeight / 2 - (kHeight / (2 * kYLimit)) * y);

    if (row >= 0 && row < kHeight && col >= 0 && col < kWidth)
      plot[row][col] = '*';
  }

  for (const auto &line : plot)
    std::cout << line << '\n';
}

const FunctionInfo *FindFunction(char key) {
  for (const auto &f : kFunctions)
    if (f.key == key)
      return &f;
  return nullptr;
}

} // namespace

int main() {
  char choice;
  while (true) {
    PrintMenu();
    if (!(std::cin >> choice))
      break;

    if (choice == 'q' || choice == 'Q')
      break;

    const FunctionInfo *info = FindFunction(choice);
    if (!info) {
      std::cout << "Unrecognized option '" << choice << "'. Try again.\n";
      continue;
    }

    std::cout << "\n"
              << info->name << " (x-axis in tenths of radians, "
              << "y-axis clamped to [-" << kYLimit << ", " << kYLimit << "])\n\n";
    RenderPlot(choice);
  }

  std::cout << "Goodbye!\n";
  return 0;
}
