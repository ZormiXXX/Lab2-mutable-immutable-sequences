#include <chrono>
#include <iostream>
#include <string>
#include <tuple>
#include "../include/Immutable/ImmutableArraySequence.hpp"
#include "../include/Immutable/ImmutableListSequence.hpp"
#include "../include/Mutable/MutableArraySequence.hpp"
#include "../include/Mutable/MutableListSequence.hpp"

namespace {

void ShowMenu() {
    std::cout << "\n╔══════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║           ЛАБОРАТОРНАЯ РАБОТА №2 - МЕНЮ                   ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  СОЗДАНИЕ:                                               ║" << std::endl;
    std::cout << "║   1. Создать MutableArraySequence                        ║" << std::endl;
    std::cout << "║   2. Создать ImmutableArraySequence                      ║" << std::endl;
    std::cout << "║   3. Создать MutableListSequence                         ║" << std::endl;
    std::cout << "║   4. Создать ImmutableListSequence                       ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  ОСНОВНЫЕ ОПЕРАЦИИ:                                      ║" << std::endl;
    std::cout << "║   5. Append (добавить в конец)                           ║" << std::endl;
    std::cout << "║   6. Prepend (добавить в начало)                         ║" << std::endl;
    std::cout << "║   7. InsertAt (вставить по индексу)                      ║" << std::endl;
    std::cout << "║   8. Concat (сцепить две последовательности)             ║" << std::endl;
    std::cout << "║   9. GetSubsequence (подпоследовательность)              ║" << std::endl;
    std::cout << "║   10. Slice (заменить фрагмент)                          ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  MAP-REDUCE И ДОПОЛНЕНИЯ:                                ║" << std::endl;
    std::cout << "║   11. Map                                                ║" << std::endl;
    std::cout << "║   12. FlatMap                                            ║" << std::endl;
    std::cout << "║   13. Reduce                                             ║" << std::endl;
    std::cout << "║   14. Where                                              ║" << std::endl;
    std::cout << "║   15. Find / TryFind                                     ║" << std::endl;
    std::cout << "║   16. Zip                                                ║" << std::endl;
    std::cout << "║   17. Unzip                                              ║" << std::endl;
    std::cout << "║   18. Split                                              ║" << std::endl;
    std::cout << "║   19. IEnumerable / IEnumerator                          ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  ИНФОРМАЦИЯ И ПРОВЕРКА:                                  ║" << std::endl;
    std::cout << "║   20. Показать последовательность                        ║" << std::endl;
    std::cout << "║   21. Показать длину                                     ║" << std::endl;
    std::cout << "║   22. Запустить модульные тесты                          ║" << std::endl;
    std::cout << "║   23. Сравнить производительность                        ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║   0. Выход                                               ║" << std::endl;
    std::cout << "╚══════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << "> ";
}

template<class T>
void PrintSequence(const std::string& label, Sequence<T>* seq) {
    std::cout << "\n    " << label << ": [";
    for (int i = 0; i < seq->GetLength(); i++) {
        std::cout << seq->Get(i);
        if (i < seq->GetLength() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

void PrintTupleSequence(const std::string& label, Sequence<std::tuple<int, char>>* seq) {
    std::cout << "\n    " << label << ": [";
    for (int i = 0; i < seq->GetLength(); i++) {
        auto item = seq->Get(i);
        std::cout << "(" << std::get<0>(item) << ", " << std::get<1>(item) << ")";
        if (i < seq->GetLength() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

void UpdateCurrentSequence(Sequence<int>*& current, Sequence<int>* updated) {
    if (updated != current) {
        delete current;
        current = updated;
    }
}

void RequireSequence(Sequence<int>* seq) {
    if (seq == nullptr) {
        throw std::runtime_error("Сначала создайте последовательность");
    }
}

template<class Factory>
double MeasureAppendPerformance(Factory factory, int operations) {
    Sequence<int>* seq = factory();
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < operations; i++) {
        Sequence<int>* updated = seq->Append(i);
        if (updated != seq) {
            delete seq;
            seq = updated;
        }
    }

    auto finish = std::chrono::high_resolution_clock::now();
    delete seq;

    return std::chrono::duration<double, std::milli>(finish - start).count();
}

void RunPerformanceComparison() {
    const int operations = 1500;

    std::cout << "\nСравнение производительности (" << operations << " операций Append)" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << "MutableArraySequence:   "
              << MeasureAppendPerformance([]() { return new MutableArraySequence<int>(); }, operations)
              << " ms" << std::endl;
    std::cout << "ImmutableArraySequence: "
              << MeasureAppendPerformance([]() { return new ImmutableArraySequence<int>(); }, operations)
              << " ms" << std::endl;
    std::cout << "MutableListSequence:    "
              << MeasureAppendPerformance([]() { return new MutableListSequence<int>(); }, operations)
              << " ms" << std::endl;
    std::cout << "ImmutableListSequence:  "
              << MeasureAppendPerformance([]() { return new ImmutableListSequence<int>(); }, operations)
              << " ms" << std::endl;
}

}  // namespace

extern void RunAllTests();

int main() {
    setlocale(LC_ALL, "Russian");

    Sequence<int>* seq = nullptr;

    std::cout << "\nЛабораторная работа №2 - Система линейных структур данных" << std::endl;
    std::cout << "=============================================================" << std::endl;

    while (true) {
        ShowMenu();
        int choice = 0;
        std::cin >> choice;

        try {
            switch (choice) {
                case 1: {
                    int arr[] = {1, 2, 3, 4, 5};
                    delete seq;
                    seq = new MutableArraySequence<int>(arr, 5);
                    std::cout << "Создана MutableArraySequence: [1, 2, 3, 4, 5]" << std::endl;
                    break;
                }
                case 2: {
                    int arr[] = {1, 2, 3, 4, 5};
                    delete seq;
                    seq = new ImmutableArraySequence<int>(arr, 5);
                    std::cout << "Создана ImmutableArraySequence: [1, 2, 3, 4, 5]" << std::endl;
                    break;
                }
                case 3: {
                    int arr[] = {1, 2, 3, 4, 5};
                    delete seq;
                    seq = new MutableListSequence<int>(arr, 5);
                    std::cout << "Создана MutableListSequence: [1, 2, 3, 4, 5]" << std::endl;
                    break;
                }
                case 4: {
                    int arr[] = {1, 2, 3, 4, 5};
                    delete seq;
                    seq = new ImmutableListSequence<int>(arr, 5);
                    std::cout << "Создана ImmutableListSequence: [1, 2, 3, 4, 5]" << std::endl;
                    break;
                }
                case 5: {
                    RequireSequence(seq);
                    int value = 0;
                    std::cout << "Введите значение: ";
                    std::cin >> value;
                    UpdateCurrentSequence(seq, seq->Append(value));
                    PrintSequence("После Append", seq);
                    break;
                }
                case 6: {
                    RequireSequence(seq);
                    int value = 0;
                    std::cout << "Введите значение: ";
                    std::cin >> value;
                    UpdateCurrentSequence(seq, seq->Prepend(value));
                    PrintSequence("После Prepend", seq);
                    break;
                }
                case 7: {
                    RequireSequence(seq);
                    int value = 0;
                    int index = 0;
                    std::cout << "Введите значение: ";
                    std::cin >> value;
                    std::cout << "Введите индекс: ";
                    std::cin >> index;
                    UpdateCurrentSequence(seq, seq->InsertAt(value, index));
                    PrintSequence("После InsertAt", seq);
                    break;
                }
                case 8: {
                    RequireSequence(seq);
                    int arr[] = {10, 20, 30};
                    Sequence<int>* extra = Sequence<int>::From(arr, 3);
                    PrintSequence("Вторая последовательность", extra);
                    UpdateCurrentSequence(seq, seq->Concat(extra));
                    PrintSequence("После Concat", seq);
                    delete extra;
                    break;
                }
                case 9: {
                    RequireSequence(seq);
                    int start = 0;
                    int end = 0;
                    std::cout << "Начальный индекс: ";
                    std::cin >> start;
                    std::cout << "Конечный индекс: ";
                    std::cin >> end;
                    Sequence<int>* sub = seq->GetSubsequence(start, end);
                    PrintSequence("Подпоследовательность", sub);
                    delete sub;
                    break;
                }
                case 10: {
                    RequireSequence(seq);
                    int index = 0;
                    int count = 0;
                    std::cout << "Индекс начала: ";
                    std::cin >> index;
                    std::cout << "Количество элементов для удаления: ";
                    std::cin >> count;

                    int arr[] = {99, 100};
                    Sequence<int>* replacement = Sequence<int>::From(arr, 2);
                    PrintSequence("Заменяющая последовательность", replacement);
                    UpdateCurrentSequence(seq, seq->Slice(index, count, replacement));
                    PrintSequence("После Slice", seq);
                    delete replacement;
                    break;
                }
                case 11: {
                    RequireSequence(seq);
                    Sequence<int>* mapped = seq->Map([](int value) { return value * 2; });
                    PrintSequence("Map (x * 2)", mapped);
                    delete mapped;
                    break;
                }
                case 12: {
                    RequireSequence(seq);
                    Sequence<int>* flatMapped = seq->FlatMap([](int value) {
                        int arr[] = {value, value * 10};
                        return Sequence<int>::From(arr, 2);
                    });
                    PrintSequence("FlatMap ([x, x*10])", flatMapped);
                    delete flatMapped;
                    break;
                }
                case 13: {
                    RequireSequence(seq);
                    int sum = seq->Reduce([](int value, int acc) { return value + acc; }, 0);
                    std::cout << "Reduce (сумма): " << sum << std::endl;

                    int custom = seq->Reduce([](int value, int acc) { return 2 * value + 3 * acc; }, 4);
                    std::cout << "Reduce по формуле 2*x + 3*acc при c=4: " << custom << std::endl;
                    break;
                }
                case 14: {
                    RequireSequence(seq);
                    Sequence<int>* filtered = seq->Where([](int value) { return value % 2 == 0; });
                    PrintSequence("Where (чётные)", filtered);
                    delete filtered;
                    break;
                }
                case 15: {
                    RequireSequence(seq);
                    int target = 0;
                    std::cout << "Введите искомое значение: ";
                    std::cin >> target;

                    try {
                        int found = seq->Find([target](int value) { return value == target; });
                        std::cout << "Find: найдено значение " << found << std::endl;
                    } catch (const ElementNotFound&) {
                        std::cout << "Find: значение не найдено" << std::endl;
                    }

                    Option<int> result = seq->TryFind([target](int value) { return value == target; });
                    if (result.IsSome()) {
                        std::cout << "TryFind: найдено значение " << result.GetValue() << std::endl;
                    } else {
                        std::cout << "TryFind: возвращено None" << std::endl;
                    }
                    break;
                }
                case 16: {
                    RequireSequence(seq);
                    char letters[] = {'A', 'B', 'C', 'D', 'E'};
                    Sequence<char>* second = Sequence<char>::From(letters, 5);
                    Sequence<std::tuple<int, char>>* zipped = seq->Zip(second);
                    PrintTupleSequence("Zip результат", zipped);
                    delete zipped;
                    delete second;
                    break;
                }
                case 17: {
                    RequireSequence(seq);
                    char letters[] = {'A', 'B', 'C', 'D', 'E'};
                    Sequence<char>* second = Sequence<char>::From(letters, 5);
                    Sequence<std::tuple<int, char>>* zipped = seq->Zip(second);
                    PrintTupleSequence("Исходная zip-последовательность", zipped);

                    auto [first, secondPart] = zipped->Unzip();
                    PrintSequence("Unzip: первая последовательность", first);
                    PrintSequence("Unzip: вторая последовательность", secondPart);

                    delete first;
                    delete secondPart;
                    delete zipped;
                    delete second;
                    break;
                }
                case 18: {
                    RequireSequence(seq);
                    Sequence<Sequence<int>*>* split = seq->Split([](int value) { return value == 3; });
                    std::cout << "\n    Split по условию (x == 3):" << std::endl;
                    for (int i = 0; i < split->GetLength(); i++) {
                        PrintSequence("Фрагмент " + std::to_string(i), split->Get(i));
                        delete split->Get(i);
                    }
                    delete split;
                    break;
                }
                case 19: {
                    RequireSequence(seq);
                    IEnumerator<int>* enumerator = seq->GetEnumerator();
                    std::cout << "\n    Обход через IEnumerator: [";
                    bool first = true;
                    while (enumerator->MoveNext()) {
                        if (!first) {
                            std::cout << ", ";
                        }
                        std::cout << enumerator->GetCurrent();
                        first = false;
                    }
                    std::cout << "]" << std::endl;
                    delete enumerator;
                    break;
                }
                case 20: {
                    RequireSequence(seq);
                    PrintSequence("Текущая последовательность", seq);
                    break;
                }
                case 21: {
                    RequireSequence(seq);
                    std::cout << "Длина последовательности: " << seq->GetLength() << std::endl;
                    break;
                }
                case 22: {
                    RunAllTests();
                    break;
                }
                case 23: {
                    RunPerformanceComparison();
                    break;
                }
                case 0: {
                    delete seq;
                    std::cout << "\nВыход из программы..." << std::endl;
                    return 0;
                }
                default:
                    std::cout << "Неверный выбор! Попробуйте снова." << std::endl;
            }
        } catch (const std::exception& error) {
            std::cout << "Ошибка: " << error.what() << std::endl;
        }
    }
}
