#include "SkipList/SkipList.h"
#include "LinkedList/LinkedList.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

// 准备测试数据
std::vector<int> prepareTestData(int testSize) {
    std::vector<int> testData;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1, 1000000);

    for (int i = 0; i < testSize; ++i) {
        testData.push_back(distribution(generator));
    }

    return testData;
}

// 测试数据大小
constexpr int testSize = 100000;

template<typename K, typename V, typename List>
void insertionTest(List& list, const std::vector<K>& keys) {
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& key : keys) {
        list.addItem(key, key);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Insertion of " << keys.size() << " elements took " << duration.count() << " milliseconds." << std::endl;
}

template<typename K, typename V, typename List>
void searchTest(List& list, const std::vector<K>& keys) {
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& key : keys) {
        list.searchItem(key);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Search of " << keys.size() << " elements took " << duration.count() << " milliseconds." << std::endl;
}

template<typename K, typename V, typename List>
void deletionTest(List& list, const std::vector<K>& keys) {
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& key : keys) {
        list.deleteItem(key);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "Deletion of " << keys.size() << " elements took " << duration.count() << " milliseconds." << std::endl;
}

int main() {
    // 准备测试数据
    std::vector<int> testData = prepareTestData(testSize);

    // 创建跳表和链表实例
    SkipList<int, int> skipList;
    LinkedList<int, int> linkedList;

    // 插入测试
    std::cout << "SkipList Insertion Test" << std::endl;
    insertionTest<int, int>(skipList, testData);
    std::cout << "LinkedList Insertion Test" << std::endl;
    insertionTest<int, int>(linkedList, testData);
    std::cout<<std::endl;
    // 查询测试
    std::cout << "SkipList Search Test" << std::endl;
    searchTest<int, int>(skipList, testData);
    std::cout << "LinkedList Search Test" << std::endl;
    searchTest<int, int>(linkedList, testData);
    std::cout<<std::endl;
    // 删除测试
    std::cout << "SkipList Deletion Test" << std::endl;
    deletionTest<int, int>(skipList, testData);
    std::cout << "LinkedList Deletion Test" << std::endl;
    deletionTest<int, int>(linkedList, testData);

    return 0;
}