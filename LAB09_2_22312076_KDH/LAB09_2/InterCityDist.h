/*
솔루션명 : LAB09_2_22312076_KDH
소스코드 : InterCityDist.h

프로그램의 목적 및 기능
    - 도시 간 거리 데이터 함수 선언
    - 도시 거리 조회 및 출력 기능 정의
    - STL map과 set 기반 거리 테이블 관리

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

#ifndef INTER_CITY_DIST_H
#define INTER_CITY_DIST_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <set>
#include <utility>
#include <string>

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
void addInterCityDist(std::map<std::pair<std::string, std::string>, int>& distanceMap, const std::string& city1, const std::string& city2, int distance);

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
bool getDistance(const std::map<std::pair<std::string, std::string>, int>& distanceMap, const std::string& city1, const std::string& city2, int& distance);

/*
* 함수명 : fgetCityPairDistTbl
* 설  명 : 파일에서 도시 거리 데이터를 읽어 map에 저장
* 매개변수 :
*   - fin : 입력 스트림
*   - cities : 도시 이름 저장 set
*   - distMap : 도시 거리 map
* 반환값 : 입력된 도시 쌍 개수
*/
int fgetCityPairDistTbl(std::istream& fin, std::set<std::string>& cities,   std::map<std::pair<std::string, std::string>, int>& distMap);

/*
* 함수명 : printDistanceTable
* 설  명 : 전체 도시 간 거리표 출력
* 매개변수 :
*   - distanceMap : 도시 거리 map
*   - cities : 도시 이름 set
* 반환값 : 없음
*/
void printDistanceTable(const std::map<std::pair<std::string, std::string>, int>& distanceMap,  const std::set<std::string>& cities);

#endif