/*
솔루션명 : HW09_4_22312076_KDH
소스코드 : InterCityDist.cpp

프로그램의 목적 및 기능
    - 도시 간 거리 데이터 관리
    - 도시 간 거리 조회 기능 제공
    - 도시 거리 테이블 출력 기능 제공

함수 목록
    - addInterCityDist
    - getDistance
    - fgetCityPairDistTbl
    - printDistanceTable

최초 프로그램 작성 정보
   - 작성자 : 김동하
   - 학 번 : 22312076
   - 소 속 : 디지털융합대학 컴퓨터학부 정보통신공학전공
   - 작성일 : 2026/05/14
*/

#include "InterCityDist.h"

using namespace std;

/*
* 함수명 : addInterCityDist
* 설  명 : 두 도시 간 거리를 양방향으로 map에 저장
* 매개변수 :
*   - distanceMap : 도시 거리 map
*   - city1 : 첫 번째 도시
*   - city2 : 두 번째 도시
*   - distance : 도시 간 거리
* 반환값 : 없음
*/
void addInterCityDist(map<pair<string, string>, int>& distanceMap, const string& city1, const string& city2, int distance) {

    distanceMap[{city1, city2}] = distance;
    distanceMap[{city2, city1}] = distance;
}

/*
* 함수명 : getDistance
* 설  명 : 두 도시 간 거리 조회
* 매개변수 :
*   - distanceMap : 도시 거리 map
*   - city1 : 첫 번째 도시
*   - city2 : 두 번째 도시
*   - distance : 조회된 거리 저장 변수
* 반환값 : 거리 존재 여부
*/
bool getDistance(const map<pair<string, string>, int>& distanceMap, const string& city1, const string& city2, int& distance) {

    auto it = distanceMap.find({ city1, city2 });//도시 쌍 검색

    if (it != distanceMap.end()) {
        distance = it->second;//거리 값 저장
        return true;
    }

    return false;
}

/*
* 함수명 : fgetCityPairDistTbl
* 설  명 : 파일에서 도시 거리 데이터를 읽어 map에 저장
* 매개변수 :
*   - fin : 입력 스트림
*   - cities : 도시 이름 저장 set
*   - distMap : 도시 거리 map
* 반환값 : 입력된 도시 쌍 개수
*/
int fgetCityPairDistTbl(istream& fin, set<string>& cities,
    map<pair<string, string>, int>& distMap) {

    string header;

    fin >> header;//파일 헤더 읽기

    string city1, city2;
    int dist;
    int count = 0;

    while (fin >> city1 >> city2 >> dist) {
        //----첫, 두번째 도시들을 저자ㅇ
        cities.insert(city1);
        cities.insert(city2);

        addInterCityDist(distMap, city1, city2, dist);//거리 정보 저장

        count++;
    }

    return count;
}

/*
* 함수명 : printDistanceTable
* 설  명 : 전체 도시 간 거리표 출력
* 매개변수 :
*   - distanceMap : 도시 거리 map
*   - cities : 도시 이름 set
* 반환값 : 없음
*/
void printDistanceTable(const map<pair<string, string>, int>& distanceMap,
    const set<string>& cities) {

    const int CITY_W = 4;
    const int COL_W = 6;

    cout << left << setw(CITY_W) << "City" << " |";

    for (const auto& c : cities)
        cout << right << setw(COL_W) << c;

    cout << endl;

    //----표 내용 출력
    cout << string(CITY_W + 1, '-') << "+";
    cout << string(COL_W * cities.size(), '-') << endl;

    for (const auto& row : cities) {
        cout << left << setw(CITY_W) << row << " |";//행 도시 출력

        for (const auto& col : cities) {

            if (row == col) {
                cout << right << setw(COL_W) << 0;//같은 도시 거리 0 출력
            }
            else {
                int d;

                if (getDistance(distanceMap, row, col, d))
                    cout << right << setw(COL_W) << d;//거리 출력
                else
                    cout << right << setw(COL_W) << "-";//거리 정보 없음 출력
            }
        }

        cout << endl;
    }
}