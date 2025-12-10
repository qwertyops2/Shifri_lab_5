#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <windows.h>

long long customGCD(long long a, long long b) {
    if (a < 0) a = -a;
    if (b < 0) b = -b;

    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

unsigned long long eulerTotient(unsigned long long n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;

    unsigned long long result = n;

    if (n % 2 == 0) {
        while (n % 2 == 0) {
            n /= 2;
        }
        result -= result / 2;
    }

    for (unsigned long long i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            while (n % i == 0) {
                n /= i;
            }
            result -= result / i;
        }
    }

    if (n > 1) {
        result -= result / n;
    }

    return result;
}

long long extendedEuclidean(long long a, long long b, long long& x, long long& y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    long long x1, y1;
    long long gcd = extendedEuclidean(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

    return gcd;
}

bool areCoprime(const std::vector<long long>& mods) {
    for (size_t i = 0; i < mods.size(); ++i) {
        for (size_t j = i + 1; j < mods.size(); ++j) {
            if (customGCD(mods[i], mods[j]) != 1) {
                return false;
            }
        }
    }
    return true;
}

long long modMultiply(long long a, long long b, long long mod) {
    a %= mod;
    b %= mod;

    if (a == 0 || b == 0) return 0;

    long long result = 0;
    a %= mod;

    while (b > 0) {
        if (b % 2 == 1) {
            result = (result + a) % mod;
        }
        a = (a * 2) % mod;
        b /= 2;
    }

    return result % mod;
}

std::pair<long long, long long> chineseRemainderTheorem(
    const std::vector<long long>& remainders,
    const std::vector<long long>& mods) {

    if (remainders.size() != mods.size()) {
        throw std::invalid_argument("Количество остатков и модулей должно совпадать");
    }

    if (!areCoprime(mods)) {
        throw std::invalid_argument("Модули должны быть попарно взаимно простыми");
    }

    long long M = 1;
    for (size_t i = 0; i < mods.size(); ++i) {
        M *= mods[i];
    }

    long long result = 0;

    for (size_t i = 0; i < mods.size(); ++i) {
        long long Mi = M / mods[i];
        long long invMi, y;
        extendedEuclidean(Mi, mods[i], invMi, y);

        invMi = (invMi % mods[i] + mods[i]) % mods[i];

        long long term1 = modMultiply(remainders[i], Mi, M);
        long long term2 = modMultiply(term1, invMi, M);
        result = (result + term2) % M;
    }

    return std::make_pair(result, M);
}

void printHelp() {
    std::cout << "ИСПОЛЬЗОВАНИЕ:\n";
    std::cout << "  lab5.exe --mode euler <число>\n";
    std::cout << "  lab5.exe --mode euclid <число1> <число2>\n";
    std::cout << "  lab5.exe --mode chinese\n";
    std::cout << "  lab5.exe (без аргументов - интерактивный режим)\n\n";

    std::cout << "Примеры:\n";
    std::cout << "  lab5.exe --mode euler 100\n";
    std::cout << "  lab5.exe --mode euclid 56 98\n";
    std::cout << "  lab5.exe --mode chinese\n";
}

int main(int argc, char* argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    if (argc == 1) {
        std::cout << "=== ИНТЕРАКТИВНЫЙ РЕЖИМ ===\n";
        std::cout << "Выберите алгоритм:\n";
        std::cout << "1. Функция Эйлера\n";
        std::cout << "2. Расширенный алгоритм Евклида\n";
        std::cout << "3. Китайская теорема об остатках\n";
        std::cout << "Введите номер (1-3): ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "=== Функция Эйлера ===\n";
            unsigned long long n;
            std::cout << "Введите число: ";
            std::cin >> n;

            auto start = std::chrono::high_resolution_clock::now();
            unsigned long long result = eulerTotient(n);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            std::cout << "φ(" << n << ") = " << result << "\n";
            std::cout << "Время: " << duration.count() << " мкс\n";
        }
        else if (choice == 2) {
            std::cout << "=== Расширенный алгоритм Евклида ===\n";
            long long a, b;
            std::cout << "Введите первое число: ";
            std::cin >> a;
            std::cout << "Введите второе число: ";
            std::cin >> b;

            long long x, y;
            auto start = std::chrono::high_resolution_clock::now();
            long long gcd = extendedEuclidean(a, b, x, y);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            std::cout << "НОД(" << a << ", " << b << ") = " << gcd << "\n";
            std::cout << "Коэффициенты: x = " << x << ", y = " << y << "\n";
            std::cout << "Проверка: " << a << "*" << x << " + " << b << "*" << y
                << " = " << (a * x + b * y) << "\n";
            std::cout << "Время: " << duration.count() << " мкс\n";
        }
        else if (choice == 3) {
            std::cout << "=== Китайская теорема об остатках ===\n";
            int k;
            std::cout << "Введите количество сравнений: ";
            std::cin >> k;

            std::vector<long long> remainders(k);
            std::vector<long long> mods(k);

            std::cout << "Введите систему сравнений (остаток модуль):\n";
            for (int i = 0; i < k; ++i) {
                std::cout << "Сравнение " << i + 1 << ": ";
                std::cin >> remainders[i] >> mods[i];
            }

            try {
                auto start = std::chrono::high_resolution_clock::now();
                std::pair<long long, long long> result = chineseRemainderTheorem(remainders, mods);
                long long solution = result.first;
                long long M = result.second;
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

                std::cout << "\nРЕЗУЛЬТАТ: x ≡ " << solution << " (mod " << M << ")\n";
                std::cout << "Время вычисления: " << duration.count() << " мкс\n";

                std::cout << "\nПроверка:\n";
                bool allCorrect = true;
                for (int i = 0; i < k; ++i) {
                    long long check = solution % mods[i];
                    std::cout << solution << " mod " << mods[i] << " = " << check
                        << " (ожидалось " << remainders[i] << ") ";
                    if (check == remainders[i]) {
                        std::cout << "✓\n";
                    }
                    else {
                        std::cout << "✗\n";
                        allCorrect = false;
                    }
                }
                if (allCorrect) {
                    std::cout << "\nВсе проверки пройдены успешно!\n";
                }
            }
            catch (const std::exception& e) {
                std::cout << "ОШИБКА: " << e.what() << "\n";
            }
        }
        else {
            std::cout << "Неверный выбор!\n";
            printHelp();
        }
        return 0;
    }

    std::string mode = "";
    std::vector<std::string> args;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--mode" && i + 1 < argc) {
            mode = argv[++i];
        }
        else if (arg == "--help" || arg == "-h" || arg == "/?") {
            printHelp();
            return 0;
        }
        else if (arg[0] != '-') {
            args.push_back(arg);
        }
    }

    if (mode.empty()) {
        std::cout << "Ошибка: не указан режим (используйте --mode)\n\n";
        printHelp();
        return 1;
    }

    if (mode == "euler") {
        if (args.size() < 1) {
            std::cout << "Ошибка: для режима 'euler' требуется число\n";
            std::cout << "Пример: lab5.exe --mode euler 100\n";
            return 1;
        }

        unsigned long long n = std::stoull(args[0]);

        auto start = std::chrono::high_resolution_clock::now();
        unsigned long long result = eulerTotient(n);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "φ(" << n << ") = " << result << "\n";
        std::cout << "Время: " << duration.count() << " мкс\n";
    }
    else if (mode == "euclid") {
        if (args.size() < 2) {
            std::cout << "Ошибка: для режима 'euclid' требуется два числа\n";
            std::cout << "Пример: lab5.exe --mode euclid 56 98\n";
            return 1;
        }

        long long a = std::stoll(args[0]);
        long long b = std::stoll(args[1]);

        long long x, y;
        auto start = std::chrono::high_resolution_clock::now();
        long long gcd = extendedEuclidean(a, b, x, y);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "НОД(" << a << ", " << b << ") = " << gcd << "\n";
        std::cout << "Коэффициенты: x = " << x << ", y = " << y << "\n";
        std::cout << "Проверка: " << a << "*" << x << " + " << b << "*" << y
            << " = " << (a * x + b * y) << "\n";
        std::cout << "Время: " << duration.count() << " мкс\n";
    }
    else if (mode == "chinese") {
        std::cout << "=== Китайская теорема об остатках ===\n";
        int k;
        std::cout << "Введите количество сравнений: ";
        std::cin >> k;

        std::vector<long long> remainders(k);
        std::vector<long long> mods(k);

        std::cout << "Введите систему сравнений (остаток модуль):\n";
        for (int i = 0; i < k; ++i) {
            std::cout << "Сравнение " << i + 1 << ": ";
            std::cin >> remainders[i] >> mods[i];
        }

        try {
            auto start = std::chrono::high_resolution_clock::now();
            std::pair<long long, long long> result = chineseRemainderTheorem(remainders, mods);
            long long solution = result.first;
            long long M = result.second;
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

            std::cout << "\nРЕЗУЛЬТАТ: x ≡ " << solution << " (mod " << M << ")\n";
            std::cout << "Время вычисления: " << duration.count() << " мкс\n";

            std::cout << "\nПроверка:\n";
            bool allCorrect = true;
            for (int i = 0; i < k; ++i) {
                long long check = solution % mods[i];
                std::cout << solution << " mod " << mods[i] << " = " << check
                    << " (ожидалось " << remainders[i] << ") ";
                if (check == remainders[i]) {
                    std::cout << "✓\n";
                }
                else {
                    std::cout << "✗\n";
                    allCorrect = false;
                }
            }
            if (allCorrect) {
                std::cout << "\nВсе проверки пройдены успешно!\n";
            }
        }
        catch (const std::exception& e) {
            std::cout << "ОШИБКА: " << e.what() << "\n";
        }
    }
    else {
        std::cout << "Неверный режим: " << mode << "\n\n";
        printHelp();
        return 1;
    }

    return 0;
}