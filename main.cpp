#include <iostream>
#include <math.h>
#include <chrono>
#include <functional>

double Runtime(const std::function<void(void)> &f) {
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<double, std::ratio<1>> duration;
    int repetitions = 10;
    double sum = .0;

    for (int i = 0; i < repetitions; ++i) {
        start = std::chrono::high_resolution_clock::now();
        f();
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        sum += (double) duration.count();
    }

    return sum / repetitions;
}

bool isPrime(int num) {
    bool flag = true;
    for (int i = 2; i <= num / 2; i++) {
        if (num % i == 0) {
            flag = false;
            break;
        }
    }
    return flag;
}

// function to add the elements of two arrays
void f(int n, int *x, bool *y) {
    for (int i = 0; i < n; i++)
        y[i] = isPrime(x[i]);
}

int main(void) {
    int N = 1 << 20; // 1M elements
//    int N = 1 << 20; // 1M elements

    int *x = new int[N];
    bool *y = new bool[N];


    // initialize x and y arrays on the host
    for (int i = 0; i < N; i++) {
        x[i] = i % 1000;

    }

    // Run kernel on 1M elements on the CPU
    std::function<void(void)> benchmark = [&x, &y, &N]() {
        f(N, x, y);
    };

    double runtime = Runtime(benchmark);

    std::cout << "Runtime: " << runtime * 1000 << std::endl;
    std::cout << "Array x: " << *x << std::endl;
    std::cout << "Array y: " << *y << std::endl;

    std::cout << "Array x: ";
    for (int j = 0; j < 10; ++j) {
        std::cout << x[j] << ",";
    }

    std::cout << "\n";
    std::cout << "Array y: ";
    for (int j = 0; j < 10; ++j) {
        std::cout << y[j] << ",";
    }
    std::cout << "\n";
    // Check for errors (all values should be 3.0f)
//        float maxError = 0.0f;
//        for (int i = 0; i < N; i++)
//            maxError = fmax(maxError, fabs(y[i] - 12.0f));
//        std::cout << "Max error: " << maxError << std::endl;

    // Free memory
    delete[] x;
    delete[] y;

    return 0;
}