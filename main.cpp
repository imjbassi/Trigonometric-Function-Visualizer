#include <iostream>
#include <iomanip>
#include <cmath>
#include <sstream>

const int width = 80;
const int height = 30;

float calculateFunctionValue(float x, char function) {
    switch (function) {
        case 's': return sin(x);
        case 'c': return cos(x);
        case 't': return tan(x);
        case 'C': return 1 / sin(x); // Cosecant
        case 'S': return 1 / cos(x); // Secant
        case 'T': return 1 / tan(x); // Cotangent
        default: return 0;
    }
}

int main() {
    char function;
    do {
        std::cout << "Enter the trigonometric function (s for sin, c for cos, t for tan, C for cosec, S for sec, T for cot; q to quit): ";
        std::cin >> function;

        if (function == 'q' || function == 'Q')
            break;

        char plot[height][width];

        // Initialize the plot array with spaces
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                plot[i][j] = ' ';
            }
        }

        // Calculate the function values and plot them
        for (float x = 0; x < 2 * M_PI; x += 0.1) {
            float y = calculateFunctionValue(x, function);
            int col = width / (2 * M_PI) * x + width / 2;
            int row = height / 2 - height / 2 * y;

            if (row >= 0 && row < height && col >= 0 && col < width) {
                plot[row][col] = '*';
            }
        }

        // Plot X and Y axes
        for (int i = 0; i < height; ++i) {
            plot[i][width / 2] = '|';
        }
        for (int j = 0; j < width; ++j) {
            plot[height / 2][j] = '-';
        }
        plot[height / 2][width / 2] = '+';

        // Add labels and tick marks to X and Y axes
        for (int i = 0; i < width; i += width / 10) {
            plot[height / 2][i] = '+';
            if (i != width / 2) {
                int xLabel = i - width / 2;
                plot[height / 2 + 1][i] = '|';
                plot[height / 2 + 2][i] = '|';
                plot[height / 2 + 2][i - 1] = '-';
                plot[height / 2 + 2][i + 1] = '-';
                std::stringstream ss;
                ss << xLabel;
                std::string label = ss.str();
                int labelPos = i - label.length() / 2;
                for (size_t k = 0; k < label.length(); ++k) {
                    plot[height / 2 + 3][labelPos + k] = label[k];
                }
            }
        }
        for (int i = 0; i < height; i += height / 10) {
            plot[i][width / 2] = '+';
            if (i != height / 2) {
                int yLabel = height / 2 - i;
                plot[i][width / 2 + 1] = '-';
                plot[i][width / 2 + 2] = '-';
                plot[i - 1][width / 2 + 2] = '|';
                plot[i + 1][width / 2 + 2] = '|';
                std::stringstream ss;
                ss << yLabel;
                std::string label = ss.str();
                int labelPos = width / 2 + 3;
                for (size_t k = 0; k < label.length(); ++k) {
                    plot[i][labelPos + k] = label[k];
                }
            }
        }

        // Display the plot
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                std::cout << plot[i][j];
            }
            std::cout << '\n';
        }
    } while (true);

    return 0;
}
