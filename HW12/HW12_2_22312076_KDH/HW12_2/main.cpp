/*
솔루션명 : HW12_2_22312076_KDH
소스코드 : main.cpp

프로그램의 목적 및 기능
    - STL multimap(RBT 기반)과 unordered_multimap(해시)의
      insert() 및 find() 수행 시간을 측정하고 비교
    - 100만 건 삽입 후 10만 건 랜덤 탐색으로 두 컨테이너의 성능 차이 확인
    - unordered_multimap의 해시 테이블 내부 정보(버킷 수, 부하율) 출력

함수 목록
    - measureInsertPerformance : 삽입 성능 측정
    - measureFindPerformance : 탐색 성능 측정
    - main

특이사항
    - unordered_multimap은 max_load_factor 0.75, reserve(NUM_DATA) 미리 설정

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/06/05
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include <utility>
#include <random>
#include <chrono>
using namespace std;
using namespace std::chrono;
using Clock = chrono::high_resolution_clock;

constexpr int NUM_DATA = 1000000;
constexpr int NUM_FIND_TEST = 100000;

/*
* 함수명 : measureInsertPerformance
* 설  명 : kvStr_pairs의 모든 쌍을 container에 삽입하고 경과 시간 반환
* 매개변수 : container - 삽입 대상 컨테이너 / kvStr_pairs - 삽입할 데이터
* 반환값 : 경과 시간
*/
template <typename Container>
long long measureInsertPerformance(Container& container,
    const vector<pair<string, string>>& kvStr_pairs)
{
    auto start = Clock::now();
    for (const auto& kvStr_pair : kvStr_pairs)
        container.insert(kvStr_pair);
    auto end = Clock::now();

    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

/*
* 함수명 : measureFindPerformance
* 설  명 : testKeys의 모든 key를 container에서 find()하고 경과 시간 반환
*          탐색된 수와 testKeys 크기가 다르면 오류 메시지 출력
* 매개변수 : container - 탐색 대상 컨테이너 / testKeys - 탐색할 키 목록
* 반환값 : 경과 시간
*/
template <typename Container>
long long measureFindPerformance(const Container& container,
    const vector<string>& testKeys)
{
    size_t foundCount = 0;
    size_t num_test_words = testKeys.size();

    auto start = Clock::now();
    for (const auto& key : testKeys)
    {
        auto itr = container.find(key);
        if (itr != container.end())
            ++foundCount;
    }
    auto end = Clock::now();

    if (num_test_words != foundCount) {
        cout << "Error in find operations : foundCount (" << foundCount;
        cout << ") is different from size of testKeys(" << num_test_words << ")\n";
    }

    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

int main()
{
    cout << "[Comparison of performance of insert() and find() in std::multimap"
        << " and std::unordered_multimap]\n";
    cout << "Total num_data = " << NUM_DATA
        << ", num_find_test = " << NUM_FIND_TEST << '\n';

    multimap<string, string>           orderedMultiMap;
    unordered_multimap<string, string> unorderedMultiMap;

    // rehash 방지를 위해 미리 버킷 확보, 부하율 0.75로 설정
    unorderedMultiMap.max_load_factor(0.75);
    unorderedMultiMap.reserve(NUM_DATA);

    //key_0 ~ key_999999, value_0 ~ value_999999 쌍 생성 후 각 컨테이너에 삽입
    cout << "\n[1] Data creations and insertions ...\n";

    vector<pair<string, string>> kv_pairs;
    kv_pairs.reserve(NUM_DATA);
    for (int i = 0; i < NUM_DATA; i++)
        kv_pairs.push_back(make_pair("key_" + to_string(i), "value_" + to_string(i)));

    long long orderedInsertTime = measureInsertPerformance(orderedMultiMap, kv_pairs);
    cout << "std::multimap insert time          : " << orderedInsertTime << " [us]\n";

    long long unorderedInsertTime = measureInsertPerformance(unorderedMultiMap, kv_pairs);
    cout << "std::unordered_multimap insert time: " << unorderedInsertTime << " [us]\n";

    //버킷별 크기를 순회해 사용된 버킷 수와 최대 버킷 크기 집계
    cout << "\n[2] Information of unordered_multimap hash table\n";

    size_t maxBucketSize = 0;
    size_t usedBucketCount = 0;

    for (size_t i = 0; i < unorderedMultiMap.bucket_count(); ++i)
    {
        size_t bucketSize = unorderedMultiMap.bucket_size(i);
        if (bucketSize > 0)
            ++usedBucketCount;
        if (bucketSize > maxBucketSize)
            maxBucketSize = bucketSize;
    }

    double usedBucketRatio =
        static_cast<double>(usedBucketCount) / unorderedMultiMap.bucket_count() * 100.0;

    cout << "size               : " << setw(8) << unorderedMultiMap.size() << '\n';
    cout << "bucket_count       : " << setw(8) << unorderedMultiMap.bucket_count() << '\n';
    cout << "used_bucket_count  : " << setw(8) << usedBucketCount << '\n';
    cout << "empty_bucket_count : " << setw(8)
        << unorderedMultiMap.bucket_count() - usedBucketCount << '\n';
    cout << "load_factor        : " << unorderedMultiMap.load_factor() << '\n';
    cout << "max_load_factor    : " << unorderedMultiMap.max_load_factor() << '\n';
    cout << "max_bucket_size    : " << maxBucketSize << '\n';
    cout << "used_bucket_ratio  : " << fixed << setprecision(2) << usedBucketRatio << " %\n";

    // 0 ~ NUM_DATA-1 범위 난수로 testKeys 구성 후 두 컨테이너에서 find() 성능 측정
    cout << "\n[3] Performance measurement results of find() for "
        << NUM_FIND_TEST << " test words\n";

    vector<string> testKeys;
    testKeys.reserve(NUM_FIND_TEST);

    mt19937 gen(12345);
    uniform_int_distribution<int> dist(0, NUM_DATA - 1);
    for (int i = 0; i < NUM_FIND_TEST; ++i)
        testKeys.push_back("key_" + to_string(dist(gen)));

    long long orderedFindTime = measureFindPerformance(orderedMultiMap, testKeys);
    long long unorderedFindTime = measureFindPerformance(unorderedMultiMap, testKeys);

    cout << "std::multimap find time          : " << setw(8) << orderedFindTime << " [us]\n";
    cout << "std::unordered_multimap find time: " << setw(8) << unorderedFindTime << " [us]\n";

    return 0;
}